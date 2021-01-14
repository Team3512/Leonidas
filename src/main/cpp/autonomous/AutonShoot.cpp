// Copyright (c) 2012-2021 FRC Team 3512. All Rights Reserved.

#include "Robot.hpp"

// Shoots balls contained within robot at hoop
void Robot::AutonShoot() {
    shooter.SetReference(2300_rpm);

    while (!shooter.AtReference()) {
        autonChooser.YieldToMain();
        if (!IsAutonomousEnabled()) {
            return;
        }
    }

    lift.Set(frc::Relay::kForward);
}
