#pragma once

#include "ofMain.h"
#include "Grid.h"

using namespace glm;


class Device {

public:
    unsigned char id;   // 0..244
    unsigned char r;    // 0..244
    unsigned char g;    // 0..244
    unsigned char b;    // 0..244
    vec2 position;
    
    
    Device(char id) {
        this->id = id;
        r = 0;
        g = 0;
        b = 0;
    }
    
    
    void update(const Grid& grid) {
        ofColor color = grid.getColorAtPosition(position);
        
       // cout << color << endl;
        r = color.r; //(float(color.r) / 255.f) * 244;
        g = color.g; //(float(color.g) / 255.f) * 244;
        b = color.b; //(float(color.b) / 255.f) * 244;
    }
};
