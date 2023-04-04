#include <cmath>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>

//class to hold the functionality for openMP img processing

class openMP_imgProcessor {
    std::vector<std::vector<double>> LapKernel_ = {
    {0, 0, 1, 0, 0},
    {0, 1, 2, 1, 0},
    {1, 2, -7, 2, 1},
    {0, 1, 2, 1, 0},
    {0, 0, 1, 0, 0}
    };

public:
ImgProcessor_OMP() {
        omp_set_num_threads(15);
    }
    void sharpenImg(cv::Mat& input);
    void brightenImg(cv::Mat& input, int brightnessLvl);
    //saturateImg
};
