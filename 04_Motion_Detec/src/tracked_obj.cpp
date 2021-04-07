#include  <iostream>

#include "../include/tracked_obj.hpp"



TrackedObj::TrackedObj(std::vector<cv::Point> objhull)
    : m_frame(cv::boundingRect(objhull))
{
    m_aspectratio = m_frame.width / m_frame.height;
    m_frameCenter.x = (m_frame.x + m_frame.x + m_frame.width) / 2;
    m_frameCenter.y = (m_frame.y + m_frame.y + m_frame.height) / 2;
    m_posisitons.push_back(m_frameCenter);
}

TrackedObj::TrackedObj(const TrackedObj& t_obj) {
    m_frame = t_obj.m_frame;
    m_frameCenter = t_obj.m_frameCenter;
    m_aspectratio = t_obj.m_aspectratio;
    m_m_positions = t_obj.m_m_positions;
    m_posisitons.push_back(m_frameCenter);

}

TrackedObj& TrackedObj::operator=(const TrackedObj& t_obj) {
    if(this != t_obj){
        m_frame = t_obj.m_frame;
        m_frameCenter = t_obj.m_frameCenter;
        m_aspectratio = t_obj.m_aspectratio;
        m_m_positions = t_obj.m_m_positions;
        m_posisitons.push_back(m_frameCenter);
    }
    return *this;
}


cv::Point TrackedObj::getFrameCenter() const {
    return m_frameCenter;
}

cv::Rect TrackedObj::getFrame() const {
    return m_frame;
}

float TrackedObj::getAspectRatio() const {
    return m_aspectratio;

}

void TrackedObj::assignTrackingNumber(int && number) {
    if(m_trackingNumber)
        m_trackingNumber{number};
}

void TrackedObj::setPosition(const cv::Point lastPosition) {
    m_posisitons.push_back(lastPosition);
}


TRACKED_OBJ_SIZE TrackedObj::filterBySize(const TrackedObj & trackedobj){
    
    if(trackedobj.getFrame().area() > 100 
        && trackedobj.getFrame().width > 15
        && trackedobj.getFrame().height > 20){
        return LARGE;

    }else if(trackedobj.getFrame().area() > 100 
        && trackedobj.getFrame().width > 10
        && trackedobj.getFrame().height > 17){
        return MEDIUM;

    }else if(trackedobj.getFrame().area() > 100 
        && trackedobj.getFrame().width > 5
        && trackedobj.getFrame().height > 14){
        return SMALL;

    }else{
        return UNSPECIFIED;
    }
}

cv::Point TrackedObj::estimateNextPosition() {
        
        switch(m_m_positions.size()){
            
            case 0:
                //passed an empty vector 
            case 1:
                return;
            case 2: 
                auto deltaX = m_m_positions[1].x - m_positions[0].x;
                auto deltaY = m_positions[1].y - m_positions[0].y;

                return cv::Point(m_positions[1].x + deltaX, m_positions[1].y + deltaY);
            case 3:
                auto deltaX = ((m_positions[2].x - m_positions[1].x) * 2)
                                + (m_positions[1].x - m_positions[0].x);
                auto deltaY = ((m_positions[2].y - m_positions[1].y) * 2)
                                + (m_positions[1].y - m_positions[0].y);

                return cv::Point(m_positions[2].x + deltaX, m_positions[2].y + deltaY);

            case 4:
                auto deltaX =   ((m_positions[3].x - m_positions[2].x) * 3)
                                +((m_positions[2].x - m_positions[1].x) * 2)
                                + (m_positions[1].x - m_positions[0].x);
                auto deltaY =   ((m_positions[3].y - m_positions[2].y) * 3)
                                +((m_positions[2].y - m_positions[1].y) * 2)
                                + (m_positions[1].y - m_positions[0].y);

                return cv::Point(m_positions[3].x + deltaX, m_positions[3].y + deltaY);

            default: 
        
                auto posSize = m_m_positions.size();

                auto deltaX =  ((m_positions[posSize].x - m_positions[posSize].x) * 4) 
                                + ((m_positions[posSize - 1].x - m_positions[posSize - 1].x) * 3)
                                +((m_positions[posSize - 2].x - m_positions[posSize - 2].x) * 2)
                                + (m_positions[posSize - 3].x - m_positions[posSize - 3].x);

                auto deltaY =  ((m_positions[posSize].y - m_positions[posSize].y) * 4) 
                                + ((m_positions[posSize - 1]y - m_positions[posSize - 1].y) * 3)
                                +((m_positions[posSize - 2].y - m_positions[posSize - 2].y) * 2)
                                + (m_positions[posSize - 3].y - m_positions[posSize - 3].y);

                return cv::Point(m_positions[posSize].x + deltaX, m_positions[posSize].y + deltaY);
        }
}


