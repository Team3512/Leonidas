// Copyright (c) 2012-2017 FRC Team 3512. All Rights Reserved.

#pragma once

//! This class controls the shooter mechanism.
/**
 * How to use this class:
 *
 * Create an instance of this class.
 * The shooter can be controlled by setting either the rotational speed
 * (SetShooterRPM) or
 * the distance to the target (SetTargetDistance).
 * Call one of these methods when a change in rotation speed or target distance
 * is known.
 *
 * After setting or changing the target, call the 'Update' method during each
 * processing cycle.
 * This class will set the shooter output motor speeds as necessary.
 *
 * Call the 'IsReady' method to determine whether the shooter has stabilized at
 * the desired
 * rotational speed.
 *
 * In order to stop the shooter from rotating, set the rotational speed or
 * distance to zero.
 */

#include <Timer.h>

class Counter;
class Victor;

//------------------------------------------------------------------------------

typedef struct TimeRPM {
    float time;
    float RPM;
} TimeRPM;

double getRPM(Counter* counter);

class ShooterController {
public:
    //! Constructor
    ShooterController();

    //! Destructor
    ~ShooterController();

    // Must call this function before attempting any other method.
    void Initialize(Counter* pEncoder, Victor* pShooterMotorLeft,
                    Victor* pShooterMotorRight);

    //! Check this property to determine whether or not the shooter has
    //! stabilized at
    //! the required rotational speed for the specified distance/RPM setting.
    bool IsReady() { return m_bIsReady; }

    //! Set the target rotational speed of the shooter, in rotations per minute
    //! (RPM).
    //! This overrides any target distance setting.
    //! The shooter may be turned off by setting an RPM value of zero.
    void SetShooterRPM(float rpm);

    //! Set the distance from the target distance, in feet.
    //! This overrides any RPM setting.
    //! The shooter may be turned off by setting a distance of zero.
    void SetTargetDistance(float ft);

    //! This method must be called during each system update cycle.
    void Update();

private:
    Counter* m_pEncoder;
    Victor* m_pShooterMotorLeft;
    Victor* m_pShooterMotorRight;

    // Timer for retrieving time between RPM values
    // (Used for integration and derivate (delta) of errors)
    Timer m_timer;

    // The desired rotational speed in RPM
    float m_fTargetRPM;

    // The motor output that corresponds to target RPM
    float m_fTargetMotorOutput;

    // The last motor output value we set the motors to
    float m_fCurrentMotorOutput;

    // The last RPM value we used in setting motor output
    float m_fCurrentRPMOutput;

    // 'true' if the shooter has achieved desired rotational speed.
    bool m_bIsReady;

    // Whether or not we ever received a valid encoder value
    bool m_bGotGoodEncoderValue;

    // The last, good value from the encoder
    float m_fLastEncoderRPM;

    // Previous error, with which to compute derivative (delta) error
    TimeRPM m_fPreviousError;

    // Accumulated error (since reset)
    float m_fAccumulatedError;

    // Proportional control gain factors
    static float GAIN_KI;
    static float GAIN_KD;
    static float GAIN_KP;

    // Any RPM value within +/- this range is considered zero
    static float ZERO_RPM_BAND;

    // Any RPM value within +/- this range is considered target met
    static float RPM_RANGE_BAND;

    //----------------------------------------------------------------------
    // Table of relationship between motor control output and the
    // shooter's rotational speed
    typedef struct MotorRPMTableEntry {
        float motorValue;
        float rotationsPerMinute;
    };

    MotorRPMTableEntry* m_motorSpeedTable;  // an array of 'MotorRPMTableEntry'
    unsigned int m_motorSpeedTableLength;   // number of table entries
    //----------------------------------------------------------------------

    //----------------------------------------------------------------------
    // Table of relationship between target distance and shooter rotation
    // speed
    typedef struct TargetDistanceRPMTableEntry {
        float targetDistanceFt;
        float rotationsPerMinute;
    };

    TargetDistanceRPMTableEntry*
        m_distanceRPMTable;  // an array of 'TargetDistanceRPMTableEntry'
    unsigned int m_distanceRPMTableLength;  // number of table entries
    //----------------------------------------------------------------------

    // Helpers...

    //! Given a distance from the target, in feet, determine the desired speed
    //! of the shooter.
    float DetermineRPMFromTargetDistance(float distanceFt);

    //! Given a rotation speed, determine corresponding motor speed.
    float DetermineMotorSpeedFromRPM(float rpm);

    void InitializeMotorSpeedTable();
    void InitializeDistanceRPMTable();
    void ResetCalibration();
    void SetMotorOutputs(float motorValue);
};
