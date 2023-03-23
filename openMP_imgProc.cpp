#include "openMP_imgProc.h"

void openMP_imgProcessor::sharpenImg(cv::Mat& input) {
    // Convert the input image to grayscale
    cv::Mat grayscale;
    cv::cvtColor(input, grayscale, cv::COLOR_BGR2GRAY);


    // Apply the kernel to the grayscale image
    #pragma omp parallel for
    for (int x = 1; x < input.cols - 1; x++) {
        for (int y = 1; y < input.rows - 1; y++) {
            double sum = 0.0;
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    sum += grayscale.at<uchar>(y + j, x + i) * LapKernel_[i + 1][j + 1];
                }
            }
            for (int c = 0; c < 3; c++) {
                input.at<cv::Vec3b>(y, x)[c] = cv::saturate_cast<uchar>(input.at<cv::Vec3b>(y, x)[c] + sum * 0.15);
            }
        }
    }
}

void openMP_imgProcessor::brightenImg(cv::Mat& image, int brightnessLvl) {
    int width = image.cols;
    int height = image.rows;
    int channels = image.channels();

    #pragma omp parallel for
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            for (int c = 0; c < channels; c++) {
                uchar& pixel = image.at<cv::Vec3b>(row, col)[c];
                pixel = cv::saturate_cast<uchar>(pixel + brightnessLvl);
            }
        }
    }
}
