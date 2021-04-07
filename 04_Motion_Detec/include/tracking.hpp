#include <opencv2/imgproc.hpp>

#include <vector>
#include<unordered_map>

#include "../include/tracked_obj.hpp"

const float ESTIMATE_THRESHOLD = 5.0f;

namespace Tracking {
    
class TrackedObjControl {

    TrackedObjControl();
    ~TrackedObjControl();

    void updateTracked(const TrackedObj &updatedObj, const int trackingNumber);
    void addNewObj(const TrackedObj &newObj ,const int trackingNumber);
    void removeUntracked(const int trackingNumber);

    TrackedObj* operator[](int trackingnumber);

    static matchCurrent2ExistingObjs();

private:

    std::unordered_map<int, TrackedObj*> * m_objTracker;
        
};

};