//=============================================================================
//File Name: OperatorControl.cpp
//Description: Handles driver controls for robot
//Author: FRC Team 3512, Spartatroniks
//=============================================================================

#include <Timer.h>
#include "OurRobot.hpp"
#include "ButtonTracker.hpp"

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

    shooterEncoder.Start();

    while ( IsEnabled() && IsOperatorControl() ) {
        DS_PrintOut();

        // update "new" value of joystick buttons
        driveStick1Buttons.updateButtons();
        driveStick2Buttons.updateButtons();
        turretStickButtons.updateButtons();

        /* ===== AIM ===== */
        if ( fabs( turretStick.GetX() ) > 0.06 ) { // manual turret movement
            rotateMotor.Set( -pow( turretStick.GetX() , 2.f ) );
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
            isShooting = !isShooting;
        }

        if ( isShooting && turretStickButtons.releasedButton( 1 ) ) {
            pidControl.SetTargetDistance( 15.f ); // * 0.00328084f
        }

        if ( isShooting ) {
            if ( shooterIsManual ) { // let driver change shooter speed manually
                shooterMotorLeft.Set( -ScaleZ(turretStick) );
                shooterMotorRight.Set( ScaleZ(turretStick) );
            }
            else { // else adjust shooter voltage to match RPM
                //pidControl.SetTargetDistance( 25.f ); // * 0.00328084f
                pidControl.Update();

                /*float encoderRPM = 60.f / ( 16.f * shooterEncoder.GetPeriod() );
                if ( encoderRPM >= 72.0 * ScaleZ(turretStick) * 60.0 ) {
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
                }*/
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
         * After the second device is told to move, the timers are stopped and
         *     reset since they aren't needed anymore.
         *
         * Note: We don't know for sure if either device moved out of the way by
         *       the time the second device needs to be moved. It's just
         *       assumed that the delay gave the first device enough time
         *       to do so.
         */

        // ===== PART 1
        if ( driveStick2Buttons.releasedButton( 1 ) ) { // if released trigger
            // start process of switching position of hammer
            isHammerDown = !isHammerDown;

            hammerClock.Start(); // used to track delay between actions

            // move the first device immediately
            if( isHammerDown ) {
                hammer.Set( true ); // if hammer should be going down, deploy it immediately
            }
            else {
                pinLock.Set( false ); // if hammer is coming back up, unlock solenoid immediately
            }
        }
        // ============

        // ===== PART 2
        // move second device after delay has passed
        if ( isHammerDown && hammerClock.Get() > 0 ) {
            // if hammer is down and delay passed, deploy locks
            pinLock.Set( true );

            // stop timer because the device has finished switching states
            hammerClock.Stop();
            hammerClock.Reset();
        }

        if ( !isHammerDown && hammerClock.Get() > 0.2 ) {
            // if hammer is coming back up after delay passed, locks are disengaged so bring hammer back up
            hammer.Set( false );

            // stop timer because the device has finished switching states
            hammerClock.Stop();
            hammerClock.Reset();
        }
        // ============
        /* ================== */

        /* ===== Shifter ===== */
        if ( driveStick1Buttons.releasedButton( 1 ) ) { // if released trigger
            shifter.Set( !shifter.Get() ); // shifts between low and high gear
        }
        /* =================== */

        // move robot based on two joystick inputs
        mainDrive.ArcadeDrive( ScaleZ( driveStick1 ) * driveStick1.GetY() , ScaleZ( driveStick2 ) * driveStick2.GetX() , false );

        Wait( 0.1 );
    }

    shooterEncoder.Stop();
}
