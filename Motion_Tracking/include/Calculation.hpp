#pragma once
#include<opencv2/core/core.hpp> 
#include <map>

namespace Util {
    double euklidDistance(const cv::Point &a, const cv::Point &b);
}