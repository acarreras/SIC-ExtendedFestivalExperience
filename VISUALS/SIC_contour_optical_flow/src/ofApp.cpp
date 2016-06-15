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

    gui->autoSizeToFitWidgets();
    ofAddListener(gui->newGUIEvent,this,&ofApp::guiEvent);
    gui->loadSettings("gui.xml");

    // OPTICAL FLOW
    ofvector.init();

    // OSC
    sender.setup(HOST, PORT);

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
            float cx = contourFinder.blobs[0].boundingRect.getCenter().x; // / camWidth; // normalized
            float cy = contourFinder.blobs[0].boundingRect.getCenter().y; // / camHeight;
            ofvector.setPoint(ofPoint(cx,cy));
            ofvector.getNextPositionAndMoveToIt();
        }
    }

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetHexColor(0xffffff);
    grayImage.draw(w*0.5,0,camWidth*0.3, camHeight*0.3);
    grayTh.draw(w*0.5+camWidth*0.3,0,camWidth*0.3, camHeight*0.3);

    if(contourFinder.nBlobs > 0){
        // we have somebody there
        ofSetColor(255,255,0);
        ofFill();
        centerX = contourFinder.blobs[0].boundingRect.getCenter().x * relAspectX;
        centerY = contourFinder.blobs[0].boundingRect.getCenter().y * relAspectY;
        ofEllipse(centerX,centerY,15,15);
    }
    contourFinder.draw(0,0,w,h);

    ofSetColor(200);
    ofDrawBitmapString("optical flow vector values: ", 20, 20);
    for(int i=0; i<ofvector.getLength(); i++){
        ofDrawBitmapString( ofToString(ofvector.getPoint().x) + "," + ofToString(ofvector.getPoint().y), 20, 40+15*i);
    }

    resultOpticalFlow = ofvector.getOpticalFlow(5);
    ofvector.setOF(resultOpticalFlow);
    smoothOpticalFlow = ofvector.getSmoothOF();
    ofSetColor(255,0,0);
    ofLine(centerX, centerY, centerX+resultOpticalFlow.x, centerY+resultOpticalFlow.y);

    mx.clear();
    mx.setAddress("/opticalflowX");
    mx.addFloatArg(smoothOpticalFlow.x);
    sender.sendMessage(mx);

    my.clear();
    my.setAddress("/opticalflowY");
    my.addFloatArg(smoothOpticalFlow.y);
    sender.sendMessage(my);

    ofSetColor(200);
    ofDrawBitmapString("smooth optical flow: " + ofToString(smoothOpticalFlow.x) + "," + ofToString(smoothOpticalFlow.y), 200, 20);
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
