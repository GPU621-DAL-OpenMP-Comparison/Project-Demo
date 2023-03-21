#include "openMP_sharpening.h"

void OpenMP_Sharpener::sharpen(cv::Mat& inputImg, cv::Mat& outputImg) {
    // Convert the input image to grayscale
    cv::Mat grayscale;
    cv::cvtColor(inputImg, grayscale, cv::COLOR_BGR2GRAY);

    // Create a copy of the input image
    outputImg = inputImg.clone();

    // Apply the kernel to the grayscale image
    #pragma omp parallel for
    for (int x = 1; x < inputImg.cols - 1; x++) {
        for (int y = 1; y < inputImg.rows - 1; y++) {
            double sum = 0.0;
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    sum += grayscale.at<uchar>(y + j, x + i) * kernel_[i + 1][j + 1];
                }
            }
            for (int c = 0; c < 3; c++) {
                #pragma omp atomic
                outputImg.at<cv::Vec3b>(y, x)[c] = cv::saturate_cast<uchar>(outputImg.at<cv::Vec3b>(y, x)[c] + sum);
            }
        }
    }
}