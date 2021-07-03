#pragma once
#include<opencv2/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/core/core.hpp> 

#include <map>
#include <iostream>
#include <vector>

#include "trackedobject.hpp"
#include "Calculation.hpp"

using ObjectDictionary = std::map<int, ObjectDetection::TrackedObject*>;
const double TRACKING_THRESHHOLD = 300.0f;

namespace ObjectDetection {

class TrackedObjectMonitor {

public:
    TrackedObjectMonitor();
    ~TrackedObjectMonitor();


    void add( int &id , TrackedObject &untracked_obj);
    void update(const int &id, TrackedObject &untracked_obj);
    void invalidateIndex(const int &id);

    void removeUntracked();
    bool checkForUpdate(TrackedObject &current_unspecified_obj);

    ObjectDictionary* get() const;

private:
    ObjectDictionary *monitor;
};
};