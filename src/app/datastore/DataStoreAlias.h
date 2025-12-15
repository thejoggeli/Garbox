#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "core/application/datastore/StoredValue.h"
#include "app/engine/HeaterEngineState.h"
#include "app/types/TempsenseState.h"
#include "modules/parts/fan/FanState.h"
#include "modules/parts/heatpad/HeatpadState.h"

namespace Garbox {

using StoredTempsenseState    = StoredValue<TempsenseState>;
using StoredBool              = StoredValue<bool>;
using StoredFloat             = StoredValue<float>;
using StoredUint32            = StoredValue<uint32_t>;
using StoredHeaterEngineState = StoredValue<HeaterEngineState>;
using StoredFanState          = StoredValue<FanState>;
using StoredHeatpadState      = StoredValue<HeatpadState>;

} // namespace Garbox