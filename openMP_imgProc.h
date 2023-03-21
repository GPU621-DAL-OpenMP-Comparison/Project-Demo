#include "openMP_sharpening.h"

//class to hold the functionality for openMP img processing

class openMP_imgProcessor {
    OpenMP_Sharpener sharpener_;
    //saturator
    //brightener
    //timeKeeping
    //processingFunct
public:
    void sharpenImg(cv::Mat& input, cv::Mat& output);
};