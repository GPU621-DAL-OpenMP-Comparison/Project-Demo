#include "openMP_imgProc.h"

void openMP_imgProcessor::sharpenImg(cv::Mat& input, cv::Mat& output) {
	sharpener_.sharpen(input, output);
}