// Copyright (c) 2012-2020 FRC Team 3512. All Rights Reserved.

#include "Robot.hpp"

void Robot::AutonShootInit() { shooter.SetReference(2300_rpm); }

// Shoots balls contained within robot at hoop
void Robot::AutonShootPeriodic() {
    if (shooter.AtReference()) {
        lift.Set(frc::Relay::kForward);
    }
}
