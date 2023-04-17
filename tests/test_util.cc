#include "helens/helens.h"
#include <assert.h>

helens::Logger::ptr g_logger = HELENS_LOG_ROOT();

void test_assert() {
    HELENS_LOG_INFO(g_logger) << helens::BacktraceToString(10);
    //HELENS_ASSERT2(0 == 1, "abcdef xx");
}

int main(int argc, char** argv) {
    test_assert();

    int arr[] = {1,3,5,7,9,11};

    HELENS_LOG_INFO(g_logger) << helens::BinarySearch(arr, sizeof(arr) / sizeof(arr[0]), 0);
    HELENS_LOG_INFO(g_logger) << helens::BinarySearch(arr, sizeof(arr) / sizeof(arr[0]), 1);
    HELENS_LOG_INFO(g_logger) << helens::BinarySearch(arr, sizeof(arr) / sizeof(arr[0]), 4);
    HELENS_LOG_INFO(g_logger) << helens::BinarySearch(arr, sizeof(arr) / sizeof(arr[0]), 13);
    HELENS_ASSERT(helens::BinarySearch(arr, sizeof(arr) / sizeof(arr[0]), 0) == -1);
    HELENS_ASSERT(helens::BinarySearch(arr, sizeof(arr) / sizeof(arr[0]), 1) == 0);
    HELENS_ASSERT(helens::BinarySearch(arr, sizeof(arr) / sizeof(arr[0]), 2) == -2);
    HELENS_ASSERT(helens::BinarySearch(arr, sizeof(arr) / sizeof(arr[0]), 3) == 1);
    HELENS_ASSERT(helens::BinarySearch(arr, sizeof(arr) / sizeof(arr[0]), 4) == -3);
    HELENS_ASSERT(helens::BinarySearch(arr, sizeof(arr) / sizeof(arr[0]), 5) == 2);
    HELENS_ASSERT(helens::BinarySearch(arr, sizeof(arr) / sizeof(arr[0]), 6) == -4);
    HELENS_ASSERT(helens::BinarySearch(arr, sizeof(arr) / sizeof(arr[0]), 7) == 3);
    HELENS_ASSERT(helens::BinarySearch(arr, sizeof(arr) / sizeof(arr[0]), 8) == -5);
    HELENS_ASSERT(helens::BinarySearch(arr, sizeof(arr) / sizeof(arr[0]), 9) == 4);
    HELENS_ASSERT(helens::BinarySearch(arr, sizeof(arr) / sizeof(arr[0]), 10) == -6);
    HELENS_ASSERT(helens::BinarySearch(arr, sizeof(arr) / sizeof(arr[0]), 11) == 5);
    HELENS_ASSERT(helens::BinarySearch(arr, sizeof(arr) / sizeof(arr[0]), 12) == -7);
    return 0;
}
