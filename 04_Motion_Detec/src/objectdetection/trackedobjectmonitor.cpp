#include "../../include/trackedobjectmonitor.hpp"

namespace ObjectDetection {

    TrackedObjectMonitor::TrackedObjectMonitor() {
        monitor = new ObjectDictionary();
    }

    TrackedObjectMonitor::~TrackedObjectMonitor() {
        delete monitor;
    }

    void TrackedObjectMonitor::add( int &id , TrackedObject &untracked_obj) {
        id++;
        monitor->insert(std::pair<int, ObjectDetection::TrackedObject*>(id, new TrackedObject(untracked_obj)));
    }

    void TrackedObjectMonitor::update(const int &id, TrackedObject &untracked_obj) {
        *(monitor->at(id)) = untracked_obj;
    }

    void TrackedObjectMonitor::invalidateIndex(const int &id) {
        if((monitor->at(id)) != nullptr){
            monitor->at(id) = nullptr;
        }
    }

    bool TrackedObjectMonitor::checkForUpdate(TrackedObject &current_unspecified_obj) {
        
        double minimalDistance(TRACKING_THRESHHOLD);
        int minimalDistanceIndex(0);
        bool tracked = false;
        bool result = false; 

        for(auto &[index,obj] : *monitor){
            
            if(obj != nullptr){
                auto distance = Util::euklidDistance(obj->nextEstimatePosition(), current_unspecified_obj.getOrigin());
                if(distance <= TRACKING_THRESHHOLD){

                    if(minimalDistance != std::min(minimalDistance, distance))
                    {
                        minimalDistance = std::min(minimalDistance, distance);
                        minimalDistanceIndex = index;
                    }
                    tracked = true;
                } 
            }
        }
            if(tracked) {
                update(minimalDistanceIndex,current_unspecified_obj);
                monitor->at(minimalDistanceIndex)->increaseTrackCount();
                result = true;
            }

            return result;
    }

    void TrackedObjectMonitor::removeUntracked(){

        for(auto &[index,obj] : *monitor){
            if(obj != nullptr){
                if(!obj->isTracked()){
                    obj->decreaseTrackCount();
                    if(obj->getTrackCount() < -3){
                        invalidateIndex(index);
                    }
                }
            }
        }
    }

    

    ObjectDictionary* TrackedObjectMonitor::get() const{
        return monitor;
    }

}