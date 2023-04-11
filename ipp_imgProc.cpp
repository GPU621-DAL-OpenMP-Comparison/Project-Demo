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
    // Measure the time it takes to perform the sharpening operation using IPP
    high_resolution_clock::time_point t1 = high_resolution_clock::now();

    // Set up the sharpening filter
    IppiMaskSize maskSize = ippMskSize3x3;
    //  Ipp8u kernel[] = { -1, -1, -1, -1, 9, -1, -1, -1, -1 };

      // Get the required buffer size
    int bufferSize = 0;
    ippiFilterSharpenBorderGetBufferSize({ width_, height_ }, maskSize, ipp8u, ipp8u, 3, &bufferSize);

    // Allocate memory for the temporary buffer
    Ipp8u* pBuffer = ippsMalloc_8u(bufferSize);


    IppiBorderType borderType = ippBorderRepl;
    Ipp8u borderValue[] = { 0, 0, 0 };
    IppStatus status = ippiFilterSharpenBorder_8u_C3R(gray8Img_.data, gray8Img_.step, outImg_.data, outImg_.step,
        { width_, height_ }, maskSize, borderType, borderValue, pBuffer);

    // Free the temporary buffer
    ippsFree(pBuffer);

    // Check for errors
    if (status != ippStsNoErr) {
        throw std::runtime_error("IPP error");
    }
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration_ = duration_cast<milliseconds>(t2 - t1).count();

    // Print the processing time in milliseconds
    std::cout << "Sharpening time: " << duration_ << " milliseconds" << std::endl;

    // Resize the output image for display
    cv::resize(outImg_, displayOutImg_, cv::Size(), displayScale_, displayScale_);

    // Display the output image for 5 seconds
    displayOutputImage(5000);

    // Save the output image to file
    saveOutputImage("sharpend_image.jpg");
}

void IppImgProc::brighten(int brightness, int scaleFactor)
{
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
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

    // Resize the output image for display
    cv::resize(outImg_, displayOutImg_, cv::Size(), displayScale_, displayScale_);

    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration_ = duration_cast<milliseconds>(t2 - t1).count();

    // Print the processing time in milliseconds
    std::cout << "Brightening time: " << duration_ << " milliseconds " << std::endl;

    // Display the output image for 5 seconds
    displayOutputImage(5000);

    // Save the output image to file
    saveOutputImage("brightened_image.jpg");
}


void IppImgProc::adjustSaturation(Ipp8u saturation)
{
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
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

    // Resize the output image for display
    cv::resize(outImg_, displayOutImg_, cv::Size(), displayScale_, displayScale_);

    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration_ = duration_cast<milliseconds>(t2 - t1).count();

    // Print the processing time in milliseconds
    std::cout << "Saturation time: " << duration_ << " milliseconds " << std::endl;

    // Display the output image for 5 seconds
    displayOutputImage(5000);

    // Save the output image to file
    saveOutputImage("Saturation.jpg");
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
    cv::namedWindow("Output Image", cv::WINDOW_NORMAL);
    cv::imshow("Output Image", displayOutImg_);
    cv::waitKey(duration_ms);
    cv::destroyWindow("Output Image");
}

void IppImgProc::saveOutputImage(const std::string& filename)
{
    cv::imwrite(filename, outImg_);
}

long long IppImgProc::getProcessingTime() const
{
    return duration_;
}
