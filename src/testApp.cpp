#include "testApp.h"
#include "math.h"

//--------------------------------------------------------------
void testApp::setup(){
    //ofSetFrameRate(1);
    ofSetVerticalSync(true);
    ofEnableSmoothing();
    // 15 pixels given for particle regular grid distance
    main_particles_system.setup(15,ofPoint(4.0/5));
    regularGrid_interactions_system.setup(2,15,ofPoint(4.0/5));
    external_interactions.setup();
    mouse.startThread(true, false);

#ifdef OSC_MSG
    external_interactions.oscReceiver.setup(12346);
#endif
#ifdef SYPHON
    mainOutputSyphonServer.setName("dancingParticules");
#endif

}
   
//--------------------------------------------------------------
void testApp::update(){

  external_interactions.update(ofPoint(mouse.x,mouse.y));
  external_interactions.run( &(main_particles_system.particles) );

  regularGrid_interactions_system.run( &(main_particles_system.particles) );
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0);
    main_particles_system.run();

//ofSleepMillis(50);

#ifdef SYPHON
	mainOutputSyphonServer.publishScreen();
#endif

}
   
//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}

void testApp::exit() {
 
    // stop the thread
    mouse.stopThread();
}
