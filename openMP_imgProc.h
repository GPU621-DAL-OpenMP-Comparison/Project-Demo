#pragma once
#include <cmath>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <omp.h>
#include <opencv2/imgproc.hpp>
#include <algorithm>
#include "Timer.h"

//class to hold the functionality for openMP img processing

class openMP_imgProcessor {
    //laplacian kernel used in sharpening
    std::vector<std::vector<double>> LapKernel_ = {
    {0, 0, 1},
    {0, 1, 2},
    {1, 2, -7}
    };

public:
    openMP_imgProcessor() { }
    void sharpenImg(cv::Mat& image);
    void brightenImg(cv::Mat& image, int brightnessLvl);
    void saturateImg(cv::Mat& image, double saturationLvl);
};
