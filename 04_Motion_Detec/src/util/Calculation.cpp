#include "../../include/Calculation.hpp"

namespace Util {
    double euklidDistance(const cv::Point &a, const cv::Point &b){
        return std::sqrt(std::pow(std::abs(b.x - a.x),2) + std::pow(std::abs(b.y - a.y),2));
    }
}