// Copyright (c) 2012-2020 FRC Team 3512. All Rights Reserved.

/* ===== Hammer =====
 * There are two parts to switching the state of this mechanism.
 *
 * 1) The first device is moved into position and out of the other one's way.
 * 2) The second device is moved after a delay to avoid a mechanical lock-up.
 *
 * When the trigger is released, the state variable switches and a timer is
 * started.
 * The first device is moved into position immediately.
 *
 * After the delay for the respective state change has passed,
 *     the second device is moved into position. This is done
 *     to make sure that there are no mechanical lock-ups caused
 *     by the devices being triggered at the same time. It may
 *     damage them.
 *
 * After the second device is told to move, the timer is stopped and reset
 *     since it isn't needed anymore.
 *
 * Note: We don't know for sure if either device moved out of the way by
 *       the time the second device needs to be moved. It's just
 *       assumed that the delay gave the first device enough time
 *       to do so.
 */

#include "LockSolenoid.hpp"

LockSolenoid::LockSolenoid(int armSolenoidChannel, int lockSolenoidChannel)
    : m_armSolenoid(armSolenoidChannel), m_lockSolenoid(armSolenoidChannel) {}

void LockSolenoid::Set(State lock) {
    // If LockSolenoid is done changing states
    if (m_lockState == m_changeTo && lock != State::kTransition) {
        // Start a potential state change
        m_changeTo = lock;
    }

    // If LockSolenoid needs to change states
    if (m_lockState != m_changeTo) {
        // Used to track delay between actions
        m_timer.Reset();
        m_timer.Start();

        /* Moves first device immediately */

        // Operates lock with single solenoid as arm
        if (m_changeTo == State::kDeployed) {
            // If hammer should be going down, deploy it immediately
            m_armSolenoid.Set(true);
        } else {
            // If hammer is coming back up, unlock solenoid immediately
            m_lockSolenoid.Set(false);
        }
    }
}

void LockSolenoid::Update() {
    if (m_lockState != m_changeTo) {
        /* If we are deploying the solenoids, use the deploy delay.
         * Otherwise, use the retract delay.
         */
        if (m_timer.Get() >
            (m_changeTo == State::kDeployed ? kDeployDelay : kRetractDelay)) {
            if (m_changeTo == State::kDeployed) {
                // If going to lock, activate lock
                m_lockSolenoid.Set(true);

            } else {
                // Else, move arm back up
                m_armSolenoid.Set(false);
            }

            // Update the status since it just finished transitioning
            m_lockState = m_changeTo;

            // Stop the timer because it's no longer needed
            m_timer.Stop();
        }
    }
}

LockSolenoid::State LockSolenoid::Get() const {
    if (m_lockState != m_changeTo) {
        return State::kTransition;
    } else {
        return m_lockState;
    }
}
