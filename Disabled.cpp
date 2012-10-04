//=============================================================================
//File Name: Disabled.cpp
//Description: This function runs when robot is in disabled state
//Author: FRC Team 3512, Spartatroniks
//=============================================================================

#include <Timer.h>
#include "OurRobot.h"

void OurRobot::Disabled() {
    mainCompressor.Stop();

    while ( IsDisabled() ) {
        DS_PrintOut();
        Wait( 0.1 );
    }
}
