#include "../../include/trackedobject.hpp"
namespace ObjectDetection {

TrackedObject::TrackedObject(std::vector<cv::Point> &countor)
    : m_boundRect(cv::boundingRect(countor))
{
    m_origin.x = (m_boundRect.x + m_boundRect.x + m_boundRect.width) / 2;
    m_origin.y = (m_boundRect.y + m_boundRect.y + m_boundRect.height) / 2;
    trackCount = 0;
    tracked = false;
    m_positionsHistory.push_back(m_origin);
}

TrackedObject::TrackedObject(const TrackedObject& other){
    m_origin = other.m_origin;
    m_boundRect = other.m_boundRect;
    trackCount = 0;
    tracked = false;
    m_positionsHistory.push_back(m_origin);
}

TrackedObject& TrackedObject::operator=(const TrackedObject& other){
    m_origin = other.m_origin;
    m_boundRect = other.m_boundRect;
    tracked = false;
    m_positionsHistory.push_back(m_origin);
    return *this;
}



cv::Point TrackedObject::getOrigin() const{
    return m_origin;
}

cv::Rect TrackedObject::getBoundingRect() const{
    return m_boundRect;
}

int TrackedObject::getTrackCount() const{
    return trackCount;
}

bool TrackedObject::isTracked() const {
    return tracked;
}

void TrackedObject::setTracked(const bool status){
    tracked = status;
} 

cv::Point TrackedObject::nextEstimatePosition(){
    int deltaX, deltaY;
    auto posSize = m_positionsHistory.size();
    switch(posSize){
        case 0: 
                break;
        case 1:
                return m_positionsHistory[0];
        case 2:
             deltaX = (m_positionsHistory[1].x - m_positionsHistory[0].x)/2;
             deltaY = (m_positionsHistory[1].y - m_positionsHistory[0].y)/2;
                return cv::Point((m_positionsHistory[1].x + deltaX) , (m_positionsHistory[1].y + deltaY) );
        case 3:
             deltaX = (((m_positionsHistory[2].x - m_positionsHistory[1].x) * 2)
                                + (m_positionsHistory[1].x - m_positionsHistory[0].x))/3;
             deltaY = (((m_positionsHistory[2].y - m_positionsHistory[1].y) * 2)
                                + (m_positionsHistory[1].y - m_positionsHistory[0].y))/3;

                return cv::Point((m_positionsHistory[2].x + deltaX) , (m_positionsHistory[2].y + deltaY) );
        case 4:
             deltaX =   (((m_positionsHistory[3].x - m_positionsHistory[2].x) * 3)
                                +((m_positionsHistory[2].x - m_positionsHistory[1].x) * 2)
                                + (m_positionsHistory[1].x - m_positionsHistory[0].x))/4;
             deltaY =   (((m_positionsHistory[3].y - m_positionsHistory[2].y) * 3)
                                +((m_positionsHistory[2].y - m_positionsHistory[1].y) * 2)
                                + (m_positionsHistory[1].y - m_positionsHistory[0].y))/4;

                return cv::Point((m_positionsHistory[3].x + deltaX) , (m_positionsHistory[3].y + deltaY) );
        default:

             deltaX =  (((m_positionsHistory[posSize - 1].x - m_positionsHistory[posSize - 2].x)) 
                                + ((m_positionsHistory[posSize - 2].x - m_positionsHistory[posSize - 3].x) * 2)
                                +((m_positionsHistory[posSize - 3].x - m_positionsHistory[posSize - 4].x) * 3)
                                + ((m_positionsHistory[posSize - 4].x - m_positionsHistory[posSize - 5].x) *4))/ 10;

             deltaY =  (((m_positionsHistory[posSize - 1].y - m_positionsHistory[posSize - 2].y)) 
                                + ((m_positionsHistory[posSize - 2].y - m_positionsHistory[posSize - 3].y) * 2)
                                +((m_positionsHistory[posSize - 3].y - m_positionsHistory[posSize - 4].y) * 3)
                                + ((m_positionsHistory[posSize - 4].y - m_positionsHistory[posSize - 5].y)*4)) / 10;

                auto debug = cv::Point((m_positionsHistory[posSize-1].x + deltaX), (m_positionsHistory[posSize-1].y + deltaY));
                return cv::Point((m_positionsHistory[posSize-1].x + deltaX), (m_positionsHistory[posSize-1].y + deltaY));
        
    }
    return cv::Point(0,0);
}

bool TrackedObject::filterBy(TrackedObjectSize requiredSize, const TrackedObject &tempObj){
    
    if(tempObj.getBoundingRect().area() > 100 
        && tempObj.getBoundingRect().width > 15
        && tempObj.getBoundingRect().height > 20){
        return LARGE;

    }else if(tempObj.getBoundingRect().area() > 100 
        && tempObj.getBoundingRect().width > 10
        && tempObj.getBoundingRect().height > 17){
        return COMMON;

    }else if(tempObj.getBoundingRect().area() > 100 
        && tempObj.getBoundingRect().width > 5
        && tempObj.getBoundingRect().height > 14){
        return SMALL;

    }else{
        return UNSPECIFIED;
    }
}

void TrackedObject::increaseTrackCount(){
    trackCount = trackCount + 1;
}

void TrackedObject::decreaseTrackCount(){
    trackCount = trackCount - 1;
}

void TrackedObject::resetTrackCount(){
    trackCount = 0;
}

}
