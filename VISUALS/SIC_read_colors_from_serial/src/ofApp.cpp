#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(true);
	ofSetLogLevel(OF_LOG_VERBOSE);

	serial.listDevices();
	vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();

	int baud = 9600;
	exit();
	serial.setup(0, baud);

	nBytesRead = 0;
	memset(bytesReadString, 0, 4);
}

//--------------------------------------------------------------
void ofApp::update(){

    if (serial.available() > 0 ){
		int nRead  = 0;  // a temp variable to keep count per read

		unsigned char bytesReturned[3];

		memset(bytesReadString, 0, 4);
		memset(bytesReturned, 0, 3);

		while( (nRead = serial.readBytes( bytesReturned, 3)) > 0){
			nTimesRead++;
			nBytesRead = nRead;
		};

		memcpy(bytesReadString, bytesReturned, 3);
		messageRead = "";
		messageRead = bytesReadString;
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	if(messageRead.find("b") != string::npos){
        ofBackground(0,0,200);
    }
    else if(messageRead.find("g") != string::npos){
        ofBackground(0,200,0);
    }
    else{
        ofBackground(200,200,200);
    }
    ofSetColor(0);
    ofDrawBitmapString(messageRead, 20,20);
}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

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

