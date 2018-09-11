#pragma once

#include "ofMain.h"
#include "Device.h"
#include "Grid.h"
#include "ofxGui.h"
#include "ofxOscControl.h"
#include "ofxPresets.h"


class ofApp : public ofBaseApp{
	
public:
    void setup();
    void update();
    void draw();
    
    void writeSerial();

    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void onSendHostInfo();
    
    ofSerial serial;
    
    Grid grid;
    vector<Device> devices;
    
    //OSC Controler
    ofxOscControl oscControl;
    
    //Osc Sender
    ofxOscSender oscSender;
    
    // GUI
    ofxPanel guiGrid, gui;
//    ofxPanel gui;
    
    ofParameterGroup parameters;
    ofParameterGroup saveOscParams;
    
    ofParameter<bool> bDrawGrid;
    ofParameter<bool> bSendHostInfo;

    ofParameter<string> oscHostName;
    ofParameter<string> oscPort;

    ofxPresets panelPresets;
    
    // --------------------------
    // Constants
    // --------------------------
    
    const char MARKER_BYTE = 255;
    
    // - @60fps end devices have a noticeable lag. This might be caused by bytes accumulating in the buffers
    //   (bytes are not sent/read fast enough and accumulate).
    // - @30fps the lag is reduced. They have time to consume the incomming data as almost as it arrives.
    // - Lower fps will result in less lag but will reduced animation smoothness (a solution to this could be
    //   implementing interpolation in the and devices).
    //
    const int FPS = 30;
    
    // The number of bytes we send in each update is important.
    // The xbee needs some time to packetize and transmit these bytes wirelessly.
    // If we send too many, the xbee can´t keep up and there will be a lag,
    // or even lost of data if the serial buffer overflows.
    //
    // Our data messages are 5 bytes long (1 message per device).
    // Xbee will packetize and send when its serial buffer is full (100 bytes).
    // In 100 bytes we can fit 20 messages of 5 bytes (100/5=20)
    // With this value we can assume that xbee will transmit after each frame.
    //
    // Play with the number of devices and the framerate to find the sweet spot. Some tests:
    // - @15fps 50 devices per frame works with no noticeable lag (high cpu use)
    // - @30fps 30 devices per frame works with no noticeable lag (high cpu use)
    // - @30fps 20 devices per frame works with no noticeable lag (low cpu use)
    //
    const int NUM_DEVICES = 13;
};

