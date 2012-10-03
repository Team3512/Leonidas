//=============================================================================
//File Name: DriverStationDisplay.cpp
//Description:
//Author: FRC Team 3512, Spartatroniks
//=============================================================================

#include "DriverStationDisplay.h"

DriverStationDisplay* DriverStationDisplay::m_dsDisplay = NULL;

DriverStationDisplay* DriverStationDisplay::getInstance() {
    if ( m_dsDisplay == NULL ) {
        m_dsDisplay = new DriverStationDisplay( "10.35.12.5" , 5615 );
    }

    return m_dsDisplay;
}

void DriverStationDisplay::sendToDS() {
    socket.send( *dynamic_cast<sf::Packet*>(this) , sourceIP , sourcePort );
}

DriverStationDisplay::DriverStationDisplay( sf::IpAddress IP , unsigned short portNumber ) : sourceIP( IP ) , sourcePort( portNumber ) {
}
