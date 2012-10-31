//=============================================================================
//File Name: DriverStationDisplay.cpp
//Description: Receives IP address from remote host then sends HUD data there
//Author: FRC Team 3512, Spartatroniks
//=============================================================================

#include "DriverStationDisplay.hpp"
#include <cstdlib>
#include <loadLib.h>
#include <rebootLib.h>
#include <fcntl.h>

DriverStationDisplay* DriverStationDisplay::m_dsDisplay = NULL;

DriverStationDisplay::~DriverStationDisplay() {
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

        else if ( std::strcmp( m_recvBuffer , "reload" ) == 0 ) {
            // Spawns kernel task to reload image
            int program = open( "ni-rt/system/AFL.out" , O_RDONLY , 0 );
            loadModule( program , LOAD_ALL_SYMBOLS | LOAD_CPLUS_XTOR_AUTO );
        }

        else if ( std::strcmp( m_recvBuffer , "newsettings" ) == 0 ) {
            // TODO reload settings file
        }

        else if ( std::strcmp( m_recvBuffer , "reboot" ) == 0 ) {
            reboot( 0 ); // reboot cRIO controller (means return 0?)
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
