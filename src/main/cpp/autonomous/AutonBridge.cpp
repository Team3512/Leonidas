// Copyright (c) 2012-2021 FRC Team 3512. All Rights Reserved.

#include <frc2/Timer.h>

#include "Robot.hpp"

// Drives to Coopertition bridge and tips it to our side
void Robot::AutonBridge() {
    bridgeArm.Set(LockSolenoid::State::kDeployed);

    // Move forward
    frc2::Timer timer;
    timer.Start();
    while (!timer.HasPeriodPassed(1_s)) {
        drivetrain.TankDrive(1.0, 1.0);

        autonChooser.YieldToMain();
        if (!IsAutonomousEnabled()) {
            return;
        }
    }

    // Slow down
    while (!timer.HasPeriodPassed(1_s)) {
        drivetrain.TankDrive(0.5, 0.5);

        autonChooser.YieldToMain();
        if (!IsAutonomousEnabled()) {
            return;
        }
    }

    drivetrain.TankDrive(0.0, 0.0);
}
