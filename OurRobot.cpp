//=============================================================================
//File Name: OurRobot.cpp
//Description: Main robot class in which all robot sensors and devices are
//             declared
//Author: FRC Team 3512, Spartatroniks
//=============================================================================

#include "OurRobot.h"
#include "DriverStationDisplay.h"

float ScaleZ( Joystick& stick) {
    return floorf( 500.f * ( 1.f - stick.GetZ() ) / 2.f ) / 500.f; // CONSTANT^-1 is step value (now 1/500)
}

DriverStationDisplay* OurRobot::driverStation = DriverStationDisplay::getInstance();

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

    shooterEncoder( 4 ),

    turretKinect( "10.35.12.192" , 5614 ) // on-board computer's IP address and port
{
    mainDrive.SetExpiration( 1.f ); // let motors run for 1 second uncontrolled before shutting them down

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
    driverStation->clear();

    *driverStation << 60.f / ( 16.f * shooterEncoder.GetPeriod() );

    *driverStation << shooterIsManual;

    *driverStation << ScaleZ(turretStick); // This was "72.f * ScaleZ(turretStick) * 60.f"

    *driverStation << static_cast<bool>( sPixelOffset < TurretKinect::pxlDeadband );

    *driverStation << sOnlineStatus;

    *driverStation << isShooting;

    *driverStation << isAutoAiming;

    /* Inserts distance to target
     * 0.00328084f converts from millimeters to feet
     */
    *driverStation << sDistance * 0.00328084f;

    driverStation->sendToDS();
    /* ====================================== */
}

START_ROBOT_CLASS(OurRobot);
