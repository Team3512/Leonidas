//=============================================================================
//File Name: ButtonTracker.cpp
//Description: Helps user determine if joystick button was just pressed or just
//             released
//Author: FRC Team 3512, Spartatroniks
//=============================================================================

#include <DriverStation.h>
#include "ButtonTracker.h"

bool ButtonTracker::m_driverStationInit = false;
DriverStation* ButtonTracker::m_driverStation = NULL;

ButtonTracker::ButtonTracker( UINT32 port ) {
	if ( !m_driverStationInit ) {
		m_driverStation = DriverStation::GetInstance();
		m_driverStationInit = true;
	}

	m_port = port;
	m_oldStates = 0;
	m_newStates = 0;
}

void ButtonTracker::updateButtons() { // makes "new" values "old" and gets even newer ones
	m_oldStates = m_newStates;
	m_newStates = m_driverStation->GetStickButtons( m_port );
}

bool ButtonTracker::pressedButton( UINT32 button ) {
	return getButtonState( m_oldStates , button ) == false && getButtonState( m_newStates , button ) == true;
}

bool ButtonTracker::releasedButton( UINT32 button ) {
	return getButtonState( m_oldStates , button ) == true && getButtonState( m_newStates , button ) == false;
}

bool ButtonTracker::getButtonState( short& buttonStates , UINT32& button ) {
	return ( (0x1 << (button-1)) & buttonStates ) != 0;
}
