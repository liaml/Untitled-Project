#include "Effect.hpp"
#include "UI.hpp"
#include <cmath>

using namespace particles;

bool Effect::initialise(unsigned int numParticles) {
    
    const unsigned int NO_PARTICLES = numParticles == 0 ? 100000 : numParticles;
    mSystem = std::make_shared<ParticleSystem>(NO_PARTICLES);
    
    mColGenerator = std::make_shared<generators::ColourGen>();
    mColGenerator->mStartColMin = glm::vec4{ 1, 1, 1, 1 };
    mColGenerator->mStartColMax = glm::vec4{ 1, 1, 1, 1 };
    mColGenerator->mEndColMin = glm::vec4{ 1, 1, 1, 0 };
    mColGenerator->mEndColMax = glm::vec4{ 1, 1, 1, 0.25 };
    
    mVelGenerator = std::make_shared<generators::VelGen>();
    mVelGenerator->mVelMin = 0.17f;
    mVelGenerator->mVelMax = 0.2f;
    
    mTimeGenerator = std::make_shared<generators::TimeGen>();
    mTimeGenerator->mTimeMin = 0.5f;
    mTimeGenerator->mTimeMax = 6.0f;
    
    // Emmitter A
    auto particleEmitterA = std::make_shared<ParticleEmitter>();
    
    particleEmitterA->mEmitRate = (float)NO_PARTICLES * 0.2f;
    
    mPosGenerator[0] = std::make_shared<generators::PosGen>();
    mPosGenerator[0]->mPos = glm::vec4{ 0.0, 0.0, 0.25, 0.0 };
    mPosGenerator[0]->mStartPosOffsetMax = glm::vec4{ 0.0, 0.0, 0.0, 0.0 };
    particleEmitterA->addGenerator(mPosGenerator[0]);
    
    particleEmitterA->addGenerator(mColGenerator);
    particleEmitterA->addGenerator(mVelGenerator);
    particleEmitterA->addGenerator(mTimeGenerator);
    
    mSystem->addEmitter(particleEmitterA);
    
    // Emitter B
    auto particleEmitterB = std::make_shared<ParticleEmitter>();
    
    particleEmitterB->mEmitRate = (float)NO_PARTICLES * 0.1f;
    
    mPosGenerator[1] = std::make_shared<generators::PosGen>();
    mPosGenerator[1]->mPos = glm::vec4{ 0.0, 0.0, -0.25, 0.0 };
    mPosGenerator[1]->mStartPosOffsetMax = glm::vec4{ 0.0, 0.0, 0.0, 0.0 };
    particleEmitterB->addGenerator(mPosGenerator[1]);
    
    particleEmitterB->addGenerator(mColGenerator);
    particleEmitterB->addGenerator(mVelGenerator);
    particleEmitterB->addGenerator(mTimeGenerator);
    
    mSystem->addEmitter(particleEmitterB);

    // Updaters
    auto timeUpdater = std::make_shared<updaters::TimeUpdater>();
    mSystem->addUpdater(timeUpdater);
    
    auto colourUpdater = std::make_shared<updaters::VelColourUpdater>();
    colourUpdater->mMinVel = glm::vec4{ -0.5f, -0.5f, -0.5f, 0.0f };
    colourUpdater->mMaxVel = glm::vec4{ 1.5f, 1.5f, 1.5f, 1.5f };
    mSystem->addUpdater(colourUpdater);
    
    // Attraction Positions
    mAttractors = std::make_shared<updaters::EffectUpdater>();
    mAttractors->add(glm::vec4{ 0.0, 0.65, 0.0, 1 }); // w component is the attraction force
    mAttractors->add(glm::vec4{ 0.0, -0.65, 0.0, 1 });
    
    mSystem->addUpdater(mAttractors);
    
    auto eulerUpdater = std::make_shared<updaters::EulerUpdater>();
    eulerUpdater->mAccConst = glm::vec4{ 0.0, 0.0, 0.0, 0.0 }; // no gravity
    mSystem->addUpdater(eulerUpdater);
    
    return true;
}

bool Effect::initialiseRenderer(void) {
    
    mRenderer = std::make_shared<ParticleRenderer>();
    mRenderer->generate(mSystem.get());
    
    return true;
}

void Effect::clean(void) {
    
    if (mRenderer) {
        mRenderer->destroy();
    }
}

void Effect::addUI() {
    
    // Variables are generateed for particles randomly distributed by the constrainment range [a, b]
    UI::AddVariableRW("Particles Min Life Span", &mTimeGenerator->mTimeMin, "group = Effect min = 0 max = 9 step = 0.1");
    UI::AddVariableRW("Particles Max Life Span", &mTimeGenerator->mTimeMax, "group = Effect min = 1 max = 10 step = 0.1");
    UI::AddVariableRW("Particles Min Velocity", &mVelGenerator->mVelMin, "group = Effect min = 0 max = 0.9 step = 0.005");
    UI::AddVariableRW("Particles Max Velocity", &mVelGenerator->mVelMax, "group = Effect min = 0.1 max = 1.0 step = 0.005");
    UI::AddVariableRW("Point Atractor A", &mAttractors->get(0).w, "group = Effect min = 0 max = 2 step = 0.01");
    UI::AddVariableRW("Point Atractor B", &mAttractors->get(1).w, "group = Effect min = 0 max = 2 step = 0.01");
}

void Effect::update(double dt) {
    
    static double time = 0.0;
    time += dt;
    
    // make emitters folow a circular/helix like orbit path
    
    mPosGenerator[0]->mPos.x = 0.25f * sin((float)time * 2.5f);
    mPosGenerator[0]->mPos.y = 0.25f * cos((float)time * 2.5f);
    mPosGenerator[0]->mPos.z = 0.175f * cos((float)time * 2.5f);
    
    mPosGenerator[1]->mPos.x = -0.25f * sin((float)time * 2.0f);
    mPosGenerator[1]->mPos.y = 0.25f * cos((float)time * 2.0f);
    mPosGenerator[1]->mPos.z = 0.175f * cos((float)time * 2.5f);
}

void Effect::systemUpdate(double dt) {
    mSystem->update(dt);
}

void Effect::renderUpdate(double dt) {
    mRenderer->update();
}

void Effect::render() {
    mRenderer->render();
}
