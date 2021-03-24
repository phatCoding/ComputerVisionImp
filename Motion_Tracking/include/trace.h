#include <opencv2/imgproc.hpp>

#include <vector>

constexpr int MAX_BUFFER_SIZE = 100;

class Trace {

public:

    Trace();
    ~Trace() = default;

    void point2Image(const cv::Mat & image);
    void add2Buffer(const cv::Point & p);

private: 

    std::vector<cv::Point> m_pointTrace{};
    int m_tail{};
    int m_head{};

};