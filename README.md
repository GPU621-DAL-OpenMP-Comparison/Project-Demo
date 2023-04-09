![Logo](https://i.ibb.co/vJLRX9Q/Image-Processing-GPU621-group3-header.png)

# Image processing using Intel's DAAL vs. OpenMP

### Introduction:

In this project we will be comparing Intel's Data Analytics Acceleration Library and OpenMP API to Optimizing Image Processing using Parallel computing and Vectorization. We designed three tasks fot this project Image Sharpening, brightness and Color Saturation. For each task the performance of both Libraries is recorded and compared in the main file.

## Authors

- [@Olive251](https://www.github.com/Olive251)
- [@jlecesne](https://www.github.com/jlecesne)
- [@ArazTey](https://www.github.com/ArazTey)

## Badges this will be updated

[![MIT License](https://img.shields.io/badge/License-MIT-green.svg)](https://choosealicense.com/licenses/mit/)
[![GPLv3 License](https://img.shields.io/badge/License-GPL%20v3-yellow.svg)](https://opensource.org/licenses/)
[![AGPL License](https://img.shields.io/badge/license-AGPL-blue.svg)](http://www.gnu.org/licenses/agpl-3.0)

## Appendix

### Data Analytics Acceleration Library Overview:

Intel's Data Analytics Library offers a robust collection of tools and algorithms that can assist programmers in building high-performance applications tailored for Intel chips. Data Analytics Library offers functionalities for:

- Parallel computing.
- Vectorization.
- Machine learning.
- Graph analytics.
- etc.

### OpenMP API Overview:

OpenMP (Open Multi-Processing) is a robust API for multi-platform shared-memory multi-processing programming in C and C++. It provides developers with compiler directives, library routines, and environment variables to use when writing parallel programs that can run on multiple processor cores. Some of the functionalities provided by OpenMP are as follows:

- Parallel computing
- Vectorization
- Thread management
- Memory management
- Loop scheduling
- etc.

### OpenMP Implementation

Utilizing the OpenCV library to modify any image with the Mat class which allows us to modify the image pixel-by-pixel as a n-dimensional array. Parrelelizing this operation with OpenMP was incredibly simple and easy to implement. All that was required was to set the parallel for declaration and some testing to determine the most number of threads that would be most efficient for our operation.

```
void openMP_imgProcessor::sharpenImg(cv::Mat& image) {
    //supressing OpenCV messages
    std::streambuf* coutbuf = std::cout.rdbuf();
    std::cout.rdbuf(nullptr);
    // Convert the image to grayscale
    cv::Mat grayscale;
    cv::cvtColor(image, grayscale, cv::COLOR_BGR2GRAY);

    // Apply the kernel to the grayscale image
    //finds areas with quick jumps from dark to light, increases contrast there
    #pragma omp parallel for
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
```

As can be seen from the sharpenImg() function above, OpenMP excels most when it comes to ease of implementation. The #pragma definition is all that is needed to convert this from a serial operation to a parallel one.

### TBB Implementation

### Run-Time Comparison

## Documentation

[Intel's DAAL Developer Guide and Reference](https://www.intel.com/content/www/us/en/develop/documentation/onedal-developer-guide-and-reference/top.html)
