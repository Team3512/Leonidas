//=============================================================================
//File Name: DriverStationDisplay.hpp
//Description:
//Author: FRC Team 3512, Spartatroniks
//=============================================================================

#ifndef DRIVER_STATION_DISPLAY_HPP
#define DRIVER_STATION_DISPLAY_HPP

#include "SFML/Network/IpAddress.hpp"
#include "SFML/Network/Packet.hpp"
#include "SFML/Network/UdpSocket.hpp"

class DriverStationDisplay : public sf::Packet {
public:
    static DriverStationDisplay* getInstance();

    // sends data currently in packet to Driver Station
    void sendToDS();

private:
    DriverStationDisplay( sf::IpAddress IP , unsigned short portNumber );

    // disallow copy and assignment
    DriverStationDisplay( const DriverStationDisplay& );
    void operator=( const DriverStationDisplay& );

    static DriverStationDisplay* m_dsDisplay;

    sf::UdpSocket socket;

    sf::IpAddress sourceIP; // IP address of Driver Station
    unsigned short sourcePort; // port to which to send data
};

#endif // DRIVER_STATION_DISPLAY_HPP
