//=============================================================================
//File Name: OurRobot.cpp
//Description: Main robot class in which all robot sensors and devices are
//             declared
//Author: FRC Team 3512, Spartatroniks
//=============================================================================

#include "OurRobot.hpp"
#include "DriverStationDisplay.hpp"

float ScaleZ( Joystick& stick) {
    return floorf( 500.f * ( 1.f - stick.GetZ() ) / 2.f ) / 500.f; // CONSTANT^-1 is step value (now 1/500)
}

DriverStationDisplay* OurRobot::driverStation = DriverStationDisplay::getInstance( 5615 );

OurRobot::OurRobot() :
    mainCompressor( 1 , 6 ),
    mainDrive( 3 , 4 , 1 , 2 ),
    driveStick1( 1 ),
    driveStick2( 2 ),
    turretStick( 3 ),
    lift( 7 ),

    shooterMotorLeft( 7 ),
    shooterMotorRight( 6 ),
    rotateMotor( 5 ),

    shooterEncoder( 6 ),

    shifter( 1 ),
    bridgeArm( 2 , 3 ),

    turretKinect( "10.35.12.6" , 5614 ), // on-board computer's IP address and port

    pidControl()
{
    mainDrive.SetExpiration( 1.f ); // let motors run for 1 second uncontrolled before shutting them down

    pidControl.Initialize( &shooterEncoder , &shooterMotorLeft , &shooterMotorRight );

    shooterIsManual = false;
    isShooting = false;
    isAutoAiming = false;
}

void OurRobot::DS_PrintOut() {
    /* ===== Print to Driver Station LCD =====
     * Packs the following variables:
     *
     * unsigned int: drive1 ScaleZ
     * unsigned int: drive2 ScaleZ
     * unsigned int: turret ScaleZ
     * bool: drivetrain is in low gear
     * unsigned char: is hammer mechanism deployed
     * unsigned int: shooter RPM
     * bool: shooter RPM control is manual
     * bool: isShooting
     * bool: isAutoAiming
     * bool: turret is locked on
     * unsigned char: Kinect is online
     * unsigned int: distance to target
     */

    // floats don't work so " * 100000" saves some precision in a UINT

    driverStation->clear();

    *driverStation << static_cast<unsigned int>(ScaleZ(driveStick1) * 100000.f);

    *driverStation << static_cast<unsigned int>(ScaleZ(driveStick2) * 100000.f);

    *driverStation << static_cast<unsigned int>(ScaleZ(turretStick) * 100000.f);

    *driverStation << static_cast<bool>( !shifter.Get() );

    *driverStation << static_cast<unsigned char>( bridgeArm.Get() );

    *driverStation << static_cast<unsigned int>(60.f / ( 16.f * shooterEncoder.GetPeriod() ) * 100000.f);

    *driverStation << shooterIsManual;

    *driverStation << isShooting;

    *driverStation << isAutoAiming;

    *driverStation << static_cast<bool>( fabs( turretKinect.getPixelOffset() ) < TurretKinect::pxlDeadband
            && turretKinect.getOnlineStatus() == sf::Socket::Done );

    *driverStation << static_cast<unsigned char>( turretKinect.getOnlineStatus() );

    *driverStation << turretKinect.getDistance();

    driverStation->receiveFromDS();
    driverStation->sendToDS();
    /* ====================================== */
}

START_ROBOT_CLASS(OurRobot);
