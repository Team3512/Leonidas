//=============================================================================
//File Name: AutonomousFeed.cpp
//Description: Reverses ball retrieval mechanism for feeding balls to another
//             team during autonomous
//Author: FRC Team 3512, Spartatroniks
//=============================================================================

#include "OurRobot.hpp"

void OurRobot::Autonomous() {
    while ( IsEnabled() && IsAutonomous() ) {
        // make ball conveyors run in reverse for all of Autonomous
        upperLift.Set( Relay::kReverse );
        lowerLift.Set( Relay::kForward );
    }
}
