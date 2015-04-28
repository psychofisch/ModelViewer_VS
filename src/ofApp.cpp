#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	//ofSetVerticalSync(true);

	//Background
	ofColor background1;
	ofColor background2;

	background1.set(200, 200, 200);
	background2.set(100, 100, 100);

	ofBackgroundGradient(background1, background2, OF_GRADIENT_LINEAR);
	//ofBackground(background2);
	//***

	//Load model
	model.loadModel("C:/Users/Psycho/Desktop/teapot_uv.obj", true);
	modelPosition.set(ofGetWidth() * 0.5, (float)ofGetHeight() * 0.75, 10);
	model.setPosition(modelPosition.x, modelPosition.y, modelPosition.z);
	rotz = 0;
	modelSelect = 0;
	model.enableMaterials();
	//***
	shading = GL_SMOOTH;
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackgroundGradient(ofColor::white, ofColor::grey, OF_GRADIENT_LINEAR);

	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	ofEnableDepthTest();
	glShadeModel(shading);

	ofPushStyle();
	ofSetColor(255);
	light.enable();
	light.setPosition(mouseX, mouseY, modelPosition.z + 200);
	ofPopStyle();

	//Floor
	int fx, fy, fz;
	fx = modelPosition.x;
	fy = modelPosition.y;
	fz = modelPosition.z;
	ofPushStyle();
	ofSetColor(200, 100, 100);
	glBegin(GL_QUADS);
	glVertex3f(fx-200, fy, fz-200);
	glVertex3f(fx-200, fy, fz+200);
	glVertex3f(fx+200, fy, fz-200);
	glVertex3f(fx+200, fy, fz+200);
	glEnd();
	ofPopStyle();
	//***

	ofPushStyle();
	ofSetColor(ofColor(100, 255, 100));
	ofPushMatrix();
	//ofTranslate(0, 0, 50);
	//ofRotate(mouseX, 1, 0, 0);
	model.drawFaces();
	ofPopMatrix();
	ofPopStyle();

	ofDisableDepthTest();
	light.disable();
	ofDisableLighting();

	ofSetColor(0);
	ofDrawBitmapString("fps: " + ofToString(ofGetFrameRate(), 2), 10, 15);
	ofDrawBitmapString("modelpos: " + ofToString(modelPosition) , 10, 25);
	ofDrawBitmapString("rotz: " + ofToString(rotz), 10, 35);
	ofDrawBitmapString("mousexy: " + ofToString(mouseX) + " - " + ofToString(mouseY), 10, 45);
	ofDrawBitmapString("lightpos: " + ofToString(light.getPosition()), 10, 55);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key)
	{
		case '1': modelPosition.z += 5;
			break;
		case '2': modelPosition.z -= 5;
			break;
		case '3': rotz += 5;
			break;
		case '4': rotz -= 5;
			break;
		case 'm': modelSelect = (modelSelect+1)%2;
			break;
		case 's':	if (shading == GL_SMOOTH)
						shading = GL_FLAT;
					else
						shading = GL_SMOOTH;
			break;
	}
	model.setPosition(modelPosition.x, modelPosition.y, modelPosition.z);
	model.setRotation(0, rotz % 360, 0, 1, 0);
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
