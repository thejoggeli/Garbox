#include "FermentationControlEngine.h"

#include "core/assert/Assert.h"

namespace Garbox {

FermentationControlEngine::FermentationControlEngine(){
    // nothing to do
}

FermentationControlEngine::Inputs& FermentationControlEngine::getInputs(){
    return mInputs;
}

const FermentationControlEngine::Outputs& FermentationControlEngine::getOutputs() const {
    return mOutputs;
}

void FermentationControlEngine::init(){
    AssertExit(!mInitialized, "FermentationControlEngine", "already initialized");
    mInitialized = true;
}

void FermentationControlEngine::reset(){
    AssertExit(mInitialized, "FermentationControlEngine", "not initialized");
    // TODO
}

void FermentationControlEngine::step(){
    AssertExit(mInitialized, "FermentationControlEngine", "not initialized");
    // TODO
}

} // namespace