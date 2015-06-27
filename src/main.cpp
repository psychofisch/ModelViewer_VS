#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main(int argc, const char* argv[]){
	//DEBUG
	/*for (int i = 0; i < argc; i++)
	cout << "ECHO:" << argv[i] << endl;*/

	bool quit = false;
	string input;
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	if (argc <= 1)
	{
		cout << "Please enter the path to a 3D model or \"1\" for the default object(see ReadMe for supported file formats):";
		cin >> input;
	}
	else
		input = argv[1];
	//try{
		ofSetupOpenGL(1024, 768, OF_WINDOW);
		ofRunApp(new ofApp(input));
	/*}
	catch (exception e){
		cout << "Something went really wrong...";
		cout << e.what();
		quit = true;
	}*/
}
