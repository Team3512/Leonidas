//=============================================================================
//File Name: OurRobot.h
//Description: Main robot class in which all robot sensors and devices are
//             declared
//Author: FRC Team 3512, Spartatroniks
//=============================================================================

#ifndef OUR_ROBOT_H
#define OUR_ROBOT_H

#include <cmath>

#include <SimpleRobot.h>

#include <Compressor.h>
#include <RobotDrive.h>
#include <Joystick.h>
#include <Relay.h>
#include <Victor.h>
#include <Solenoid.h>
#include "PhotoEncoder.h"

#include "Kinect/TurretKinect.h"

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
    Relay upperLift;
    Relay lowerLift;

    Victor shooterMotorLeft;
    Victor shooterMotorRight;
    Victor rotateMotor;

    Solenoid pinLock;
    Solenoid hammer;
    Solenoid shifter;

    PhotoEncoder shooterEncoder;

    TurretKinect turretKinect;

    bool shooterIsManual;
    bool isShooting;
    bool isAutoAiming;

    void DS_PrintOut(); // prints messages to driverStation LCD
};

#endif // OUR_ROBOT_H
