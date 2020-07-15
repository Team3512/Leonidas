// Copyright (c) 2012-2020 FRC Team 3512. All Rights Reserved.

/* LockSolenoid class
 * ====================
 *
 * Operates either single or double solenoid and locks it in deployed position
 * with either single or double solenoid
 *
 * For this class to work properly:
 *
 * ARMS:
 * Single solenoid:              "false" position must be retracted (unlocked)
 * position
 * Double solenoid:              "kReverse" position must be retracted
 * (unlocked) position
 *
 * LOCKS:
 * Single solenoid:              "false" position must be retracted (unlocked)
 * position
 * Double solenoid:              "kReverse" position must be retracted
 * (unlocked) position
 *
 * ====================
 * Give zero to second solenoid channel if using a single solenoid for either
 * arm or lock
 * Give zero to both solenoid channels if not using that feature
 * ==========
 *
 */

#pragma once

#include <DoubleSolenoid.h>
#include <Solenoid.h>
#include <Timer.h>

class LockSolenoid {
public:
    enum Status { Retracted = 0, Deployed = 1, Transition = 2 };

    LockSolenoid(uint32_t armSolenoidChannelA, uint32_t lockSolenoidChannelA,
                 uint32_t armSolenoidChannelB = 0,
                 uint32_t lockSolenoidChannelB = 0);

    virtual ~LockSolenoid();

    // Must be set to either Retracted or Deployed
    void Set(Status lock);

    // Sets delay between when solenoid fires and when lock engages
    void SetDeployDelay(double time);

    // Sets delay between when lock disengages and when solenoid retracts
    void SetRetractDelay(double time);

    // Continues transition of LockSolenoid state and updates the status
    // received from Get()
    void Update();

    // Returns either Retracted, Deployed, or Transition
    Status Get();

private:
    uint32_t m_armSolenoidChannelA;
    uint32_t m_armSolenoidChannelB;

    uint32_t m_lockSolenoidChannelA;
    uint32_t m_lockSolenoidChannelB;

    double m_deployDelay;
    double m_retractDelay;

    Solenoid* m_armSolenoid;
    DoubleSolenoid* m_armDoubleSolenoid;

    Solenoid* m_lockSolenoid;
    DoubleSolenoid* m_lockDoubleSolenoid;

    Timer m_timer;

    Status m_lockState;
    Status m_changeTo;
};
