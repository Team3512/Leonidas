//=============================================================================
//File Name: AutonBridge.cpp
//Description: Drives to Coopertition bridge and tips it to our side
//Author: FRC Team 3512, Spartatroniks
//=============================================================================

#include "../OurRobot.hpp"

// autoTime is handled from within the main Autonomous call in Autonomous.cpp

void OurRobot::AutonBridge() {
    shifter.Set( false ); // Set robot's drivetrain to low gear
    bridgeArm.Set( LockSolenoid::Deployed );

    while ( IsEnabled() && IsAutonomous() ) {
        bridgeArm.Update();

        // Slows down robot as it approaches the Coopertition bridge
        if ( autoTime.Get() < 1.f ) {
            mainDrive.SetLeftRightMotorOutputs( 1.f , 1.f );
        }
        else if ( autoTime.Get() < 2.f ) {
            mainDrive.SetLeftRightMotorOutputs( 0.5f , 0.5f );
        }
        else {
            mainDrive.SetLeftRightMotorOutputs( 0.f , 0.f );
        }

        Wait( 0.1 );
    }
}
