//=============================================================================
//File Name: TurretKinect.hpp
//Description: Declares Kinect for aiming turret
//Author: FRC Team 3512, Spartatroniks
//=============================================================================

#ifndef TURRET_KINECT_HPP
#define TURRET_KINECT_HPP

#include "KinectBase.hpp"

class TurretKinect : public KinectBase {
public:
    static const unsigned int pxlDeadband;

    TurretKinect( sf::IpAddress IP , unsigned short portNumber );

    signed short getPixelOffset();
    unsigned int getDistance();
    signed char getTargetSelect();

    void setPixelOffset( signed short var );
    void setDistance( unsigned int var );
    void setTargetSelect( signed char var );

    void clearValues(); // own definition of clearValues()

protected:
    // own definitions of packet manipulation functions
    void insertPacket();
    void insertPacketMutexless( sf::Packet& tempPacket );
    void extractPacket();

private:
    // Packet data
    signed short pixelOffset; // amount of pixels turret is off of center (is from -320 to 320)
    unsigned int distance;
    signed char targetSelect;
    //unsigned char dummyVar;
    /* FIXME "dummyVar" isn't and should never be used.
     * Including it in packet insertion and extraction somehow prevents the robot code from crashing during data receive.
     */
};

#endif // TURRET_KINECT_HPP
