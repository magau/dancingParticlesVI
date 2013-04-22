#include "testApp.h"

Interaction :: Interaction( int interaction_Id) {
   iType = interaction_Id;
}


/*
The "p_props" variable handles the properties of the
particle that will be actuated by the interactions.
*/
void Interaction :: interact(struct particle_props* p_props) {
  
   switch ( iType ) {
       //mouse repulsive interaction
       case 10:
           mouseRepulsiveByLocation(p_props);
           break;

       //mouse repulsive accelaration interaction
       case 1:
           mouseRepulsiveByAccelaration(p_props);
           break;

       ////fixed position atractive interaction
       //case 20:
       //    assignedPointAtractiveByLocation(p_props);
       //cout<<"loc: "<<props.loc<<endl;
       //    break;

       //fixed position atractive accelaration interaction
       case 2:
           assignedPointAtractiveByAceleration(p_props);
           break;

       //dynamic position (from other particle) change color
       case 3:
           assignedParticleChangeColorByDist(p_props);
           break;

#ifdef OSC_MSG
       //folow osc Location
       case 4:
           folowOscLocation(p_props);
           break;
#endif
       //dynamic position (from other particle) repulsive accelaration interaction 
       case 5:
           assignedParticleRepulsiveByAccelaration(p_props);
           break;
   }
}

#ifdef OSC_MSG
void Interaction :: folowOscLocation(struct particle_props* p_props) {
    //we use props.loc to handle the oscLocation
    p_props->loc = testApp::osc_location;
}
#endif

void Interaction :: assignedParticleRepulsiveByAccelaration(struct particle_props* p_props) {
    float dist,dx,dy,weight,weight_fact,min_dist,acc;
    min_dist = 2 * Particles_System::grid_ds;
    weight_fact = 0.5;
    weight = max_scaleDist*weight_fact;
    //weight = max_scaleDist*1/15;
    dist = ofDist(p_props->loc.x,p_props->loc.y,inher_props->loc.x,inher_props->loc.y);
    dx = p_props->loc.x - inher_props->loc.x;
    dy = p_props->loc.y - inher_props->loc.y;
    if (dist < min_dist) dist = min_dist;
    acc = weight / pow(dist,2);
    //p_props->acc.x = acc * dx / dist;
    //p_props->acc.y = acc * dy / dist;
    p_props->acc.x = - acc * dx / dist;
    p_props->acc.y = - acc * dy / dist;
    p_props_update(p_props);
}

void Interaction :: mouseRepulsiveByLocation(struct particle_props* p_props) {
    float dist,dx,dy,weight,weight_fact,min_dist,acc;
    min_dist = 2 * Particles_System::grid_ds;
    //weight = max_scaleDist/2;
#ifdef OSC_MSG
    //we use props.loc to handle the oscLocation
//    props.loc = testApp::osc_location;
    //weight_fact = I1_weightFact;
    weight_fact = 0.5;
#else
    props.loc = Interaction::mouse_location;
    weight_fact = 0.5;
#endif
    weight = max_scaleDist*weight_fact;

    dist = ofDist(p_props->loc.x,p_props->loc.y,props.loc.x,props.loc.y);
    dx = p_props->loc.x - props.loc.x;
    dy = p_props->loc.y - props.loc.y;
    if (dist < min_dist) dist = min_dist;
    acc = weight / pow(dist,2);
    p_props->loc.x += dx * acc;
    p_props->loc.y += dy * acc;
    p_props_update(p_props);
}


void Interaction :: mouseRepulsiveByAccelaration(struct particle_props* p_props) {
    float dist,dx,dy,weight,weight_fact,min_dist,acc;
    min_dist = 2 * Particles_System::grid_ds;
#ifdef OSC_MSG
    //we use props.loc to handle the oscLocation
    props.loc = testApp::osc_location;
    //weight_fact = I1_weightFact;
    weight_fact = 0.05;
#else
    props.loc = Interaction::mouse_location;
    weight_fact = 0.05;
#endif
    weight = max_scaleDist*weight_fact;
    //weight = max_scaleDist*1/15;
    dist = ofDist(p_props->loc.x,p_props->loc.y,props.loc.x,props.loc.y);
    dx = p_props->loc.x - props.loc.x;
    dy = p_props->loc.y - props.loc.y;
    if (dist < min_dist) dist = min_dist;
    acc = weight / pow(dist,2);
    //p_props->acc.x = acc * dx / dist;
    //p_props->acc.y = acc * dy / dist;
    p_props->acc.x = - acc * dx / dist;
    p_props->acc.y = - acc * dy / dist;
    p_props_update(p_props);
}

//void Interaction :: assignedPointAtractiveByLocation(struct particle_props* p_props) {
//    float dist,dx,dy,weight,weight_fact,min_dist,acc;
//    min_dist = 2 * Particles_System::grid_ds;
//    //weight = max_scaleDist*1/5;///2;
//#ifdef OSC_MSG
//    //weight_fact = I2_weightFact;
//    //relax_fact = I1and2_relax_Fact;
//    weight_fact = 0.17;
//    //relax_fact = 0.98;
//#else
//    weight_fact = 0.17;
//    //relax_fact = 0.98;
//#endif
//    weight = max_scaleDist*weight_fact;
//
//    dist = ofDist(p_props->loc.x,p_props->loc.y,props.loc.x,props.loc.y);
//    dx = p_props->loc.x - props.loc.x;
//    dy = p_props->loc.y - props.loc.y;
//    if (dist < min_dist) dist = min_dist;
//    acc = weight / pow(dist,2);
//    p_props->loc.x += - dx * acc;
//    p_props->loc.y += - dy * acc;
//    p_props_update(p_props);
//}

void Interaction :: assignedPointAtractiveByAceleration(struct particle_props* p_props){
    float dist,dx,dy,weight,weight_fact,relax_fact,min_dist,acc;
    min_dist = 2 * Particles_System::grid_ds;
#ifdef OSC_MSG
    //weight_fact = I2_weightFact;
    //relax_fact = I1and2_relax_Fact;
    weight_fact = 0.2;
    relax_fact = 0.98;
#else
    weight_fact = 0.2;
    relax_fact = 0.98;
#endif

    weight = max_scaleDist*weight_fact;
//    weight = max_scaleDist/2;//2/5;
    dist = ofDist(p_props->loc.x,p_props->loc.y,props.loc.x,props.loc.y);
    dx = p_props->loc.x - props.loc.x;
    dy = p_props->loc.y - props.loc.y;
    if (dist < min_dist) dist = min_dist;
    //if (-min_dist < dx && dx < min_dist) dx = (min_dist >= 0 ? 1 : -1) * min_dist;
    //if (-min_dist < dy && dx < min_dist) dy = (min_dist >= 0 ? 1 : -1) * min_dist;
    acc = weight / pow(dist,2);
    p_props->acc.x += - acc * dx / dist;
    p_props->acc.y += - acc * dy / dist;
    p_props_update(p_props,float(1.0835-2.709/(30*relax_fact+2.501)));
    //p_props_update(p_props,float(999.0/1000));
}

void Interaction :: assignedParticleChangeColorByDist(struct particle_props* p_props){
    float dist,weight,min_dist;
    min_dist = 2 * Particles_System::grid_ds;
    weight = 1.0/max_scaleDist;
    dist = ofDist(p_props->loc.x,p_props->loc.y,inher_props->loc.x,inher_props->loc.y);
    if (dist < min_dist) dist = min_dist;
    p_props->color = ofColor(int(255*(1-dist*weight)));
}

//Declatration of static members of 'Interaction class'
int Interaction :: max_scaleDist;
int Interaction :: I1_weightFact;
int Interaction :: I2_weightFact;
int Interaction :: I1and2_relax_Fact;
ofPoint Interaction :: mouse_location;

// Method to update location
void Interaction :: p_props_update(struct particle_props* p_props,float relax) {
    p_props->vel += p_props->acc/2;
    //relaxation
    p_props->vel *= relax;
    p_props->loc += p_props->vel;
}

Interaction_II :: Interaction_II(int interaction_Id, ofPoint location) : Interaction(interaction_Id){
            iType = interaction_Id;
            props.loc = location;
}

void Interaction_II :: interact(struct particle_props* p_props, int i) {
//    assignedPointAtractiveByLocation(p_props);
//void Interaction :: assignedPointAtractiveByLocation(struct particle_props* p_props) {
    float dist,dx,dy,weight,weight_fact,min_dist,acc;
    min_dist = 2 * Particles_System::grid_ds;
    cout<<"interact"<<endl;
//    //weight = max_scaleDist*1/5;///2;
//#ifdef OSC_MSG
//    //weight_fact = I2_weightFact;
//    //relax_fact = I1and2_relax_Fact;
//    weight_fact = 0.17;
//    //relax_fact = 0.98;
//#else
//    weight_fact = 0.17;
//    //relax_fact = 0.98;
//#endif
//    weight = max_scaleDist*weight_fact;
//
//    dist = ofDist(p_props->loc.x,p_props->loc.y,props.loc.x,props.loc.y);
//    dx = p_props->loc.x - props.loc.x;
//    dy = p_props->loc.y - props.loc.y;
//    if (dist < min_dist) dist = min_dist;
//    acc = weight / pow(dist,2);
//    p_props->loc.x += - dx * acc;
//    p_props->loc.y += - dy * acc;
//    p_props_update(p_props);
}

void Particle :: bound_particles_location(){
   int offset = props.rad;
   if (props.loc.x < offset){
       props.loc.x = offset;
   } else if (props.loc.x > ofGetWindowWidth()-offset){
       props.loc.x = ofGetWindowWidth()-offset;
   }
   if (props.loc.y < offset){
       props.loc.y = offset;
   } else if (props.loc.y > ofGetWindowHeight()-offset){
       props.loc.y = ofGetWindowHeight()-offset;
   }
}

void Particle :: setup_grid_particleSpecificInteractions(ofPoint init_location) {
    //fixed position atractive interaction by location
    interactions.push_back(Interaction_II(20,init_location));
    //cout<<"after iType 20: "<<interactions.size()<<endl;
    //interactions.push_back(Interaction(20,init_location));
/*
    //fixed position atractive interaction by accelaration
    interactions.push_back(Interaction(2,init_location));

    //mouse repulsive interaction by location
    interactions.push_back(Interaction(10));
    //mouse repulsive interaction by accelaration
    interactions.push_back(Interaction(1));
    //dynamic position (from other particle) change color
    interactions.push_back(Interaction(3));
*/
}

Interaction* Particle::getInteractionByTypeNum(int iType){
    int module_iType;
    Interaction* result = (Interaction*)NULL;
    for (u_int i=0;i<interactions.size();i++){
        module_iType = interactions[i].iType < 0 ? -1*interactions[i].iType : interactions[i].iType;
        if (module_iType == iType){
            result = &interactions[i];
            break;
        }
    }
    return result;
}

void Particle::enable_interaction(int iType){
    //If the iType is negative it is multiplied by -1 to be enabeled
    //cout<<"enabel p:"<<props.id<<" iType:"<<iType<<endl;
    if (getInteractionByTypeNum(iType)->iType < 0){
        getInteractionByTypeNum(iType)->iType *= -1;
    }
}

void Particle::disable_interaction(int iType){
    //The iType is multiplied by -1 to be disabeled 
    //cout<<"disable p:"<<props.id<<" iType:"<<iType<<endl;
    if (getInteractionByTypeNum(iType)->iType > 0){
        getInteractionByTypeNum(iType)->iType *= -1;
    }
}

void Particle :: update_interactions() {
    //Interact with the particle interactions:
    for (u_int i=0;i<interactions.size();i++){
        interactions[i].interact(&props,i);
        //cout<<"!"<<endl;
        //cout<<"iType: "<<interactions[i].iType<<endl;    
    }
    //Interact with the particles system interactions:
    //cout<<"interactions_FromParticlesSystem->size():"<<interactions_FromParticlesSystem->size()<<endl;
    for (u_int i=0;i<interactions_FromParticlesSystem->size();i++){
        //interactions_FromParticlesSystem[i]->interact(&props);
        (*interactions_FromParticlesSystem)[i].interact(&props);
        //cout<<"iType: "<<(*interactions_FromParticlesSystem)[i].iType<<endl;    
    }

    bound_particles_location();
}

Particle :: Particle(ofPoint init_location) {
    //set the initial position of the particle
    ////velocity = ofPoint(ofRandom(-1,1),ofRandom(-2,0));
    //velocity.set(ofRandom(-1,1),ofRandom(-2,0));
    //acceleration = ofPoint(0,0.05);
    props.loc = init_location;
    props.vel.set(0,0);
    props.acc.set(0,0);
    props.shapeAxis.set(6,6);
    props.rad=3;
    props.color = ofColor(255);
}

void Particle :: run() {
    update_interactions();
    display();
}


// Method to display
void Particle :: display() {
    //ofColor(...);
    //ofFill();
    ofSetColor(props.color);
    ofEllipse(props.loc.x,props.loc.y,props.shapeAxis.x,props.shapeAxis.y);
}

//Declatration of static members of 'testApp class'
ofPoint Particles_System::grid_offset;
int Particles_System::grid_ds;

//--------------------------------------------------------------
void Particles_System::init_regularGrid(int particles_distance){
    //This method ("constructor") builds a 2D regular grid particles system.
    u_int i,j,w,h;
    ofPoint thisParticle_init_location;
    ofPoint grid_num;

    grid_ds = particles_distance;
    w = ofGetWindowWidth();
    h = ofGetWindowHeight();
    grid_offset.x = (w - (w-2*grid_ds)/grid_ds * grid_ds)/2;
    grid_offset.y = (h - (h-2*grid_ds)/grid_ds * grid_ds)/2;
    grid_num.set(1+(w-2*grid_offset.x)/grid_ds,1+(h-2*grid_offset.y)/grid_ds);

    for (i=0; i<grid_num.y; i++){
        thisParticle_init_location.y = i*grid_ds + grid_offset.y;
        for (j=0; j<grid_num.x; j++){
            thisParticle_init_location.x = j * grid_ds + grid_offset.x;
            particles.push_back(Particle(thisParticle_init_location));
            particles[particles.size()-1].props.id = j + i * grid_num.x;
        }
    }
}

//--------------------------------------------------------------
void Particles_System::setup_grid_interactions(){
    /*
     Nota:
     All interactions depending from any particle
     location or other property retrieved from a 
     pointer pointing to particle.props address must
     be set after all the particles belonging to the
     system being added or every time a new particle
     is added or removed.
     This is due to the vector contents sequential
     arrangement.
    */
    u_int i;
    //Setting "max_scaleDist" iquals main window diagonal length.
    Interaction::max_scaleDist = ofDist(0,0,ofGetWindowWidth(),ofGetWindowHeight());
    Interaction::I1_weightFact = 0.08; 
    Interaction::I2_weightFact = 0.5; 
    Interaction::I1and2_relax_Fact = 0.98; 

    //Add all the interactions that will interact with the grid particle system:

#ifndef OSC_MSG
    //mouse repulsive interaction by location
    interactions.push_back(Interaction(10));
    //cout<<"after iType 10: "<<interactions.size()<<endl;
#endif
//    //mouse repulsive interaction by accelaration
//    interactions.push_back(Interaction(1));

/*
    //dynamic position (from other particle) change color
    interactions.push_back(Interaction(3));


    //fixed position atractive interaction by location
    interactions.push_back(Interaction(20,init_location));
    //fixed position atractive interaction by accelaration
    interactions.push_back(Interaction(2,init_location));
*/

//    cout<<"interactions_FromParticlesSystem.size():"<<interactions.size()<<endl;
    for (i=0;i<particles.size();i++){
    //Add pointer to the interactions vector of the particles system to the particle:
        particles[i].interactions_FromParticlesSystem = &interactions;
    //Add all the particle specific interactions:
        particles[i].setup_grid_particleSpecificInteractions(particles[i].props.loc);
    }

//    //Setup interactions:
//    // disabel mouse repulsive interaction by accelaration
//    disable_systemInteractions(1,getInteractionsByTypeNum(1).size()); 
//    // disabel fixed position atractive interaction by accelaration
//    disable_particlesInteractions(2,getInteractionsByTypeNum(2).size()); 

   // //Setting interactive particles:
   // //Setting particle id==255 as interactive particle
   // setup_interactive_particle(155,3);
   // //set interactive particle color red 
   // particles[155].props.color = ofColor(255,0,0);

}

//void Particles_System::setup_systemInteractive_particle(u_int interactive_particle_id, int iType){
//    u_int i;
//    //vector <>
//    for (i=0;i<particles.size();i++){
//        if (i == interactive_particle_id) {
//            //disabel color interaction from the 1st particle
//            particles[i].disable_interaction(iType);
//        } else {
//            // set inher_props equals 1st particle props
//            particles[i].getInteractionByTypeNum(iType)->inher_props = &(particles[interactive_particle_id].props);
//        }
//    }
//}
//
//void Particles_System::setup_interactive_particle(u_int interactive_particle_id, int iType){
//    u_int i;
//    //vector <>
//    for (i=0;i<particles.size();i++){
//        if (i == interactive_particle_id) {
//            //disabel color interaction from the 1st particle
//            particles[i].disable_interaction(iType);
//        } else {
//            // set inher_props equals 1st particle props
//            particles[i].getInteractionByTypeNum(iType)->inher_props = &(particles[interactive_particle_id].props);
//        }
//    }
//}

void Particles_System::enable_systemInteractions(int iType, int numInteractionsToEnabel){
    //If the iType is negative it is multiplied by -1 to be enabeled
    //cout<<"enabel p:"<<props.id<<" iType:"<<iType<<endl;
    vector <Interaction*> thisType_interactions = getInteractionsByTypeNum(iType);
    int enabeledInteractionsNum = count_enabeledSystemInteractions(iType);
    //cout<<"enabeledInteractionsNum:"<<enabeledInteractionsNum<<endl;

    if (enabeledInteractionsNum != numInteractionsToEnabel){
        for (u_int i=0; i < thisType_interactions.size(); i++){
            if ( enabeledInteractionsNum < numInteractionsToEnabel &&
                thisType_interactions[i]->iType < 0 ) {

                thisType_interactions[i]->iType *= -1;
                enabeledInteractionsNum++;

            } else if ( enabeledInteractionsNum > numInteractionsToEnabel &&
                        thisType_interactions[i]->iType > 0 ) {

                thisType_interactions[i]->iType *= -1;
                enabeledInteractionsNum--;

            }
        }
    }
}

int Particles_System::count_enabeledSystemInteractions(int iType){
    vector <Interaction*> thisType_interactions = getInteractionsByTypeNum(iType);
    int result = 0;
    for (u_int i=0; i < thisType_interactions.size(); i++){
        if ( thisType_interactions[i]->iType > 0 ){
            result++;
        }
    }
    return result;
}

vector <Interaction*> Particles_System::getInteractionsByTypeNum(int iType){
    int module_iType;
    vector <Interaction*> result;// = (vector <Interaction>*)NULL;
    for (u_int i=0; i < interactions.size(); i++){
        module_iType = interactions[i].iType < 0 ? -1*interactions[i].iType : interactions[i].iType;
        if (module_iType == iType){
            result.push_back(&interactions[i]);
            //break;
        }
    }
    return result;
}

//--------------------------------------------------------------
void Particles_System::enable_particlesInteraction(int iType,u_int particle_id){
    /*
     Nota:
     iTipe - Interaction id
     particle_id - (optional)
    */
    u_int i;
    for (i=0;i<particles.size();i++){
        //if (particle_id == (int)NULL || particle_id == particles[i].props.id){
        if (particle_id == 0 || i == particle_id){
            particles[i].enable_interaction(iType);
        }
    }
}

//--------------------------------------------------------------
void Particles_System::disable_particlesInteraction(int iType,u_int particle_id){
    /*
     Nota:
     iTipe - Interaction id
     particle_id - (optional)
    */
    u_int i;
    for (i=0;i<particles.size();i++){
        //if (particle_id == (int)NULL || particle_id == particles[i].props.id){
        if (particle_id == 0 || i == particle_id){
            particles[i].disable_interaction(iType);
        }
    }
}

//--------------------------------------------------------------
void Particles_System::run(){
    //cout<<"start run..."<<endl;
    for (u_int i=0; i < particles.size(); i++) {
        particles[i].run();
    }
    //cout<<"exit run."<<endl;
}

#ifdef OSC_MSG
//Declatration of static members of 'testApp class'
ofPoint testApp::osc_location;

//--------------------------------------------------------------
void testApp :: updateOscMessages(){
    ofPoint location;
    u_int i;
    vector <Interaction*> thisType_interactions;

    int iType = 10; //Provisorio
    //cout<<"updateOscMessages() start..."<<endl;
    if(!oscReceiver.hasWaitingMessages()){
        grid_particles_system.enable_systemInteractions(iType,0);
    } else {
        while (oscReceiver.hasWaitingMessages()){
            ofxOscMessage oscMsg;
            oscReceiver.getNextMessage( &oscMsg );
//            if ( strcmp( oscMsg.getAddress().c_str(), "/I1/loc" ) == 0 ) {
//                osc_location.set(oscMsg.getArgAsInt32(0),oscMsg.getArgAsInt32(1));
//            } else if( strcmp( oscMsg.getAddress().c_str(), "/I1/wF" ) == 0 ) {
//                Interaction::I1_weightFact = oscMsg.getArgAsInt32(0);
//            } else if( strcmp( oscMsg.getAddress().c_str(), "/I2/wF" ) == 0 ) {
//                Interaction::I2_weightFact = oscMsg.getArgAsInt32(0);
//            } else if( strcmp( oscMsg.getAddress().c_str(), "/I1&2/rF" ) == 0 ) {
//                Interaction::I1and2_relax_Fact = oscMsg.getArgAsInt32(0);
//            } else if( strcmp( oscMsg.getAddress().c_str(), "/I1&2/on" ) == 0 ) {
//               grid_particles_system.disable_systemInteraction(10); 
//               grid_particles_system.disable_systemInteraction(20); 
//               grid_particles_system.enable_systemInteraction(1); 
//               grid_particles_system.enable_systemInteraction(2); 
//            } else if( strcmp( oscMsg.getAddress().c_str(), "/I10&20/on" ) == 0 ) {
//               grid_particles_system.disable_systemInteraction(1); 
//               grid_particles_system.disable_systemInteraction(2); 
//               grid_particles_system.enable_systemInteraction(10); 
//               grid_particles_system.enable_systemInteraction(20); 
//            }

            thisType_interactions = grid_particles_system.getInteractionsByTypeNum(iType);

            if ( strcmp( oscMsg.getAddress().c_str(), "/topview/xyz" ) == 0 ) {
                u_int iNum = oscMsg.getArgAsInt32(0);
                    
                //cout<<"size:"<<thisType_interactions.size()<<" iNum:"<<iNum<<endl;
                if (thisType_interactions.size() < iNum){
                    //Add this type of interactions:
                    for (i=0; i < iNum - thisType_interactions.size(); i++){
                        grid_particles_system.interactions.push_back(Interaction(iType));
                        //grid_particles_system.interactions.push_back(Interaction(1));
                    }
                    //Update pointers:
                    for (i=0; i < grid_particles_system.particles.size(); i++){
                    //Add pointer to the interactions vector of the particles system to the particle:
                        grid_particles_system.particles[i].interactions_FromParticlesSystem = &grid_particles_system.interactions;
                    }

                    // disabel mouse repulsive interaction by accelaration
                    grid_particles_system.enable_systemInteractions(iType,thisType_interactions.size());
                    // disabel fixed position atractive interaction by accelaration
                    //disable_particlesInteractions(1,getInteractionsByTypeNum(2).size()); 

                } else if (thisType_interactions.size() >= iNum){
                    grid_particles_system.enable_systemInteractions(iType,iNum);
                }

                u_int updatedLocationInteractions = 0;

                for(i=0; i<thisType_interactions.size(); i++){
                    // if this is enabaled:
//                    cout<<"iType:"<<thisType_interactions[i]->iType<<" i:"<<i<<endl;
                    if(thisType_interactions[i]->iType > 0){
                        thisType_interactions[i]->props.loc.x = ofGetWindowWidth() * oscMsg.getArgAsFloat(1+i*3);
                        oscMsg.getArgAsFloat(2+i*3);
                        thisType_interactions[i]->props.loc.y = ofGetWindowHeight() * oscMsg.getArgAsFloat(3+i*3);
                        updatedLocationInteractions++;
                        if(updatedLocationInteractions == iNum)break;
                    }
                }

            }
            //cout<<"add:"<<oscMsg.getAddress()<<
            //       " x:"<<oscMsg.getArgAsInt32(0)<<
            //       " y:"<<oscMsg.getArgAsInt32(1)<<endl;
            ////cout<<ofGetElapsedTimeMillis()<<endl;
        }
    }
    //cout<<"exit"<<endl;
}
#endif

//--------------------------------------------------------------
void testApp::setup(){
    //ofSetFrameRate(1);
    ofSetVerticalSync(true);
    ofEnableSmoothing();
    // 20 pixels given for particle regular grid distance
    grid_particles_system.init_regularGrid(15);
    grid_particles_system.setup_grid_interactions(); 

#ifdef OSC_MSG
    oscReceiver.setup(12346);
    // For testing purpose of traking the osc location we add 
    // a green particle to folow the given position
    oscPositionParticle.interactions.push_back(Interaction(4));
    oscPositionParticle.props.color = ofColor(0,255,0);
#endif

#ifdef SYPHON
	mainOutputSyphonServer.setName("dancingParticules");
#endif

}

//--------------------------------------------------------------
void testApp::update(){
#ifdef OSC_MSG
    updateOscMessages();
#else
    Interaction::mouse_location.set(ofGetMouseX(),ofGetMouseY());
#endif
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0);
    grid_particles_system.run();
    //ofSleepMillis(3000):

#ifdef SYPHON
	mainOutputSyphonServer.publishScreen();
#endif

#ifdef OSC_MSG
    //oscPositionParticle.run();
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
