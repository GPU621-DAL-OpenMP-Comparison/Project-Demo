#pragma once
#include <string>
#include "openMP_imgProc.h"
#include "Timer.h"

class Tester {
    cv::Mat img_;
    std::string outputPath_;
    std::string outputPostfix_;
    openMP_imgProcessor processor_ = openMP_imgProcessor::openMP_imgProcessor();
    //iDal_imgProcessor
    Timer timer;
public:
    Tester(std::string imgPath) {
        img_ = cv::imread(imgPath);

        outputPath_ = imgPath.substr(0, imgPath.find_last_of("."));
        outputPath_ += "_modified";
        outputPostfix_ = imgPath.substr(imgPath.find_last_of("."), imgPath.length());
    }

    //display img. selection 0 for original, 1 for OMP modified, 2 for DAAL;
    void display_img(int selection) {
        if (!img_.empty()) {
            //supressing OpenCV messages
            std::streambuf* coutbuf = std::cout.rdbuf();
            std::cout.rdbuf(nullptr);

            //make reasonable display size
            if (selection == 0) {
                cv::Mat disp = img_.clone();
                cv::resize(disp, disp, cv::Size(960, 540));

                //display image
                cv::namedWindow("Original Img");
                cv::imshow("Original Img", disp);
                //press any key to close display and continue
                cv::waitKey(0);
                cv::destroyWindow("Original Img");
            }
            if (selection == 1) {
                cv::Mat disp_bright = cv::imread(outputPath_ + "_brightened_omp" + outputPostfix_);
                cv::resize(disp_bright, disp_bright, cv::Size(960, 540));

                //display image
                std::string b_windowName = "OMP - Brightened";
                cv::namedWindow(b_windowName);
                cv::imshow(b_windowName, disp_bright);
                //press any key to close display and continue
                cv::waitKey(0);
                cv::destroyWindow(b_windowName);

                cv::Mat disp_sharp = cv::imread(outputPath_ + "_sharpened_omp" + outputPostfix_);
                cv::resize(disp_sharp, disp_sharp, cv::Size(960, 540));
                std::string s_winName = "OMP - Sharpened";
                cv::imshow(s_winName, disp_sharp);
                cv::waitKey(0);
                cv::destroyWindow(s_winName);
            }
            if (selection == 2) {
                std::cerr << "DAAL implementation not yet built!\n";
            }
            

            //stop supressing
            std::cout.rdbuf(coutbuf);
        }
    }

    void omp_brighten(int level) {
        cv::Mat outputImg = img_.clone();

        timer.reset();
        timer.start();
        processor_.brightenImg(outputImg, level);
        timer.stop();
        std::cout << "Image brightening time: " << timer.currtime() << std::endl;

        cv::imwrite(outputPath_ + "_brightened_omp" + outputPostfix_, outputImg);
    }

    void omp_sharpen() {
        cv::Mat outputImg = img_.clone();

        timer.reset();
        timer.start();
        processor_.sharpenImg(outputImg);
        timer.stop();
        std::cout << "Image sharpening time: " << timer.currtime() << std::endl;

        cv::imwrite(outputPath_ + "_sharpened_omp" + outputPostfix_, outputImg);
    }
};