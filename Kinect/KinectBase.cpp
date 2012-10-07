//=============================================================================
//File Name: KinectBase.cpp
//Description: Defines interface for all types of Kinect usage
//Author: FRC Team 3512, Spartatroniks
//=============================================================================

#include "KinectBase.hpp"
#include <Timer.h>

bool KinectBase::closeThreads = false;

KinectBase::KinectBase( sf::IpAddress IP , unsigned short portNumber ) : socketThread( &KinectBase::receive , this ) , sourceIP( IP ) , sourcePort( portNumber ) {
	kinectSocket.bind( portNumber );
	socketThread.launch();
}

KinectBase::~KinectBase() {
	closeThreads = true;

	socketThread.wait(); // waits for receive thread to end
	kinectSocket.unbind();
}

sf::Socket::Status KinectBase::getOnlineStatus() {
    sf::Socket::Status temp;

    valueMutex.lock();
    temp = onlineStatus;
    valueMutex.unlock();

    return temp;
}

void KinectBase::setOnlineStatus( sf::Socket::Status var ) {
    valueMutex.lock();
    onlineStatus = var;
    valueMutex.unlock();
}

void KinectBase::send() {
	sf::Packet sendOnlyPacket;
	insertPacketMutexless( sendOnlyPacket );

	sendSocket.send( sendOnlyPacket , sourceIP , sourcePort );
}

void KinectBase::receive() {
	sf::Socket::Status sOnlineStatus; // used to store safe value for Kinect's packet data status

	while ( !closeThreads ) {
		packetMutex.lock();
		onlineStatus = kinectSocket.receive( packet , receiveIP , receivePort );
		packetMutex.unlock();

		// get a safe version of onlineStatus
		valueMutex.lock();
		sOnlineStatus = onlineStatus;
		valueMutex.unlock();

		if ( sOnlineStatus == sf::Socket::Done ) { // data received
			extractPacket(); // unpack new data
			valueAge.restart();
		}
		else if ( sOnlineStatus == sf::Socket::Disconnected || sOnlineStatus == sf::Socket::Error ) { // if socket failed
			clearValues();
		}
		else if ( valueAge.getElapsedTime().asMilliseconds() > 500 ) { // if values received are older than 500ms clear them
			clearValues();
		}

		// else do nothing because the socket is fine; it just doesn't have any new values
	}
}

// KinectBase::clearValues() should be called at end of all clearValues() implementations to reset the timer
void KinectBase::clearValues() {
	valueAge.restart();
}
