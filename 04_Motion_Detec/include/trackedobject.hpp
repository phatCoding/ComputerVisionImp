#pragma once
#include<opencv2/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/core/core.hpp>

#include <vector>

namespace ObjectDetection {
 
enum TrackedObjectSize{
    SMALL,
    COMMON,
    LARGE,
    UNSPECIFIED
};

class TrackedObject {
public:
    
    TrackedObject(std::vector<cv::Point> &countor);
    TrackedObject(const TrackedObject& other);

    TrackedObject& operator=(const TrackedObject& other);

    cv::Point getOrigin() const;
    cv::Rect getBoundingRect() const;

    int getTrackCount() const;
    bool isTracked() const;

    void push_back(const cv::Point);

    cv::Point nextEstimatePosition();
    static bool filterBy(TrackedObjectSize requiredSize, const TrackedObject &tempObj);

    void setTracked(const bool status);
    void resetTrackCount();
    void increaseTrackCount();
    void decreaseTrackCount();  

private:

    cv::Point   m_origin;
    cv::Rect    m_boundRect;
    std::vector<cv::Point> m_positionsHistory{};

    int trackCount;
    bool tracked{false};
};
};