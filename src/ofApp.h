#pragma once

#define L_POINT "Point"
#define L_DIR "Directional"
#define L_SPOT "Spot"
#define L_UNKNWN "Unknown"

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{

	public:
		ofApp(std::string);

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
		//string getLightMode(ofLight l);
		
		ofxAssimpModelLoader model;
		ofMaterial material;
		std::string modelPath;
		ofLight light;

		ofPoint modelPosition;
		ofPoint lightPosition;
		int rotz;
		int modelSelect;
		int shading;
		float oldX, mouseDiff;

		//bool wire;
		ofxPanel gui;
		ofxVec3Slider center;
		ofxVec3Slider rotation;
		ofxFloatSlider scale;
		ofxToggle wire;
		ofxToggle floor;
		ofxColorSlider color;
		ofxColorSlider lightColor;
		ofxLabel lightMode;
};
