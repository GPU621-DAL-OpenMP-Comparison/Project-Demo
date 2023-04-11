#pragma once
#include <cmath>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencv2/imgproc.hpp>

class serial_imgProcessor {
    //laplacian kernel used in sharpening
    std::vector<std::vector<double>> LapKernel_ = {
    {0, 0, 1, 0, 0},
    {0, 1, 2, 1, 0},
    {1, 2, -7, 2, 1},
    {0, 1, 2, 1, 0},
    {0, 0, 1, 0, 0}
    };

public:
    serial_imgProcessor() {}
    void sharpenImg(cv::Mat& image) {
        //supressing OpenCV messages
        std::streambuf* coutbuf = std::cout.rdbuf();
        std::cout.rdbuf(nullptr);
        // Convert the image to grayscale
        cv::Mat grayscale;
        cv::cvtColor(image, grayscale, cv::COLOR_BGR2GRAY);

        // Apply the kernel to the grayscale image
        //finds areas with quick jumps from dark to light, increases contrast there
        for (int x = 1; x < image.cols - 1; x++) {
            for (int y = 1; y < image.rows - 1; y++) {
                double sum = 0.0;
                for (int i = -1; i <= 1; i++) {
                    for (int j = -1; j <= 1; j++) {
                        sum += grayscale.at<uchar>(y + j, x + i) * LapKernel_[i + 1][j + 1];
                    }
                }
                //apply filter
                for (int c = 0; c < 3; c++) {
                    image.at<cv::Vec3b>(y, x)[c] = cv::saturate_cast<uchar>(image.at<cv::Vec3b>(y, x)[c] + sum * 0.99);
                }
            }
        }

        //stop supressing
        std::cout.rdbuf(coutbuf);
    }
    void brightenImg(cv::Mat& image, int brightnessLvl) {
        //supressing OpenCV messages
        std::streambuf* coutbuf = std::cout.rdbuf();
        std::cout.rdbuf(nullptr);

        int width = image.cols;
        int height = image.rows;
        int channels = image.channels();

        for (int row = 0; row < height; row++) {
            for (int col = 0; col < width; col++) {
                for (int c = 0; c < channels; c++) {
                    uchar& pixel = image.at<cv::Vec3b>(row, col)[c];
                    pixel = cv::saturate_cast<uchar>(pixel + brightnessLvl);
                }
            }
        }

        //stop supressing
        std::cout.rdbuf(coutbuf);
    }
    void saturateImg(cv::Mat& image, double saturationLvl) {
        //supressing OpenCV messages
        std::streambuf* coutbuf = std::cout.rdbuf();
        std::cout.rdbuf(nullptr);

        //HSV stands for hue saturation value
        cv::Mat hsv;
        cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);

        for (int y = 0; y < hsv.rows; ++y)
        {
            for (int x = 0; x < hsv.cols; ++x)
            {
                // Get pixel value
                cv::Vec3b color = hsv.at<cv::Vec3b>(cv::Point(x, y));

                // Increase saturation by saturation Lvl color[1] is for saturation 
                color[1] = cv::saturate_cast<uchar>(color[1] * saturationLvl);

                // Set pixel value
                hsv.at<cv::Vec3b>(cv::Point(x, y)) = color;
            }
        }

        cv::cvtColor(hsv, image, cv::COLOR_HSV2BGR);

        std::cout.rdbuf(coutbuf);
    }
};