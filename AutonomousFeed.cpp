//=============================================================================
//File Name: AutonomousFeed.cpp
//Description: Reverses ball retrieval mechanism for feeding balls to another
//             team during autonomous
//Author: FRC Team 3512, Spartatroniks
//=============================================================================

#include "OurRobot.h"

void OurRobot::Autonomous() {
	while ( IsEnabled() && IsAutonomous() ) {
		upperLift.Set( Relay::kOn );
		upperLift.Set( Relay::kReverse );
		lowerLift.Set( Relay::kOn );
		lowerLift.Set( Relay::kForward );
	}
}
