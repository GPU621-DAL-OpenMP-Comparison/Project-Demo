#include <cmath>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <omp.h>

//class to hold the functionality for openMP img processing

class openMP_imgProcessor {
    //laplacian kernel used in sharpening
    std::vector<std::vector<double>> LapKernel_ = {
    {0, 0, 1, 0, 0},
    {0, 1, 2, 1, 0},
    {1, 2, -7, 2, 1},
    {0, 1, 2, 1, 0},
    {0, 0, 1, 0, 0}
    };

public:
openMP_imgProcessor() {
        omp_set_num_threads(15);
    }
    void sharpenImg(cv::Mat& image);
    void brightenImg(cv::Mat& image, int brightnessLvl);
};
