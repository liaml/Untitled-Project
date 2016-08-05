#include "Particles.hpp"
#include <algorithm>
#include <iterator>

namespace particles {
    
    void ParticleData::generate(unsigned int maxSize) {
        
        mCount = maxSize;
        mCountAlive = 0;
        
        mPos.reset(new glm::vec4[maxSize]);
        mVel.reset(new glm::vec4[maxSize]);
        mAcc.reset(new glm::vec4[maxSize]);
        
        mCol.reset(new glm::vec4[maxSize]);
        mStartCol.reset(new glm::vec4[maxSize]);
        mEndCol.reset(new glm::vec4[maxSize]);
        
        mTime.reset(new glm::vec4[maxSize]);
        mAlive.reset(new bool[maxSize]);
    }
    
    void ParticleData::kill(unsigned int id) {
        
        mAlive[id] = false;
        swapData(id, (mCountAlive - 1));
        --mCountAlive;
    }
    
    void ParticleData::activate(unsigned int id) {
        
        mAlive[id] = true;
        ++mCountAlive;
    }
    
    void ParticleData::swapData(unsigned int a, unsigned int b) {
        
        // Swap dead particle with last particle in the alive list
        
        mPos[a] = mPos[b];
        mVel[a] = mVel[b];
        mAcc[a] = mAcc[b];
        
        mCol[a] = mCol[b];
        mStartCol[a] = mStartCol[b];
        mEndCol[a] = mEndCol[b];

        mTime[a] = mTime[b];
    }

    /// Particle Emitter Class
    
    void ParticleEmitter::emit(double dt, particles::ParticleData *p) {
        
        const unsigned int newParticlesMax = static_cast<unsigned int>(dt * mEmitRate);
        const unsigned int start = p->mCountAlive;
        const unsigned int end = std::min(start + newParticlesMax, (p->mCount - 1));
        
        for (auto gen = mGenerators.begin(); gen != mGenerators.end(); ++gen) {
            
            auto& genRef = *gen;
            genRef->generate(dt, p, start, end);
        }
        
        // wake particles
        for (unsigned int i = start; i < end; ++i) {
            p->activate(i);
        }
    }
    
    /// Particle System Class
    
    ParticleSystem::ParticleSystem(unsigned int maxCount) {
        
        // Bug: mAliveParticles remains on 0
        
        mCount = maxCount;
        mParticles.generate(maxCount);
        mAliveParticles.generate(maxCount);
        
        for (unsigned int i = 0; i < maxCount; ++i) {
            mParticles.mAlive[i] = false;
        }
    }
    
    void ParticleSystem::update(double dt) {
        
        
        for (auto em = mEmitters.begin(); em != mEmitters.end(); ++em) {
            
            auto& emRef = *em;
            emRef->emit(dt, &mParticles);
        }
        
        for (unsigned int i = 0; i < mCount; ++i) {
            mParticles.mAcc[i] = glm::vec4(0.0f);
        }
        
        // run the updaters
        for (auto up = mUpdaters.begin(); up != mUpdaters.end(); ++up) {
            
            auto& upRef = *up;
            upRef->update(dt, &mParticles);
        }
    }
}
