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

    //OSCControl
    oscControl.setup(7000);
    oscControl.addParameterGroup(&parameters);
    
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
    guiGrid.add(grid.parameters);
    
    gui.setup();
    parameters.setName("led_candles");
    parameters.add(bDrawGrid.set("draw_grid", true));
    parameters.add(useMovie.set("use_movie", true));
    parameters.add(movieOffsetX.set("movie_offsetX", 0, 0, 500));
    parameters.add(movieOffsetY.set("movie_offsetY", 0, 0, 500));
    parameters.add(drawMovie.set("draw_movie", true));
    
    gui.add(parameters);
    
    // video
    movie.load("movies/fire_loop.mp4");
    movie.setLoopState(OF_LOOP_NORMAL);
    movie.play();
}


void ofApp::update(){
    oscControl.update();
    
    if (useMovie) {
        // update video
        movie.update();
        // update grid
        grid.setFrom(movie.getPixels(), movieOffsetX, movieOffsetY);
    }
    else {
        // update grid
        grid.fade();
        grid.fillWithRandom();
    }
    
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
    
    if (drawMovie) {
        movie.draw(20,500);
        ofPushStyle();
        ofNoFill();
        ofDrawRectangle(20+movieOffsetX, 500+movieOffsetY, 80, 20);
        ofPopStyle();
    }
    
    ofDrawBitmapString(ofToString(ofGetFrameRate()), 15, 20+420);
    ofDrawBitmapString("target fps: " + ofToString(ofGetTargetFrameRate()), 15, 35+420);
    ofDrawBitmapString("messages per frame: " + ofToString(devices.size()), 15, 50+420);
    
    guiGrid.draw();
    gui.draw();
}



void ofApp::keyPressed(int key){
    if (key == OF_KEY_RETURN){
        oscControl.sendAllParameters("169.254.255.12", 8000);
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
