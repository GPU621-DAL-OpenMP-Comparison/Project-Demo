![Logo](https://i.ibb.co/vJLRX9Q/Image-Processing-GPU621-group3-header.png)

# Image processing using Intel's DAAL vs. OpenMP

### Introduction:

In this project we will be comparing Intel's Integrated Performance Primitives and OpenMP API to optimize image processing using parallel computing and vectorization. We selected three tasks for this project: image sharpening, brightening, and adjusting the saturation of an image. The run-time of each task is recorded and able to be compared by our demo program. We will also be comparing the implementation for each toolset we utilize.

In order to be able to more easily engage with image files, we will be utilizing the OpenCV library, leaning especially on the Mat class therein. The Mat class allows us to access the image as a n-dimensional array. Furthermore with our implementation we are able to rely on our parellelization choices instead of that built into the OpenCV library.

We had originally intended to use Intel's Data Analytics Acceleration Library, but as work progressed on the project we realized that the library was not well suited to our needs. Intel's oneAPI DAL (Data Analytics Library) was our chosen library to complete this project. However, due to changes to our team and the nature of the project we wanted to pursue, we decided to use Intel IPP (Integrated Performance Primitives) instead of DAL. DAL is a robust and capable library for data analytics and machine learning. It is designed with linear algebra and statistical operations in mind. DAL offers parallelization capabilities but is not explicitly optimized for image processing operations. Image processing involves working with large arrays of pixel data, which requires specialized data structures and memory access patterns. DAL's focus on linear algebra and statistical operations may not be well-suited to these procedures. Other libraries specifically designed for image processing, such as OpenCV, offer better performance because they can take advantage of GPUs' parallel processing for Image processing. Also, they offer a more significant number of features than DAL. Libraries specifically designed for image processing, such as OpenCV, can take advantage of GPUs' parallel processing capabilities for image processing.

Incorporating Intel's oneAPI DAL into image processing applications is only efficient when massive datasets of Image Data need processing or very computationally intensive operations such as image compression, dimensionality reduction, or feature extraction is required. DAL's optimized algorithms provide a performance advantage over other libraries when performing heavy linear algebra and statistical functions. DAL also offers excellent flexibility for creating custom algorithms. 

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

In this class declaration for what will hold the OpenMP parallelized functionality we include a Laplacian kernel which will be applied to the sample images in order to sharpen details. How this is achieved is essentially highlighting areas on a greyscale version of the orignal image where the picture goes quickly from light to dark, and applies that highlight to the same locations on the original image. For those familiar with Laplacian filters, you may notice that ours is very much non-standard. Through testing we determined that this was the filter that created the best results across all use cases, though it should be noted that the scaling done when applying the highlight in the sharpening operation that is currently set to 0.99 in the code below would need to be reduced significantly if applied to artist illustrations.

The sharpening process is the most interesting of the processes as it provides a similar effect to an artist adding white lines around the outlines of an illustration, which is often a stylistic choice made in character illustration. 

```
class openMP_imgProcessor {
    //laplacian kernel used in sharpening
    std::vector<std::vector<double>> LapKernel_ = {
    {0, 0, 1},
    {0, 1, 2},
    {1, 2, -7}
    };

public:
    openMP_imgProcessor() { }
    void sharpenImg(cv::Mat& image);
    void brightenImg(cv::Mat& image, int brightnessLvl);
    void saturateImg(cv::Mat& image, double saturationLvl);
};
```

As can be seen from the sharpenImg() function above, OpenMP excels most when it comes to ease of implementation. The #pragma definition is all that is needed to convert this from a serial operation to a parallel one.

### TBB Implementation

The TBB implementation was relatively simple, though not quite as simple as the OpenMP implementation. It's class declaration is essentially the same as the OpenMP image processor, and uses the same Laplacian kernel. The primary difference is that instead of being able to simply use a #pragma to parallelize the code, we use the parallel_for functionality from TBB. We use the dimensions of the image to get the range, and then placed our functionality inside the lambda to be passed into the parallel_for call. 

### Results

Testing these libraries in image manipulation displays some interesting differences in their runtime. In everything but the saturation process, the IPP implementations had the fastest run times by considerable margins, though it took around 2.5x longer to alter the image saturation, it was more than twice as fast in the brightening and took around a fifth of the time needed for the OpenMP and TBB parallelized sharpening operations.

The OpenMP and TBB solutions were similar in runtime but the TBB solutions were slightly faster. This is likely due to needing less overhead for the threading than the OpenMP processes. Both are relatively simple to implement so we believe that TBB should generally be the preference between the two tools in these image manipulation applications.

Of course, as can be seen from the charts below, each parallelized option is far faster than the serial implementation of these processes. 

![results table](https://github.com/GPU621-DAL-OpenMP-Comparison/Project-Demo/blob/main/testResults/ResultsSheet.PNG?raw=true)

![chart without serial runtime](https://github.com/GPU621-DAL-OpenMP-Comparison/Project-Demo/blob/main/testResults/ResultsChart_woutSerial.PNG?raw=true)

![chart with serial runtime](https://github.com/GPU621-DAL-OpenMP-Comparison/Project-Demo/blob/main/testResults/ResultsChart_wSerial.PNG?raw=true)


## Documentation

[Intel's DAAL Developer Guide and Reference](https://www.intel.com/content/www/us/en/develop/documentation/onedal-developer-guide-and-reference/top.html)
