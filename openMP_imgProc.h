#include <cmath>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>

//class to hold the functionality for openMP img processing

class openMP_imgProcessor {
    std::vector<std::vector<double>> LapKernel_ = { {0, -1, 0}, {-1, 4, -1}, {0, -1, 0} };

public:
    void sharpenImg(cv::Mat& input);
    void brightenImg(cv::Mat& input, int brightnessLvl);
    //saturateImg
};