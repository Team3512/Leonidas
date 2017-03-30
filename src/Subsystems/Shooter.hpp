// Copyright (c) FRC Team 3512, Spartatroniks 2012-2017. All Rights Reserved.

#pragma once

/* Usage:
 *
 * Once the shooter is initialized, the PID controller always runs in the
 * background. The following control modes are available:
 *
 * * In each mode, F is always 1 / maxSpeed, so the feed forward term
 *   is always a decimal between 0 and 1 inclusive.
 *
 * PID:
 *     * The P, I, and D terms are set to predetermined values and used in
 *       conjunction with the feed forward term to facilitate faster
 *       convergence to the target RPM.
 *
 * BangBang:
 *     * When PIDWrite(1) is called, the value computed with the PID loop is
 *       ignored and the correct value is calculated in during the function
 *       call.
 *     * If the current RPM is above the target RPM, the motors are shut off.
 *       When the current RPM drifts to below the target RPM, the motors are
 *       given full power until the current RPM rises above the target RPM
 *       again.
 *
 * Manual:
 *     * The P, I, and D terms are set to 0 so the feed forward term is the
 *       only value returned from the PID loop.
 *     * Without the error correction of the three other terms, the PID loop
 *       essentially becomes an open loop, as the setpoint, the only variable
 *       affecting the feed forward term, is set directly by the user.
 *
 */

#include <PIDController.h>
#include <PIDOutput.h>
#include <PIDSource.h>
#include <Victor.h>

#include "PhotoEncoder.hpp"

class Shooter : public PIDSource, public PIDOutput {
public:
    static const float maxSpeed;

    Shooter(uint32_t motor1, uint32_t motor2, uint32_t encChannel,
            uint32_t encTeeth, float encGearRatio);
    virtual ~Shooter();

    typedef enum { PID = 0, BangBang, Manual } ControlMode;

    void start();
    void stop();
    bool isShooting();

    // Returns true when the shooter RPM is on target
    bool isReady();

    // Set shooter to the given RPM
    void setRPM(float wheelSpeed);

    // Set shooter to a fraction of the maximum speed [0..1]
    void setScale(float scaleFactor);

    // Returns current RPM from encoder
    float getRPM();

    // Get setpoint of PID loop
    float getTargetRPM();

    // Change and get with what control method the target RPM is to be reached
    void setControlMode(ControlMode mode);
    ControlMode getControlMode();

    void setPID(float p, float i, float d);

private:
    Victor m_shooterMotor1;
    Victor m_shooterMotor2;

    PhotoEncoder m_shooterEncoder;

    PIDController m_shooterPID;
    float m_setpoint;

    bool m_isShooting;

    ControlMode m_controlMode;

    // Flips between true and false after every setRPM(1) call
    bool m_setpointStatus;

    // PID constants
    float m_P;
    float m_I;
    float m_D;
    float m_F;

    // Used by PID controller to get RPM
    double PIDGet();

    // PID controller output makes shooter wheels move
    void PIDWrite(float output);
};
