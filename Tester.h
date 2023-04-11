#pragma once
#include <string>
#include "openMP_imgProc.h"
#include "serial_imgproc.h"
#include "ipp_imgProc.h"
#include "Timer.h"

class Tester {
    cv::Mat img_;
    std::string outputPath_;
    std::string outputPostfix_;
    std::string imgPath_;

    openMP_imgProcessor omp_processor_ = openMP_imgProcessor();
    serial_imgProcessor serial_processor_ = serial_imgProcessor();
    IppImgProc ipp_processor_;

    Timer timer;
public:
    Tester(std::string imgPath) {
        imgPath_ = imgPath;
        img_ = cv::imread(imgPath);
        if (img_.empty()) std::cerr << "empty img!\n" << "unable to locate img at " << imgPath << std::endl;

        

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

                //J Saturate
                cv::Mat disp_saturate = cv::imread(outputPath_ + "_saturated_omp" + outputPostfix_);
                cv::resize(disp_saturate, disp_saturate, cv::Size(960, 540));
                std::string sat_winName = "OMP - Saturated";
                cv::imshow(sat_winName, disp_saturate);
                cv::waitKey(0);
                cv::destroyWindow(sat_winName);


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
        omp_processor_.brightenImg(outputImg, level);
        timer.stop();
        std::cout << "Image brightening time w/ OpenMP: " << timer.currtime() << std::endl;

        cv::imwrite(outputPath_ + "_brightened_omp" + outputPostfix_, outputImg);

    }

    void omp_sharpen() {
        cv::Mat outputImg = img_.clone();

        timer.reset();
        timer.start();
        omp_processor_.sharpenImg(outputImg);
        timer.stop();
        std::cout << "Image brightening time w/ OpenMP: " << timer.currtime() << std::endl;

        cv::imwrite(outputPath_ + "_sharpened_omp" + outputPostfix_, outputImg);
    }

    void omp_saturate(int level) {
        cv::Mat outputImg = img_.clone();
        timer.reset();
        timer.start();
        omp_processor_.saturateImg(outputImg, level);
        timer.stop();
        std::cout << "Image brightening time w/ OpenMP: " << timer.currtime() << std::endl;
        cv::imwrite(outputPath_ + "_saturated_omp" + outputPostfix_, outputImg);
    }

    void ipp_all() {
        ipp_processor_ = IppImgProc(imgPath_);

        ipp_processor_.displayInputImage(5000);
        ipp_processor_.sharpening();
        ipp_processor_.brighten(50, 0);
        ipp_processor_.adjustSaturation(0);
    }

    void serial_brighten(int level) {
        cv::Mat outputImg = img_.clone();

        timer.reset();
        timer.start();
        omp_processor_.brightenImg(outputImg, level);
        timer.stop();
        std::cout << "Image brightening time: " << timer.currtime() << std::endl;

        cv::imwrite(outputPath_ + "_brightened_serial" + outputPostfix_, outputImg);

    }

    void serial_sharpen() {
        cv::Mat outputImg = img_.clone();

        timer.reset();
        timer.start();
        omp_processor_.sharpenImg(outputImg);
        timer.stop();
        std::cout << "Image sharpening time: " << timer.currtime() << std::endl;

        cv::imwrite(outputPath_ + "_sharpened_serial" + outputPostfix_, outputImg);
    }

    void serial_saturate(int level) {
        cv::Mat outputImg = img_.clone();
        timer.reset();
        timer.start();
        omp_processor_.saturateImg(outputImg, level);
        timer.stop();
        std::cout << "Image saturating time: " << timer.currtime() << std::endl;
        cv::imwrite(outputPath_ + "_saturated_serial" + outputPostfix_, outputImg);
    }

};