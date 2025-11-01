#pragma once

namespace Garbox {

void AssertDebug(bool value, const char* context, const char* message);
void AssertExit(bool value, const char* context, const char* message);

}