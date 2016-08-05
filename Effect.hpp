#ifndef Effect_hpp
#define Effect_hpp

#include <memory>
#include "Particles.hpp"
#include "Generators.hpp"
#include "Renderer.hpp"
#include "Updaters.hpp"

class Effect {
    
public:
    
    static const unsigned int DEFAULT_PARTICLE_NUM_FLAG { 0 };
    
private:
    
    std::shared_ptr<particles::ParticleSystem> mSystem;
    std::shared_ptr<particles::ParticleRenderer> mRenderer;
    
    std::shared_ptr<particles::generators::PosGen> mPosGenerator[5];
    std::shared_ptr<particles::generators::VelGen> mVelGenerator;

    std::shared_ptr<particles::generators::ColourGen> mColGenerator;
    std::shared_ptr<particles::generators::TimeGen> mTimeGenerator;
    std::shared_ptr<particles::updaters::EffectUpdater> mAttractors;
    
public:
    
    Effect(void) { }
    ~Effect(void) { }
    
    bool initialise(unsigned int numParticles);
    bool initialiseRenderer(void);
    
    void clean(void);
    
    void addUI(void);
    
    void update(double dt);
    void systemUpdate(double dt);
    void renderUpdate(double dt);
    
    void render(void);

    int numAllParticles(void) { return mSystem->numAllParticles(); }
    int numAliveParticles(void) { return mSystem->numAliveParticles(); }
};

#endif /* Effect_hpp */