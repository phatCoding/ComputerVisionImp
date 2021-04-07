#include "../include/tracking.hpp"

namespace Tracking {

    TrackedObjControl::TrackedObjControl() {
        objTracker = new std::unordered_map<int, TrackedObj*>();
    }

    TrackedObjControl::~TrackedObjControl() {
        delete objTracker;
    }

    void TrackedObjControl::updateTracked(const TrackedObj &updatedObj, const int trackingNumber){
        m_objTracker->at(trackingNumber) = updatedObj;
    }

    void TrackedObjControl::addNewObj(const TrackedObj &newObj ,const int trackingNumber){
        m_objTracker[trackingNumber] = new TrackedObj(newObj);
    }

    void TrackedObjControl::removeUntracked(const int trackingNumber) {
        delete m_objTracker->at(trackingNumber);
        m_objTracker->erase(trackingNumber);
    }

    size_t TrackedObjControl::size() const {
        return m_objTracker->size();
    }

    

}