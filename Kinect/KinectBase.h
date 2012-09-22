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

	void send();
	void receive();

	virtual void insertPacket() = 0;
	virtual void extractPacket() = 0;
	virtual void clearValues() = 0;
	/* This zeroes all data in the packet (used if socket connection fails or data is old).
	 * KinectBase::clearValues() should be called at end of all clearValues() implementations to reset the timer
	 */

	sf::Socket::Status onlineStatus;
	sf::Mutex valueMutex;

protected:
	sf::Thread socketThread;
	sf::UdpSocket kinectSocket;

	sf::Clock valueAge;

	sf::IpAddress sourceIP;
	unsigned short sourcePort;

	sf::IpAddress receiveIP;
	unsigned short receivePort;

	sf::Packet packet;
	sf::Mutex packetMutex;

private:
	static bool closeThreads;
};

#endif // KINECT_BASE_H
