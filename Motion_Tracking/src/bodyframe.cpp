#include "../include/bodyframe.h"

Bodyframe::Bodyframe(std::vector<cv::Point> bodyhull)
    : m_frame(cv::boundingRect(bodyhull))
{
    m_aspectratio = m_frame.width / m_frame.height;
    m_frameCenter.x = (m_frame.x + m_frame.x + m_frame.width) / 2;
    m_frameCenter.y = (m_frame.y + m_frame.y + m_frame.height) / 2;
}

cv::Point Bodyframe::getFrameCenter() const {
    return m_frameCenter;
}

cv::Rect Bodyframe::getFrame() const {
    return m_frame;
}

float Bodyframe::getAspectRatio() const {
    return m_aspectratio;

}

BODY_FRAME_SIZE Bodyframe::checkBodyFrameSize(const Bodyframe & body){
    
    if(body.getFrame().area() > 100 
        && body.getFrame().width > 15
        && body.getFrame().height > 20){
        return LARGE;

    }else if(body.getFrame().area() > 100 
        && body.getFrame().width > 10
        && body.getFrame().height > 17){
        return MEDIUM;

    }else{
        return SMALL;
    }
}
