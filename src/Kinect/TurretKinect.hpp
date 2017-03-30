// Copyright (c) FRC Team 3512, Spartatroniks 2012-2017. All Rights Reserved.

#pragma once

#include "KinectBase.hpp"

class TurretKinect : public KinectBase {
public:
    static const uint32_t pxlDeadband = 5;

    TurretKinect(sf::IpAddress IP, uint16_t portNumber);

    int16_t getPixelOffset();
    uint32_t getDistance();
    int8_t getTargetSelect();

    void setPixelOffset(int16_t var);
    void setDistance(uint32_t var);
    void setTargetSelect(int8_t var);

protected:
    // derived definitions of packet manipulation functions
    void insertPacketMutexless(PacketStruct& insertHere);
    void extractPacketMutexless(PacketStruct& extractHere);
    void clearValuesMutexless();

private:
    // Packet data
    int16_t pixelOffset;  // amount of pixels turret is off of center (is
                          // from -320 to 320)
    uint32_t distance;
    int8_t targetSelect;
};
