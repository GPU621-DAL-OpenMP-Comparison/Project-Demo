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

    void brighten(int level) {
        cv::Mat outputImg = img_.clone();

        timer.reset();
        timer.start();
        processor_.brightenImg(outputImg, level);
        timer.stop();
        std::cout << "Image brightening time: " << timer.currtime() << std::endl;

        cv::imwrite(outputPath_ + "_brightened" + outputPostfix_, outputImg);
    }

    void sharpen() {
        cv::Mat outputImg = img_.clone();

        timer.reset();
        timer.start();
        processor_.sharpenImg(outputImg);
        timer.stop();
        std::cout << "Image sharpening time: " << timer.currtime() << std::endl;

        cv::imwrite(outputPath_ + "_sharpened" + outputPostfix_, outputImg);
        std::cout << "Altered image save to at \"" << outputPath_ + "_sharpened" + outputPostfix_ << "\"\n";
    }
};