//=============================================================================
//File Name: DriveKinect.cpp
//Description: Declares Kinect for driving robot
//Author: FRC Team 3512, Spartatroniks
//=============================================================================

#include "DriveKinect.h"

DriveKinect::DriveKinect( sf::IpAddress IP , unsigned short portNumber ) : KinectBase( IP , portNumber ) {
	clearValues();
}

void DriveKinect::insertPacket() {
	packetMutex.lock();
	valueMutex.lock();
	packet << right << left;
	valueMutex.unlock();
	packetMutex.unlock();
}

void DriveKinect::extractPacket() {
	packetMutex.lock();
	valueMutex.lock();
	packet >> right >> left;
	valueMutex.unlock();
	packetMutex.unlock();
}

void DriveKinect::clearValues() {
	valueMutex.lock();
	right = 0;
	left = 0;
	KinectBase::clearValues();
	valueMutex.unlock();
}
