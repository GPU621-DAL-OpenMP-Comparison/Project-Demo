#include "ipp_imgProc.h"
#include <ipp.h>
#include <opencv2/opencv.hpp>
#include <chrono>
using namespace cv;
using namespace std::chrono;

IppImgProc::IppImgProc(const std::string& filename)
    : img_(cv::imread(filename)), displayScale_(0.3), outImg_(cv::Mat::zeros(img_.size(), img_.type()))
{
    if (img_.empty())
        throw std::runtime_error("Failed to load image");

    img_.convertTo(gray8Img_, CV_8U);
    width_ = gray8Img_.cols;
    height_ = gray8Img_.rows;
    numThreads_ = std::thread::hardware_concurrency();
    ippSetNumThreads(numThreads_);
}

void IppImgProc::sharpening()
{
    // Set up the sharpening filter
    IppiMaskSize maskSize = ippMskSize5x5;

    // Get the required buffer size
    int bufferSize = 0;
    // ippiFilterSharpenBorderGetBufferSize({ width_, height_ }, maskSize, ipp8u, ipp8u, 3, &bufferSize);
    ippiFilterLaplaceBorderGetBufferSize({ width_, height_ }, maskSize, ipp8u, ipp8u, 3, &bufferSize);
    // Allocate memory for the temporary buffer
    Ipp8u* pBuffer = ippsMalloc_8u(bufferSize);

    IppiBorderType borderType = ippBorderConst;
    Ipp8u borderValue[] = { 0, 0, 0 };
    // ippiFilterSharpenBorder_8u_C3R
    IppStatus status = ippiFilterLaplaceBorder_8u_C3R(gray8Img_.data, gray8Img_.step, outImg_.data, outImg_.step,
        { width_, height_ }, maskSize, borderType, borderValue, pBuffer);

    // Free the temporary buffer
    ippsFree(pBuffer);

    // Check for errors
    if (status != ippStsNoErr) {
        throw std::runtime_error("IPP error");
    }

    // Define the blending ratio (0.0 to 1.0)
    double alpha = 0.82;

    // Blend the original image and the output image
    cv::addWeighted(img_, alpha, outImg_, 1 - alpha, 0.0, outImg_);


}

void IppImgProc::brighten(int brightness, int scaleFactor)
{   
    // Create a matrix for the output image
    outImg_ = cv::Mat::zeros(img_.size(), img_.type());

    // Convert the image to 8-bit format
    img_.convertTo(gray8Img_, CV_8U);

    // Get the image dimensions
    width_ = gray8Img_.cols;
    height_ = gray8Img_.rows;

    // Create an IPP image for the input image data
    IppiSize roi = { width_, height_ };
    Ipp8u* pData = gray8Img_.data;
    int step = gray8Img_.step;

    // Set up the brightening parameters
    Ipp8u value[3] = { static_cast<Ipp8u>(brightness), static_cast<Ipp8u>(brightness), static_cast<Ipp8u>(brightness) };
    IppStatus status = ippiAddC_8u_C3RSfs(pData, step, value, outImg_.data, outImg_.step, roi, scaleFactor);

    // Measure the time it takes to perform the brightening operation using IPP

    status = ippiAddC_8u_C3RSfs(pData, step, value, outImg_.data, outImg_.step, roi, scaleFactor);
   

    // Check for errors
    if (status != ippStsNoErr) {
        throw std::runtime_error("IPP error");
    }

}


void IppImgProc::adjustSaturation(Ipp8u saturation)
{
    // Create an IPP image for the input image data
    IppiSize roi = { width_, height_ };
    Ipp8u* pData = gray8Img_.data;
    int step = gray8Img_.step;

    // Convert the image from RGB to HSV color space
    int channels[] = { 0, 1, 2 };
    IppiSize size = { width_, height_ };
    Ipp8u* pSrc = pData;
    int srcStep = step;
    Ipp8u* pDst = pData;
    int dstStep = step;
    ippiRGBToHSV_8u_C3R(pSrc, srcStep, pDst, dstStep, size);

    // Set the saturation value
    Ipp8u* pHSV = pData;
    Ipp8u* pSaturation = &saturation;
    for (int y = 0; y < height_; y++) {
        for (int x = 0; x < width_; x++) {
            Ipp8u* pPixel = pHSV + y * step + x * 3;
            pPixel[1] = (Ipp8u)std::min(pSaturation[0], pPixel[1]);
        }
    }

    // Convert the image back to RGB color space
    pSrc = pData;
    srcStep = step;
    pDst = outImg_.data;
    dstStep = step;
    ippiHSVToRGB_8u_C3R(pSrc, srcStep, pDst, dstStep, size);

}


void IppImgProc::displayInputImage(int duration_ms)
{
  //  Mat displayOutImg;
   // resize(outImg, displayOutImg, Size(), displayScale, displayScale);

    cv::namedWindow("Input Image", cv::WINDOW_NORMAL);
   // cv::resize(img_, cv::Mat(), cv::Size(), displayScale_, displayScale_);
    cv::imshow("Input Image", img_);
    cv::waitKey(duration_ms);
    cv::destroyWindow("Input Image");
}

void IppImgProc::displayOutputImage(int duration_ms)
{
    //supressing OpenCV messages
    std::streambuf* coutbuf = std::cout.rdbuf();
    std::cout.rdbuf(nullptr);

    cv::namedWindow("Output Image - IPP", cv::WINDOW_NORMAL);
    cv::imshow("Output Image - IPP", displayOutImg_);
    cv::waitKey(duration_ms);
    cv::destroyWindow("Output Image - IPP");

    //stop cv message supression
    std::cout.rdbuf(coutbuf);
}

void IppImgProc::saveOutputImage(const std::string& filename)
{
    cv::imwrite(filename, outImg_);
}

long long IppImgProc::getProcessingTime() const
{
    return duration_;
}
