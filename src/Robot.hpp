// Copyright (c) 2012-2017 FRC Team 3512. All Rights Reserved.

#pragma once

#include <cmath>

#include <Compressor.h>
#include <Joystick.h>
#include <Relay.h>
#include <RobotDrive.h>
#include <SimpleRobot.h>
#include <Solenoid.h>
#include <Timer.h>
#include <Victor.h>

#include "AutonContainer.hpp"
#include "Kinect/TurretKinect.hpp"
#include "LiveGrapherHost/graphhost-p.hpp"
#include "LockSolenoid.hpp"
#include "Settings.hpp"
#include "Subsystems/Shooter.hpp"

// scales joystick's Z axis values from -1 .. 1 to 0 .. 1, but within same range
// of movement
float ScaleZ(Joystick& stick);

class DriverStationDisplay;

class Robot : public SimpleRobot, public Settings {
public:
    DriverStationDisplay*
        driverStation;  // used for sending data to the Driver Station

    Robot();
    ~Robot();

    void Autonomous();
    void AutonShoot();
    void AutonBridge();
    void AutonFeed();

    void OperatorControl();
    void Disabled();

private:
    uint32_t lastTime;
    uint32_t startTime;

    AutonContainer<Robot> autonModes;

    Compressor mainCompressor{1, 6};

    RobotDrive mainDrive{3, 4, 1, 2};

    Joystick driveStick1{1};
    Joystick driveStick2{2};
    Joystick turretStick{3};
    Relay lift{7};

    Victor rotateMotor{5};

    Shooter shooter{7, 6, 6, 16, 1.f};

    LockSolenoid bridgeArm{2, 3};

    // single board computer's IP address and port
    TurretKinect turretKinect{getValueFor("SBC_IP"),
                              std::atoi(getValueFor("SBC_Port").c_str())};

    bool isAutoAiming = false;

    // Determines which autonomous mode is run
    char autonMode = 0;

    // Used for timing in all Autonomous routines
    Timer autoTime;

    // The LiveGrapher host
    GraphHost pidGraph{3513};

    void DS_PrintOut();  // prints messages to driverStation LCD
};
