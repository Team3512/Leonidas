// Copyright (c) 2012-2017 FRC Team 3512. All Rights Reserved.

#pragma once

/* Usage:
 *
 * This class counts the number of gear teeth which have passed using a Counter
 * and Hall's Effect sensor plugged into a DIO channel. It returns the RPM of
 * the shooter wheel given the gear ratio and number of teeth on the gear.
 */

#include <pthread.h>

#include <list>

#include <Counter.h>

#include "../RollingAverage.hpp"

class PhotoEncoder {
public:
    PhotoEncoder(uint32_t channel, uint32_t teeth, float gearRatio);
    ~PhotoEncoder();

    // Start and stop internal counter
    void start();
    void stop();

    void setTeethPerRevolution(uint32_t teeth);

    void setGearRatio(float ratio);

    // Set number of RPM values to average together
    void setAverageSize(uint32_t size);

    // Returns average RPM
    float getRPM();

    // Returns true if RPM is increasing on average
    bool isIncreasing();

    // Set sample rate of RPM data collection thread
    void setSampleRate(uint32_t sampleRate);

private:
    // Counts number of pulses from Hall's Effect sensor
    Counter m_counter;

    // Averages values from encoder
    RollingAverage m_rpmAverager;

    // Used for determining if motor is going backward
    std::list<float> m_rpmList;

    /* Represents conversion factor between RPM of gear and RPM of shooter
     * wheel
     */
    float m_gearRatio;

    // Number of teeth per revolution of gear
    uint32_t m_teeth;

    // Rate at which data collection thread retrieves data from counter
    float m_sampleRate;

    /* ===== Thread variables ===== */
    // Samples values from encoder at given time interval for averaging
    pthread_t m_sampleThread;

    // Used for getting and setting variables between main and sampling thread
    pthread_mutex_t m_dataMutex;

    volatile bool m_isRunning;

    /* Function ran by sampling thread; takes pointer to current class
     * instance as first argument
     */
    static void* threadFunc(void* object);
    /* ============================ */
};
