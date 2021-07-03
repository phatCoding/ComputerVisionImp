#pragma once
#include<opencv2/core/core.hpp> 
#include<opencv2/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>

#include<map>

#include "trackedobjectmonitor.hpp"

namespace Util {
    namespace Paint{
        void paintObjects(cv::Mat &image, const ObjectDetection::TrackedObjectMonitor &monitor);
        void paintNumbers(cv::Mat &image, const int number, const cv::Point &origin );
    }
}