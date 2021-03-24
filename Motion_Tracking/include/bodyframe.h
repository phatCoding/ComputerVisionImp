#include <opencv2/imgproc.hpp>

#include <vector>

enum BODY_FRAME_SIZE {
    SMALL,
    MEDIUM,
    LARGE
};

class Bodyframe {

public:

Bodyframe(std::vector<cv::Point> bodyhull);

cv::Point getFrameCenter() const;

cv::Rect getFrame() const;

float getAspectRatio() const;

static BODY_FRAME_SIZE checkBodyFrameSize(const Bodyframe & body);

private:

cv::Rect m_frame{};
cv::Point m_frameCenter{};
float m_aspectratio{}; 

};