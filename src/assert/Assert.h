#pragma once

namespace Garbox {

void FailDebug(const char* context, const char* message);
void FailExit(const char* context, const char* message);

void AssertDebug(bool value, const char* context, const char* message);
void AssertExit(bool value, const char* context, const char* message);

}