#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include<iostream>
#include<memory>

#include "../include/tracking.hpp"



int main() {
    /*******************Space for defining Variables************************/
    
    // Define char for later esc of Video
    char chCheckForEscKey = 0;

    //
    int conTrackingNumber(0);
    bool firstFrame(true);

    // Two Frames for image subtraction
    cv::Mat img_1;
    cv::Mat img_2;

    // Videooutput
    cv::VideoCapture capVideo;

    // Tracked Objs
    Tracking::TrackedObjControl alreadyTrackedObjs = TrackedObjControl();
    /***********************************************************************/

    //capVideo.open("04_Motion_Detec/olympiabrückeverkehr.mp4");
    capVideo.open("../../04_Motion_Detec/olympiabrückeverkehr.mp4");


    if (!capVideo.isOpened()) {                                                 
        std::cout << "\nerror reading video file" << std::endl << std::endl;      
        return(0);                                                            
    }

    if (capVideo.get(cv::CAP_PROP_FRAME_COUNT) < 2) {
        std::cout << "\nerror: video file must have at least two frames";
        return(0);
    }

    capVideo.read(img_1);
    capVideo.read(img_2);

    while (capVideo.isOpened() && chCheckForEscKey != 27) {

        cv::Mat img_1_c = img_1.clone();
        cv::Mat img_2_c = img_2.clone();

        cv::rotate(img_1_c,img_1_c, cv::ROTATE_90_CLOCKWISE);
        cv::rotate(img_2_c,img_2_c, cv::ROTATE_90_CLOCKWISE);

        //Apply Color Conv and Filtering
        cv::cvtColor(img_1_c, img_1_c, 6);
        cv::cvtColor(img_2_c, img_2_c, 6);

        cv::medianBlur(img_1_c, img_1_c,3);
        cv::medianBlur(img_2_c, img_2_c,3);

        // Get Image Subtraction
        cv::Mat imgDiff;
        cv::absdiff(img_1_c, img_2_c, imgDiff);
        cv::threshold(imgDiff, imgDiff, 30, 255.0, cv::THRESH_BINARY);

        // Apply further filtering on black and white image to remove noise
        cv::erode(imgDiff,imgDiff, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5)));
        cv::dilate(imgDiff,imgDiff, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(9, 9)), cv::Point(-1,-1), 2);

        cv::imshow("Motion_Video", imgDiff);

        cv::Mat imgTresh = imgDiff.clone();

        std::vector<std::vector<cv::Point> > contours;
        cv::findContours(imgTresh, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        cv::Mat contourImg(imgTresh.size(), CV_8UC3, cv::Scalar(0.0, 0.0, 0.0));
        
        cv::drawContours(contourImg, contours , -1, cv::Scalar(255.0, 255.0, 255.0),1);
    
        cv::imshow("Contour", contourImg);

        std::vector<std::vector<cv::Point> > convexHulls(contours.size());

        for (unsigned int i = 0; i < contours.size(); i++) {
            cv::convexHull(contours[i], convexHulls[i]);
        }

        img_2_c = img_2.clone();
        cv::rotate(img_2_c,img_2_c, cv::ROTATE_90_CLOCKWISE);

        for(auto const & conxH : convexHulls){
            TrackedObj tempObj(conxH);
            if(TrackedObj::filterBySize(tempObj) == TRACKED_OBJ_SIZE::LARGE){

                if(firstFrame){
                    alreadyTrackedObjs.addNewObj(tempObj, conTrackingNumber++);
                } else {
                    for(const auto & singleObj : alreadyTrackedObjs){
                        
                    }
                }

                cv::rectangle(img_2_c,cv::boundingRect(conxH), cv::Scalar(0.0, 0.0, 255.0), 2);
                cv::circle(img_2_c,tempObj.getFrameCenter(),3,cv::Scalar(255.0, 255.0, 255.0));
            }
        }
        
        cv::imshow("Track", img_2_c);

        img_1 = img_2.clone();           // move frame 1 up to where frame 2 is


        if ((capVideo.get(cv::CAP_PROP_POS_FRAMES) + 1) < capVideo.get(cv::CAP_PROP_FRAME_COUNT)) {       // if there is at least one more frame
        capVideo.read(img_2);
        firstFrame = false;                       // read it
        }
        else {                                                  // else
        std::cout << "end of video\n";                      // show end of video message
        break;                                              // and jump out of while loop
        }

        chCheckForEscKey = cv::waitKey(3);
    }

    if (chCheckForEscKey != 27) {               // if the user did not press esc (i.e. we reached the end of the video)
        cv::waitKey(0);                         // hold the windows open to allow the "end of video" message to show
    }

};