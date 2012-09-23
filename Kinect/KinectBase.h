//=============================================================================
//File Name: KinectBase.h
//Description: Defines interface for all types of Kinect usage
//Author: FRC Team 3512, Spartatroniks
//=============================================================================

#ifndef KINECT_BASE_H
#define KINECT_BASE_H

#include "../SFML/Network/IpAddress.hpp"
#include "../SFML/Network/Packet.hpp"
#include "../SFML/Network/UdpSocket.hpp"
#include "../SFML/System/Clock.hpp"
#include "../SFML/System/Mutex.hpp"
#include "../SFML/System/Thread.hpp"

class KinectBase {
public:
	KinectBase( sf::IpAddress IP , unsigned short portNumber ); // takes IP and portNumber to which to bind
	virtual ~KinectBase();

	void send(); // sends data to on-board computer
	void receive(); // receives data from on-board computer

	/* The next two methods are used for packing a derived Kinect class's packet.
	 * Each Kinect's packet is unique, so the method must be defined by the derived class.
	 */

	// Packs data into packet for sending to on-board computer
	virtual void insertPacket() = 0;

	// Unpacks data received from on-board computer
	virtual void extractPacket() = 0;

	/* This zeroes all data in the packet (used if socket connection fails or data is old).
     * KinectBase::clearValues() should be called at end of all clearValues() implementations to reset the timer
     */
	virtual void clearValues() = 0;

	sf::Socket::Status onlineStatus;
	sf::Mutex valueMutex; // locks data received from packet

protected:
	sf::Thread socketThread;
	sf::UdpSocket kinectSocket;

	sf::Clock valueAge; // used to throw away old values

	sf::IpAddress sourceIP;
	unsigned short sourcePort;

	sf::IpAddress receiveIP;
	unsigned short receivePort;

	sf::Packet packet;
	sf::Mutex packetMutex; // locks packet for insertion, extraction, and receive operations

private:
	static bool closeThreads;
};

#endif // KINECT_BASE_H