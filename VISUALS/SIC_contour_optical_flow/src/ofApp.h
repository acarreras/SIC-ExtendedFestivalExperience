#pragma once

#include "ofMain.h"
#include "ofxUI.h"
#include "ofxOpenCv.h"

#include "OpticalFlowVector.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void exit();

		// APP
        float w, h;
        float relAspectX;
        float relAspectY;

        // GUI
        void guiEvent(ofxUIEventArgs &e);
        ofxUISuperCanvas *gui;

        // CAM
        ofVideoGrabber vidGrabber;
        int camWidth, camHeight;

        ofxCvColorImage colorImg;
        ofxCvGrayscaleImage grayImage;
		ofxCvGrayscaleImage grayTh;

        ofxCvContourFinder contourFinder;

		float threshold;
		int blur = 1;
		bool invert = false;
		bool mirror = true;

		float minBlobSize, maxBlobSize, numBlobs;

		float centerX, centerY;

		// OPTICAL FLOW
		OpticalFlowVector ofvector;
		ofPoint resultOpticalFlow;

};
