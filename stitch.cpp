#include"stitch.h"

using namespace std;

namespace cv {

Stitch Stitch::createDefault(bool try_use_gpu)
{
    Stitch stitcher;
    stitcher.setRegistrationResol(0.6);
    stitcher.setSeamEstimationResol(0.1);
    stitcher.setCompositingResol(ORIG_RESOL);
    stitcher.setPanoConfidenceThresh(1);
    stitcher.setWaveCorrection(true);
    stitcher.setWaveCorrectKind(detail::WAVE_CORRECT_HORIZ);
    stitcher.setFeaturesMatcher(new detail::BestOf2NearestMatcher(try_use_gpu));
    stitcher.setBundleAdjuster(new detail::BundleAdjusterRay());

#if defined(HAVE_OPENCV_GPU) && !defined(DYNAMIC_CUDA_SUPPORT)
    if (try_use_gpu && gpu::getCudaEnabledDeviceCount() > 0)
    {
#if defined(HAVE_OPENCV_NONFREE)
        stitcher.setFeaturesFinder(new detail::SurfFeaturesFinderGpu());
#else
        stitcher.setFeaturesFinder(new detail::OrbFeaturesFinder());
#endif
        stitcher.setWarper(new SphericalWarperGpu());
        stitcher.setSeamFinder(new detail::GraphCutSeamFinderGpu());
    }
    else
#endif
    {
#ifdef HAVE_OPENCV_NONFREE
        stitcher.setFeaturesFinder(new detail::SurfFeaturesFinder());
#else
        stitcher.setFeaturesFinder(new detail::OrbFeaturesFinder());
#endif
        stitcher.setWarper(new SphericalWarper());
        stitcher.setSeamFinder(new detail::GraphCutSeamFinder(detail::GraphCutSeamFinderBase::COST_COLOR));
    }
    stitcher.setExposureCompensator(new detail::BlocksGainCompensator());
    stitcher.setBlender(new detail::MultiBandBlender(try_use_gpu));

    return stitcher;
}


Stitch::Status Stitch::estimateTransform(InputArray images)
{
    return estimateTransform(images, vector<vector<Rect> >());
}

Stitch::Status Stitch::estimateTransform(InputArray images, const vector<vector<Rect> > &rois)
{
    images.getMatVector(imgs_);
    rois_ = rois;

    Status status;
    if ((status = matchImages()) != OK)
        return status;

    estimateCameraParams();

    return OK;
}


Stitch::Status Stitch::composePanorama2(OutputArray pano, std::vector<cv::Mat> &img_warp,std::vector<cv::Mat> &nodilate_mask,vector<Point> &corners_,std::vector<cv::Mat> &Ks,std::vector<detail::CameraParams> &cameras_s)
{
    return composePanorama2(vector<Mat>(), pano,img_warp,nodilate_mask,corners_,Ks,cameras_s);
}

Stitch::Status Stitch::composePanorama2(InputArray images ,OutputArray pano, std::vector<cv::Mat> &img_warp,std::vector<cv::Mat> &nodilate_mask,vector<Point> &corners_,vector<Mat> &Ks,std::vector<detail::CameraParams> &cameras_s)
{    
    LOGLN("Warping images (auxiliary)... ");
    vector<Mat> imgs;
    images.getMatVector(imgs);
    if (!imgs.empty())
    {
        CV_Assert(imgs.size() == imgs_.size());
        Mat img;
        seam_est_imgs_.resize(imgs.size());

        for (size_t i = 0; i < imgs.size(); ++i)
        {
            imgs_[i] = imgs[i];
            resize(imgs[i], img, Size(), seam_scale_, seam_scale_);
            seam_est_imgs_[i] = img.clone();
        }

        vector<Mat> seam_est_imgs_subset;
        vector<Mat> imgs_subset;

        for (size_t i = 0; i < indices_.size(); ++i)
        {
            imgs_subset.push_back(imgs_[indices_[i]]);
            seam_est_imgs_subset.push_back(seam_est_imgs_[indices_[i]]);
        }

        seam_est_imgs_ = seam_est_imgs_subset;
        imgs_ = imgs_subset;
    }

    Mat &pano_ = pano.getMatRef();


#if ENABLE_LOG
    int64 t = getTickCount();
#endif

    vector<Point> corners(imgs_.size());
    vector<Mat> masks_warped(imgs_.size());
    vector<Mat> images_warped(imgs_.size());
    vector<Size> sizes(imgs_.size());
    vector<Mat> masks(imgs_.size());

    // Prepare image masks
    for (size_t i = 0; i < imgs_.size(); ++i)
    {
        masks[i].create(seam_est_imgs_[i].size(), CV_8U);
        masks[i].setTo(Scalar::all(255));
    }
    // Warp images and their masks
    w = warper_->create(float(warped_image_scale_ * seam_work_aspect_));
    for (size_t i = 0; i < imgs_.size(); ++i)
    {
        Mat_<float> K;
        cameras_[i].K().convertTo(K, CV_32F);
        K(0,0) *= (float)seam_work_aspect_;
        K(0,2) *= (float)seam_work_aspect_;
        K(1,1) *= (float)seam_work_aspect_;
        K(1,2) *= (float)seam_work_aspect_;

        corners[i] = w->warp(seam_est_imgs_[i], K, cameras_[i].R, INTER_LINEAR, BORDER_REFLECT, images_warped[i]);
        //qDebug()<<"0000"<<images_warped[i].cols;
        //cv::Mat temp =images_warped[i].clone();

        cv::waitKey(0);
        sizes[i] = images_warped[i].size();
        //qDebug()<<"Sizes cor "<<sizes[i].width;

        //    cv::Point  p = cv::Point(corners[i].x,corners[i].y);
        //    corners_.push_back(p);

        w->warp(masks[i], K, cameras_[i].R, INTER_NEAREST, BORDER_CONSTANT, masks_warped[i]);
    }

    vector<Mat> images_warped_f(imgs_.size());
    for (size_t i = 0; i < imgs_.size(); ++i)
    {
        images_warped[i].convertTo(images_warped_f[i], CV_32F);
    }

    LOGLN("Warping images, time: " << ((getTickCount() - t) / getTickFrequency()) << " sec");

    // Find seams
    exposure_comp_->feed(corners, images_warped, masks_warped);
    seam_finder_->find(images_warped_f, corners, masks_warped);

    // Release unused memory
    seam_est_imgs_.clear();
    images_warped.clear();
    images_warped_f.clear();
    masks.clear();

    LOGLN("Compositing...");
#if ENABLE_LOG
    t = getTickCount();
#endif

    Mat img_warped, img_warped_s;
    Mat dilated_mask, seam_mask, mask, mask_warped;

    //double compose_seam_aspect = 1;
    double compose_work_aspect = 1;
    bool is_blender_prepared = false;

    double compose_scale = 1;
    bool is_compose_scale_set = false;

    Mat full_img, img;
    for (size_t img_idx = 0; img_idx < imgs_.size(); ++img_idx)
    {
        LOGLN("Compositing image #" << indices_[img_idx] + 1);

        // Read image and resize it if necessary
        full_img = imgs_[img_idx];


        if (!is_compose_scale_set)
        {
            if (compose_resol_ > 0)
                compose_scale = min(1.0, sqrt(compose_resol_ * 1e6 / full_img.size().area()));
            is_compose_scale_set = true;

            // Compute relative scales
            //compose_seam_aspect = compose_scale / seam_scale_;
            compose_work_aspect = compose_scale / work_scale_;

            // Update warped image scale
            warped_image_scale_ *= static_cast<float>(compose_work_aspect);
            w = warper_->create((float)warped_image_scale_);

            // Update corners and sizes
            for (size_t i = 0; i < imgs_.size(); ++i)
            {
                // Update intrinsics
                cameras_[i].focal *= compose_work_aspect;
                cameras_[i].ppx *= compose_work_aspect;
                cameras_[i].ppy *= compose_work_aspect;

                // Update corner and size
                Size sz = full_img_sizes_[i];
                if (std::abs(compose_scale - 1) > 1e-1)
                {
                    sz.width = cvRound(full_img_sizes_[i].width * compose_scale);
                    sz.height = cvRound(full_img_sizes_[i].height * compose_scale);
                }

                Mat K;
                cameras_[i].K().convertTo(K, CV_32F);
                Rect roi = w->warpRoi(sz, K, cameras_[i].R);
                //qDebug()<<"ROI w = "<<roi.width;
                corners[i] = roi.tl();
                sizes[i] = roi.size();
            }
        }
        if (std::abs(compose_scale - 1) > 1e-1)
            resize(full_img, img, Size(), compose_scale, compose_scale);
        else
            img = full_img;

        full_img.release();
        Size img_size = img.size();

        Mat K;
        cameras_[img_idx].K().convertTo(K, CV_32F);

        // Warp the current image
        w->warp(img, K, cameras_[img_idx].R, INTER_LINEAR, BORDER_REFLECT, img_warped);


        // Warp the current image mask
        img_warp.push_back(img_warped);
        Ks.push_back(K);
        cameras_s.push_back(cameras_[img_idx]);
        mask.create(img_size, CV_8U);
        mask.setTo(Scalar::all(255));

        w->warp(mask, K, cameras_[img_idx].R, INTER_NEAREST, BORDER_CONSTANT, mask_warped);



        // Compensate exposure
        exposure_comp_->apply((int)img_idx, corners[img_idx], img_warped, mask_warped);

        nodilate_mask.push_back(mask_warped);
        //cv::imshow(QString::number(img_idx).toStdString()+"di",mask_warped);

        img_warped.convertTo(img_warped_s, CV_16S);
        img_warped.release();
        img.release();
        mask.release();

        // Make sure seam mask has proper size
        dilate(masks_warped[img_idx], dilated_mask, Mat());

        resize(dilated_mask, seam_mask, mask_warped.size());

        Mat mask_warpeds = seam_mask & mask_warped;

        //Ptr<detail::Blender> blender_x;//= new Ptr<detail::Blender>;
        if (!is_blender_prepared)
        {
            blender_->prepare(corners, sizes);
            is_blender_prepared = true;
        }
        // Blend the current image
        blender_->feed(img_warped_s, mask_warpeds, corners[img_idx]);
        //dilate_mask.push_back(mask_warpeds);

    }

    Mat result, result_mask;


    blender_->blend(result, result_mask);

    LOGLN("Compositing, time: " << ((getTickCount() - t) / getTickFrequency()) << " sec");

    // Preliminary result is in CV_16SC3 format, but all values are in [0,255] range,
    // so convert it to avoid user confusing
    result.convertTo(pano_, CV_8U);
    //qDebug()<<"pano_ "<<pano_.cols<<pano_.rows;
    for(int i=0;i<4;i++)
    {
        int xscal = result.cols/sizes[i].width;
        int yscal = result.rows/sizes[i].height;
        int x = corners[i].x*xscal;
        int y = corners[i].y*yscal;
        cv::Point p = cv::Point(x,y);
        corners_.push_back(p);
    }
//    cv::Point  p = cv::Point(corners[i].x,corners[i].y);
//    corners_.push_back(p);

    return OK;
}

Stitch::Status Stitch::composePanorama3(InputArray otherimages ,std::vector<Mat> &img_warp, vector<Mat> &Ks, std::vector<detail::CameraParams> &cameras_s)
{
    return composePanorama3(vector<Mat>(),otherimages,img_warp,Ks,cameras_s);
}

Stitch::Status Stitch::composePanorama3(InputArray images,InputArray otherimages ,std::vector<Mat> &img_warp, vector<Mat> &Ks, std::vector<detail::CameraParams> &cameras_s)
{
    LOGLN("Warping images (auxiliary)... ");
    vector<Mat> imgs;
    vector<Mat> others;
    images.getMatVector(imgs);
    otherimages.getMatVector(others);
    if (!imgs.empty())
    {

        CV_Assert(imgs.size() == imgs_.size());
        Mat img;
        seam_est_imgs_.resize(imgs.size());

        for (size_t i = 0; i < imgs.size(); ++i)
        {
            imgs_[i] = imgs[i];
            resize(imgs[i], img, Size(), seam_scale_, seam_scale_);
            seam_est_imgs_[i] = img.clone();
        }

        vector<Mat> seam_est_imgs_subset;
        vector<Mat> imgs_subset;

        for (size_t i = 0; i < indices_.size(); ++i)
        {
            imgs_subset.push_back(imgs_[indices_[i]]);
            seam_est_imgs_subset.push_back(seam_est_imgs_[indices_[i]]);
        }

        seam_est_imgs_ = seam_est_imgs_subset;
        imgs_ = imgs_subset;
    }

    //Mat &pano_ = pano.getMatRef();


#if ENABLE_LOG
    int64 t = getTickCount();
#endif
    qDebug()<<"003";
    vector<Point> corners(imgs_.size());
    vector<Mat> masks_warped(imgs_.size());
    vector<Mat> images_warped(imgs_.size());
    vector<Size> sizes(imgs_.size());
    vector<Mat> masks(imgs_.size());

    // Prepare image masks
    for (size_t i = 0; i < imgs_.size(); ++i)
    {
        masks[i].create(seam_est_imgs_[i].size(), CV_8U);
        masks[i].setTo(Scalar::all(255));
    }

    // Warp images and their masks
    w = warper_->create(float(warped_image_scale_ * seam_work_aspect_));
    for (size_t i = 0; i < imgs_.size(); ++i)
    {
        Mat_<float> K;
        cameras_[i].K().convertTo(K, CV_32F);
        K(0,0) *= (float)seam_work_aspect_;
        K(0,2) *= (float)seam_work_aspect_;
        K(1,1) *= (float)seam_work_aspect_;
        K(1,2) *= (float)seam_work_aspect_;

        corners[i] = w->warp(others[i], Ks[i], cameras_s[i].R, INTER_LINEAR, BORDER_REFLECT, images_warped[i]);

        sizes[i] = images_warped[i].size();

        w->warp(masks[i], Ks[i], cameras_s[i].R, INTER_NEAREST, BORDER_CONSTANT, masks_warped[i]);
        img_warp.push_back(images_warped[i]);
    }

    return OK;

}


Stitch::Status Stitch::stitch2(InputArray images, OutputArray pano, std::vector<cv::Mat> &img_warp, std::vector<cv::Mat> &nodilate_mask,vector<Point> &corners_,std::vector<cv::Mat> &Ks,std::vector<detail::CameraParams> &cameras_s)
{

    Status status = estimateTransform(images);
    if (status != OK)
        return status;
    return composePanorama2(pano,img_warp, nodilate_mask,corners_,Ks,cameras_s);
}

Stitch::Status Stitch::stitch3(InputArray images,InputArray otherimages ,std::vector<Mat> &img_warp, vector<Mat> &Ks, std::vector<detail::CameraParams> &cameras_s)
{
    Status status = estimateTransform(images);
    if (status != OK)
        return status;
    return composePanorama3(images,otherimages,img_warp,Ks,cameras_s);
}

Stitch::Status Stitch::matchImages()
{
    if ((int)imgs_.size() < 2)
    {
        LOGLN("Need more images");
        return ERR_NEED_MORE_IMGS;
    }
    work_scale_ = 1;
    seam_work_aspect_ = 1;
    seam_scale_ = 1;
    bool is_work_scale_set = false;
    bool is_seam_scale_set = false;
    Mat full_img, img;
    features_.resize(imgs_.size());
    seam_est_imgs_.resize(imgs_.size());
    full_img_sizes_.resize(imgs_.size());

    LOGLN("Finding features...");
#if ENABLE_LOG
    int64 t = getTickCount();
#endif

    for (size_t i = 0; i < imgs_.size(); ++i)
    {
        full_img = imgs_[i];
        full_img_sizes_[i] = full_img.size();

        if (registr_resol_ < 0)
        {
            img = full_img;
            work_scale_ = 1;
            is_work_scale_set = true;
        }
        else
        {
            if (!is_work_scale_set)
            {
                work_scale_ = min(1.0, sqrt(registr_resol_ * 1e6 / full_img.size().area()));
                is_work_scale_set = true;
            }
            resize(full_img, img, Size(), work_scale_, work_scale_);
        }
        if (!is_seam_scale_set)
        {
            seam_scale_ = min(1.0, sqrt(seam_est_resol_ * 1e6 / full_img.size().area()));
            seam_work_aspect_ = seam_scale_ / work_scale_;
            is_seam_scale_set = true;
        }

        if (rois_.empty())
            (*features_finder_)(img, features_[i]);
        else
        {
            vector<Rect> rois(rois_[i].size());
            for (size_t j = 0; j < rois_[i].size(); ++j)
            {
                Point tl(cvRound(rois_[i][j].x * work_scale_), cvRound(rois_[i][j].y * work_scale_));
                Point br(cvRound(rois_[i][j].br().x * work_scale_), cvRound(rois_[i][j].br().y * work_scale_));
                rois[j] = Rect(tl, br);
            }
            (*features_finder_)(img, features_[i], rois);
        }
        features_[i].img_idx = (int)i;
        LOGLN("Features in image #" << i+1 << ": " << features_[i].keypoints.size());

        resize(full_img, img, Size(), seam_scale_, seam_scale_);
        seam_est_imgs_[i] = img.clone();
    }

    // Do it to save memory
    features_finder_->collectGarbage();
    full_img.release();
    img.release();

    LOGLN("Finding features, time: " << ((getTickCount() - t) / getTickFrequency()) << " sec");

    LOG("Pairwise matching");
#if ENABLE_LOG
    t = getTickCount();
#endif
    qDebug()<<"0011";
    qDebug()<<features_.size()<<pairwise_matches_.size();//<<matching_mask_.size();
    while(pairwise_matches_.size()==0)
    {
        (*features_matcher_)(features_, pairwise_matches_, matching_mask_);
    }

    features_matcher_->collectGarbage();

    LOGLN("Pairwise matching, time: " << ((getTickCount() - t) / getTickFrequency()) << " sec");

    // Leave only images we are sure are from the same panorama
    indices_ = detail::leaveBiggestComponent(features_, pairwise_matches_, (float)conf_thresh_);
    vector<Mat> seam_est_imgs_subset;
    vector<Mat> imgs_subset;
    vector<Size> full_img_sizes_subset;
    for (size_t i = 0; i < indices_.size(); ++i)
    {
        imgs_subset.push_back(imgs_[indices_[i]]);
        seam_est_imgs_subset.push_back(seam_est_imgs_[indices_[i]]);
        full_img_sizes_subset.push_back(full_img_sizes_[indices_[i]]);
    }

    seam_est_imgs_ = seam_est_imgs_subset;
    imgs_ = imgs_subset;
    full_img_sizes_ = full_img_sizes_subset;

    if ((int)imgs_.size() < 2)
    {
        LOGLN("Need more images");
        return ERR_NEED_MORE_IMGS;
    }

    return OK;
}


void Stitch::estimateCameraParams()
{
    detail::HomographyBasedEstimator estimator;
    estimator(features_, pairwise_matches_, cameras_);

    for (size_t i = 0; i < cameras_.size(); ++i)
    {
        Mat R;
        cameras_[i].R.convertTo(R, CV_32F);
        cameras_[i].R = R;
        LOGLN("Initial intrinsic parameters #" << indices_[i] + 1 << ":\n " << cameras_[i].K());
    }

    bundle_adjuster_->setConfThresh(conf_thresh_);
    (*bundle_adjuster_)(features_, pairwise_matches_, cameras_);

    // Find median focal length and use it as final image scale
    vector<double> focals;
    for (size_t i = 0; i < cameras_.size(); ++i)
    {
        LOGLN("Camera #" << indices_[i] + 1 << ":\n" << cameras_[i].K());
        focals.push_back(cameras_[i].focal);
    }

    std::sort(focals.begin(), focals.end());
    if (focals.size() % 2 == 1)
        warped_image_scale_ = static_cast<float>(focals[focals.size() / 2]);
    else
        warped_image_scale_ = static_cast<float>(focals[focals.size() / 2 - 1] + focals[focals.size() / 2]) * 0.5f;

    if (do_wave_correct_)
    {
        vector<Mat> rmats;
        for (size_t i = 0; i < cameras_.size(); ++i)
            rmats.push_back(cameras_[i].R);
        detail::waveCorrect(rmats, wave_correct_kind_);
        for (size_t i = 0; i < cameras_.size(); ++i)
            cameras_[i].R = rmats[i];
    }
}

} // namespace cv
