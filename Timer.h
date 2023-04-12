#pragma once

#include <chrono>
using namespace std::chrono;


class Timer {

    long long duration_;
    high_resolution_clock::time_point t1_;
    high_resolution_clock::time_point t2_;
public:
    Timer() { reset(); };
    void reset() {
        duration_ = 0;
        t1_ = t2_ = {};
    }
    void start() {
        t1_ = high_resolution_clock::now();
    }
    void stop() {
        t2_ = high_resolution_clock::now();
        duration_ = duration_cast<milliseconds>(t2_ - t1_).count();
    }
    long long currtime() {
        return duration_;
    }
};
