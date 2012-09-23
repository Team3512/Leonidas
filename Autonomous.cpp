//=============================================================================
//File Name: Autonomous.cpp
//Description: Holds autonomous code for robot
//Author: FRC Team 3512, Spartatroniks
//=============================================================================

#include <Timer.h>
#include "OurRobot.h"

void OurRobot::Autonomous() {
	mainCompressor.Start();

	Timer autoTime;
	autoTime.Start();

	turretKinect.clearValues();

	rotateMotor.Set( 0 );

	bool aiming = true;

	/* 1) Start shooter motors and aim turret for four seconds
	 * 2) Wait two more seconds for shooter motors to finish spinning up
	 * 2) Move ball conveyors up to shoot ball */

	while ( IsEnabled() && IsAutonomous() ) {
		DS_PrintOut();

		if ( aiming ) {
			turretKinect.valueMutex.lock();
			if ( fabs( turretKinect.pixelOffset ) > TurretKinect::pxlDeadband ) { // if turret isn't locked on, then aim
				rotateMotor.Set( static_cast<float>(turretKinect.pixelOffset) / 320.f ); // "pixelOffset / 320" produces abs() function from -1 to 1 for smooth tracking
			}
			else {
				aiming = false;
			}
			turretKinect.valueMutex.unlock();
		}

		if ( shooterEncoder.getRPM() < 2300 ) {
			shooterMotorLeft.Set( -1 );
			shooterMotorRight.Set( 1 );
		}
		else {
			shooterMotorLeft.Set( 0 );
			shooterMotorRight.Set( 0 );
		}

		if ( autoTime.Get() > 6.f ) { // gives shooter six seconds to spin up before starting ball conveyor
			// start conveyor moving balls up to shooter
			upperLift.Set( Relay::kForward );
			lowerLift.Set( Relay::kReverse );
		}
	}
}
