// Copyright (c) FRC Team 3512, Spartatroniks 2012-2017. All Rights Reserved.

#include "DriveKinect.hpp"

DriveKinect::DriveKinect(sf::IpAddress IP, uint16_t portNumber)
    : KinectBase(IP, portNumber) {
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

void DriveKinect::setRight(float var) {
    valueMutex.lock();
    right = var;
    valueMutex.unlock();
}

void DriveKinect::setLeft(float var) {
    valueMutex.lock();
    left = var;
    valueMutex.unlock();
}

void DriveKinect::insertPacketMutexless(PacketStruct& insertHere) {
    insertHere.packet << right << left;
}

void DriveKinect::extractPacketMutexless(PacketStruct& extractHere) {
    extractHere.packet >> right >> left;
}

void DriveKinect::clearValuesMutexless() {
    right = 0;
    left = 0;
}
