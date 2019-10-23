#pragma once

#include <algorithm>
#include <functional>

template<typename T, typename Compare>
T const& clamp(T const& value, T const& low, T const& high, Compare cmp) {
    if (cmp(value, low)) {
        return low;
    } else if (cmp(high, value)) {
        return high;
    }

    return value;
}

template<typename T>
T const& clamp(T const& value, T const& low, T const& high) {
    return clamp(value, low, high, std::less<T>());
}

