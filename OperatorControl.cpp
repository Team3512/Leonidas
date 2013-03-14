//=============================================================================
//File Name: OperatorControl.cpp
//Description: Handles driver controls for robot
//Author: FRC Team 3512, Spartatroniks
//=============================================================================

#include <Timer.h>
#include "OurRobot.hpp"
#include "ButtonTracker.hpp"

void OurRobot::OperatorControl() {
    mainCompressor.Start();

    isAutoAiming = false;

    bridgeArm.Set( LockSolenoid::Retracted );

    ButtonTracker driveStick1Buttons( 1 );
    ButtonTracker driveStick2Buttons( 2 );
    ButtonTracker turretStickButtons( 3 );

    shooter.start();

    while ( IsEnabled() && IsOperatorControl() ) {
        DS_PrintOut();

        // update "new" value of joystick buttons
        driveStick1Buttons.updateButtons();
        driveStick2Buttons.updateButtons();
        turretStickButtons.updateButtons();

        /* ===== AIM ===== */
        if ( fabs( turretStick.GetX() ) > 0.06 ) { // manual turret movement
            // Squaring gives finer control with a smaller joystick movement while retaining max speed
            rotateMotor.Set( -( turretStick.GetX() * fabs(turretStick.GetX() ) ) );
        }
        else if ( isAutoAiming ) { // let autoAiming take over if activated and user isn't rotating turret
            if( fabs( turretKinect.getPixelOffset() ) < TurretKinect::pxlDeadband ) { // deadband so shooter won't jitter
                rotateMotor.Set(0);
            }
            else {
                /* Set(x) = x / 320
                 * -320 <= x <= 320 therefore Set( -1 <= x / 320 <= 1 )
                 * smooth tracking because as the turret aims closer to the target, the motor value gets smaller
                 */
                rotateMotor.Set( static_cast<float>( turretKinect.getPixelOffset() ) / 320.f );
            }
        }
        else {
            rotateMotor.Set( 0 );
        }
        /* =============== */


        /* ===================== AutoAim ===================== */
        if ( turretStickButtons.releasedButton( 3 ) ) {
            isAutoAiming = !isAutoAiming; // toggles autoAim
        }
        /* =================================================== */

        /* ================= Target Selection ================ */
        // selecting target to left of current target
        if ( turretStickButtons.releasedButton( 4 ) ) {
            turretKinect.setTargetSelect( -1 );

            turretKinect.send();
        }

        // selecting target to right of current target
        if ( turretStickButtons.releasedButton( 5 ) ) {
            turretKinect.setTargetSelect( 1 );

            turretKinect.send();
        }
        /* =================================================== */


        /* ============== Toggle Shooter Motors ============== */
        // turns shooter on/off
        if ( turretStickButtons.releasedButton( 1 ) ) { // if released trigger
            if ( shooter.isShooting() ) {
                shooter.stop();
            }
            else {
                shooter.start();
            }
        }

        shooter.setScale( ScaleZ(turretStick) );

        // toggle manual RPM setting vs setting with encoder input
        if ( turretStickButtons.releasedButton( 2 ) ) {
            if ( shooter.getControlMode() == Shooter::Manual  ) {
                shooter.setControlMode( Shooter::PID );
            }
            else {
                shooter.setControlMode( Shooter::Manual );
            }
        }
		/* =================================================== */


        /* ===== Ball Intake/Conveyor ===== */
        if ( turretStick.GetRawButton( 6 ) ) {
            lift.Set( Relay::kForward ); // move lift up
        }
        else if ( turretStick.GetRawButton( 7 ) ) {
            lift.Set( Relay::kReverse ); // move lift down
        }
        else {
            lift.Set( Relay::kOff ); // turn off lift
        }
        /* ================================ */

        /* ===== Bridge Arm ===== */
        if ( driveStick2Buttons.releasedButton( 1 ) ) {
            LockSolenoid::Status armState = bridgeArm.Get();

            /* Toggles state of bridge arm between Retracted and Deployed
             * (Doesn't toggle if the arm is transitioning)
             */
            if ( armState == LockSolenoid::Retracted ) {
                bridgeArm.Set( LockSolenoid::Deployed );
            }
            else if ( armState == LockSolenoid::Deployed ) {
                bridgeArm.Set( LockSolenoid::Retracted );
            }
        }

        // Makes sure bridge arm completed transitioning between states
        bridgeArm.Update();
        /* ====================== */

        // Reset PID constants
        if ( turretStickButtons.releasedButton( 8 ) ) {
            Settings::update();

            shooter.setPID( atof( getValueFor( "PID_P" ).c_str() ) , atof( getValueFor( "PID_I" ).c_str() ) , atof( getValueFor( "PID_D" ).c_str() ) );
        }

        // move robot based on two joystick inputs
        mainDrive.ArcadeDrive( ScaleZ( driveStick1 ) * driveStick1.GetY() , ScaleZ( driveStick2 ) * driveStick2.GetX() , false );

        Wait( 0.1 );
    }

    shooter.stop();
}
