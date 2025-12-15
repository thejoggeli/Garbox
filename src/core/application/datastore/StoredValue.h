#pragma once

#include "core/time/Time.h"

namespace Garbox {

class DataStoreAbs;

/**
 * Template class to store a plain C++ value in data store.
 */
template<typename T>
class StoredValue {
public:

    T value() const {
        return mValue;
    }

    T lastValue() const {
        return mLastValue;
    }

    bool hasValue() const {
        return mHasValue;
    }

    bool hasLastValue() const {
        return mHasLastValue;
    }

    uint32_t lastUpdateTimeSeconds() const {
        return mLastUpdateTimeSeconds;
    }
    
    operator T() const {
        return mValue;
    }

private:

    StoredValue(){
        // nothing to do
    }

    ~StoredValue(){
        // nothing to do
    }

    T mValue = static_cast<T>(0);
    T mLastValue = static_cast<T>(0);
    bool mHasValue = false;
    bool mHasLastValue = false;
    uint32_t mLastUpdateTimeSeconds = 0;

    bool reset(){
        if(!mHasValue){
            return false;
        }
        mValue = static_cast<T>(0);
        mLastValue = static_cast<T>(0);
        mHasValue = false;
        mHasLastValue = false;
        return true;
    }

    bool update(T newValue){

        // check if value changes or becomes valid with this update
        bool changed = (newValue != mValue) || !mHasValue;

        // update last value
        mLastValue = mValue;
        mHasLastValue = true;

        // update value
        mValue = mValue;
        mHasValue = true;

        // update time
        mLastUpdateTimeSeconds = Time::GetTickSeconds();
        
        return changed;
    }

    friend class DataStore;
    friend class DataStoreAbs;

    // disallow copy and move 
    StoredValue(const StoredValue&) = delete;
    StoredValue& operator=(const StoredValue&) = delete;
    StoredValue(StoredValue&&) = delete;
    StoredValue& operator=(StoredValue&&) = delete;

};

} // namespace Garbox



