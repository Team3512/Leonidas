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

    /* 1) Start aiming turret and start shooter motors
     * 2) After 6 seconds, move ball conveyors up to shoot ball */

    while ( IsEnabled() && IsAutonomous() ) {
        DS_PrintOut();

        if ( aiming ) {
            turretKinect.valueMutex.lock();
            if ( fabs( turretKinect.pixelOffset ) > TurretKinect::pxlDeadband ) { // if turret isn't locked on, then aim
                /* Set(x) = x / 320
                 * {x | -320 < x < 320 } -> Set( { y | -1 < y < 1 } )
                 * smooth tracking because the turret aims closer to the target, the motor value gets smaller
                 */
                rotateMotor.Set( static_cast<float>(turretKinect.pixelOffset) / 320.f );
            }
            else { // stop aiming
                aiming = false;
            }
            turretKinect.valueMutex.unlock();
        }

        /* gives full power to shooter motors until it reaches set point,
         * then turn them off until the shooter slows down again
         */
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