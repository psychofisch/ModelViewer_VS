#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	string input;
	//while (1)
	//{
		cout << "Please enter the path to a *.obj File (or \"1\" for the default object):";
		cin >> input;
		ofSetupOpenGL(1024, 768, OF_WINDOW);
		ofRunApp(new ofApp(input));
	//}
}
