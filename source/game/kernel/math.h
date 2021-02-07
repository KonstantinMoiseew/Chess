#pragma once

template <typename T>
int sgn(T val)
{
    return (T(0) < val) - (val < T(0));
}

template <typename T>
T min(T a, T b)
{
    if (a < b)
        return a;
    return b;
}

template <typename T>
T max(T a, T b)
{
    if (a > b)
        return a;
    return b;
}
