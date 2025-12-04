#pragma once

namespace Garbox::LvglLittleFs {

void Init(char driveLetter);
bool IsInitialized();
char GetDriveLetter();

} // namespace