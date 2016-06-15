#pragma once

#define MAX_LENGTH          100

#include "ofMain.h"

class OpticalFlowVector{
    public:
        vector <ofPoint> points;
        int currentPosition;
        int length = MAX_LENGTH;

        OpticalFlowVector(){};

        void init(){
            currentPosition = 0;
            for(int i=0; i<length; i++){
              ofPoint p;
              p.set(0,0);
              points.push_back(p);
            }
        }

        void setLengthAndReset(int l){
            length = max(l, MAX_LENGTH);
            points.resize(length); // allocate space
            for(int i=0; i<points.size(); i++){
              ofPoint p;
              p.set(0,0);
              points[i] = p;
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
            points[index] = p;
        }

        ofPoint getPointAt(int index){
            return points[index];
        }

        void setPoint(ofPoint p){
            points[currentPosition] = p;
        }

        ofPoint getPoint(){
            return points[currentPosition];
        }

        ofPoint getOpticalFlow(int numStepsBefore){
            ofPoint res;
            res = getPointAt(currentPosition) - getPointAt(getPrevPosition(numStepsBefore));
            return res;
        }
};
