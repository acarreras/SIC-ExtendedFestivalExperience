#pragma once

#define MAX_LENGTH_CENTROID         20
#define MAX_LENGTH_OPTICAL_FLOW     10

#include "ofMain.h"

class OpticalFlowVector{
    public:
        vector <ofPoint> centroidpoints;
        deque <ofPoint> opticalflows;
        int currentPosition;
        int length = MAX_LENGTH_CENTROID;

        OpticalFlowVector(){};

        void init(){
            currentPosition = 0;
            for(int i=0; i<length; i++){
              ofPoint p;
              p.set(0,0);
              centroidpoints.push_back(p);
            }

            for(int i=0; i<MAX_LENGTH_OPTICAL_FLOW; i++){
              ofPoint p;
              p.set(0,0);
              opticalflows.push_back(p);
            }
        }

        void setLengthAndReset(int l){
            length = max(l, MAX_LENGTH_CENTROID);
            centroidpoints.resize(length); // allocate space
            for(int i=0; i<centroidpoints.size(); i++){
              ofPoint p;
              p.set(0,0);
              centroidpoints[i] = p;
            }
        }

        int getLength(){
            return length;
        }

        int getNextPositionAndMoveToIt(){
            currentPosition++;
            if(currentPosition > length){
                currentPosition = 0;
            }
        }

        int getPrevPositionAndMoveToIt(){
            currentPosition--;
            if(currentPosition < 0){
                currentPosition = length-1;
            }
        }

        int getPrevPosition(int steps){
            int pos = currentPosition - steps;
            if(pos < 0){
                pos = length-steps;
            }
            return pos;
        }

        void setPointAt(int index, ofPoint p){
            centroidpoints[index] = p;
        }

        ofPoint getPointAt(int index){
            return centroidpoints[index];
        }

        void setPoint(ofPoint p){
            centroidpoints[currentPosition] = p;
        }

        ofPoint getPoint(){
            return centroidpoints[currentPosition];
        }

        ofPoint getOpticalFlow(int numStepsBefore){
            ofPoint res;
            res = getPointAt(currentPosition) - getPointAt(getPrevPosition(numStepsBefore));
            return res;
        }

        ofPoint getOFAt(int index){
            return opticalflows[index];
        }

        void setOpticalFlow(ofPoint p){
            opticalflows.pop_front();
            opticalflows.push_back(p);
        }

        ofPoint getSmoothOpticalFlow(){
            ofPoint res;
            res.set(0,0);
            for(int i=0; i<MAX_LENGTH_OPTICAL_FLOW-1; i++){
                res = res + (opticalflows[i] * 0.05); // 9*0.05 = 0.45
            }
            res = res + opticalflows[MAX_LENGTH_OPTICAL_FLOW-1]*0.55;
            return res;
        }
};
