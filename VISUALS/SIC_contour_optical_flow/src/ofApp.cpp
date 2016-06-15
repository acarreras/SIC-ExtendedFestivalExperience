#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    // APP
    ofSetLogLevel(OF_LOG_SILENT);
    ofSetVerticalSync(true);
    //ofSetFrameRate(25);
    //ofHideCursor();
    w = ofGetWidth();
    h = ofGetHeight();

    // CAM
    camWidth = 320;
    camHeight = 240;
    vidGrabber.setDeviceID(1);
    vidGrabber.setVerbose(true);
    vidGrabber.initGrabber(camWidth, camHeight);

    colorImg.allocate(camWidth, camHeight);
	grayImage.allocate(camWidth, camHeight);
	grayTh.allocate(camWidth, camHeight);

	threshold = 80;
	minBlobSize = 20;
	maxBlobSize = w*h*0.3;
	numBlobs = 1;

	relAspectX = w/camWidth;
	relAspectY = h/camHeight;

	timeBetweenSendMessage = 500;

	// GUI
    gui = new ofxUISuperCanvas("SONAR INNOVATION CHALLENGE 2016", 0,0, w, h);
    gui->addSpacer();
    gui->addFPS();
    gui->addSpacer();
    gui->addSlider("threshold", 0, 255, &threshold);
    gui->addToggle("invert", &invert);
    gui->addToggle("mirror", &mirror);
    gui->addSlider("minBlobSize", 20, 9000, &minBlobSize);
    gui->addSlider("maxBlobSize", 2000, w*h, &maxBlobSize);
    gui->addSlider("numBlobs", 0, 10, &numBlobs)->setIncrement(1);
    gui->addSpacer();
    gui->addSlider("timeBetweenSendMessage(ms)", 10, 3000, &timeBetweenSendMessage);

    gui->autoSizeToFitWidgets();
    ofAddListener(gui->newGUIEvent,this,&ofApp::guiEvent);
    gui->loadSettings("gui.xml");

    // OPTICAL FLOW
    ofvector.init();

    // OSC
    sender.setup(HOST, PORT);
    initTimeSendMessage = ofGetElapsedTimeMillis();

}

//--------------------------------------------------------------
void ofApp::update(){
    ofBackground(0);

    bool bNewFrame = false;

	vidGrabber.update();
	bNewFrame = vidGrabber.isFrameNew();

    if(bNewFrame){
        colorImg.setFromPixels(vidGrabber.getPixels(), camWidth, camHeight);
        grayImage = colorImg;
        grayImage.mirror(false,mirror);
        grayTh = grayImage;
        grayTh.threshold(threshold, invert);
        grayTh.blurGaussian(blur);

        contourFinder.findContours(grayTh, minBlobSize, maxBlobSize, numBlobs, false);

        if(contourFinder.nBlobs > 0){
            // centroid
            centerX = contourFinder.blobs[0].boundingRect.getCenter().x; // / camWidth; // normalized
            centerY = contourFinder.blobs[0].boundingRect.getCenter().y; // / camHeight;
            ofvector.setPoint(ofPoint(centerX,centerY));
            ofvector.getNextPositionAndMoveToIt();

            // current optical flow
            resultOpticalFlow = ofvector.getOpticalFlow(5);

            // smoothed optical flow
            ofvector.setOpticalFlow(resultOpticalFlow);
            smoothOpticalFlow = ofvector.getSmoothOpticalFlow();
        }
    }

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetHexColor(0xffffff);
    grayImage.draw(w*0.5,0,camWidth*0.3, camHeight*0.3);
    grayTh.draw(w*0.5+camWidth*0.3,0,camWidth*0.3, camHeight*0.3);

    if(contourFinder.nBlobs > 0){ // we have somebody there
        // center of mass, blob centroid
        ofSetColor(255,255,0);
        ofFill();
        ofEllipse(centerX * relAspectX, centerY * relAspectY,15,15);

        // current optical flow
        ofSetColor(255,0,0);
        ofLine(centerX*relAspectX, centerY*relAspectY, (centerX+resultOpticalFlow.x)*relAspectX, (centerY+resultOpticalFlow.y)*relAspectY);

        // smoothed optical flow
        ofSetColor(0,255,0);
        ofLine(centerX*relAspectX, centerY*relAspectY, (centerX+smoothOpticalFlow.x)*relAspectX, (centerY+smoothOpticalFlow.y)*relAspectY);
        ofRect((centerX+resultOpticalFlow.x)*relAspectX, (centerY+resultOpticalFlow.y)*relAspectY, 3, 3);

        if(initTimeSendMessage - ofGetElapsedTimeMillis() > timeBetweenSendMessage){
            mx.clear();
            mx.setAddress("/opticalflowX");
            mx.addFloatArg(smoothOpticalFlow.x);
            sender.sendMessage(mx);

            my.clear();
            my.setAddress("/opticalflowY");
            my.addFloatArg(smoothOpticalFlow.y);
            sender.sendMessage(my);

            initTimeSendMessage = ofGetElapsedTimeMillis();
        }
        contourFinder.draw(0,0,w,h);
    }

    ofSetColor(200);
    ofDrawBitmapString("smooth optical flow: " + ofToString(smoothOpticalFlow.x) + "," + ofToString(smoothOpticalFlow.y), 20, 380);
    ofDrawBitmapString("optical flow vector values: ", 20, 400);
    for(int i=0; i<ofvector.getLength(); i++){
        ofDrawBitmapString( ofToString(ofvector.getPoint().x) + "," + ofToString(ofvector.getPoint().y), 20, 420+15*i);
    }

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'g' || key == 'G') {
        gui->toggleVisible();
    }
    if (key == 's' || key == 'S'){
		vidGrabber.videoSettings();
	}
    if (key == '+'){
		blur += 2;
	}
	if (key == '-'){
		blur -= 2;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}

//--------------------------------------------------------------
void ofApp::exit(){
    // GUI
    gui->saveSettings("gui.xml");
    delete gui;
}

//--------------------------------------------------------------
void ofApp::guiEvent(ofxUIEventArgs &e){
    string name = e.getName();
    int kind = e.getKind();

    //cout << "guiEvent" << endl;
    //cout << "  name: " << name << endl;
    //cout << "  kind: " << kind << endl;

    if(name == "novaEsfera"){

    }
}
