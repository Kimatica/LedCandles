#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
//    ofSetVerticalSync(true);
    
    // - @60fps end devices have a noticeable lag. This might be caused by bytes accumulating in the buffers
    //   (bytes are not sent/read fast enough and accumulate).
    // - @30fps the lag is reduced. They have time to consume the incomming data as almost as it arrives.
    // - Lower fps will result in less lag but will reduced animation smoothness (a solution to this could be
    //   implementing interpolation in the and devices).
    ofSetFrameRate(30);
	
	ofBackground(0);
	
	serial.listDevices();
	vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
    
    int baud = 57600;
    serial.setup("/dev/tty.usbserial-AD0267J7", baud);
    
    redPct = 0;
    bluePct = 0;
    
    activeDevice = 0;
}


void ofApp::update(){
    float freq = ofMap(ofGetMouseX(), 0, ofGetWidth(), 0, 50);
    
    redPct = sin(ofGetElapsedTimef() * freq);
    redPct = (redPct < 0.f) ? 0 : 1;  // square wave
    //if(redPct < 0) redPct = 0;          // clamp
    bluePct = cos(ofGetElapsedTimef() * freq/2);
    bluePct = (bluePct < 0.f) ? 0 : 1;// square wave
    //if(bluePct < 0) bluePct = 0;        // clamp
    
    
    // Send data frames to the xbee:
    //
    // The number of bytes we send in each update is important.
    // The xbee needs some time to packetize and transmit these bytes wirelessly.
    // If we send too many, the xbee can´t keep up and there will be a lag,
    // or even lost of data if the serial buffer overflows.
    //
    // Our data messages are 5 bytes long.
    // Xbee will packetize and send when its serial buffer is full (100 bytes).
    // In 100 bytes we can fit 20 messages of 5 bytes (100/5=20)
    // With this value we can assume that xbee will transmit after each frame.
    
    
    // play with this value and framerate to find the sweet spot:
    // - @15fps 50 messages per frame works with no noticeable lag (high cpu use)
    // - @30fps 30 messages per frame works with no noticeable lag (high cpu use)
    // - @30fps 20 messages per frame works with no noticeable lag (low cpu use)
    int numMessages = 20;
    
    for(int i = 0; i < numMessages; i++) {
        float r = 0;
        float b = 0;
        
        if (activeDevice == i) {
            r = redPct;
            b = bluePct;
        }
        
        serial.writeByte(char(255));       // marker byte
        serial.writeByte(char(i));         // destination device id
        serial.writeByte(char(r*254));     // r
        serial.writeByte(char(0));         // g
        serial.writeByte(char(b*254));     // b
    }
    
    if (activeDevice >= 10) {
        activeDevice = 0;
    } else {
        //if (ofGetFrameNum() % 3 == 0) {
            activeDevice++;
        //}
    }
}


void ofApp::draw(){
    ofPushStyle();
    ofSetColor(redPct*255, 0, bluePct*255);
    ofDrawCircle(ofGetWidth()/2-100, ofGetHeight()/2, 100);
    ofPopStyle();
    
    ofDrawBitmapString(ofToString(ofGetFrameRate()), 15, 20);
    ofDrawBitmapString("target fps: " + ofToString(30), 15, 35);
    ofDrawBitmapString("messages per frame: " + ofToString(20), 15, 50);
}


void ofApp::keyPressed(int key){
    
}


void ofApp::keyReleased(int key){ 
    
}


void ofApp::mouseMoved(int x, int y){
	
}


void ofApp::mousePressed(int x, int y, int button){
	
}


void ofApp::mouseReleased(int x, int y, int button){
	
}
