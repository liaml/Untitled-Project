#include "Renderer.hpp"
#include "Particles.hpp"

#include <glew.h>
#include <string>

namespace particles {
    
    void ParticleRenderer::generate(particles::ParticleSystem* sys) {
        
        mSystem = sys;
        
        const unsigned int count = sys->numAllParticles();
        
        glGenVertexArrays(1, &mVAO);
        glBindVertexArray(mVAO);
        
        glGenBuffers(1, &mPosBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, mPosBuffer);
        glBufferData(GL_ARRAY_BUFFER, (sizeof(float) * 4 * count), nullptr, GL_STREAM_DRAW);
        
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, false, sizeof(float) * 4, (void*)(sizeof(float) * 0));
        
        glGenBuffers(1, &mColBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, mColBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * count, nullptr, GL_STREAM_DRAW);
        
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, false, sizeof(float) * 4, (void*)(sizeof(float) * 0));
        
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    
    void ParticleRenderer::destroy(void) {
        
        if (mPosBuffer != 0) {
            
            glDeleteBuffers(1, &mPosBuffer);
            mPosBuffer = 0;
        }
        
        if (mColBuffer != 0) {
            
            glDeleteBuffers(1, &mColBuffer);
            mColBuffer = 0;
        }
    }
    
    void ParticleRenderer::update(void) {
        
        assert(mSystem != nullptr);
        assert(mPosBuffer > 0 && mColBuffer > 0);
        
        const unsigned int count = mSystem->numAliveParticles();
        if (count > 0) {
            
            glBindBuffer(GL_ARRAY_BUFFER, mPosBuffer);
            float* ptr = (float*)(mSystem->finalData()->mPos.get());
            glBufferSubData(GL_ARRAY_BUFFER, 0, count * sizeof(float) * 4, ptr);
            
            glBindBuffer(GL_ARRAY_BUFFER, mColBuffer);
            ptr = (float*)(mSystem->finalData()->mCol.get());
            glBufferSubData(GL_ARRAY_BUFFER, 0, count * sizeof(float) * 4, ptr);
            
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
    }
    
    void ParticleRenderer::render(void) {
        
        glBindVertexArray(mVAO);
        
        const unsigned int count = mSystem->numAliveParticles();
        if (count > 0) {
            glDrawArrays(GL_POINTS, 0, count);
        }
        
        glBindVertexArray(0);
    }
}