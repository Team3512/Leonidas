// Copyright (c) 2012-2017 FRC Team 3512. All Rights Reserved.

#include "RollingAverage.hpp"

#include <numeric>

RollingAverage::RollingAverage(uint32_t size) : m_size(size) {}

void RollingAverage::addValue(float value) {
    m_values.push_back(value);

    while (m_values.size() > m_size) {
        m_values.pop_front();
    }
}

void RollingAverage::setSize(uint32_t size) {
    // While the new size is less than old size, remove values
    while (size < m_size) {
        m_values.pop_front();
    }

    m_size = size;
}

float RollingAverage::getAverage() {
    return std::accumulate(m_values.begin(), m_values.end(), 0.f) /
           m_values.size();
}
