//=============================================================================
//File Name: TurretKinect.cpp
//Description: Declares Kinect for aiming turret
//Author: FRC Team 3512, Spartatroniks
//=============================================================================

#include "TurretKinect.h"

const unsigned int TurretKinect::pxlDeadband = 5;

TurretKinect::TurretKinect( sf::IpAddress IP , unsigned short portNumber ) : KinectBase( IP , portNumber ) {
	clearValues();
}

signed short TurretKinect::getPixelOffset() {
    signed short temp;

    valueMutex.lock();
    temp = pixelOffset;
    valueMutex.unlock();

    return temp;
}

unsigned int TurretKinect::getDistance() {
    signed short temp;

    valueMutex.lock();
    temp = distance;
    valueMutex.unlock();

    return temp;
}

signed char TurretKinect::getTargetSelect() {
    signed short temp;

    valueMutex.lock();
    temp = targetSelect;
    valueMutex.unlock();

    return temp;
}

void TurretKinect::setPixelOffset( signed short var ) {
    valueMutex.lock();
    pixelOffset = var;
    valueMutex.unlock();
}

void TurretKinect::setDistance( unsigned int var ) {
    valueMutex.lock();
    distance = var;
    valueMutex.unlock();
}

void TurretKinect::setTargetSelect( signed char var ) {
    valueMutex.lock();
    targetSelect = var;
    valueMutex.unlock();
}

void TurretKinect::clearValues() {
    valueMutex.lock();
    pixelOffset = 0;
    distance = 0;
    targetSelect = 0;
    //dummyVar = 0;
    KinectBase::clearValues();
    valueMutex.unlock();
}

void TurretKinect::insertPacket() {
	packetMutex.lock();
	valueMutex.lock();
	packet << pixelOffset << distance << targetSelect;// << dummyVar;
	valueMutex.unlock();
	packetMutex.unlock();
}

void TurretKinect::insertPacketMutexless( sf::Packet& tempPacket ) {
    tempPacket << targetSelect;
}

void TurretKinect::extractPacket() {
	packetMutex.lock();
	valueMutex.lock();
	packet >> pixelOffset >> distance >> targetSelect;// >> dummyVar;
	valueMutex.unlock();
	packetMutex.unlock();
}
