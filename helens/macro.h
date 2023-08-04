/**
 * @file macro.h
 * @brief 常用宏的封装
 * @author helens.yin
 * @email 564628276@qq.com
 * @date 2019-06-01
 * @copyright Copyright (c) 2019年 helens.yin All rights reserved (www.helens.top)
 */
#ifndef __HELENS_MACRO_H__
#define __HELENS_MACRO_H__

#include <string.h>
#include <assert.h>
#include "log.h"
#include "util.h"

#if defined __GNUC__ || defined __llvm__
/// LIKCLY 宏的封装, 告诉编译器优化,条件大概率成立
#   define HELENS_LIKELY(x)       __builtin_expect(!!(x), 1)
/// LIKCLY 宏的封装, 告诉编译器优化,条件大概率不成立
#   define HELENS_UNLIKELY(x)     __builtin_expect(!!(x), 0)
#else
#   define HELENS_LIKELY(x)      (x)
#   define HELENS_UNLIKELY(x)      (x)
#endif

/// 断言宏封装
#define HELENS_ASSERT(x) \
    if(HELENS_UNLIKELY(!(x))) { \
        HELENS_LOG_ERROR(HELENS_LOG_ROOT()) << "ASSERTION: " #x \
            << "\nbacktrace:\n" \
            << helens::BacktraceToString(100, 2, "    "); \
        assert(x); \
    }

/// 断言宏封装
#define HELENS_ASSERT2(x, w) \
    if(HELENS_UNLIKELY(!(x))) { \
        HELENS_LOG_ERROR(HELENS_LOG_ROOT()) << "ASSERTION: " #x \
            << "\n" << w \
            << "\nbacktrace:\n" \
            << helens::BacktraceToString(100, 2, "    "); \
        assert(x); \
    }

#endif
