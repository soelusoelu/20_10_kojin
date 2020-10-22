#pragma once

//バイクアッドフィルタで扱うフィルタの種類
enum class FilterType {
    LOW_PASS_FILTER,
    HIGH_PASS_FILTER,
    BAND_PASS_FILTER,
    NOTCH_FILTER,
    LOW_PASS_ONE_POLE_FILTER,
    HIGH_PASS_ONE_POLE_FILTER
};
