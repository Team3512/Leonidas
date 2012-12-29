//=============================================================================
//File Name: OurRobot.cpp
//Description: Main robot class in which all robot sensors and devices are
//             declared
//Author: FRC Team 3512, Spartatroniks
//=============================================================================

#include "OurRobot.hpp"
#include "DriverStationDisplay.hpp"

float ScaleZ( Joystick& stick) {
    // CONSTANT^-1 is step value (now 1/500)
    return floorf( 500.f * ( 1.f - stick.GetZ() ) / 2.f ) / 500.f;
}

OurRobot::OurRobot() :
    Settings( "/ni-rt/system/IPSettings.txt" ),

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

    // single board computer's IP address and port
    turretKinect( getValueFor( "SBC_IP" ) , atoi( getValueFor( "SBC_Port" ).c_str() ) ),

    pidControl()
{
    driverStation = DriverStationDisplay::getInstance( atoi( Settings::getValueFor( "DS_Port" ).c_str() ) );

    autonModes.addMethod( "Shoot" , &OurRobot::AutonShoot , this );
    autonModes.addMethod( "Bridge" , &OurRobot::AutonBridge , this );
    autonModes.addMethod( "Feed" , &OurRobot::AutonFeed , this );

    // let motors run for up to 1 second uncontrolled before shutting them down
    mainDrive.SetExpiration( 1.f );

    pidControl.Initialize( &shooterEncoder , &shooterMotorLeft , &shooterMotorRight );

    shooterIsManual = false;
    isShooting = false;
    isAutoAiming = false;

    autonMode = 0;
}

OurRobot::~OurRobot() {
    DriverStationDisplay::freeInstance();
    driverStation = NULL;
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
     *
     * Autonomous Modes (contained in rest of packet):
     * std::string: autonomous routine name
     * ...
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

    for ( unsigned int i ; i < autonModes.size() ; i++ ) {
        *driverStation << autonModes.name( i );
    }

    driverStation->receiveFromDS( &autonMode );
    driverStation->sendToDS();
    /* ====================================== */
}

START_ROBOT_CLASS(OurRobot);
