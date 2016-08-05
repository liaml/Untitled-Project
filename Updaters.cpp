#include "Updaters.hpp"
#include <algorithm>
#include <glm/common.hpp>
#include <glm/gtc/random.hpp>

namespace particles {
    
    namespace updaters {
    
        void EulerUpdater::update(double dt, particles::ParticleData* p) {
            
            const glm::vec4 globalA { dt * mAccConst.x,
                                      dt * mAccConst.y,
                                      dt * mAccConst.z,
                0.0 }; // gravity not used
            
            glm::vec4* acc = p->mAcc.get();
            glm::vec4* vel = p->mVel.get();
            glm::vec4* pos = p->mPos.get();
            
            // Basic Forward Euler integration scheme
            for (unsigned int i = 0; i < p->mCountAlive; ++i) {
            
                acc[i] += globalA;
                vel[i] += (float)dt * acc[i];
                pos[i] += (float)dt * vel[i];
            }
        }
        
        void EffectUpdater::update(double dt, particles::ParticleData* p) {
            
            glm::vec4* acc = p->mAcc.get();
            glm::vec4* pos = p->mPos.get();
            
            glm::vec4 offset;
            float force;
        
            for (unsigned int i = 0; i < p->mCountAlive; ++i) {
                
                for (unsigned int j = 0; j < (unsigned int)mAttractors.size(); j++) {
                    
                    // calculate new acceleration of particles due to points of attraction
                    offset.x = mAttractors[j].x - pos[i].x;
                    offset.y = mAttractors[j].y - pos[i].y;
                    offset.z = mAttractors[j].z - pos[i].z;
                    
                    force = mAttractors[j].w / glm::dot(offset, offset); // .w is the force component
                    acc[i] += offset * force; // a
                }
            }
        }
        
        void VelColourUpdater::update(double dt, particles::ParticleData* p) {
                
            glm::vec4* col = p->mCol.get();
            glm::vec4* startCol = p->mStartCol.get();
            glm::vec4* endCol = p->mEndCol.get();
            
            glm::vec4* time = p->mTime.get();
            
            glm::vec4* vel = p->mVel.get();
            
            float scaleR, scaleG, scaleB;
            
            // Translate velocity into a RGBA colour value
            
            float diffR = mMaxVel.x - mMinVel.x;
            float diffG = mMaxVel.y - mMinVel.y;
            float diffB = mMaxVel.z - mMinVel.z;
            
            for (unsigned int i = 0; i < p->mCountAlive; ++i) {
                
                scaleR = (vel[i].x - mMinVel.x) / diffR;
                scaleG = (vel[i].y - mMinVel.y) / diffG;
                scaleB = (vel[i].z - mMinVel.z) / diffB;
                
                col[i].r = scaleR;
                col[i].g = scaleG;
                col[i].b = scaleB;
                col[i].a = glm::mix(startCol[i].a, endCol[i].a, time[i].z); // blend
            }
        }
        
        void TimeUpdater::update(double dt, particles::ParticleData* p) {
            
            unsigned int end = p->mCountAlive;
            
            glm::vec4* time = p->mTime.get();
            
            if (p->mCountAlive == 0) {
                return;
            }
            
            for (unsigned int i = 0; i < end; ++i) {
                
                time[i].x -= dt;
                
                // Interpolate values 0 (start of life) to 1 (end of life)
                time[i].z = 1.0f - (time[i].x * time[i].w); // .w is 1.0/max life time
                
                // Bug: killing wrong particle
                
                if (time[i].x < 0.0f) { // when particle reaches end of life, kill it
                    
                    p->kill(i);
                    
                    if (p->mCountAlive < p->mCountAlive) {
                        p->mCountAlive = p->mCount;
                    }
                }
            }
        }
    }
}