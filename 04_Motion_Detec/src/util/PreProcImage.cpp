#include "../../include/PreProcImage.hpp"

namespace Util
{
    void imageSubtraction(const cv::Mat &img1, const cv::Mat &img2 , cv::Mat &imgDiff, bool rotate){
        
        cv::Mat img_1_c = img1;
        cv::Mat img_2_c = img2;

        if(rotate){
            cv::rotate(img_1_c,img_1_c, cv::ROTATE_90_CLOCKWISE);
            cv::rotate(img_2_c,img_2_c, cv::ROTATE_90_CLOCKWISE);
        }

        //Apply Color Conv and Filtering
        cv::cvtColor(img_1_c, img_1_c, 6);
        cv::cvtColor(img_2_c, img_2_c, 6);

        cv::medianBlur(img_1_c, img_1_c,3);
        cv::medianBlur(img_2_c, img_2_c,3);

        // Get Image Subtraction
        cv::absdiff(img_1_c, img_2_c, imgDiff);
        cv::threshold(imgDiff, imgDiff, 30, 255.0, cv::THRESH_BINARY);

        // Apply further filtering on black and white image to remove noise
        cv::erode(imgDiff,imgDiff, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5)));
        cv::dilate(imgDiff,imgDiff, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(9, 9)), cv::Point(-1,-1), 7);
    }

    void imageConvexHulls(const cv::Mat &imgDiff , const OutlineVector &convexhulls ){

        OutlineVector contours;
        cv::findContours(imgDiff, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
        //convexhulls(contours.size());

        for (unsigned int i = 0; i < contours.size(); i++)
        {
            cv::convexHull(contours[i], convexhulls[i]);
        }
    }

}

