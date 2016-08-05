#ifndef ParticlesGenerator_hpp
#define ParticlesGenerator_hpp

#include <vector>
#include "particles.hpp"

namespace particles {
    
    namespace generators {
        
        class PosGen : public ParticleGenerator {
            
        public:
            
            glm::vec4 mPos;
            glm::vec4 mStartPosOffsetMax;
            
        public:
            
            PosGen(void) : mPos(0.0), mStartPosOffsetMax(0.0) { }
            
            virtual void generate(double dt, ParticleData* p, unsigned int startId, unsigned int endId);
        };
        
        class ColourGen : public ParticleGenerator {
            
        public:
            
            glm::vec4 mStartColMin, mStartColMax;
            glm::vec4 mEndColMin, mEndColMax;
            
        public:
            
            ColourGen(void) : mStartColMin(0.0), mStartColMax(0.0), mEndColMin(0.0), mEndColMax(0.0) { }
            
            virtual void generate(double dt, ParticleData* p, unsigned int startId, unsigned int endId) override;
        };
        
        class VelGen : public ParticleGenerator {
        
        public:
            
            float mVelMin, mVelMax;
            
        public:
            
            VelGen(void) : mVelMin(0.0), mVelMax(0.0) { }
            
            virtual void generate(double dt, ParticleData* p, unsigned int startId, unsigned int endId) override;
        };
        
        class TimeGen : public ParticleGenerator {
            
        public:
            
            float mTimeMin, mTimeMax;
            
        public:
            
            TimeGen(void) : mTimeMin(0.0), mTimeMax(0.0) { }
            
            virtual void generate(double dt, ParticleData* p, unsigned int startId, unsigned int endId) override;
        };
    }
}

#endif /* ParticlesGenerator_hpp */