//class to hold the openMP sharpening functionality
#include <cmath>
#include <vector>
#include <opencv2/opencv.hpp>


class OpenMP_Sharpener {
	cv::Mat* image_;
	std::vector<std::vector<double>> kernel_ = { {0, -1, 0}, {-1, 4, -1}, {0, -1, 0} };
	
    
public:
	void sharpen(cv::Mat& inputImg, cv::Mat& outputImg);
};