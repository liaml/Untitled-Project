#ifndef Particles_hpp
#define Particles_hpp

#include <vector>
#include <memory>
#include <detail/type_vec4.hpp>

namespace particles {
    
    /// Base Classes for Particle System Support
    
    class ParticleData {
        
    public:
        
        std::unique_ptr<glm::vec4[]> mPos, mVel, mAcc;
        
        std::unique_ptr<glm::vec4[]> mCol, mStartCol, mEndCol;
       
        std::unique_ptr<glm::vec4[]> mTime;
        std::unique_ptr<bool[]> mAlive;
        
        unsigned int mCount{ 0 };
        unsigned int mCountAlive{ 0 };
        
    public:
        
        ParticleData(void) { }
        explicit ParticleData(unsigned int maxCount) { generate(maxCount); }
        ~ParticleData(void) { }
                
        void generate(unsigned int maxSize);
        void kill(unsigned int id);
        void activate(unsigned int id);
        void swapData(unsigned int a, unsigned int b);
    };
    
    class ParticleGenerator {
        
    public:
        
        ParticleGenerator(void) { }
        virtual ~ParticleGenerator(void) { }
        
        virtual void generate(double dt, ParticleData *p, unsigned int startId, unsigned int endId) = 0;
    };
    
    class ParticleEmitter {
        
    protected:
        
        std::vector<std::shared_ptr<ParticleGenerator>> mGenerators;
        
    public:
        
        float mEmitRate{ 0.0 };
        
    public:
        
        ParticleEmitter(void) { }
        virtual ~ParticleEmitter(void) { }
        
        // calls the generators and at the end activates particle
        virtual void emit(double dt, ParticleData* p);
        
        void addGenerator(std::shared_ptr<ParticleGenerator> gen) { mGenerators.push_back(gen); }
    };
    
    class ParticleUpdater {
        
    public:
        
        ParticleUpdater(void) { }
        virtual ~ParticleUpdater(void) { }
        
        virtual void update(double dt, ParticleData* p) = 0;
    };
    
    class ParticleSystem {
        
    protected:
        
        ParticleData mParticles;
        ParticleData mAliveParticles;
        
        size_t mCount;
        
        std::vector<std::shared_ptr<ParticleEmitter>> mEmitters;
        std::vector<std::shared_ptr<ParticleUpdater>> mUpdaters;
        
    public:
        
        explicit ParticleSystem(unsigned int maxCount);
        virtual ~ParticleSystem(void) { };
        
        ParticleSystem(const ParticleSystem&) = delete;
        ParticleSystem& operator=(const ParticleSystem&) = delete;
        
        virtual void update(double dt);
        
        virtual unsigned int numAllParticles(void) const { return mParticles.mCount; }
        virtual unsigned int numAliveParticles(void) const { return mParticles.mCountAlive; }
        
        void addEmitter(std::shared_ptr<ParticleEmitter> em) { mEmitters.push_back(em); }
        void addUpdater(std::shared_ptr<ParticleUpdater> up) { mUpdaters.push_back(up); }
        
        ParticleData* finalData(void) { return& mParticles; }
    };
}

#endif /* Particles_hpp */