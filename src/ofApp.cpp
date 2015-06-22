#include "ofApp.h"
#include <math.h>
#include <iostream>
#include <fstream>

string getLightMode(ofLight);
string switchLightMode(ofLight &l);

//--------------------------------------------------------------
ofApp::ofApp(std::string input)
{
	modelPath = input;
}

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
	//model.loadModel("C:/Users/Psycho/Desktop/teapot_uv.obj", true);
	if (modelPath == "1")
		model.loadModel("G:/AnimationStuff/Models/AH-64_Apache/AH-64_Apache/AH-64_Apache.obj", true);
	else
		model.loadModel(modelPath, true);
	modelPosition.set(ofGetWidth() * 0.5, (float)ofGetHeight() * 0.75, 10);
	model.setPosition(modelPosition.x, modelPosition.y, modelPosition.z);
	rotz = 0;
	modelSelect = 0;
	model.enableMaterials();
	//***
	lightPosition = modelPosition;
	lightPosition.x += 300;
	light.setPosition((ofVec3f)lightPosition);
	light.setPointLight();
	oldX = mouseX;
	shading = GL_SMOOTH;
	wire = false;

	gui.setup(); // most of the time you don't need a name
	gui.add(wire.setup("Wireframe", false));
	gui.add(floor.setup("Floor", false));
	//gui.add(center.setup("Center", ofVec3f(ofGetWidth()*.5, ofGetHeight()*.5), ofVec3f(0, 0), ofVec3f(ofGetWidth(), ofGetHeight())));
	gui.add(center.setup("Model Position", ofVec3f(ofGetWidth()*.5, ofGetHeight()*.5), ofVec3f(0, 0, -1000), ofVec3f(1920, 1080, 1000)));
	gui.add(rotation.setup("Model Rotation", ofVec3f(0), ofVec3f(-180), ofVec3f(180)));
	gui.add(scale.setup("Model Scale", 1, 0.1, 10));
	gui.add(color.setup("Material Color", ofColor(100, 100, 140), ofColor(0, 0), ofColor(255, 255)));
	gui.add(lightColor.setup("Light Color", ofColor(255, 255) , ofColor(0, 0), ofColor(255, 255)));
	gui.add(lightMode.setup("Light Mode", "<unset>"));
	lightMode = getLightMode(light);
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

	modelPosition.set(center);
	model.setPosition(modelPosition.x, modelPosition.y, modelPosition.z);
	model.setRotation(0, rotation->x, 1, 0, 0);
	model.setRotation(1, rotation->y, 0, 1, 0);
	model.setRotation(2, rotation->z, 0, 0, 1);
	model.setScale(scale, scale, scale);

	//Light
	ofPushStyle();

	//ofSetColor(255);
	//ofSetColor(lightColor);
	light.setDiffuseColor((ofColor)lightColor);
	light.enable();
	mouseDiff = (mouseX - oldX)/2;
	lightPosition.y = mouseY;
	lightPosition.rotate(mouseDiff, (ofVec3f)modelPosition, ofVec3f(0, 1, 0));
	light.rotate(mouseDiff, 0, 0, 1);
	//lightPosition.set(mouseX, mouseY, modelPosition.z + 200);
	light.setPosition((ofVec3f)lightPosition);

	ofPopStyle();
	//****

	//Floor
	if (floor)
	{
		int fx, fy, fz;
		fx = modelPosition.x;
		fy = modelPosition.y;
		fz = modelPosition.z;
		ofPushStyle();
		ofSetColor(200, 100, 100);
		glBegin(GL_QUADS);
		glVertex3f(fx - 200, fy, fz - 200);
		glVertex3f(fx - 200, fy, fz + 200);
		glVertex3f(fx + 200, fy, fz + 200);
		glVertex3f(fx + 200, fy, fz - 200);
		glEnd();
		ofPopStyle();
	}
	//***

	ofPushStyle();
	//ofSetColor(ofColor(100, 255, 100));
	ofSetColor(color);
	ofPushMatrix();
	//ofTranslate(0, 0, 50);
	//ofRotate(mouseX, 1, 0, 0);
	if (wire == true)
		model.drawWireframe();
	else
		model.drawFaces();
	ofPopMatrix();
	ofPopStyle();

	ofDisableDepthTest();
	light.disable();
	ofDisableLighting();

	ofSetColor(0);
	ofDrawBitmapString("fps: " + ofToString(ofGetFrameRate(), 2), 500, 15);
	ofDrawBitmapString("modelpos: " + ofToString(modelPosition) , 500, 25);
	//ofDrawBitmapString("rotation: " + ofToString(rotation), 500, 35);
	ofDrawBitmapString("mousexy: " + ofToString(mouseX) + " - " + ofToString(mouseY), 500, 45);
	ofDrawBitmapString("lightpos: " + ofToString(lightPosition), 500, 55);

	oldX = mouseX;

	gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key)
	{
		case '1': modelPosition.z += 5;
			break;
		case '2': modelPosition.z -= 5;
			break;
		case '3': rotz = (rotz + 5) % 360;
			break;
		case '4': rotz = (rotz - 5) % 360;
			break;
		case 'm': modelSelect = (modelSelect+1)%2;
			break;
		case 'w': wire = !wire;
			break;
		case 's':	if (shading == GL_SMOOTH)
						shading = GL_FLAT;
					else
						shading = GL_SMOOTH;
			break;
		case 'l':	lightMode = switchLightMode(light);
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
	center = ofVec3f(w*0.5, h*0.5, 10);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

string getLightMode(ofLight l)
{
	cout << l.getType();
	if (l.getIsDirectional())
		return L_DIR;
	else if (l.getIsPointLight())
		return L_POINT;
	else if (l.getIsSpotlight())
		return L_SPOT;
	else
		return L_UNKNWN;
}

string switchLightMode(ofLight &l)
{
	string t = getLightMode(l);
	if (t == L_POINT)
		l.setDirectional();
	else if (t == L_DIR)
		l.setSpotlight();
	else //if (t == L_SPOT)
		l.setPointLight();

	return getLightMode(l);
}
