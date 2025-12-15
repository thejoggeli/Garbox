#pragma once

#include "core/application/datastore/StoredValue.h"

namespace Garbox {
    
class DataStoreAbs {
protected:

    DataStoreAbs();
    ~DataStoreAbs();

private:

    // disallow copy and move
    DataStoreAbs(const DataStoreAbs&) = delete;
    DataStoreAbs& operator=(const DataStoreAbs&) = delete;
    DataStoreAbs(DataStoreAbs&&) = delete;
    DataStoreAbs& operator=(DataStoreAbs&&) = delete;

};

} // namespace 