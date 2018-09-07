#pragma once

#include "ofMain.h"

using namespace glm;


class Device {

public:
    char id;
    // current color values (0..1)
    float r;
    float g;
    float b;
    // color interpolation
    float startR;
    float startG;
    float startB;
    float endR;
    float endG;
    float endB;
    uint64_t durationMillis;  // interpolation duration in millisecs
    uint64_t startMillis;     // system time when interpolation starts
    bool completed;           // interpolation completed
    // jitter amount per channel (0..1)
    float jitterR;
    float jitterG;
    float jitterB;
    //
    vec2 position;
    
    
    Device(char id) {
        this->id = id;
        r = 0;
        g = 0;
        b = 0;
        startR = 0;
        startG = 0;
        startB = 0;
        endR = 0;
        endG = 0;
        endB = 0;
        jitterR = 0;
        jitterG = 0;
        jitterB = 0;
        completed = true;
    }
    
    
    void setTargetColor() {
        startMillis = ofGetElapsedTimeMillis();
    }
    
    
    void update() {
        uint64_t elapsedMillis = ofGetElapsedTimeMillis() - startMillis;
        
        if (elapsedMillis < durationMillis) {
            float pct = float(elapsedMillis) / float(durationMillis);
            r = ofLerp(startR, endR, pct);
            
            
            // debug
            //printCurrentColor();
        }
        else if (!completed)
        {
            
        }
    }
    
    
    void draw() {
        
    }
};
