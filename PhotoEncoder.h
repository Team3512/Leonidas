//=============================================================================
//File Name: PhotoEncoder.h
//Description: Handles interfacing with a custom photo-interrupt encoder
//Author: FRC Team 3512, Spartatroniks
//=============================================================================

#ifndef PHOTO_ENCODER_H
#define PHOTO_ENCODER_H

#include <SensorBase.h>

class I2C;

class PhotoEncoder : public SensorBase {
public:
	PhotoEncoder( UINT32 slot , UINT32 address );
	virtual ~PhotoEncoder();
	virtual float getRPM();

protected:
	I2C* m_i2c;

private:
	UINT32 address;

	UINT32 data;
	UINT8 dataArray[4];
};

#endif // PHOTO_ENCODER_H
