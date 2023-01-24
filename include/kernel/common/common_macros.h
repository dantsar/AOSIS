#pragma once

#define UNUSED(x) ((void)x)

#define CHECK_BIT_SET(num, index) ((num) & (1U << (index)))
#define SET_BIT(num, index)       ((num) | (1U << (index)))
#define CLEAR_BIT(num, index)     ((num) & ~(1U << (index)))
