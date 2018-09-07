#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{
	
public:
    void setup();
    void update();
    void draw();

    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    
    
    ofSerial	serial;
    
    float redPct;
    float bluePct;
    
    int activeDevice;
};

