#include "TickRunner.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

namespace Garbox {

TickRunner::TickRunner(size_t maxTickPhaseHandlers, uint32_t periodMillis):
    mTickPhaseHandlers(maxTickPhaseHandlers),
    mPeriodMillis(periodMillis) {
    // nothing to do
}

void TickRunner::setTickStartHandler(Handler handler){
    mTickStartHandler = handler;
}

void TickRunner::setTickEndHandler(Handler handler){
    mTickEndHandler = handler;
}

void TickRunner::registerTickPhase(Handler handler, uint32_t delayMillis){
    if(!mTickPhaseHandlers.push({ handler, delayMillis })){
        TriggerExit("TickRunner", "register tick failed");
    }
}

void TickRunner::run(){

    TickType_t lastWakeTime = xTaskGetTickCount();

    while(true){

        // call tick start handler
        if(mTickStartHandler){
            mTickStartHandler();
        }

        // call tick phases handlers
        for(TickPhase& phase : mTickPhaseHandlers){

            // wait until delay time for current tick passed
            if(phase.delayMillis > 0){
                vTaskDelayUntil(&lastWakeTime, phase.delayMillis);
            }

            // call handler for current tick
            phase.handler();
        }

        // call tick end handler
        if(mTickEndHandler){
            mTickEndHandler();
        }

        // wait until period is complete
        vTaskDelayUntil(&lastWakeTime, mPeriodMillis);
    }
}

} //  namespace
