// Copyright (c) FRC Team 3512, Spartatroniks 2012-2017. All Rights Reserved.

#include "TurretKinect.hpp"

const uint32_t TurretKinect::pxlDeadband;

TurretKinect::TurretKinect(sf::IpAddress IP, uint16_t portNumber)
    : KinectBase(IP, portNumber) {
    clearValues();
}

int16_t TurretKinect::getPixelOffset() {
    int16_t temp;

    valueMutex.lock();
    temp = pixelOffset;
    valueMutex.unlock();

    return temp;
}

uint32_t TurretKinect::getDistance() {
    int16_t temp;

    valueMutex.lock();
    temp = distance;
    valueMutex.unlock();

    return temp;
}

int8_t TurretKinect::getTargetSelect() {
    int16_t temp;

    valueMutex.lock();
    temp = targetSelect;
    valueMutex.unlock();

    return temp;
}

void TurretKinect::setPixelOffset(int16_t var) {
    valueMutex.lock();
    pixelOffset = var;
    valueMutex.unlock();
}

void TurretKinect::setDistance(uint32_t var) {
    valueMutex.lock();
    distance = var;
    valueMutex.unlock();
}

void TurretKinect::setTargetSelect(int8_t var) {
    valueMutex.lock();
    targetSelect = var;
    valueMutex.unlock();
}

void TurretKinect::insertPacketMutexless(PacketStruct& insertHere) {
    insertHere.packet << targetSelect;
}

void TurretKinect::extractPacketMutexless(PacketStruct& extractHere) {
    extractHere.packet >> pixelOffset >> distance >> targetSelect;
}

void TurretKinect::clearValuesMutexless() {
    pixelOffset = 0;
    distance = 0;
    targetSelect = 0;
}
