#include "FermentationEngine.h"

#include "core/assert/Assert.h"

namespace Garbox {

FermentationEngine::FermentationEngine(){
    // nothing to do
}

FermentationEngine::Inputs& FermentationEngine::getInputs(){
    return mInputs;
}

const FermentationEngine::Outputs& FermentationEngine::getOutputs() const {
    return mOutputs;
}

void FermentationEngine::init(){
    AssertExit(!mInitialized, "FermentationEngine", "already initialized");
    mInitialized = true;
}

void FermentationEngine::reset(){
    AssertExit(mInitialized, "FermentationEngine", "not initialized");
    // TODO
}

void FermentationEngine::step(){
    AssertExit(mInitialized, "FermentationEngine", "not initialized");
    // TODO
}

} // namespace