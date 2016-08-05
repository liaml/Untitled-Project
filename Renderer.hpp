#ifndef ParticleRenderer_hpp
#define ParticleRenderer_hpp

#include <memory>

namespace particles {
    
    class ParticleSystem;
    
    class ParticleRenderer {

    protected:
        
        ParticleSystem* mSystem { nullptr };
        
        unsigned int mPosBuffer { 0 };
        unsigned int mColBuffer { 0 };
        unsigned int mVAO { 0 };
        
    public:
        
        ParticleRenderer(void) { }
        ~ParticleRenderer(void) { }
        
        void generate(ParticleSystem* sys);
        
        void destroy(void);
        void update(void);
        void render(void);
    };
}

#endif /* ParticleRenderer_hpp */
