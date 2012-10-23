//=============================================================================
//File Name: LockSolenoid.hpp
//Description: Deploys a solenoid and engages a solenoid mechanical lock on it.
//Author: FRC Team 3512, Spartatroniks
//=============================================================================

/* LockSolenoid class
 * ====================
 *
 * Operates either single or double solenoid and locks it in deployed position
 * with either single or double solenoid
 *
 * For this class to work properly:
 *
 * ARMS:
 * Single solenoid:              "false" position must be retracted (unlocked) position
 * Double solenoid:              "kReverse" position must be retracted (unlocked) position
 *
 * LOCKS:
 * Single solenoid:              "false" position must be retracted (unlocked) position
 * Double solenoid:              "kReverse" position must be retracted (unlocked) position
 *
 * ====================
 * Give zero to second solenoid channel if using a single solenoid for either arm or lock
 * Give zero to both solenoid channels if not using that feature
 * ==========
 *
 */

#ifndef LOCK_SOLENOID_HPP
#define LOCK_SOLENOID_HPP

#include "Solenoid.h"
#include "DoubleSolenoid.h"
#include "Timer.h"

class LockSolenoid {
public:
    enum Status {
        Retracted = 0,
        Deployed = 1,
        Transition = 2
    };

	LockSolenoid( UINT32 armSolenoidChannelA ,
	    UINT32 lockSolenoidChannelA ,
	    UINT32 armSolenoidChannelB = 0 ,
	    UINT32 lockSolenoidChannelB = 0 );

	virtual ~LockSolenoid();

	// Must be set to either Retracted or Deployed
	void Set( Status lock );

	// Sets delay between when solenoid fires and when lock engages
	void SetDeployDelay( double time );

	// Sets delay between when lock disengages and when solenoid retracts
	void SetRetractDelay( double time );

	// Continues transition of LockSolenoid state and updates the status received from Get()
	void Update();

	// Returns either Retracted, Deployed, or Transition
	Status Get();

private:
	UINT32 m_armSolenoidChannelA;
	UINT32 m_armSolenoidChannelB;

	UINT32 m_lockSolenoidChannelA;
	UINT32 m_lockSolenoidChannelB;

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

#endif // LOCK_SOLENOID_HPP
