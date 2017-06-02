// Copyright (c) 2012-2017 FRC Team 3512. All Rights Reserved.

#pragma once

#include "KinectBase.hpp"

class DriveKinect : public KinectBase {
public:
    DriveKinect(sf::IpAddress IP, uint16_t portNumber);

    float getRight();
    float getLeft();

    void setRight(float var);
    void setLeft(float var);

protected:
    // derived definitions of packet manipulation functions
    void insertPacketMutexless(PacketStruct& insertHere);
    void extractPacketMutexless(PacketStruct& extractHere);
    void clearValuesMutexless();

private:
    // Packet data
    float right;
    float left;
};
