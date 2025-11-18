#include "ButtonState.h"

namespace Garbox {

const char* ButtonStateToString(ButtonState state){
    switch(state){
    case ButtonState::Released: return "Released";
    case ButtonState::Pressed: return "Pressed";
    case ButtonState::PressedLong: return "PressedLong";
    default: return "Unknown";
    }
}

}