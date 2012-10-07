//=============================================================================
//File Name: DriveKinect.cpp
//Description: Declares Kinect for driving robot
//Author: FRC Team 3512, Spartatroniks
//=============================================================================

#include "DriveKinect.hpp"

DriveKinect::DriveKinect( sf::IpAddress IP , unsigned short portNumber ) : KinectBase( IP , portNumber ) {
	clearValues();
}

float DriveKinect::getRight() {
    float temp;

    valueMutex.lock();
    temp = right;
    valueMutex.unlock();

    return temp;
}

float DriveKinect::getLeft() {
    float temp;

    valueMutex.lock();
    temp = left;
    valueMutex.unlock();

    return temp;
}

void DriveKinect::setRight( float var ) {
    valueMutex.lock();
    right = var;
    valueMutex.unlock();
}

void DriveKinect::setLeft( float var ) {
    valueMutex.lock();
    left = var;
    valueMutex.unlock();
}

void DriveKinect::insertPacketMutexless( sf::Packet& insertHere ) {
    insertHere << right << left;
}

void DriveKinect::extractPacketMutexless( sf::Packet& extractHere ) {
    extractHere >> right >> left;
}

void DriveKinect::clearValuesMutexless() {
    right = 0;
    left = 0;
}
