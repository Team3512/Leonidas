//=============================================================================
//File Name: KinectBase.cpp
//Description: Defines interface for all types of Kinect usage
//Author: FRC Team 3512, Spartatroniks
//=============================================================================

#include "KinectBase.h"
#include <iostream> // FIXME
#include <Timer.h>

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
	sf::Packet sendOnlyPacket;
	std::cout << "inserting send data...";
	insertPacketMutexless( sendOnlyPacket );
	std::cout << " done\n";

	std::cout << " sending...";
	sendSocket.send( sendOnlyPacket , sourceIP , sourcePort );
	std::cout << " done\n";
}

void KinectBase::receive() {
	sf::Socket::Status sOnlineStatus; // used to store safe value for Kinect's packet data status

	while ( !closeThreads ) {
		packetMutex.lock();
		std::cout << "receiving...";
		onlineStatus = kinectSocket.receive( packet , receiveIP , receivePort );
		std::cout << " done\n";
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
		Wait( 0.25 );
	}
}

// KinectBase::clearValues() should be called at end of all clearValues() implementations to reset the timer
void KinectBase::clearValues() {
	valueAge.restart();
}
