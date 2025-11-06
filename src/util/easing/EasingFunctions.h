#pragma once
#include <math.h>

namespace Garbox {

class EasingFunctions {
public:

    struct ElasticParams {
        float amplitude;
        float period;
    };

    // ----------- Simple easings -----------
    static float EaseLinear(float t, const void* p = nullptr);
    static float EaseInQuad(float t, const void* p = nullptr);
    static float EaseOutQuad(float t, const void* p = nullptr);
    static float EaseInOutQuad(float t, const void* p = nullptr);
    static float EaseInSine(float t, const void* p = nullptr);
    static float EaseOutSine(float t, const void* p = nullptr);
    static float EaseInOutSine(float t, const void* p = nullptr);

    // ----------- Elastic easings -----------
    static float EaseInElastic(float t, const void* p = nullptr);
    static float EaseOutElastic(float t, const void* p = nullptr);
    static float EaseInOutElastic(float t, const void* p = nullptr);

    // ----------- Common parameter presets -----------
    static const ElasticParams* GetElasticLightParams();
    static const ElasticParams* GetElasticMediumParams();
    static const ElasticParams* GetElasticStrongParams();

};

} // namespace
