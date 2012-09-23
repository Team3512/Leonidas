//=============================================================================
//File Name: TurretKinect.h
//Description: Declares Kinect for aiming turret
//Author: FRC Team 3512, Spartatroniks
//=============================================================================

#ifndef TURRET_KINECT_H
#define TURRET_KINECT_H

#include "KinectBase.h"

class TurretKinect : public KinectBase {
public:
    // Packet data
    signed short pixelOffset; // amount of pixels turret is off of center (is from -320 to 320)
    unsigned int distance;
    signed char targetSelect;
    unsigned char dummyVar;
    /* FIXME "dummyVar" isn't and should never be used.
     * Including it in packet insertion and extraction somehow prevents the robot code from crashing during data receive.
     */

    typedef enum {
        pxlDeadband = 5
    } targetting;

    TurretKinect( sf::IpAddress IP , unsigned short portNumber );

    void clearValues(); // own definition of clearValues()

protected:
    // own definitions of packet manipulation functions
    void insertPacket();
    void extractPacket();
};

#endif // TURRET_KINECT_H
