//=============================================================================
//File Name: LockSolenoid.cpp
//Description: Deploys a solenoid and engages a solenoid mechanical lock on it.
//Author: FRC Team 3512, Spartatroniks
//=============================================================================

/* ===== Hammer =====
 * There are two parts to switching the state of this mechanism.
 *
 * 1) The first device is moved into position and out of the other one's way.
 * 2) The second device is moved after a delay to avoid a mechanical lock-up.
 *
 * When the trigger is released, the state variable switches and a timer is started.
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

LockSolenoid::LockSolenoid( UINT32 armSolenoidChannelA ,
        UINT32 lockSolenoidChannelA ,
        UINT32 armSolenoidChannelB ,
        UINT32 lockSolenoidChannelB ) {
    m_deployDelay = 0.2f;
    m_retractDelay = 0.f;
    m_lockState = Retracted;

    // Initializes arm solenoid
    if ( armSolenoidChannelA != 0 ) {
        if ( armSolenoidChannelB != 0 ) {
            m_armDoubleSolenoid = new DoubleSolenoid( armSolenoidChannelA , armSolenoidChannelB );
            m_armSolenoid = NULL;
        }
        else {
            m_armSolenoid = new Solenoid( armSolenoidChannelA );
            m_armDoubleSolenoid = NULL;
        }
    }
    else {
        m_armSolenoid = NULL;
        m_armDoubleSolenoid = NULL;
    }

    // Initializes lock solenoid
    if ( lockSolenoidChannelA != 0 ) {
        if ( lockSolenoidChannelB != 0 ) {
            m_lockDoubleSolenoid = new DoubleSolenoid( lockSolenoidChannelA , lockSolenoidChannelB );
            m_lockSolenoid = NULL;
        }
        else {
            m_lockSolenoid = new Solenoid( lockSolenoidChannelA );
            m_lockDoubleSolenoid = NULL;
        }
    }
    else {
        m_lockSolenoid = NULL;
        m_lockDoubleSolenoid = NULL;
    }
}

LockSolenoid::~LockSolenoid() {
    delete m_armSolenoid;
    delete m_armDoubleSolenoid;

    delete m_lockSolenoid;
    delete m_lockDoubleSolenoid;
}

void LockSolenoid::Set( Status lock ) {
    // If LockSolenoid is done changing states
    if ( m_lockState == m_changeTo && lock != Transition ) {
        // Start a potential state change
        m_changeTo = lock;
    }

    // If LockSolenoid needs to change states
    if ( m_lockState != m_changeTo ) {
        // Used to track delay between actions
        m_timer.Reset();
        m_timer.Start();

        /* Moves first device immediately */

        // Operates lock with double solenoid as arm
        if ( m_armDoubleSolenoid != NULL ) {
            // If hammer should be going down, deploy it immediately
            if( m_changeTo == Deployed ) {
                m_armDoubleSolenoid->Set( DoubleSolenoid::kForward );
            }

            // If hammer is coming back up, unlock solenoid immediately
            else {
                if ( m_armDoubleSolenoid != NULL ) {
                    m_lockDoubleSolenoid->Set( DoubleSolenoid::kReverse );
                }
                else if ( m_lockSolenoid != NULL ) {
                    m_lockSolenoid->Set( Retracted );
                }
            }
        }

        // Operates lock with single solenoid as arm
        else if ( m_armSolenoid != NULL ) {
            // If hammer should be going down, deploy it immediately
            if ( m_changeTo == Deployed ) {
                m_armSolenoid->Set( Deployed );
            }

            // If hammer is coming back up, unlock solenoid immediately
            else {
                if ( m_armDoubleSolenoid != NULL ) {
                    m_lockDoubleSolenoid->Set( DoubleSolenoid::kReverse );
                }
                else if ( m_lockSolenoid != NULL ) {
                    m_lockSolenoid->Set( Retracted );
                }
            }
        }
    }
}

void LockSolenoid::SetDeployDelay( double time ) {
    m_deployDelay = time;
}

void LockSolenoid::SetRetractDelay( double time ) {
    m_retractDelay = time;
}

void LockSolenoid::Update() {
    if ( m_lockState != m_changeTo ) {
        /* If we are deploying the solenoids, use the deploy delay.
         * Otherwise, use the retract delay.
         */
        if ( m_timer.Get() > ( m_changeTo == Deployed ? m_deployDelay : m_retractDelay ) ) {
            // If going to lock, activate lock
            if ( m_changeTo == Deployed ) {
                if ( m_lockDoubleSolenoid != NULL ) {
                    m_lockDoubleSolenoid->Set( DoubleSolenoid::kForward );
                }
                else if ( m_lockSolenoid != NULL ) {
                    m_lockSolenoid->Set( Deployed );
                }
            }

            // Else, move arm back up
            else {
                if ( m_armDoubleSolenoid != NULL ) {
                    m_armDoubleSolenoid->Set( DoubleSolenoid::kReverse );
                }
                else if ( m_armSolenoid != NULL ) {
                    m_armSolenoid->Set( Retracted );
                }
            }

            // Update the status since it just finished transitioning
            m_lockState = m_changeTo;

            // Stop the timer because it's no longer needed
            m_timer.Stop();
        }
    }
}

LockSolenoid::Status LockSolenoid::Get() {
    // If LockSolenoid is switching states
    if ( m_lockState != m_changeTo ) {
        return Transition; // Returns 2
    }

    // If LockSolenoid is deployed
    else if ( m_lockState == Deployed ) {
       return Deployed; // Returns 1
    }

    // LockSolenoid is retracted
    else {
        return Retracted; // Returns 0
    }
}
