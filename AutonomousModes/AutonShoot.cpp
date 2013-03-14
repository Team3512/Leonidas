//=============================================================================
//File Name: AutonShoot.cpp
//Description: Shoots balls contained within robot at hoop
//Author: FRC Team 3512, Spartatroniks
//=============================================================================

#include "../OurRobot.hpp"

// autoTime is handled from within the main Autonomous call in Autonomous.cpp

void OurRobot::AutonShoot() {
    shooter.start();

    turretKinect.clearValues();

    rotateMotor.Set( 0 );

    bool aiming = true;

    /* 1) Start aiming turret and start shooter motors
     * 2) After 6 seconds, move ball conveyors up to shoot ball
     */

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

        shooter.setRPM( 2300.f );

        if ( autoTime.Get() > 6.f ) { // gives shooter six seconds to spin up before starting ball conveyor
            // start conveyor moving balls up to shooter
            lift.Set( Relay::kForward );
        }

        Wait( 0.1 );
    }

    shooter.stop();
}
