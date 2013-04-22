#pragma once

//#define SYPHON
//#define OSC_MSG

#ifdef SYPHON
#include "ofxSyphon.h"
#endif
	
#ifdef OSC_MSG
#include "ofxOsc.h"
#endif	

#include "ofMain.h"


struct particle_props{
        u_int id;
        int rad;
        ofPoint shapeAxis;
        ofPoint loc;
        ofPoint vel;
        ofPoint acc;
        ofColor color;
        };

class Interaction{
    public:
        /*
         The iType variable is used inside the interact method
         to switch to the respective interaction process.
         The inher_props holds a given particle props
         to create an interacive particle.
         (Used in '3' iType interaction)
        */

        static ofPoint mouse_location;
        static int max_scaleDist;
        static int I1_weightFact;
        static int I2_weightFact;
        static int I1and2_relax_Fact;
        
        int iType;
        float weigth_fact;
        struct particle_props props;
        struct particle_props* inher_props;

        // Use location to set fix position interaction
        Interaction(int interaction_Id);
        // Method to update location
        void p_props_update(struct particle_props* p_props,float relax=1);        
        virtual void interact(struct particle_props* p_props);
        void mouseRepulsiveByLocation(struct particle_props* p_props);
        void mouseRepulsiveByAccelaration(struct particle_props* p_props);
        void assignedPointAtractiveByLocation(struct particle_props* p_props);
        void assignedPointAtractiveByAceleration(struct particle_props* p_props);
        void assignedParticleChangeColorByDist(struct particle_props* p_props);
#ifdef OSC_MSG
        void folowOscLocation(struct particle_props* p_props); 
#endif
        void assignedParticleRepulsiveByAccelaration(struct particle_props* p_props);
};

//fixed position atractive interaction by location
class Interaction_II : public Interaction{
    public:
        int newAttr;
        Interaction_II(int interaction_Id, ofPoint location);
        void interact(struct particle_props* p_props,int i);
};

class Particle {
    public:
        struct particle_props props;
        vector <Interaction> interactions;
        vector <Interaction>* interactions_FromParticlesSystem;
     
        // Constructor to setup the particle
        // initial location 
        Particle(ofPoint l=ofPoint(-1,-1));
        // Method that call the update and display methods 
        void run();        
        // Method to display
        void display();
        // Method to setup particle interactions
        void setup_grid_particleSpecificInteractions(ofPoint loc);
        void update_interactions();
        void enable_interaction(int iType);
        void disable_interaction(int iType);
        Interaction* getInteractionByTypeNum(int iType);
        void bound_particles_location();
};

class Particles_System {
    public:
        static ofPoint grid_offset;
        static int grid_ds;
        vector <Particle> particles;
        vector <Interaction> interactions;
        void init_regularGrid(const int particles_distance);
        void setup_grid_interactions();
        void enable_particlesInteraction(int iType, u_int particle_id=0); 
        void disable_particlesInteraction(int iType, u_int particle_id=0);
        void enable_systemInteractions(int iType, int numInteractionsToEnabel=1); 
        void disable_systemInteractions(int iType, int numInteractions=1);
        int count_enabeledSystemInteractions(int iType);
        void setup_interactive_particle(u_int interactive_particle_id, int iType);
        vector <Interaction*> getInteractionsByTypeNum(int iType);
        void run();
};


class testApp : public ofBaseApp{
	public:

                Particles_System grid_particles_system;
#ifdef SYPHON
	        ofxSyphonServer mainOutputSyphonServer;
#endif
#ifdef OSC_MSG
                static ofPoint osc_location;
                ofxOscReceiver oscReceiver;
                Particle oscPositionParticle;
	        void updateOscMessages();
#endif	

		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
};
