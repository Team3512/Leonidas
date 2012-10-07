//=============================================================================
//File Name: AutonomousNothing.cpp
//Description: Does nothing but immediately exit Autonomous. USED ONLY FOR
//             DEBUG PURPOSES
//Author: FRC Team 3512, Spartatroniks
//=============================================================================

#include <Timer.h>
#include "OurRobot.hpp"

void OurRobot::Autonomous() {
    while ( IsEnabled() && IsAutonomous() ) {
        Wait( 0.01 );
    }
}
