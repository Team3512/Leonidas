//=============================================================================
//File Name: DriverStationDisplay.hpp
//Description:
//Author: FRC Team 3512, Spartatroniks
//=============================================================================

#ifndef DRIVER_STATION_DISPLAY_HPP
#define DRIVER_STATION_DISPLAY_HPP

/* This class allows you to pack data into an SFML packet and send it to an
 * application on the DriverStation that displays it in a GUI.
 *
 * USAGE:
 * 1) Call DriverStationDisplay::getInstance() to create an instance of this
 *    class
 * 2) Call clear() on the pointer to empty the packet before adding new data
 * 3) Add new data with the << operator (e.g. *dsPtr << 4.f; *dsPtr << myVar;)
 * 4) After all data is packed, call sendToDS() to send the data to the Driver
 *    Station
 *
 * Extract the received packet on the DriverStation with the >> operator like
 * any other SFML packet.
 *
 * Note: Make sure that the application on the Driver Station extracts the same
 *       types from the received packet in the same order in which you inserted
 *       them.
 */

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
