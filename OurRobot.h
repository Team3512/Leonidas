//=============================================================================
//File Name: OurRobot.h
//Description: Main robot class in which all robot sensors and devices are
//             declared
//Author: FRC Team 3512, Spartatroniks
//=============================================================================

#ifndef OUR_ROBOT_H
#define OUR_ROBOT_H

#include <cmath>
#include <DriverStationLCD.h>

#include <SimpleRobot.h>

#include <Compressor.h>
#include <RobotDrive.h>
#include <Joystick.h>
#include <Relay.h>
#include <Victor.h>
#include <Solenoid.h>
#include "PhotoEncoder.h"

#include "Kinect/TurretKinect.h"

float ScaleZ( Joystick& stick ); // scales joystick's Z axis from -1 .. 1 at its limits to 0 .. 1 within same range of movement

class OurRobot : public SimpleRobot {
public:
	static DriverStationLCD* driverStation;

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
