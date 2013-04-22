#include "ParticleSystem.h"
#include "ofMain.h"

class Interaction : public Particle {
    public:
        u_int id;
        u_int type;
        Particle_props* props_ptr;
        int max_dist;
        int min_dist; 
        int timer;
        float weigth_fact;
        virtual void run(Particle* actuated_particle);
        virtual void interact(Particle* actuated_particle);
        virtual void interact(vector<Particle>* particlesVect_ptr);

};

class Interaction_I : public Interaction{
    public:
        Interaction_I(Particle_props init_props);
        void interact(vector<Particle>* particlesVect_ptr);
};

class Interaction_II : public Interaction{
    public:
        Interaction_II(Particle_props init_props);
        void interact(vector<Particle>* particlesVect_ptr);
};

class Interaction_III : public Interaction{
    public:
        Interaction_III(Particle_props init_props);
        void interact(vector<Particle>* particlesVect_ptr);
};

class Interaction_IV : public Interaction{
    public:
        ofVec3f temp_loc;
        vector <Interaction_III> generated_interactions;
        Interaction_IV(Particle_props init_props);
        void interact(vector<Particle>* particlesVect_ptr);
};

class Interactions_System {
    public:
        u_int default_type; 
        Particle_props props;
        //vector <Interaction> interactions;
        //Add all the 'Interaction' derived classes
        //that you are planing to switch.
        //   vector <Interaction_I> interactions_I;
        // .
        // .
        // .

        //set up the interactions for the system particles

        virtual void setup();//u_int interaction_type, int interactions_distance, ofPoint grid_size=ofPoint(1));
        virtual void update();
        virtual void run(vector<Particle>* particlesVect_ptr);
};

class RegularGrid_IS : public Interactions_System {
    public:
        vector <Interaction_II> interactions;
        int grid_ds;
        void run(vector<Particle>* particlesVect_ptr);
        void setup(u_int interaction_type, int interactions_distance, ofPoint grid_size=ofPoint(1));
};

class mouse_IS : public Interactions_System {
    public:
        vector <Interaction_I> interactions;
        void setup();
        void update(ofVec3f locat);
        void run(vector<Particle>* particlesVect_ptr);
};

class mouseWaveSrcGenerator_IS : public Interactions_System {
    public:
        vector <Interaction_IV> interactions;
        int min_dist; 
        u_int default_type;
        void setup();
        void update(ofVec3f locat);
        void run(vector<Particle>* particlesVect_ptr);
};

//class OSC_Interactions_System : public Interactions_System{
//    public:
//        ofxOscReceiver oscReceiver;
//        //update() - read osc message; add\remove 
//        //           interactions from this interactions system.
//        void update();
//        //run() - update properties(example: location) for
//        //        all the interactions of this system.
//        void run();
//        //interact() - call the update_interactions() method of the actuated
//        //             particle or particle system with the 'allInteractionsPtr'
//        //             as input argument.
//        void interact();
//};

