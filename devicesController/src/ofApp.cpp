#include "ofApp.h"

using namespace glm;


void ofApp::setup(){
    ofSetVerticalSync(true);
    ofSetFrameRate(FPS);
	
	ofBackground(0);
	
    // open serial
	serial.listDevices();
	vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
    
    int baud = 57600;
    serial.setup("/dev/tty.usbserial-AD0267J7", baud);

    // init grid
    vec2 size(800, 200);
    vec2 resolution(80, 20);
    grid.setup(size, resolution);
    
    // init devices
    int stepX = size.x / NUM_DEVICES;
    int x = stepX / 2;
    int y = size.y / 2;
    for (int i = 0; i < NUM_DEVICES; i++) {
        Device device((unsigned char)i);
        device.position.x = x + i * stepX;
        device.position.y = y;
        devices.push_back(device);
    }
    
    // gui
    guiGrid.setup();
    gui.setup();
    
    gui.setPosition(ofGetWidth()-guiGrid.getWidth()-20, 0);
    guiGrid.setPosition(ofGetWidth()-guiGrid.getWidth()-20, 150);
    
    bSendHostInfo.set("<-- Change OSC Info", false);
    
    saveOscParams.add(bSendHostInfo);
    saveOscParams.add(oscHostName.set("Host Name"));
    saveOscParams.add(oscPort.set("Host Port"));
    gui.add(saveOscParams);
   
    parameters.setName("led_candles");
    parameters.add(bDrawGrid.set("draw_grid", true));
    parameters.add(grid.parameters);
    guiGrid.add(parameters);
    
    //preset
    panelPresets.setup("presets", 10);
    panelPresets.setPosition(805, 402);
    panelPresets.setParameters(&parameters);
    panelPresets.load();
    
    //OSCControl
    oscControl.setup(7000);
    oscControl.addParameterGroup(&parameters);
}

void ofApp::update(){
    oscControl.update();
    
    // update grid
    grid.fade();
    grid.fillWithRandom();

    // update devices
    for (auto& d : devices) {
        d.update(grid);
    }
    
    writeSerial();
}

void ofApp::writeSerial() {
    if (!serial.isInitialized())
        return;
    
    const unsigned char maxColValue = 254;
    for (auto& d : devices) {
        serial.writeByte(MARKER_BYTE);
        serial.writeByte(d.id);
        serial.writeByte( (d.r <= maxColValue) ? d.r : maxColValue);
        serial.writeByte( (d.g <= maxColValue) ? d.g : maxColValue);
        serial.writeByte( (d.b <= maxColValue) ? d.b : maxColValue);
    }
}

void ofApp::draw(){
    
    if (bDrawGrid) {
        grid.draw();
       
    }
    
    if (bSendHostInfo){
        oscControl.setRemoteHostName(oscHostName);
        oscControl.setRemotePort(oscPort);
        bSendHostInfo = !bSendHostInfo;
    }
    
    ofPushStyle();
    {
        ofSetLineWidth(2);
        for (auto& d : devices) {
            vec2 offsetPos( d.position.x, d.position.y + 200);
            ofSetColor(d.r, d.g, d.b);
            ofDrawCircle(offsetPos, 20);
            ofNoFill();
            ofSetColor(30);
            ofDrawCircle(offsetPos, 20);
            ofDrawRectangle(0, 200, 800, 200);
            ofFill();
            ofSetColor(255);
            ofDrawBitmapStringHighlight(ofToString(int(d.id)), offsetPos + vec2(-5, 50));
        }
    }
    ofPopStyle();
    
    ofDrawBitmapString(ofToString(ofGetFrameRate()), 15, 20+420);
    ofDrawBitmapString("target fps: " + ofToString(ofGetTargetFrameRate()), 15, 35+420);
    ofDrawBitmapString("messages per frame: " + ofToString(devices.size()), 15, 50+420);
    
    guiGrid.draw();
    gui.draw();
    
    panelPresets.draw();
}

void ofApp::exit(){
    panelPresets.save();
}



void ofApp::keyPressed(int key){
    if (key == OF_KEY_RETURN){
        oscControl.sendAllParameters();
    }
}


void ofApp::keyReleased(int key){ 
    
}


void ofApp::mouseMoved(int x, int y){
	
}


void ofApp::mousePressed(int x, int y, int button){
	
}


void ofApp::mouseReleased(int x, int y, int button){
	
}
