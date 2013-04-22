#include "ofMain.h"

class Particle_props{
    public:
        u_int id;
        ofVec3f locat;
        ofVec3f veloc;
        ofVec3f accel;
        ofPoint loc;
        ofPoint vel;
        ofPoint acc;
        ofColor color;
        int rad;
        float relax_threshold;
        float relax_fact;
        bool isAlive;
        Particle_props();
};

class Particle {
    public:
        Particle_props props;

        virtual void setup(Particle_props _props);
        virtual void run();
        void update();
        void bound_particles_location();
        void set_speedLimit(int max_speed);
        void enable_screenElasticBoundery();
        virtual void display();
};


class Particles_System {
    public:
        vector <Particle> particles;
        //set up the system particles initial position
        virtual void setup();
        //call the run() method for every particles
        void run();
};


class RegularGrid_PS : public Particles_System{
    public:
        void setup(int particles_distance, ofPoint grid_size=ofPoint(1));
};

