// Copyright (c) 2012-2021 FRC Team 3512. All Rights Reserved.

#pragma once

#include <cmath>

#include <frc/Encoder.h>
#include <frc/Joystick.h>
#include <frc/Relay.h>
#include <frc/SpeedControllerGroup.h>
#include <frc/TimedRobot.h>
#include <frc/Victor.h>
#include <frc/controller/PIDController.h>
#include <frc/drive/DifferentialDrive.h>

#include "AutonomousChooser.hpp"
#include "LockSolenoid.hpp"
#include "Shooter.hpp"

class Robot : public frc::TimedRobot {
public:
    Robot();

    void DisabledInit() override;
    void TeleopInit() override;

    void RobotPeriodic() override;
    void AutonomousPeriodic() override;
    void TeleopPeriodic() override;

    void AutonBridge();
    void AutonShoot();

private:
    // Drivetrain Motors
    frc::Victor frontLeft{2};
    frc::Victor frontRight{0};
    frc::Victor rearLeft{3};
    frc::Victor rearRight{1};

    frc::SpeedControllerGroup leftGrbx{frontLeft, rearLeft};
    frc::SpeedControllerGroup rightGrbx{frontRight, rearRight};
    frc::DifferentialDrive drivetrain{leftGrbx, rightGrbx};

    Shooter shooter;

    frc::Joystick driveStick1{1};
    frc::Joystick driveStick2{2};
    frc::Joystick turretStick{3};

    frc::Relay lift{7};

    frc::Victor turretMotor{5};

    LockSolenoid bridgeArm{2, 3};

    frc3512::AutonomousChooser autonChooser{"No-op", [] {}};
};
