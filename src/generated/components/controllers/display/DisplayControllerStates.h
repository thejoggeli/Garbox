// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#include "generated/states/types/DisplayStatusState.h"
#include "generated/states/types/DisplayDiagnosticsState.h"

namespace Garbox {

class DisplayControllerStates final {
public:

    DisplayControllerStates(
        DisplayStatusState& displayStatusState, // write
        DisplayDiagnosticsState& displayDiagnosticsState // write
    );

    // disallow copy and move
    DisplayControllerStates(const DisplayControllerStates&) = delete;
    DisplayControllerStates& operator=(const DisplayControllerStates&) = delete;
    DisplayControllerStates(DisplayControllerStates&&) = delete;
    DisplayControllerStates& operator=(DisplayControllerStates&&) = delete;

    // writable states
    DisplayStatusState& displayStatus;
    DisplayDiagnosticsState& displayDiagnostics;

};

} // namespace