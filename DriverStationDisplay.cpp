//=============================================================================
//File Name: DriverStationDisplay.cpp
//Description: Receives IP address from remote host then sends HUD data there
//Author: FRC Team 3512, Spartatroniks
//=============================================================================

#include "DriverStationDisplay.hpp"
#include <cstdlib>
#include <cstring>

DriverStationDisplay* DriverStationDisplay::m_dsDisplay = NULL;

DriverStationDisplay::~DriverStationDisplay() {
    m_socket.unbind();
    std::free( m_recvBuffer );
}

DriverStationDisplay* DriverStationDisplay::getInstance( unsigned short dsPort ) {
    if ( m_dsDisplay == NULL ) {
        m_dsDisplay = new DriverStationDisplay( dsPort );
    }

    return m_dsDisplay;
}

void DriverStationDisplay::sendToDS() {
    if ( m_dsIP != sf::IpAddress::None ) {
        m_socket.send( *static_cast<sf::Packet*>(this) , m_dsIP , m_dsPort );
    }

    // Used for testing purposes
    m_socket.send( *static_cast<sf::Packet*>(this) , sf::IpAddress( 10 , 35 , 12 , 42 ) , m_dsPort );
}

void DriverStationDisplay::receiveFromDS() {
    if ( m_socket.receive( m_recvBuffer , 256 , m_recvAmount , m_recvIP , m_recvPort ) == sf::Socket::Done ) {
        if ( std::strcmp( m_recvBuffer , "connect" ) == 0 ) {
            m_dsIP = m_recvIP;
            m_dsPort = m_recvPort;
        }
    }
}

DriverStationDisplay::DriverStationDisplay( unsigned short portNumber ) : m_dsIP( sf::IpAddress::None ) , m_dsPort( portNumber ) {
    m_socket.bind( portNumber );
    m_socket.setBlocking( false );
    m_recvBuffer = static_cast<char*>( std::malloc( 256 ) );
}

DriverStationDisplay::DriverStationDisplay( const DriverStationDisplay& ) {
}

void DriverStationDisplay::operator=( const DriverStationDisplay& ) {
}
