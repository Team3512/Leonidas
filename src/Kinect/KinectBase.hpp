// Copyright (c) 2012-2017 FRC Team 3512. All Rights Reserved.

#pragma once

#include <mutex>
#include <thread>

#include <Timer.h>

#include "../SFML/Network/IpAddress.hpp"
#include "../SFML/Network/Packet.hpp"
#include "../SFML/Network/UdpSocket.hpp"

typedef struct PacketStruct {
    sf::Packet packet;
    std::mutex mutex;
} PacketStruct;

class KinectBase {
public:
    KinectBase(
        sf::IpAddress IP,
        uint16_t portNumber);  // takes IP and portNumber to which to bind
    virtual ~KinectBase();

    sf::Socket::Status getOnlineStatus();
    void setOnlineStatus(sf::Socket::Status var);

    void send();  // sends data to single board computer

    /* Zeroes all data in the packet (has mutex wrapping)
     * Used if socket connection fails or data is old
     */
    void clearValues();

protected:
    /* The next two methods are used for packing a derived Kinect class's
     * packet.
     * Each Kinect's packet is unique, so the method must be defined by the
     * derived class.
     */

    // Packs data into user-provided packet for sending to single board computer
    // (NO MUTEXES)
    virtual void insertPacketMutexless(PacketStruct& insertHere) = 0;

    // Unpacks data received from single board computer into user-provided
    // packet (NO MUTEXES)
    virtual void extractPacketMutexless(PacketStruct& extractHere) = 0;

    // Zeroes all data in the packet
    virtual void clearValuesMutexless() = 0;

    // Used for locking a packet during send, receive, and manipulation
    // functions
    PacketStruct receiver;
    PacketStruct sender;

    // locks data received from packet
    std::mutex valueMutex;

private:
    // tells the receive thread to exit when the object instance is destructed
    volatile bool closeThread = false;

    std::thread socketThread;

    sf::UdpSocket kinectSocket;
    sf::UdpSocket sendSocket;

    sf::IpAddress sendIP;  // IP address of data receiver when sending
    uint16_t sendPort;     // stores port of remote sender when data is received

    sf::IpAddress receiveIP;
    uint16_t receivePort;

    Timer valueAge;  // used to throw away old values

    sf::Socket::Status onlineStatus;

    void receive();  // receives data from on-board computer

    // Packs data into sendPacket for sending to single board computer (has
    // mutex wrapping)
    void insertPacket(PacketStruct& insertHere);

    // Unpacks data from packet received from single board computer (has mutex
    // wrapping)
    void extractPacket(PacketStruct& extractHere);
};
