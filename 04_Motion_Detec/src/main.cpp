#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <memory>

#include "../include/PreProcImage.hpp"
#include "../include/Paint.hpp"

const bool ROTATE = true;

int main()
{
    // Define char for later esc of Video
    char chCheckForEscKey = 0;
    bool firstFrame = true;
    int conTrackingNumber{0}, frameCount{0};

    cv::Mat img_1;
    cv::Mat img_2;

    // Videooutput
    cv::VideoCapture capVideo;
    
    //
    ObjectDetection::TrackedObjectMonitor tracking_monitor;
    std::vector<ObjectDetection::TrackedObject> possibleObjects;

    // Vidoe 1
    //capVideo.open("../../04_Motion_Detec/fussgeanger.avi");

    // Video 2
    capVideo.open("../../04_Motion_Detec/olympiabrückeverkehr.mp4");

    if (!capVideo.isOpened())
    {
        std::cout << "\nerror reading video file" << std::endl
                  << std::endl;
        return (0);
    }

    if (capVideo.get(cv::CAP_PROP_FRAME_COUNT) < 2)
    {
        std::cout << "\nerror: video file must have at least two frames";
        return (0);
    }

    capVideo.read(img_1);
    capVideo.read(img_2);

    while (capVideo.isOpened() && chCheckForEscKey != 27)
    {

        cv::Mat img_1_c = img_1.clone(), img_2_c = img_2.clone(), imgDiff;

        Util::imageSubtraction(img_1_c, img_2_c, imgDiff, ROTATE);
        std::vector<std::vector<cv::Point> > contours;
        cv::findContours(imgDiff, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
        std::vector<std::vector<cv::Point> > convexHulls(contours.size());

        for (unsigned int i = 0; i < contours.size(); i++) {
            cv::convexHull(contours[i], convexHulls[i]);
        }


        for (auto &conxH : convexHulls)
        {
            ObjectDetection::TrackedObject temporaryObj(conxH);
            if(ObjectDetection::TrackedObject::filterBy(ObjectDetection::TrackedObjectSize::COMMON, temporaryObj)){
                
                if(firstFrame){
                    tracking_monitor.add(conTrackingNumber,temporaryObj);
                } else {
                    if(!tracking_monitor.checkForUpdate(temporaryObj)){
                        tracking_monitor.add(conTrackingNumber,temporaryObj);
                    }
                }
           }
        }

        tracking_monitor.removeUntracked();

        if(firstFrame){
            firstFrame = !firstFrame;
        }

        img_2_c = img_2.clone();

        if(ROTATE)
            cv::rotate(img_2_c, img_2_c, cv::ROTATE_90_CLOCKWISE);

        Util::Paint::paintObjects(img_2_c, tracking_monitor);
        cv::imshow("Track", img_2_c);

        img_1 = img_2.clone(); // move frame 1 up to where frame 2 is
        frameCount++;

        if ((capVideo.get(cv::CAP_PROP_POS_FRAMES) + 1) < capVideo.get(cv::CAP_PROP_FRAME_COUNT))
        { // if there is at least one more frame
            capVideo.read(img_2);
            
        }
        else
        {                                  // else
            std::cout << "end of video\n"; // show end of video message
            break;                         // and jump out of while loop
        }
        chCheckForEscKey = cv::waitKey(10);
    }

    if (chCheckForEscKey != 27)
    {                   // if the user did not press esc (i.e. we reached the end of the video)
        cv::waitKey(0); // hold the windows open to allow the "end of video" message to show
    }
};