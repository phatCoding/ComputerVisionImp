#include "../../include/Paint.hpp"

namespace Util {
    namespace Paint {

        void paintObjects(cv::Mat &image, const ObjectDetection::TrackedObjectMonitor &monitor){
            for(auto const &[index,obj] : *(monitor.get())){
                if(obj != nullptr){
                    cv::rectangle(image, obj->getBoundingRect(),cv::Scalar(0.0, 0.0, 255.0));
                    paintNumbers(image,index,obj->getOrigin());
                }
            }
        }

        void paintNumbers(cv::Mat &image, const int number,const cv::Point &origin){
            auto font = cv::FONT_HERSHEY_SIMPLEX;
            std::string numberString = std::to_string(number);
            cv::putText(image,numberString , origin + cv::Point(10,-10), font, 2,(255,255,255),1);
        }
    }
}