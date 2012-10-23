//=============================================================================
//File Name: AutonomousFeed.cpp
//Description: Reverses ball retrieval mechanism for feeding balls to another
//             team during autonomous
//Author: FRC Team 3512, Spartatroniks
//=============================================================================

#include <Timer.h>
#include "../OurRobot.hpp"

void OurRobot::Autonomous() {
    mainCompressor.Start();

    while ( IsEnabled() && IsAutonomous() ) {
        DS_PrintOut();

        // make ball conveyors run in reverse for all of Autonomous
        lift.Set( Relay::kReverse );

        Wait( 0.1 );
    }
}
