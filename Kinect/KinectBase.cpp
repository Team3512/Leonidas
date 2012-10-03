//=============================================================================
//File Name: KinectBase.cpp
//Description: Defines interface for all types of Kinect usage
//Author: FRC Team 3512, Spartatroniks
//=============================================================================

#include "../SFML/System/Sleep.hpp"
#include "KinectBase.h"

bool KinectBase::closeThreads = false;

KinectBase::KinectBase( sf::IpAddress IP , unsigned short portNumber ) : socketThread( &KinectBase::receive , this ) , sourceIP( IP ) , sourcePort( portNumber ) {
	kinectSocket.bind( portNumber );
	kinectSocket.setBlocking( false );
	socketThread.launch();
}

KinectBase::~KinectBase() {
	closeThreads = true;

	socketThread.wait(); // waits for receive thread to end
	kinectSocket.unbind();
}

void KinectBase::send() {
	packetMutex.lock();
	packet.clear();
	packetMutex.unlock();

	insertPacket();

	packetMutex.lock();
	kinectSocket.send( packet , sourceIP , sourcePort );
	packetMutex.unlock();
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
		sf::sleep( sf::milliseconds( 250 ) );
	}
}

// KinectBase::clearValues() should be called at end of all clearValues() implementations to reset the timer
void KinectBase::clearValues() {
	valueAge.restart();
}
