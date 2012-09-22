//=============================================================================
//File Name: TurretKinect.cpp
//Description: Declares Kinect for aiming turret
//Author: FRC Team 3512, Spartatroniks
//=============================================================================

#include "TurretKinect.h"

TurretKinect::TurretKinect( sf::IpAddress IP , unsigned short portNumber ) : KinectBase( IP , portNumber ) {
	clearValues();
}

void TurretKinect::insertPacket() {
	packetMutex.lock();
	valueMutex.lock();
	packet << pixelOffset << distance << targetSelect << dummyVar;
	valueMutex.unlock();
	packetMutex.unlock();
}

void TurretKinect::extractPacket() {
	packetMutex.lock();
	valueMutex.lock();
	packet >> pixelOffset >> distance >> targetSelect >> dummyVar;
	valueMutex.unlock();
	packetMutex.unlock();
}

void TurretKinect::clearValues() {
	valueMutex.lock();
	pixelOffset = 0;
	distance = 0;
	targetSelect = 0;
	dummyVar = 0;
	KinectBase::clearValues();
	valueMutex.unlock();
}
