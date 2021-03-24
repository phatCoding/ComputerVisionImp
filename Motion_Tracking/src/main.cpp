#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include<iostream>

#include"../include/bodyframe.h"
#include "../include/trace.h"

const cv::Scalar SCALAR_BLACK = cv::Scalar(0.0, 0.0, 0.0);
const cv::Scalar SCALAR_WHITE = cv::Scalar(255.0, 255.0, 255.0);
const cv::Scalar SCALAR_BLUE = cv::Scalar(255.0, 0.0, 0.0);
const cv::Scalar SCALAR_GREEN = cv::Scalar(0.0, 200.0, 0.0);
const cv::Scalar SCALAR_RED = cv::Scalar(0.0, 0.0, 255.0);

int main(void) {

    cv::VideoCapture capVideo;
    cv::Mat imgFrame1;
    cv::Mat imgFrame2;

    capVideo.open("../../Motion_Tracking/768x576.avi");

    if (!capVideo.isOpened()) {                                                 
        std::cout << "\nerror reading video file" << std::endl << std::endl;      
        return(0);                                                            
    }

    if (capVideo.get(cv::CAP_PROP_FRAME_COUNT) < 2) {
        std::cout << "\nerror: video file must have at least two frames";
        return(0);
    }

    capVideo.read(imgFrame1);
    capVideo.read(imgFrame2);
    char chCheckForEscKey = 0;

    Trace * bodytrace = new Trace();

    while (capVideo.isOpened() && chCheckForEscKey != 27) {

        std::vector<Bodyframe> bodys;

        cv::Mat imgFrame1Copy = imgFrame1.clone();
        cv::Mat imgFrame2Copy = imgFrame2.clone();

        cv::Mat imgDifference;

        cv::cvtColor(imgFrame1Copy, imgFrame1Copy, 6);
        cv::cvtColor(imgFrame2Copy, imgFrame2Copy, 6);

        cv::GaussianBlur(imgFrame1Copy, imgFrame1Copy, cv::Size(3, 3), 0);
        cv::GaussianBlur(imgFrame2Copy, imgFrame2Copy, cv::Size(3, 3), 0);

        cv::absdiff(imgFrame1Copy, imgFrame2Copy, imgDifference);

        cv::threshold(imgDifference, imgDifference, 50, 255.0, cv::THRESH_BINARY);

        cv::Mat structuringElement5x5 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));

        cv::dilate(imgDifference, imgDifference, structuringElement5x5);
        cv::dilate(imgDifference, imgDifference, structuringElement5x5);

        cv::morphologyEx(imgDifference, imgDifference, cv::MORPH_OPEN, structuringElement5x5);

        cv::Mat imgThreshCopy = imgDifference.clone();

        std::vector<std::vector<cv::Point> > contours;

        cv::findContours(imgThreshCopy, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

        std::vector<std::vector<cv::Point> > convexHulls(contours.size());

        for (unsigned int i = 0; i < contours.size(); i++) {
            cv::convexHull(contours[i], convexHulls[i]);
        }

        imgFrame2Copy = imgFrame2.clone();

        for(auto const & convexHull : convexHulls){
            Bodyframe possible_body(convexHull);
        
            if(Bodyframe::checkBodyFrameSize(possible_body) == BODY_FRAME_SIZE::LARGE){
                bodys.push_back(possible_body);
            }
        }

        for (auto const & body : bodys) {                                                  
            cv::rectangle(imgFrame2Copy, body.getFrame(), SCALAR_RED, 2);            
            bodytrace->add2Buffer(body.getFrameCenter());
        }
        
        bodytrace->point2Image(imgFrame2Copy);
        cv::imshow("Motion_Video", imgFrame2Copy);
        
        imgFrame1 = imgFrame2.clone();           // move frame 1 up to where frame 2 is

        if ((capVideo.get(cv::CAP_PROP_POS_FRAMES) + 1) < capVideo.get(cv::CAP_PROP_FRAME_COUNT)) {       // if there is at least one more frame
            capVideo.read(imgFrame2);                            // read it
        }
        else {                                                  // else
            std::cout << "end of video\n";                      // show end of video message
            break;                                              // and jump out of while loop
        }
        chCheckForEscKey = cv::waitKey(1);
    }

    delete bodytrace;

    if (chCheckForEscKey != 27) {               // if the user did not press esc (i.e. we reached the end of the video)
        cv::waitKey(0);                         // hold the windows open to allow the "end of video" message to show
    }


    return(0);
}