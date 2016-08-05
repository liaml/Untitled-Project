#include "Generators.hpp"
#include <algorithm>
#include <glm/common.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtc/constants.hpp>

namespace  particles {
    
    namespace generators {
        
        void PosGen::generate(double dt, particles::ParticleData* p, unsigned int startId, unsigned int endId) {
            
            glm::vec4 posMin { (mPos.x - mStartPosOffsetMax.x), (mPos.y - mStartPosOffsetMax.y), (mPos.z - mStartPosOffsetMax.z), 1.0 };
            glm::vec4 posMax { (mPos.x + mStartPosOffsetMax.x), (mPos.y + mStartPosOffsetMax.y), (mPos.z + mStartPosOffsetMax.z), 1.0 };
            
            for (unsigned int i = startId; i < endId; ++i) {
                p->mPos[i] = glm::linearRand(posMin, posMax); // Linear Random distribution cloud of particle positions calculated at each timestep
            }
        }
        
        void ColourGen::generate(double dt, ParticleData* p, unsigned int startId, unsigned int endId) {
            
            for (unsigned int i = startId; i < endId; ++i) {
    
                // calculated initial colour and end colour (ranged linear random distribution)
                p->mStartCol[i] = glm::linearRand(mStartColMin, mStartColMax);
                p->mEndCol[i] = glm::linearRand(mEndColMin, mEndColMax);
            }
        }
        
        void VelGen::generate(double dt, ParticleData* p, unsigned int startId, unsigned int endId) {
            
            float phi, theta, v, r;
            
            for (unsigned int i = startId; i < endId; ++i) {
                
                // xyz random velocity
                
                phi = glm::linearRand(-glm::pi<double>(), glm::pi<double>());
                theta = glm::linearRand(-glm::pi<double>(), glm::pi<double>());
                v = glm::linearRand(mVelMin, mVelMax);
                
                r = v * sinf(phi);
                p->mVel[i].z = v * cosf(phi);
                p->mVel[i].x = r * cosf(theta);
                p->mVel[i].y = r * sinf(theta);
            }
        }
        
        void TimeGen::generate(double dt, particles::ParticleData* p, unsigned int startId, unsigned int endId) {
            
            for (unsigned int i = startId; i < endId; ++i) {
                
                p->mTime[i].x = p->mTime[i].y = glm::linearRand(mTimeMax, mTimeMax);
                p->mTime[i].z = 0.0f;
                p->mTime[i].w = 1.0f / p->mTime[i].x;
            }
        }
    }
}