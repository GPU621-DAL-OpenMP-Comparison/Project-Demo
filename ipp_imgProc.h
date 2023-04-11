#ifndef IPP_IMGPROC_H
#define IPP_IMGPROC_H

#include <ipp.h>
#include <opencv2/opencv.hpp>
#include <chrono>
#include <stdexcept>

class IppImgProc
{
public:
	IppImgProc() {}
	// Constructor that loads an image from file
	explicit IppImgProc(const std::string& filename);

	// Process the loaded image using IPP
	void sharpening();

	void brighten(int brightness, int scaleFactor);

	// Display the input image for a given duration in milliseconds (default is 5000)
	void displayInputImage(int duration_ms = 5000);

	// Display the output image for a given duration in milliseconds (default is 5000)
	void displayOutputImage(int duration_ms = 5000);

	void adjustSaturation(Ipp8u saturation);

	// Save the output image to file
	void saveOutputImage(const std::string& filename);

	// Get the processing time in milliseconds
	long long getProcessingTime() const;

private:
	cv::Mat img_; // Original image
	cv::Mat gray8Img_; // Image converted to 8-bit format
	cv::Mat outImg_; // Output image
	cv::Mat displayOutImg_; // Output image scaled for display
	double displayScale_; // Scale factor for displaying the images
	int width_, height_; // Image dimensions
	int numThreads_; // Number of threads to be used
	long long duration_; // Processing time in milliseconds
};

#endif // IPP_IMGPROC_H
