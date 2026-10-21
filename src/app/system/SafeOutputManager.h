#pragma once

namespace Garbox {

class SafeOutputManager {
public:
    static void Init();
    static void Apply();
    static bool IsApplied();
};

} // namespace Garbox
