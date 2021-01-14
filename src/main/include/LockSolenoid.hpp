// Copyright (c) 2012-2021 FRC Team 3512. All Rights Reserved.

#pragma once

#include <frc/Solenoid.h>
#include <frc2/Timer.h>
#include <units/time.h>

/**
 * Operates either single or double solenoid and locks it in deployed position
 * with either single or double solenoid
 *
 * For this class to work properly:
 *
 * ARMS:
 * Single solenoid: "false" position must be retracted (unlocked) position.
 *
 * LOCKS:
 * Single solenoid: "false" position must be retracted (unlocked) position.
 */
class LockSolenoid {
public:
    enum class State { kRetracted, kDeployed, kTransition };

    LockSolenoid(int armSolenoidChannel, int lockSolenoidChannel);

    // Must be set to either Retracted or Deployed
    void Set(State lock);

    // Continues transition of LockSolenoid state and updates the status
    // received from Get()
    void Update();

    // Returns either Retracted, Deployed, or Transition
    State Get() const;

private:
    // Sets delay between when solenoid fires and when lock engages
    static constexpr units::second_t kDeployDelay = 0.2_s;

    // Sets delay between when lock disengages and when solenoid retracts
    static constexpr units::second_t kRetractDelay = 0_s;

    frc::Solenoid m_armSolenoid;
    frc::Solenoid m_lockSolenoid;

    frc2::Timer m_timer;

    State m_lockState = State::kRetracted;
    State m_changeTo;
};
