//=============================================================================
//File Name: OperatorControl.cpp
//Description: Handles driver controls for robot
//Author: FRC Team 3512, Spartatroniks
//=============================================================================

#include <Timer.h>
#include "OurRobot.h"
#include "ButtonTracker.h"

void OurRobot::OperatorControl() {
    Timer hammerClock;
    bool isHammerDown = false;

    mainCompressor.Start();

    isShooting = false;
    isAutoAiming = false;

    pinLock.Set( false );
    hammer.Set( false );

    ButtonTracker driveStick1Buttons( 1 );
    ButtonTracker driveStick2Buttons( 2 );
    ButtonTracker turretStickButtons( 3 );

    signed short sPixelOffset; // used to store safe value for Kinect's pixel offset

    while ( IsEnabled() && IsOperatorControl() ) {
        DS_PrintOut();

        // update "new" value of joystick buttons
        driveStick1Buttons.updateButtons();
        driveStick2Buttons.updateButtons();
        turretStickButtons.updateButtons();

        /* ===== AIM ===== */
        if ( fabs( turretStick.GetX() ) > 0.1 ) { // manual turret movement outside of -0.5 to 0.5 range, up to 0.5 speed
            rotateMotor.Set( -turretStick.GetX() / 2.f );
        }
        else if ( isAutoAiming ) { // let autoAiming take over if activated and user isn't rotating turret
            turretKinect.valueMutex.lock();

            sPixelOffset = turretKinect.pixelOffset; // store value so it can be accessed safely elsewhere

            turretKinect.valueMutex.unlock();

            if( fabs( sPixelOffset ) < TurretKinect::pxlDeadband ) { // deadband so shooter won't jitter
                rotateMotor.Set(0);
            }
            else {
                /* Set(x) = x / 320
                 * -320 <= x <= 320 therefore Set( -1 <= x / 320 <= 1 )
                 * smooth tracking because as the turret aims closer to the target, the motor value gets smaller
                 */
                rotateMotor.Set( static_cast<float>(sPixelOffset) / 320.f );
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
            turretKinect.valueMutex.lock();
            turretKinect.targetSelect = -1;
            turretKinect.valueMutex.unlock();

            turretKinect.send();
        }

        // selecting target to right of current target
        if ( turretStickButtons.releasedButton( 5 ) ) {
            turretKinect.valueMutex.lock();
            turretKinect.targetSelect = 1;
            turretKinect.valueMutex.unlock();

            turretKinect.send();
        }
        /* =================================================== */


        /* ============== Toggle Shooter Motors ============== */
        // turns shooter on/off
        if ( turretStickButtons.releasedButton( 1 ) ) { // if released trigger
            isShooting = !isShooting;
        }

        if ( isShooting ) {
            if ( shooterIsManual ) { // let driver change shooter speed manually
                shooterMotorLeft.Set( -ScaleZ(turretStick) );
                shooterMotorRight.Set( ScaleZ(turretStick) );
            }
            else { // else adjust shooter voltage to match RPM
                if ( 60.f / ( 16.f * shooterEncoder.GetPeriod() ) < 72.0 * ScaleZ(turretStick) * 60.0 ) {
                    shooterMotorLeft.Set( -1 );
                    shooterMotorRight.Set( 1 );
                }
                else {
                    shooterMotorLeft.Set( 0 );
                    shooterMotorRight.Set( 0 );
                }
            }
        }
        else {
            shooterMotorLeft.Set( 0 );
            shooterMotorRight.Set( 0 );
        }

        // toggle manual RPM setting vs setting with encoder input
        if ( turretStickButtons.releasedButton( 2 ) ) {
            shooterIsManual = !shooterIsManual;
        }
		/* =================================================== */


        /* ===== Ball Intake/Conveyor ===== */
        if ( turretStick.GetRawButton( 6 ) ) { // move upper lift up
            upperLift.Set( Relay::kForward );
        }
        else if ( turretStick.GetRawButton( 11 ) ) { // move upper lift down
            upperLift.Set( Relay::kReverse );
        }
        else {
            upperLift.Set( Relay::kOff ); // turn off lift
        }

        if ( turretStick.GetRawButton( 7 ) ) { // move lower lift up
            lowerLift.Set( Relay::kReverse );
        }
        else if ( turretStick.GetRawButton( 10 ) ) { // move lower lift down
            lowerLift.Set( Relay::kForward );
		}
        else {
            lowerLift.Set( Relay::kOff ); // turn off lift
        }

        // ---------- Full Lift Forward ----------
        if ( turretStick.GetRawButton( 8 ) ) { // move all lifts up
            upperLift.Set( Relay::kForward );
            lowerLift.Set( Relay::kReverse );
        }

        // ---------- Full Lift Reverse ----------
        if ( turretStick.GetRawButton( 9 ) ) { // move all lifts down
            upperLift.Set( Relay::kReverse );
            lowerLift.Set( Relay::kForward );
        }
        /* ================================ */

        /* ===== Hammer ===== */
        if ( driveStick2Buttons.releasedButton( 1 ) ) { // if released trigger
            isHammerDown = !isHammerDown;

            hammerClock.Start();

            if( isHammerDown ) {
                hammer.Set( true ); // if hammer should be going down, deploy it immediately
            }
            else {
                pinLock.Set( false ); // if hammer is coming back up, unlock solenoid immediately
            }
        }

        if ( isHammerDown && hammerClock.Get() > 0 ) {
            pinLock.Set( true ); // if hammer is down after delay passed, deploy locks

            hammerClock.Stop();
            hammerClock.Reset();
        }
        else if ( hammerClock.Get() > 0.2 ) {
            hammer.Set( false ); // if hammer coming back up after delay passed, locks are disengaged so bring hammer back up

            hammerClock.Stop();
            hammerClock.Reset();
        }
        /* ================== */

        /* ===== Shifter ===== */
        if ( driveStick1Buttons.releasedButton( 1 ) ) { // if released trigger
            shifter.Set( !shifter.Get() ); // shifts between low and high gear
        }
        /* =================== */

        mainDrive.ArcadeDrive( ScaleZ( driveStick1 ) * driveStick1.GetY() , driveStick2.GetX() , false ); //moves robot based on two joystick inputs

        Wait( 0.1 );
    }
}
