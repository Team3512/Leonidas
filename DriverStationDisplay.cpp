//=============================================================================
//File Name: DriverStationDisplay.cpp
//Description:
//Author: FRC Team 3512, Spartatroniks
//=============================================================================

#include "DriverStationDisplay.hpp"

DriverStationDisplay* DriverStationDisplay::m_dsDisplay = NULL;

DriverStationDisplay* DriverStationDisplay::getInstance() {
    if ( m_dsDisplay == NULL ) {
        m_dsDisplay = new DriverStationDisplay( "10.35.12.9" , 5615 );
    }

    return m_dsDisplay;
}

void DriverStationDisplay::sendToDS() {
    socket.send( *static_cast<sf::Packet*>(this) , dsIP , dsPort );
    // TODO remove the second send before competition
    socket.send( *static_cast<sf::Packet*>(this) , sf::IpAddress( "10.35.12.42" ) , dsPort );
}

DriverStationDisplay::DriverStationDisplay( sf::IpAddress IP , unsigned short portNumber ) : dsIP( IP ) , dsPort( portNumber ) {
}

DriverStationDisplay::DriverStationDisplay( const DriverStationDisplay& ) {
}

void DriverStationDisplay::operator=( const DriverStationDisplay& ) {
}
