#include "ofApp.h"
#include <math.h>
#include <iostream>
#include <fstream>

//--------------------------------------------------------------
ofApp::ofApp(std::string input)
{
	modelPath = input;
}

void ofApp::setup(){
	bool FileExist;

	//Background
	ofColor background1;
	ofColor background2;

	background1.set(200, 200, 200);
	background2.set(100, 100, 100);

	ofBackgroundGradient(background1, background2, OF_GRADIENT_LINEAR);
	//***
	//Load model
	if (modelPath == "1")
		FileExist =	model.loadModel("G:/FH/Sem4/ECG/download/of_v0.8.4_vs_release/apps/myApps/ModelViewer_VS/DefaultObject/Apple.obj", true);
	else
		FileExist = model.loadModel(modelPath, true);
	if (!FileExist)
		exit();
	modelPosition.set(ofGetWidth() * 0.5, (float)ofGetHeight() * 0.75, 10);
	model.setPosition(modelPosition.x, modelPosition.y, modelPosition.z);
	rotz = 0;
	modelSelect = 0;
	model.enableMaterials();
	//***

	//Light
	light.setPointLight();
	light2.setPointLight();
	//***

	oldX = mouseX;
	shading = GL_SMOOTH;
	wire = false;

	gui.setup(); // most of the time you don't need a name
	gui.add(wire.setup("Wireframe", false));
	gui.add(floor.setup("Floor", false));
	gui.add(center.setup("Model Position", ofVec3f(ofGetWidth()*.5, ofGetHeight()*.5), ofVec3f(0, 0, -1000), ofVec3f(1920, 1080, 1000)));
	gui.add(rotation.setup("Model Rotation", ofVec3f(0), ofVec3f(-180), ofVec3f(180)));
	gui.add(scale.setup("Model Scale", 1, 0, 2));
	gui.add(color.setup("Material Color", ofColor(200, 200, 200), ofColor(0, 0), ofColor(255, 255)));
	gui.add(textureOn.setup("Texture", true));
	gui.add(lightColor.setup("Light Color", ofColor(200, 230, 255, 255) , ofColor(0, 0), ofColor(255, 255)));
	gui.add(lightMode.setup("Light Mode", "<unset>"));
	gui.add(light2On.setup("Light2", true));
	gui.add(light2Strength.setup("Light2 Strength", 0.9, 0, 1));
	hide = false;

	light.setPointLight();
	lightMode = getLightMode(light);

	lightRotate = false;
	modelRotate = false;
	modelScale = false;

	windowResized(1024, 768);

	//DEBUG
	vector<string> v = model.getMeshNames();
	for (int i = 0; i < model.getMeshCount(); i++)
		cout << i << ":" << v[i] << endl;
	material = model.getMaterialForMesh(0);
	cout << material.getDiffuseColor() << endl;
	cout << material.getShininess() << endl;
	cout << material.getAmbientColor() << endl;
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

	//Calculate the mouse movement between two frames
	mouseDiffX = (mouseX - oldX) / 2;
	mouseDiffY = (mouseY - oldY) / 2;

	//Model Position and Rotation
	modelPosition.set(center);
	model.setPosition(modelPosition.x, modelPosition.y, modelPosition.z);
	if (modelScale && mouseX != 0)
	{
		scale = ((float)mouseX / ofGetWindowWidth()) * 2;
	}
	if (modelRotate)
	{
		rotation = ((ofVec3f)rotation + ofVec3f(0, mouseDiffX, mouseDiffY));
		if (rotation->y > 180)
			rotation = ofVec3f(rotation->x, ((int)(rotation->y + 180) % 360) - 180, rotation->z);
		else if (rotation->y < -180)
			rotation = ofVec3f(rotation->x, ((int)(rotation->y - 180) % 360) + 180, rotation->z);

		if (rotation->z > 180)
			rotation = ofVec3f(rotation->x, rotation->y, ((int)(rotation->z + 180) % 360) - 180);
		else if (rotation->z < -180)
			rotation = ofVec3f(rotation->x, rotation->y, ((int)(rotation->z - 180) % 360) + 180);
	}
	model.setRotation(0, rotation->x, 1, 0, 0);
	model.setRotation(1, rotation->y, 0, 1, 0);
	model.setRotation(2, rotation->z, 0, 0, 1);
	model.setScale(scale, scale, scale);
	//***

	//Light
	light.setDiffuseColor((ofColor)lightColor);
	light.enable();
	if (light2On)
	{
		light2Color.set(lightColor);
		light2Color.setHueAngle(light2Color.getHueAngle() + 180);
		light2Color.setBrightness(light2Color.getBrightness()*light2Strength);
		light2.setDiffuseColor(ofColor(light2Color));
		light2.enable();
	}
	if (lightRotate)
	{
		lightPosition.y = mouseY;
		light2Position.y = mouseY+100;
		lightPosition.rotate(mouseDiffX, (ofVec3f)modelPosition, ofVec3f(0, 1, 0));
		light2Position.rotate(mouseDiffX, (ofVec3f)modelPosition, ofVec3f(0, 1, 0));
		light.setPosition((ofVec3f)lightPosition);
		light2.setPosition((ofVec3f)light2Position);
	}
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

	//Model Material, Color and Drawing
	ofPushStyle();
	ofSetColor(color);
	model.disableMaterials();
	if (textureOn)
		model.enableTextures();
	else
		model.disableTextures();

	if (wire == true)
		model.drawWireframe();
	else
	{
		model.drawFaces();
	}
	ofPopStyle();
	//***

	light.disable();
	light2.disable();
	ofDisableLighting();

	ofSetColor(lightColor);
	ofSphere(lightPosition, 10);
	if (light2On)
	{
		ofSetColor(light2Color);
		ofSphere(light2Position, 5);
	}

	ofDisableDepthTest();

	//DEBUG
	//ofSetColor(0);
	//ofDrawBitmapString("fps: " + ofToString(ofGetFrameRate(), 2), 500, 15);
	//ofDrawBitmapString("modelpos: " + ofToString(modelPosition) , 500, 25);
	//ofDrawBitmapString("mousexy: " + ofToString(mouseX) + " - " + ofToString(mouseY), 500, 45);
	//ofDrawBitmapString("lightpos: " + ofToString(lightPosition), 500, 55);

	//to calculate the difference from the mouse between frames
	oldX = mouseX;
	oldY = mouseY;

	//to see the gui
	if (!hide)
		gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key)
	{
		case 'm': modelSelect = (modelSelect+1)%2;
			break;
		case 'w': wire = !wire;
			break;
		case 't': textureOn = !textureOn;
			break;
		case 's':	if (shading == GL_SMOOTH)
						shading = GL_FLAT;
					else
						shading = GL_SMOOTH;
			break;
		case 'h':	hide = !hide;
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
	//cout << button;
	switch (button){
		case 0: lightRotate = true;
			break;
		case 1: modelScale = true;
			break;
		case 2: modelRotate = true;
			break;
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	switch (button){
		case 0: lightRotate = false;
			break;
		case 1: modelScale = false;
			break;
		case 2: modelRotate = false;
			break;
	}
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
	center = ofVec3f(w*0.5, h*0.5, 10);
	LightRepositioning();
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

string ofApp::getLightMode(ofLight l)
{
	//cout << l.getType();
	if (l.getIsDirectional())
		return L_DIR;
	else if (l.getIsPointLight())
		return L_POINT;
	else if (l.getIsSpotlight())
		return L_SPOT;
	else
		return L_UNKNWN;
}

string ofApp::switchLightMode(ofLight &l)
{
	//**ISSUE**
	//Point -> Directional -> Works
	//Directional -> Spot/Point -> Works
	//Spot -> Point -> BEEP! Light is still a Spot...
	//***
	//**ISSUE**
	//Directional: Model is just dark... -> check Light Direction!
	//***

	string t = getLightMode(l);
	if (t == L_POINT)
		l.setDirectional();
	//else if (t == L_DIR)
		//l.setSpotlight();
	else //if (t == L_SPOT)
		l.setPointLight();

	cout << getLightMode(l) << endl;
	return getLightMode(l);
}

void ofApp::LightRepositioning()
{
	lightPosition.set(center);
	light2Position.set(center);

	lightPosition.x *= 0.6;
	light.setPosition((ofVec3f)lightPosition);

	light2Position.z += center->x * 0.6;
	light2.setPosition((ofVec3f)light2Position);
}
