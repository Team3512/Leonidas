//=============================================================================
//File Name: OurRobot.hpp
//Description: Main robot class in which all robot sensors and devices are
//             declared
//Author: FRC Team 3512, Spartatroniks
//=============================================================================

#ifndef OUR_ROBOT_HPP
#define OUR_ROBOT_HPP

#include <cmath>

#include <SimpleRobot.h>

#include <Compressor.h>
#include <RobotDrive.h>
#include <Joystick.h>
#include <Relay.h>
#include <Victor.h>
#include <Counter.h>
#include <Solenoid.h>

#include "LockSolenoid.hpp"
#include "Kinect/TurretKinect.hpp"
#include "ShooterController.hpp"

// scales joystick's Z axis values from -1 .. 1 to 0 .. 1, but within same range of movement
float ScaleZ( Joystick& stick );

class DriverStationDisplay;

class OurRobot : public SimpleRobot {
public:
    static DriverStationDisplay* driverStation; // used for sending data to the Driver Station

    OurRobot();

    void Autonomous();
    void OperatorControl();
    void Disabled();

private:
    Compressor mainCompressor;

    RobotDrive mainDrive;

    Joystick driveStick1;
    Joystick driveStick2;
    Joystick turretStick;
    Relay lift;

    Victor shooterMotorLeft;
    Victor shooterMotorRight;
    Victor rotateMotor;

    Counter shooterEncoder;

    Solenoid shifter;
    LockSolenoid bridgeArm;

    TurretKinect turretKinect;

    ShooterController pidControl;

    bool shooterIsManual;
    bool isShooting;
    bool isAutoAiming;

    // Used for timing in all Autonomous routines
    Timer autoTime;

    void DS_PrintOut(); // prints messages to driverStation LCD
};

#endif // OUR_ROBOT_HPP
