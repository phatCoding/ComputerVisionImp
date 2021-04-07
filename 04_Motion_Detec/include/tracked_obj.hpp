#include <opencv2/imgproc.hpp>

#include <optional>
#include <vector>

enum TRACKED_OBJ_SIZE {
    SMALL,
    MEDIUM,
    LARGE,
    UNSPECIFIED
};

class TrackedObj {

public:

    TrackedObj(std::vector<cv::Point> objhull);

    TrackedObj(const TrackedObj& t_obj);

    TrackedObj& operator=(const TrackedObj& t_obj);

    cv::Point getFrameCenter() const;

    cv::Rect getFrame() const;

    float getAspectRatio() const;

    void assignTrackingNumber(int && number);

    void setPosition(const cv::Point lastPosition);

    static TRACKED_OBJ_SIZE filterBySize(const TrackedObj & trackedobj);

    cv::Point estimateNextPosition();


private:

    
    cv::Rect m_frame{};
    cv::Point m_frameCenter{};
    float m_aspectratio{}; 

    std::optional<int> m_trackingNumber;
    std::vector<cv::Point> m_positions;

};