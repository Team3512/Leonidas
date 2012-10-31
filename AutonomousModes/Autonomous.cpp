//=============================================================================
//File Name: Autonomous.cpp
//Description: Holds autonomous code for robot
//Author: FRC Team 3512, Spartatroniks
//=============================================================================

#include <Timer.h>
#include "../OurRobot.hpp"

void OurRobot::Autonomous() {
    mainCompressor.Start();
    autoTime.Reset();
    autoTime.Start();

    shooterEncoder.Start();

    turretKinect.clearValues();

    rotateMotor.Set( 0 );

    bool aiming = true;

    /* 1) Start aiming turret and start shooter motors
     * 2) After 6 seconds, move ball conveyors up to shoot ball */

    while ( IsEnabled() && IsAutonomous() ) {
        DS_PrintOut();

        if ( aiming ) {
            // if turret isn't locked on, then aim
            if ( fabs( turretKinect.getPixelOffset() ) > TurretKinect::pxlDeadband ) {
                /* Set(x) = x / 320
                 * x is always in range: -320 <= x <= 320,
                 * therefore Set( x / 320.f ) always gets a value between -1 and 1
                 * smooth tracking because as the turret aims closer to the target, the motor value gets smaller
                 */
                rotateMotor.Set( static_cast<float>( turretKinect.getPixelOffset() ) / 320.f );
            }
            else { // stop aiming
                aiming = false;
            }
        }

        /* gives full power to shooter motors until it reaches set point,
         * then turn them off until the shooter slows down again
         */
        float encoderRPM = 60.f / ( 16.f * shooterEncoder.GetPeriod() );
        if ( encoderRPM >= 2300.f ) {
            shooterMotorLeft.Set( 0 );
            shooterMotorRight.Set( 0 );
        }
        else if ( encoderRPM > 2242.f ) {
            shooterMotorLeft.Set( -0.3f );
            shooterMotorRight.Set( 0.3f );
        }
        else {
            shooterMotorLeft.Set( -1 );
            shooterMotorRight.Set( 1 );
        }

        if ( autoTime.Get() > 6.f ) { // gives shooter six seconds to spin up before starting ball conveyor
            // start conveyor moving balls up to shooter
            lift.Set( Relay::kForward );
        }

        Wait( 0.1 );
    }

    shooterEncoder.Stop();
}
