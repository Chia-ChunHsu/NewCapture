#ifndef STITCH
#define STITCH
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv.hpp"

#include "opencv2/stitching/warpers.hpp"
#include "opencv2/stitching/detail/matchers.hpp"
#include "opencv2/stitching/detail/motion_estimators.hpp"
#include "opencv2/stitching/detail/exposure_compensate.hpp"
#include "opencv2/stitching/detail/seam_finders.hpp"
#include "opencv2/stitching/detail/blenders.hpp"
#include "opencv2/stitching/detail/camera.hpp"
//#include "myblender.h"


#include <QDebug>

namespace cv {
class Stitch
{
public:
    enum { ORIG_RESOL = -1 };
    enum Status { OK, ERR_NEED_MORE_IMGS };

    // Creates stitcher with default parameters
    static Stitch createDefault(bool try_use_gpu = false);

    double registrationResol() const { return registr_resol_; }
    void setRegistrationResol(double resol_mpx) { registr_resol_ = resol_mpx; }

    double seamEstimationResol() const { return seam_est_resol_; }
    void setSeamEstimationResol(double resol_mpx) { seam_est_resol_ = resol_mpx; }

    double compositingResol() const { return compose_resol_; }
    void setCompositingResol(double resol_mpx) { compose_resol_ = resol_mpx; }

    double panoConfidenceThresh() const { return conf_thresh_; }
    void setPanoConfidenceThresh(double conf_thresh) { conf_thresh_ = conf_thresh; }

    bool waveCorrection() const { return do_wave_correct_; }
    void setWaveCorrection(bool flag) { do_wave_correct_ = flag; }

    detail::WaveCorrectKind waveCorrectKind() const { return wave_correct_kind_; }
    void setWaveCorrectKind(detail::WaveCorrectKind kind) { wave_correct_kind_ = kind; }

    Ptr<detail::FeaturesFinder> featuresFinder() { return features_finder_; }
    const Ptr<detail::FeaturesFinder> featuresFinder() const { return features_finder_; }
    void setFeaturesFinder(Ptr<detail::FeaturesFinder> features_finder)
        { features_finder_ = features_finder; }

    Ptr<detail::FeaturesMatcher> featuresMatcher() { return features_matcher_; }
    const Ptr<detail::FeaturesMatcher> featuresMatcher() const { return features_matcher_; }
    void setFeaturesMatcher(Ptr<detail::FeaturesMatcher> features_matcher)
        { features_matcher_ = features_matcher; }

    const cv::Mat& matchingMask() const { return matching_mask_; }
    void setMatchingMask(const cv::Mat &mask)
    {
        CV_Assert(mask.type() == CV_8U && mask.cols == mask.rows);
        matching_mask_ = mask.clone();
    }

    Ptr<detail::BundleAdjusterBase> bundleAdjuster() { return bundle_adjuster_; }
    const Ptr<detail::BundleAdjusterBase> bundleAdjuster() const { return bundle_adjuster_; }
    void setBundleAdjuster(Ptr<detail::BundleAdjusterBase> bundle_adjuster)
        { bundle_adjuster_ = bundle_adjuster; }

    Ptr<WarperCreator> warper() { return warper_; }
    const Ptr<WarperCreator> warper() const { return warper_; }
    void setWarper(Ptr<WarperCreator> creator) { warper_ = creator; }

    Ptr<detail::ExposureCompensator> exposureCompensator() { return exposure_comp_; }
    const Ptr<detail::ExposureCompensator> exposureCompensator() const { return exposure_comp_; }
    void setExposureCompensator(Ptr<detail::ExposureCompensator> exposure_comp)
        { exposure_comp_ = exposure_comp; }

    Ptr<detail::SeamFinder> seamFinder() { return seam_finder_; }
    const Ptr<detail::SeamFinder> seamFinder() const { return seam_finder_; }
    void setSeamFinder(Ptr<detail::SeamFinder> seam_finder) { seam_finder_ = seam_finder; }
    Ptr<detail::Blender> myblender() { return blender_; }
    const Ptr<detail::Blender> myblender() const { return blender_; }
    void setBlender(Ptr<detail::Blender> b) { blender_ = b; }

    Status estimateTransform(InputArray images);
    Status estimateTransform(InputArray images, const std::vector<std::vector<Rect> > &rois);

//    Status estimateTransform2(InputArray images);
//    Status estimateTransform2(InputArray images, const std::vector<std::vector<Rect> > &rois);



    Status composePanorama2(OutputArray pano, std::vector<cv::Mat> &img_warp,std::vector<cv::Mat> &nodilate_mask,std::vector<cv::Point> &corners,vector<Mat> &Ks,std::vector<detail::CameraParams> &cameras_s);
    Status composePanorama2(InputArray images, OutputArray pano, std::vector<cv::Mat> &img_warp,std::vector<cv::Mat> &nodilate_mask,std::vector<cv::Point> &corners,vector<Mat> &Ks,std::vector<detail::CameraParams> &cameras_s);

    Status composePanorama3(InputArray otherimages ,std::vector<Mat> &img_warp, vector<Mat> &Ks, std::vector<detail::CameraParams> &cameras_s);
    Status composePanorama3(InputArray images,InputArray otherimages ,std::vector<Mat> &img_warp, vector<Mat> &Ks, std::vector<detail::CameraParams> &cameras_s);


    Status stitch2(InputArray images,OutputArray pano, std::vector<cv::Mat> &img_warp,std::vector<cv::Mat> &nodilate_mask  , std::vector<cv::Point> &corners,vector<cv::Mat> &Ks,std::vector<detail::CameraParams> &cameras_s);

    Status stitch3(InputArray images,InputArray otherimages, std::vector<cv::Mat> &img_warp,vector<cv::Mat> &Ks,std::vector<detail::CameraParams> &cameras_s);


    std::vector<int> component() const { return indices_; }
    std::vector<detail::CameraParams> cameras() const { return cameras_; }
    double workScale() const { return work_scale_; }

private:
    Stitch() {}
    Ptr<detail::RotationWarper> w;
    Ptr<detail::RotationWarper> w1;

    Status matchImages();
    void estimateCameraParams();

    double registr_resol_;
    double seam_est_resol_;
    double compose_resol_;
    double conf_thresh_;
    Ptr<detail::FeaturesFinder> features_finder_;
    Ptr<detail::FeaturesMatcher> features_matcher_;
    cv::Mat matching_mask_;
    Ptr<detail::BundleAdjusterBase> bundle_adjuster_;
    bool do_wave_correct_;
    detail::WaveCorrectKind wave_correct_kind_;
    Ptr<WarperCreator> warper_;
    Ptr<detail::ExposureCompensator> exposure_comp_;
    Ptr<detail::SeamFinder> seam_finder_;
    Ptr<detail::Blender> blender_;
    //Ptr<detail::Blender> bledner2;

    std::vector<cv::Mat> imgs_;
    std::vector<cv::Mat> imgs_2;
    std::vector<std::vector<cv::Rect> > rois_;
    std::vector<std::vector<cv::Rect> > rois_2;
    std::vector<cv::Size> full_img_sizes_;
    std::vector<detail::ImageFeatures> features_;
    std::vector<detail::MatchesInfo> pairwise_matches_;
    std::vector<cv::Mat> seam_est_imgs_;
    std::vector<cv::Mat> seam_est_imgs_2;
    std::vector<int> indices_;
    std::vector<detail::CameraParams> cameras_;
    double work_scale_;
    double seam_scale_;
    double seam_work_aspect_;
    double warped_image_scale_;

};

}




#endif // STITCH

