#pragma once

namespace Garbox {

void TriggerDebug(const char* context, const char* message);
void TriggerExit(const char* context, const char* message);

void AssertDebug(bool value, const char* context, const char* message);
void AssertExit(bool value, const char* context, const char* message);

}