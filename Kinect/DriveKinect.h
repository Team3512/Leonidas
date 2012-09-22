//=============================================================================
//File Name: DriveKinect.h
//Description: Declares Kinect for driving robot
//Author: FRC Team 3512, Spartatroniks
//=============================================================================

#ifndef DRIVE_KINECT_H
#define DRIVE_KINECT_H

#include "KinectBase.h"

class DriveKinect : public KinectBase {
public:
	float right;
	float left;

	DriveKinect( sf::IpAddress IP , unsigned short portNumber );

	void clearValues();

protected:
	void insertPacket();
	void extractPacket();
};

#endif // DRIVE_KINECT_H
