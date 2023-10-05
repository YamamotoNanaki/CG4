#pragma once

#define WarningDisabledNum(num) __pragma(warning(disable:num))

#define WarningDisabledPush __pragma(warning(push))
#define WarningDisabledPop __pragma(warning(pop))

#define WarningDisabledAll \
WarningDisabledNum(26812) \
WarningDisabledNum(2220) \
WarningDisabledNum(2143) \
WarningDisabledNum(2447) \
WarningDisabledNum(4430) \
WarningDisabledNum(2365) \
WarningDisabledNum(3646) \
WarningDisabledNum(2040) \
WarningDisabledNum(2065) \
WarningDisabledNum(2144) \
WarningDisabledNum(3861) \
WarningDisabledNum(1003) \

#define WarningDisabled \
WarningDisabledPush \
WarningDisabledAll