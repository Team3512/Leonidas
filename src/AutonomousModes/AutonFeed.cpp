// Copyright (c) FRC Team 3512, Spartatroniks 2017. All Rights Reserved.

//=============================================================================
// File Name: AutonFeed.cpp
// Description: Reverses ball retrieval mechanism for feeding balls to another
//             team during autonomous
// Author: FRC Team 3512, Spartatroniks
//=============================================================================

#include "../Robot.hpp"

void Robot::AutonFeed() {
    while (IsEnabled() && IsAutonomous()) {
        DS_PrintOut();

        // make ball conveyors run in reverse for all of Autonomous
        lift.Set(Relay::kReverse);

        Wait(0.1);
    }
}
