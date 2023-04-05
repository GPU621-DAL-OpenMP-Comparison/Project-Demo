#include "openMP_imgProc.h"

void openMP_imgProcessor::sharpenImg(cv::Mat& image) {
    //supressing OpenCV messages
    std::streambuf* coutbuf = std::cout.rdbuf();
    std::cout.rdbuf(nullptr);
    // Convert the image to grayscale
    cv::Mat grayscale;
    cv::cvtColor(image, grayscale, cv::COLOR_BGR2GRAY);


    // Apply the kernel to the grayscale image
    #pragma omp parallel for
    for (int x = 1; x < image.cols - 1; x++) {
        for (int y = 1; y < image.rows - 1; y++) {
            double sum = 0.0;
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    sum += grayscale.at<uchar>(y + j, x + i) * LapKernel_[i + 1][j + 1];
                }
            }
            for (int c = 0; c < 3; c++) {
                image.at<cv::Vec3b>(y, x)[c] = cv::saturate_cast<uchar>(image.at<cv::Vec3b>(y, x)[c] + sum * 0.99);
            }
        }
    }

    //stop supressing
    std::cout.rdbuf(coutbuf);
}

void openMP_imgProcessor::brightenImg(cv::Mat& image, int brightnessLvl) {
    //supressing OpenCV messages
    std::streambuf* coutbuf = std::cout.rdbuf();
    std::cout.rdbuf(nullptr);

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

    //stop supressing
    std::cout.rdbuf(coutbuf);
}
