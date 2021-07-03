#pragma once
#include<opencv2/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/core/core.hpp>

using OutlineVector = std::vector<std::vector<cv::Point>>;

namespace Util {


    void imageSubtraction(const cv::Mat &img1, const cv::Mat &img2 , cv::Mat &imgDiff, bool rotate);
    void imageConvexHulls(const cv::Mat &imgDiff , const OutlineVector &convexhulls );

};