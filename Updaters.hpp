#ifndef ParticleUpdaters_hpp
#define ParticleUpdaters_hpp

#include <vector>
#include "Particles.hpp"

namespace particles {
    
    namespace updaters {
        
        class EulerUpdater : public particles::ParticleUpdater {
            
        public:
            
            glm::vec4 mAccConst;
            
        public:
            
            EulerUpdater() : mAccConst(0.0) { }
            
            virtual void update(double dt, ParticleData* p) override;
        };
        
        class EffectUpdater : public particles::ParticleUpdater {
            
        protected:
            
            std::vector<glm::vec4> mAttractors; // 'w' component holds force magnitude
            
        public:
            
            virtual void update(double dt, ParticleData* p) override;
            
            unsigned int collectionSize(void) const { return (unsigned int)mAttractors.size(); }
            void add(const glm::vec4& attr) { mAttractors.push_back(attr); }
            glm::vec4& get(unsigned int id) { return mAttractors[id]; }
        };
        
        class VelColourUpdater : public ParticleUpdater {
            
        public:
            
            glm::vec4 mMinVel, mMaxVel;
            
        public:
            
            VelColourUpdater() : mMinVel(0.0), mMaxVel(1.0) { }
            
            virtual void update(double dt, ParticleData* p) override;
        };
        
        class TimeUpdater : public ParticleUpdater {
            
        public:
            
            virtual void update(double dt, ParticleData* p) override;
        };
    }
}

#endif /* ParticleUpdaters_hpp */