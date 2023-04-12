#pragma once
#include <cmath>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <omp.h>
#include <opencv2/imgproc.hpp>
#include <algorithm>
#include <tbb/tbb.h>
//
class tbb_imgProcessor {
    std::vector<std::vector<double>> LapKernel_ = {
    {0, 0, 1},
    {0, 1, 2},
    {1, 2, -7}
    };

public:
    tbb_imgProcessor() { }
    void sharpenImg(cv::Mat& image);
    void brightenImg(cv::Mat& image, int brightnessLvl);
    void saturateImg(cv::Mat& image, double saturationLvl);
};