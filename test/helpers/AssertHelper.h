#pragma once

#include <unity.h>
#include "assert/AssertHandling.h"

namespace GarboxTest {

inline uint32_t AssertDebugCount = 0;
inline uint32_t AssertExitCount = 0;

inline void ResetAssertCounters(){
    AssertDebugCount = 0;
    AssertExitCount  = 0;
}

inline void InstallAssertHandlers(){
    ResetAssertCounters();
    Garbox::AssertHandling::SetDebugHandler([](const char*, const char*, int32_t){
        AssertDebugCount++;
    });
    Garbox::AssertHandling::SetExitHandler([](const char*, const char*, int32_t){
        AssertExitCount++;
    });
}

inline void ClearAssertHandlers(){
    Garbox::AssertHandling::SetDebugHandler(nullptr);
    Garbox::AssertHandling::SetExitHandler(nullptr);
}

} // namespace GarboxTest

#define TEST_ASSERT_DEBUG_CALLED() \
    TEST_ASSERT_TRUE_MESSAGE(GarboxTest::AssertDebugCount > 0, "Expected AssertDebug")

#define TEST_ASSERT_EXIT_CALLED() \
    TEST_ASSERT_TRUE_MESSAGE(GarboxTest::AssertExitCount > 0, "Expected AssertExit")

#define TEST_ASSERT_NO_ASSERTS() \
    TEST_ASSERT_EQUAL_UINT32(0, GarboxTest::AssertDebugCount); \
    TEST_ASSERT_EQUAL_UINT32(0, GarboxTest::AssertExitCount)

#define TEST_RESET_ASSERTS() \
    GarboxTest::ResetAssertCounters()
