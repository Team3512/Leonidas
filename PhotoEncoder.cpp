//=============================================================================
//File Name: PhotoEncoder.cpp
//Description: Handles interfacing with a custom photo-interrupt encoder
//Author: FRC Team 3512, Spartatroniks
//=============================================================================

#include "PhotoEncoder.h"
#include <DigitalModule.h>
#include <I2C.h>
#include <iostream>//TODO

PhotoEncoder::PhotoEncoder( UINT32 slot , UINT32 address ) : m_i2c( NULL ) {
	DigitalModule* module = DigitalModule::GetInstance( slot );
	if( module ) {
		m_i2c = module->GetI2C( address );
		m_i2c->SetCompatibilityMode( true );
	}
}

PhotoEncoder::~PhotoEncoder() {
	delete m_i2c;
	m_i2c = NULL;
}

float PhotoEncoder::getRPM() {
	if ( m_i2c->Transaction( NULL , 0 , dataArray , 4 ) ) {
		std::cout << "FAIL ";
		//return 0;
	}

	data = dataArray[3];
	data |= dataArray[2] << 8;
	data |= dataArray[1] << 16;
	data |= dataArray[0] << 24;

	std::cout << "data=" << data << "\n";
	return ( 1250000.f * 60.f ) / ( 16.f * data );
}
