//=============================================================================
//File Name: OurRobot.cpp
//Description: Main robot class in which all robot sensors and devices are
//             declared
//Author: FRC Team 3512, Spartatroniks
//=============================================================================

#include <DriverStationLCD.h>
#include "OurRobot.h"

float ScaleZ( Joystick& stick) {
	return floorf( 500.f * ( 1.f - stick.GetZ() ) / 2.f ) / 500.f; // CONSTANT^-1 is step value (now 1/500)
}

DriverStationLCD* OurRobot::driverStation = DriverStationLCD::GetInstance();

OurRobot::OurRobot() :
	mainCompressor( 1 , 6 ),
	mainDrive( 3 , 4 , 1 , 2 ),
	driveStick1( 1 ),
	driveStick2( 2 ),
	turretStick( 3 ),
	upperLift( 7 ),
	lowerLift( 8 ),

	shooterMotorLeft( 7 ),
	shooterMotorRight( 6 ),
	rotateMotor( 5 ),
	pinLock( 3 ),

	hammer( 2 ),
	shifter( 1 ),

	shooterEncoder( 1 , 0x20 ),

	turretKinect( "10.35.12.192" , 5614 ) // on-board computer's IP address and port
{
	mainDrive.SetExpiration( 1.f );

	shooterIsManual = true;
	isShooting = false;
	isAutoAiming = false;
}

void OurRobot::DS_PrintOut() {
	/* ===== Kinect Packet Temp Storage ===== */
	// retrieve all values needed from Kinect here to reduce lock time
	turretKinect.valueMutex.lock();

	sf::Socket::Status sOnlineStatus = turretKinect.onlineStatus;
	signed short sPixelOffset = turretKinect.pixelOffset;
	unsigned int sDistance = turretKinect.distance;

	turretKinect.valueMutex.unlock();
	/* ====================================== */

	/* ===== Print to Driver Station LCD ===== */
	driverStation->Clear();

	driverStation->Printf( DriverStationLCD::kUser_Line1 , 1 , "RPM=%f" , shooterEncoder.getRPM() );

	if ( shooterIsManual ) {
		driverStation->Printf( DriverStationLCD::kUser_Line2 , 1 , "MANUAL TargetRPM=%f" , 72.0 * ScaleZ(turretStick) * 60.0 ); // 72 = max RPS of shooter
	}
	else {
		driverStation->Printf( DriverStationLCD::kUser_Line2 , 1 , "AUTO   TargetRPM=%f" , 72.0 * ScaleZ(turretStick) * 60.0 ); // 72 = max RPS of shooter
	}

	driverStation->Printf( DriverStationLCD::kUser_Line3 , 1 , "Lock [ ]" );

	if ( sPixelOffset < TurretKinect::pxlDeadband ) {
		driverStation->Printf( DriverStationLCD::kUser_Line3 , 7 , "x" );
	}
    else {
		driverStation->Printf( DriverStationLCD::kUser_Line3 , 7 , " " );
    }

	driverStation->Printf( DriverStationLCD::kUser_Line3 , 10 , "ScaleZ=%f" , ScaleZ(turretStick) );

	driverStation->Printf( DriverStationLCD::kUser_Line4 , 1 , "KinectOnline [ ]" );

	if ( sOnlineStatus == sf::Socket::Done ) {
		driverStation->Printf( DriverStationLCD::kUser_Line4 , 15 , "x" );
	}
    else if ( sOnlineStatus == sf::Socket::NotReady ) {
		driverStation->Printf( DriverStationLCD::kUser_Line4 , 15 , "?" );
    }
	else {
		driverStation->Printf( DriverStationLCD::kUser_Line4 , 15 , " " );
	}

	if ( isShooting ) {
		driverStation->Printf( DriverStationLCD::kUser_Line5 , 1 , "shooter ON " );
	}
    else {
		driverStation->Printf( DriverStationLCD::kUser_Line5 , 1 , "shooter OFF" );
    }

	if ( isAutoAiming ) {
		driverStation->Printf( DriverStationLCD::kUser_Line5 , 13 , "aAim ON " );
	}
	else {
		driverStation->Printf( DriverStationLCD::kUser_Line5 , 13 , "aAim OFF" );
	}

	driverStation->Printf( DriverStationLCD::kUser_Line6 , 1 , "Dist=%f%s" , sDistance * 0.00328084f , " ft" ); // converts from mm to ft

	driverStation->UpdateLCD();
	/* ====================================== */
}

START_ROBOT_CLASS(OurRobot);
