#pragma once

#include "FilterType.h"

struct FilterParam {
    FilterType type;
    float cutoffFrequency;
    float qualityFactor;
};
