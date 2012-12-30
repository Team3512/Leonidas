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
            isShooting = !isShooting;
        }

        // toggle manual RPM setting vs setting with encoder input
        if ( turretStickButtons.releasedButton( 2 ) ) {
            shooterIsManual = !shooterIsManual;
        }
        /* =================================================== */

        bridgeArm.Update();

        Wait( 0.1 );
    }
}
