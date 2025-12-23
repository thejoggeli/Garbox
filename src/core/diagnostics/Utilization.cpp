#include "Utilization.h"

#include "core/assert/Assert.h"
#include "core/time/Time.h"

namespace Garbox {

static bool sIsBusy = false;
static uint32_t sLastTimestampMicros = 0;
static uint32_t sBusyAccumulatedMicros = 0;
static MovingAverageFilter sFilter = MovingAverageFilter(25, 1);

void Utilization::BeginBusy(){
    if(sIsBusy){
        return;
    }
    sLastTimestampMicros = Time::GetMicros();
    sIsBusy = true;
}

void Utilization::EndBusy(){
    if(!sIsBusy){
        return;
    }
    const uint32_t nowMicros = Time::GetMicros();
    sBusyAccumulatedMicros += nowMicros - sLastTimestampMicros;
    sLastTimestampMicros = nowMicros;
    sIsBusy = false;
}

void Utilization::Tick(){

    // close open busy interval
    if(sIsBusy){
        EndBusy();
    }

    sFilter.apply(sBusyAccumulatedMicros);
    sBusyAccumulatedMicros = 0;
}

uint32_t Utilization::GetBusyMicros(){
    return sFilter.getFilteredValue();
}

} // namespace
