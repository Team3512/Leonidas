//=============================================================================
//File Name: AutonomousBridge.cpp
//Description: Drives to Coopertition bridge and tips it to our side
//Author: FRC Team 3512, Spartatroniks
//=============================================================================

#include <Timer.h>
#include "../OurRobot.hpp"

void OurRobot::Autonomous() {
    mainCompressor.Start();

    Timer autoTime;
    autoTime.Start();

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
