// Copyright (c) 2012-2020 FRC Team 3512. All Rights Reserved.

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
#include <frc2/Timer.h>
#include <units/units.h>

#include "LockSolenoid.hpp"
#include "Shooter.hpp"
#include "autonselector/AutonSelector.hpp"

class Robot : public frc::TimedRobot {
public:
    Robot();

    void DisabledInit() override;
    void AutonomousInit() override;
    void TeleopInit() override;

    void RobotPeriodic() override;
    void AutonomousPeriodic() override;
    void TeleopPeriodic() override;

    void AutonShootInit();
    void AutonShootPeriodic();
    void AutonBridgeInit();
    void AutonBridgePeriodic();

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

    frc3512::AutonSelector autonSelector{5800};

    // Used for timing in all Autonomous routines
    frc2::Timer autonTimer;
};
