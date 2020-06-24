// Copyright (c) 2012-2020 FRC Team 3512. All Rights Reserved.

#include "Robot.hpp"

#include <cmath>

Robot::Robot() {
    autonSelector.AddAutoMethod("Bridge",
                                std::bind(&Robot::AutonBridgeInit, this),
                                std::bind(&Robot::AutonBridgePeriodic, this));
    autonSelector.AddAutoMethod("Shoot",
                                std::bind(&Robot::AutonShootInit, this),
                                std::bind(&Robot::AutonShootPeriodic, this));
}

void Robot::DisabledInit() { bridgeArm.Set(LockSolenoid::State::kRetracted); }

void Robot::AutonomousInit() {
    autonTimer.Reset();
    autonTimer.Start();

    autonSelector.ExecAutonomousInit();

    autonTimer.Stop();
}

void Robot::TeleopInit() { bridgeArm.Set(LockSolenoid::State::kRetracted); }

void Robot::RobotPeriodic() { shooter.Update(); }

void Robot::AutonomousPeriodic() { autonSelector.ExecAutonomousPeriodic(); }

void Robot::TeleopPeriodic() {
    // Aim
    if (std::abs(turretStick.GetX()) > 0.06) {  // manual turret movement
        // Squaring gives finer control with a smaller joystick movement
        // while retaining max speed
        turretMotor.Set(-turretStick.GetX() * std::abs(turretStick.GetX()));
    }

    // Turns shooter on/off
    if (turretStick.GetRawButtonPressed(1)) {
        if (shooter.IsEnabled()) {
            shooter.Disable();
        } else {
            shooter.Disable();
        }
    }

    // Ball intake/conveyor
    if (turretStick.GetRawButton(6)) {
        lift.Set(frc::Relay::kForward);  // move lift up
    } else if (turretStick.GetRawButton(7)) {
        lift.Set(frc::Relay::kReverse);  // move lift down
    } else {
        lift.Set(frc::Relay::kOff);  // turn off lift
    }

    // Bridge arm
    if (driveStick2.GetRawButtonPressed(1)) {
        auto armState = bridgeArm.Get();

        /* Toggles state of bridge arm between Retracted and Deployed
         * (Doesn't toggle if the arm is transitioning)
         */
        if (armState == LockSolenoid::State::kRetracted) {
            bridgeArm.Set(LockSolenoid::State::kDeployed);
        } else if (armState == LockSolenoid::State::kDeployed) {
            bridgeArm.Set(LockSolenoid::State::kRetracted);
        }
    }

    // Makes sure bridge arm completed transitioning between states
    bridgeArm.Update();

    drivetrain.CurvatureDrive(driveStick1.GetY(), driveStick2.GetX(),
                              driveStick2.GetRawButton(2));
}

#ifndef RUNNING_FRC_TESTS
int main() { return frc::StartRobot<Robot>(); }
#endif
