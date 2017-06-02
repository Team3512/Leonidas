// Copyright (c) 2012-2017 FRC Team 3512. All Rights Reserved.

#pragma once

#include <list>

class RollingAverage {
public:
    explicit RollingAverage(uint32_t size);

    void addValue(float value);
    void setSize(uint32_t size);
    float getAverage();

private:
    // Holds values to be averaged
    std::list<float> m_values;

    // Number of values to average
    uint32_t m_size;
};
