#include "InteractionSystem.h"    

void Interaction :: interact(Particle* actuated_particle){
}
 
void Interaction :: interact(vector<Particle>* particlesVect_ptr){
}

void Interaction :: run(Particle* actuated_particle){
    update();
    interact(actuated_particle);
}

Interaction_I :: Interaction_I(Particle_props init_props=Particle_props()) {
    type = 1;
    props = init_props;
    max_dist = ofDist(0,0,ofGetWindowWidth(),ofGetWindowHeight());
}

void Interaction_I :: interact(vector<Particle>* particlesVect_ptr){
    float dist,dx,dy,weight,weight_fact,acc;
    Particle* actuated_particle;

    weight_fact = 0.25;

    weight = max_dist*weight_fact;

    for (u_int j=0; j<particlesVect_ptr->size(); j++){
        actuated_particle = &(*particlesVect_ptr)[j];

        //dist = ofDist(actuated_particle->props.locat.x,actuated_particle->props.locat.y,props.locat.x,props.locat.y);
        dist = props.locat.distance(actuated_particle->props.locat);
        dx = actuated_particle->props.locat.x - props.locat.x;
        dy = actuated_particle->props.locat.y - props.locat.y;
        
        //if (dist < min_dist) dist = min_dist;
        //min_dist=40;
        //if (dist < min_dist) dist = 25;//min_dist;
        acc = weight / pow(dist,2);

        actuated_particle->props.accel.x += dx * acc;
        actuated_particle->props.accel.y += dy * acc;
        //Do not aplly relaxation at the bounthery wall  
        int offset = actuated_particle->props.rad;
        if (actuated_particle->props.locat.x > offset &&
            actuated_particle->props.locat.x < ofGetWindowWidth() - offset &&
            actuated_particle->props.locat.y > offset &&
            actuated_particle->props.locat.y < ofGetWindowHeight() - offset ){

            actuated_particle->props.relax_fact = 0.7;
        }
    }
}

Interaction_II :: Interaction_II(Particle_props init_props=Particle_props()) {
    id = 2;
    props = init_props;
    max_dist = ofDist(0,0,ofGetWindowWidth(),ofGetWindowHeight());
}

void Interaction_II :: interact(vector<Particle>* particlesVect_ptr){

       Particle* actuated_particle;
       float dist,dx,dy,weight,weight_fact,acc;
       weight_fact = 0.1;
       min_dist = 40;

       weight = max_dist*weight_fact;
       actuated_particle = &(*particlesVect_ptr)[props.id];
       //dist = ofDist(actuated_particle->props.locat.x,actuated_particle->props.locat.y,props.locat.x,props.locat.y);
       dist = props.locat.distance(actuated_particle->props.locat);
       dx = actuated_particle->props.locat.x - props.locat.x;
       dy = actuated_particle->props.locat.y - props.locat.y;
       
       if (dist < min_dist) dist = min_dist;
       acc = weight / pow(dist,2);

       actuated_particle->props.accel.x += - dx * acc;
       actuated_particle->props.accel.y += - dy * acc;

      // //Over relaxation
      // if (dist == min_dist) {
      //     actuated_particle->props.relax_fact *= 0.95;
      //     //actuated_particle->props.relax_fact *= 0.7;
      // }

}

Interaction_III :: Interaction_III(Particle_props init_props=Particle_props()) {
    type = 3;
    props = init_props;
    max_dist = ofDist(0,0,ofGetWindowWidth(),ofGetWindowHeight());
    timer = 0;
}

void Interaction_III :: interact(vector<Particle>* particlesVect_ptr){
    Particle* actuated_particle;
    float dist,weight,weight_fact,acc,size_ds;
    ofPoint ds, dir, wavePos;
    int wave_speed = 4;

    min_dist=80;
    weight_fact = 0.05;

    weight = max_dist*weight_fact;
    props.isAlive = false;
    for (u_int j=0; j<particlesVect_ptr->size(); j++){
        actuated_particle = &(*particlesVect_ptr)[j];
        ds.x = actuated_particle->props.locat.x - props.locat.x;
        ds.y = actuated_particle->props.locat.y - props.locat.y;
        size_ds = ofDist(0,0,ds.x,ds.y);
        dir.set(ds.x/size_ds,ds.y/size_ds);
        
        wavePos.set(props.locat.x + timer * dir.x, props.locat.y + timer * dir.y);
        //ds.x = actuated_particle->props.loc.x - props.loc.x ;
        //ds.y = actuated_particle->props.loc.y - props.loc.y ;
        ds.x = actuated_particle->props.locat.x - wavePos.x;
        ds.y = actuated_particle->props.locat.y - wavePos.y;

        //dist = ofDist(actuated_particle->props.loc.x,actuated_particle->props.loc.y,props.loc.x,props.loc.y);
        dist = ofDist(actuated_particle->props.locat.x,actuated_particle->props.locat.y,wavePos.x,wavePos.y);
        if (!props.isAlive && dist < max_dist) props.isAlive = true;
        
        if (dist < min_dist) dist = min_dist;
        //if (dist < min_dist) dist = 25;//min_dist;
        acc = weight / pow(dist,2);

        actuated_particle->props.accel.x += ds.x * acc;
        actuated_particle->props.accel.y += ds.y * acc;

//        cout<<"dir.x:"<<dir.x<<" wavePos.x:"<<wavePos.x<<" ds.x:"<<ds.x<<" dist:"<<dist<<endl;

        //Do not aplly relaxation at the bounthery wall  
        int offset = actuated_particle->props.rad;
        if (actuated_particle->props.locat.x > offset &&
            actuated_particle->props.locat.x < ofGetWindowWidth() - offset &&
            actuated_particle->props.locat.y > offset &&
            actuated_particle->props.locat.y < ofGetWindowHeight() - offset ){

            actuated_particle->props.relax_fact = 0.9;
            //actuated_particle->props.relax_fact = 0.7;
        }

   } 

   if (props.isAlive) timer += wave_speed;
    
}

Interaction_IV :: Interaction_IV(Particle_props init_props=Particle_props()) {
    id = 4;
    props = init_props;
    max_dist = ofDist(0,0,ofGetWindowWidth(),ofGetWindowHeight());
}

void Interaction_IV :: interact(vector<Particle>* particlesVect_ptr){
    
    int max_speed = 2;
    int max_t = 100;
    int min_t = max_t;
    // Get min timer value
    for (u_int i=0; i < generated_interactions.size(); i++ ){
        min_t = generated_interactions[i].timer;
    }

    //if (min_t >= max_t){//min_dist){
    if (ofDist(temp_loc.x,temp_loc.y,props.locat.x,props.locat.y) > max_speed && min_t >= max_t){//min_dist){
        generated_interactions.push_back(Interaction_III());
        generated_interactions.back().props.locat = props.locat;
    }
    temp_loc = props.locat;

    for (u_int i=0; i<generated_interactions.size(); i++){
        generated_interactions[i].interact( particlesVect_ptr );

        //Must be integrated in interactions manager...
        if (!generated_interactions[i].props.isAlive) {
            generated_interactions.erase(generated_interactions.begin()+i);
        }
    }

} 

void Interactions_System::setup(){

}

void Interactions_System::update(){

}

void Interactions_System::run(vector<Particle>* particlesVect_ptr){

}

void RegularGrid_IS::run(vector<Particle>* particlesVect_ptr){
    for (u_int i=0; i<interactions.size(); i++){
        interactions[i].interact( particlesVect_ptr );
    }
}

void RegularGrid_IS::setup(u_int interaction_type, int interactions_distance, ofPoint grid_size){
    //This method ("constructor") builds a 2D regular grid particles system.
    // particles_distance - pixels
    // grid_size - [0 < grid_size <1] Relative to window size.
    u_int i,j,win_w,win_h,grd_w,grd_h;
    Particle_props init_props;
    ofPoint grid_offset;
    ofPoint grid_num;
    init_props.veloc.set(0,0);
    init_props.accel.set(0,0);
    init_props.rad = 6;
    init_props.relax_fact = 1.0;
    init_props.color = ofColor(255);

    grid_ds = interactions_distance;
    win_w = ofGetWindowWidth();
    win_h = ofGetWindowHeight();
    grd_w = round(win_w*grid_size.x);
    grd_h = round(win_h*grid_size.y);
    
    //Speed relaxation controled by distance threshold in pixels,
    //this two variables can be assigned by osc or other external event.
    //props.relax_threshold = 2*grid_ds; //???Set props.relax_threshold >= 6
    //relax value [0 ; 1] - 0->max relaxation; 1->min relaxation
    props.relax_fact = 1.0;
    //1/x type cuve
    //props.relax_fact = float(1.0835-2.709/(30*props.relax+2.501));

    //grid_offset.x = (win_w - (grd_w-2*grid_ds)/grid_ds * grid_ds)/2;
    //grid_offset.y = (win_h - (grd_h-2*grid_ds)/grid_ds * grid_ds)/2;
    //grid_num.set(1+(win_w-2*grid_offset.x)/grid_ds,1+(win_h-2*grid_offset.y)/grid_ds);
    grid_offset.x = (win_w - grd_w)/2;
    grid_offset.y = (win_h - grd_h)/2;
    grid_num.set(grd_w/grid_ds , grd_h/grid_ds);

//One particle interaction for testing
//grid_num.set(1);
//init_props.loc.x = win_w/2;//
//init_props.loc.y = win_h/2;//
    //create all interactions:
    //interactionsTypeManager(interaction_type,grid_num.x*grid_num.y); 
    //setup all interactions:
    for (i=0; i<grid_num.y; i++){
        //init_props.loc.y = i*grid_ds + grid_offset.y;
        init_props.locat.y = i*grid_ds + grid_offset.y;
        for (j=0; j<grid_num.x; j++){
            //init_props.loc.x = j * grid_ds + grid_offset.x;
            init_props.locat.x = j * grid_ds + grid_offset.x;
            init_props.id = j+i*grid_num.x;
            interactions.push_back(Interaction_II(init_props));
            //pointer to the props (relax) value of the IS.
            interactions[j+i*grid_num.x].min_dist = grid_ds*2;
            interactions[j+i*grid_num.x].props_ptr = &props;
            //allInteractions_ptr[j+i*grid_num.x]->setup(init_props);
            //allInteractions_ptr[j+i*grid_num.x]->min_dist = grid_ds*2;//5.0/2.0;//2*grid_ds;5;//PORQUÊ???? 5 pixels; 1/(->0)->oo
            //allInteractions_ptr[j+i*grid_num.x]->props_ptr = &props;
        }
    }
}

void mouse_IS :: setup(){
    interactions.push_back(Interaction_I());
    //allInteractions_ptr[0]->min_dist = 5;//PORQUÊ???? 5 pixels; 1/(->0)->oo
}

void mouse_IS :: update(ofVec3f locat){
    interactions[0].props.locat = locat;
    //if(allInteractions_ptr[0]->type != default_type) {
    //    interactionsTypeManager();
    //    interactionsTypeManager(default_type,1);
    //}
    //allInteractions_ptr[0]->props.loc.set(ofGetMouseX(),ofGetMouseY());
}

void mouse_IS::run(vector<Particle>* particlesvect_ptr){
    for (u_int i=0; i<interactions.size(); i++){
        interactions[i].interact(particlesvect_ptr);
    }
}

void mouseWaveSrcGenerator_IS :: setup(){
    interactions.push_back(Interaction_IV());
}

void mouseWaveSrcGenerator_IS :: update(ofVec3f locat){
    interactions[0].props.locat = locat;
}

void mouseWaveSrcGenerator_IS::run(vector<Particle>* particlesvect_ptr){
    for (u_int i=0; i<interactions.size(); i++){
        interactions[i].interact(particlesvect_ptr);
    }
}


//void OSC_Interactions_System :: update(){
////       ofPoint location;
////       u_int i;
////       vector <Interaction*> thisType_interactions;
////   
//    //cout<<"updateOscMessages() start..."<<endl;
//    if(!oscReceiver.hasWaitingMessages()){
//        //clear all:
//        interactionsTypeManager();
//
//    } else {
//        
//        while (oscReceiver.hasWaitingMessages()){
//            ofxOscMessage oscMsg;
//            oscReceiver.getNextMessage( &oscMsg );
//
//            //thisType_interactions = grid_particles_system.getInteractionsByTypeNum(iType);
//  
//            if ( strcmp( oscMsg.getAddress().c_str(), "/topview/xyz" ) == 0 ) {
//                u_int nElements = oscMsg.getArgAsInt32(0);
//
//                if (nElements != allInteractions_ptr.size()){
//                    interactionsTypeManager(default_type,nElements);
//                }
//
//                for(u_int i=0; i<nElements; i++){
//                    allInteractions_ptr[i]->props.loc.x = ofGetWindowWidth() * oscMsg.getArgAsFloat(1+i*3);
//                    oscMsg.getArgAsFloat(2+i*3);
//                    allInteractions_ptr[i]->props.loc.y = ofGetWindowHeight() * oscMsg.getArgAsFloat(3+i*3);
//                }
//  
//            }
//        }
//    }
//    //cout<<"exit"<<endl;
//}

