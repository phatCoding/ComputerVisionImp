#include<opencv2/highgui/highgui.hpp>

#include "../include/trace.h"

Trace::Trace(){
}

void Trace::point2Image(const cv::Mat & image) {

    int m_tail_cpy{m_tail};
    while(m_tail_cpy != m_head)
    {

        if(m_tail_cpy == MAX_BUFFER_SIZE + 1)
            m_tail_cpy = 0;

        cv::circle(image, m_pointTrace[m_tail_cpy],1,cv::Scalar(0.0, 0.0, 0.0),3);
        m_tail_cpy++;
    }
}

void Trace::add2Buffer(const cv::Point & p) {

    if(m_tail == MAX_BUFFER_SIZE + 1){
        m_tail = 0;
    }

    if(m_head == MAX_BUFFER_SIZE + 1){
        m_head = 0;
        m_tail++;
    }

    if(m_head < m_tail){
        m_tail++;
        m_pointTrace[m_head] = p;
    }else {
        m_pointTrace.push_back(p);
    }

    m_head++;
}