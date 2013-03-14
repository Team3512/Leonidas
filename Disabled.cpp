//=============================================================================
//File Name: Disabled.cpp
//Description: This function runs when robot is in disabled state
//Author: FRC Team 3512, Spartatroniks
//=============================================================================

#include <Timer.h>
#include "OurRobot.hpp"
#include "ButtonTracker.hpp"

void OurRobot::Disabled() {
    mainCompressor.Stop();
    bridgeArm.Set( LockSolenoid::Retracted );

    ButtonTracker driveStick1Buttons( 1 );
    ButtonTracker driveStick2Buttons( 2 );
    ButtonTracker turretStickButtons( 3 );

    while ( IsDisabled() ) {
        DS_PrintOut();

        // update "new" value of joystick buttons
        driveStick1Buttons.updateButtons();
        driveStick2Buttons.updateButtons();
        turretStickButtons.updateButtons();

        /* ===================== AutoAim ===================== */
        if ( turretStickButtons.releasedButton( 3 ) ) {
            isAutoAiming = !isAutoAiming; // toggles autoAim
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

        bridgeArm.Update();

        // Reset PID constants
        if ( turretStickButtons.releasedButton( 8 ) ) {
            Settings::update();

            shooter.setPID( atof( getValueFor( "PID_P" ).c_str() ) , atof( getValueFor( "PID_I" ).c_str() ) , atof( getValueFor( "PID_D" ).c_str() ) );
        }

        Wait( 0.1 );
    }
}
