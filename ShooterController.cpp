// Implementation of class ShooterController
#include <Victor.h>
#include <Counter.h>
#include "ShooterController.hpp"
#include <iostream> // FIXME

double getRPM( Counter* counter ) {
    return 60.f / ( 16.f * counter->GetPeriod() );
}

//------------------------------------------------------------------------------
// Static items
// NOTE: Adjust these based on empirical testing.

// Proportional control gain values
float ShooterController::GAIN_KP = 0.5F;
float ShooterController::GAIN_KI = 0.0F;
float ShooterController::GAIN_KD = 0.0F;

// Zero RPM band
float ShooterController::ZERO_RPM_BAND = 1.0F;

//
float ShooterController::RPM_RANGE_BAND = 100.0F;

//------------------------------------------------------------------------------
// Constructor

ShooterController::ShooterController()
{
	m_pEncoder = NULL;
	m_pShooterMotorLeft = NULL;
	m_pShooterMotorRight = NULL;

	m_fTargetRPM = 0.0F;
	m_fTargetMotorOutput = 0.0F;
	m_bIsReady = false;
	m_fPreviousError.RPM = 0.0F;
	m_fPreviousError.time = 0.0F;
	m_fAccumulatedError = 0.0F;
	m_fCurrentMotorOutput = 0.0F;
	m_fCurrentRPMOutput = 0.0F;
	m_bGotGoodEncoderValue = false;
	m_fLastEncoderRPM = 0.0F;

	InitializeMotorSpeedTable();
	InitializeDistanceRPMTable();
}

//------------------------------------------------------------------------------
// Destructor

ShooterController::~ShooterController()
{
	delete m_motorSpeedTable;
	delete m_distanceRPMTable;
}

//------------------------------------------------------------------------------
// Given a known RPM, determine the corresponding motor value.
// This is the motor output value for a stable system.

float ShooterController::DetermineMotorSpeedFromRPM( float fRPM )
{
	float result;

	// Make sure we return zero for zero.
	if (fRPM == 0.0F)
	{
		result = 0.0F;
	}
	else
	{
		// Find the table entry in which the given RPM fits.
		unsigned int index;
		for (index = 1; index < m_motorSpeedTableLength; index++)
		{
			int indexPrevious = index - 1;
			float previousRotations = m_motorSpeedTable[indexPrevious].rotationsPerMinute;
			float currentRotations = m_motorSpeedTable[index].rotationsPerMinute;

			if (fRPM >= previousRotations && fRPM <= currentRotations)
			{
				// Interpolate between previous and current table entries.
				float p = (fRPM - previousRotations) / (currentRotations - previousRotations);
				float previousMotorValue = m_motorSpeedTable[indexPrevious].motorValue;
				result = previousMotorValue + p*(m_motorSpeedTable[index].motorValue - previousMotorValue);
				break;
			}
		}

		// Should always find an entry, but just in case, use largest possible value.
		if (index >= m_motorSpeedTableLength)
			result = m_motorSpeedTable[ m_motorSpeedTableLength-1 ].motorValue;
	}

	return result;
}

//------------------------------------------------------------------------------
// Given a distance from the target, compute the desired RPM.
// Returns zero if not within shooting range.

float ShooterController::DetermineRPMFromTargetDistance( float distanceFt )
{
	float fResultRPM = 0.0F;

	// Make sure we return zero for zero.
	if (distanceFt > 0.0F)
	{
		// Find the table entry that includes the specified distance.
		for (unsigned int index = 1; index < m_distanceRPMTableLength; index++)
		{
			// Interpolate between previous and current table entries.
			unsigned int indexPrevious = index - 1;
			float previousDistance = m_distanceRPMTable[ indexPrevious ].targetDistanceFt;
			float currentDistance = m_distanceRPMTable[ index ].targetDistanceFt;
			if (distanceFt >= previousDistance && distanceFt <= currentDistance)
			{
				float p = (distanceFt - previousDistance) / (currentDistance - previousDistance);
				float previousRPMValue = m_distanceRPMTable[indexPrevious].rotationsPerMinute;
				fResultRPM = previousRPMValue + p*(m_distanceRPMTable[index].rotationsPerMinute - previousRPMValue);
				break;
			}
		}
	}

	return fResultRPM;
}

//------------------------------------------------------------------------------

void ShooterController::Initialize( Counter* pEncoder, Victor* pShooterMotorLeft, Victor* pShooterMotorRight )
{
	m_pEncoder = pEncoder;
	m_pShooterMotorLeft = pShooterMotorLeft;
	m_pShooterMotorRight = pShooterMotorRight;

	m_timer.Reset();
	m_timer.Start();
}

//------------------------------------------------------------------------------

void ShooterController::InitializeDistanceRPMTable()
{
	// Create table to convert between shooting distance and rotational speed.
	// Include only valid shooting ranges.

	m_distanceRPMTableLength = 2;
	m_distanceRPMTable = new TargetDistanceRPMTableEntry[ m_distanceRPMTableLength ];

	// TODO Add more values to the table
	// Must fill table in increasing value of distance.
	m_distanceRPMTable[0].targetDistanceFt = 12.0F;
	m_distanceRPMTable[0].rotationsPerMinute = 2242.0F;

	m_distanceRPMTable[1].targetDistanceFt = 15.0F;
	m_distanceRPMTable[1].rotationsPerMinute = 2242.0F;
}

//------------------------------------------------------------------------------

void ShooterController::InitializeMotorSpeedTable()
{
	// Create table to convert between rotational speed and motor output.

	m_motorSpeedTableLength = 11;
	m_motorSpeedTable = new MotorRPMTableEntry[m_motorSpeedTableLength];

	// Must fill table in increasing order of RPM.

	// By definition, no power means zero RPM.
	m_motorSpeedTable[0].motorValue = 0.0F;
	m_motorSpeedTable[0].rotationsPerMinute = 0.0F;

	m_motorSpeedTable[1].motorValue = 0.1F;
	m_motorSpeedTable[1].rotationsPerMinute = 238.1F;

	m_motorSpeedTable[2].motorValue = 0.2F;
	m_motorSpeedTable[2].rotationsPerMinute = 2018.4F;

	m_motorSpeedTable[3].motorValue = 0.3F;
	m_motorSpeedTable[3].rotationsPerMinute = 2914.7F;

	m_motorSpeedTable[4].motorValue = 0.4F;
	m_motorSpeedTable[4].rotationsPerMinute = 3402.6F;

	m_motorSpeedTable[5].motorValue = 0.5F;
	m_motorSpeedTable[5].rotationsPerMinute = 3695.0F;

	m_motorSpeedTable[6].motorValue = 0.6F;
	m_motorSpeedTable[6].rotationsPerMinute = 3856.2F;

	m_motorSpeedTable[7].motorValue = 0.7F;
	m_motorSpeedTable[7].rotationsPerMinute = 3991.1F;

	m_motorSpeedTable[8].motorValue = 0.8F;
	m_motorSpeedTable[8].rotationsPerMinute = 4086.0F;

	m_motorSpeedTable[9].motorValue = 0.9F;
	m_motorSpeedTable[9].rotationsPerMinute = 4161.1F;

	// By definition, the last table entry is the maximum possible shooter rotational speed.
	m_motorSpeedTable[10].motorValue = 1.0F;
	m_motorSpeedTable[10].rotationsPerMinute = 4232.0F;}

//------------------------------------------------------------------------------
// Resets error calculation, which becomes void when the target value changes.

void ShooterController::ResetCalibration()
{
	// Previous error calculation is void.
	m_fPreviousError.RPM = 0.0F;
	m_fPreviousError.time = 0.0F;

	m_fAccumulatedError = 0.0F;

	// Determine motor output that corresponds to target RPM.
	m_fTargetMotorOutput = DetermineMotorSpeedFromRPM( m_fTargetRPM );

	if (m_bGotGoodEncoderValue)
	{
		// Since we have an encoder reading, force an update based on the latest
		// encoder value.
		Update();
	}
	else
	{
		// Since we have no encoder value to go on, just set the output motors to the target value.
		m_fCurrentRPMOutput = m_fTargetRPM;
		SetMotorOutputs( m_fTargetMotorOutput );
	}
}

//------------------------------------------------------------------------------
// Actually outputs to the shooter's left and right motors.
//
//	fValue - Motor output control value (0..1)

void ShooterController::SetMotorOutputs( float fMotorValue )
{
	// Safety checks
	if (fMotorValue < 0.0F)
		fMotorValue = 0.0F;
	else if (fMotorValue > 1.0F)
		fMotorValue = 1.0F;

	// Make sure zero is not negative zero.
	if (fMotorValue == 0.0F)
	{
		m_pShooterMotorLeft->Set( 0.0F );
		m_pShooterMotorRight->Set( 0.0F );
	}
	else
	{
		// Motors turn in opposite directions, since they are on opposite sides
		// of the shooter wheel.
		m_pShooterMotorLeft->Set( -fMotorValue );
		m_pShooterMotorRight->Set( fMotorValue );
	}

	m_fCurrentMotorOutput = fMotorValue;
}

//------------------------------------------------------------------------------

void ShooterController::SetShooterRPM( float rpm )
{
	if (rpm < ZERO_RPM_BAND)
		rpm = 0.0F;

	// If target RPM is (significantly) different from current target, or zero...
	float delta = rpm - m_fTargetRPM;
	if ((delta < -ZERO_RPM_BAND || delta > ZERO_RPM_BAND) || rpm == 0.0F)
	{
		// Our target has changed.
		m_fTargetRPM = rpm;
		ResetCalibration();
	}
}

//------------------------------------------------------------------------------

void ShooterController::SetTargetDistance( float ft )
{
	// What rotational speed do we need for this distance?
	float fRPM = DetermineRPMFromTargetDistance( ft );
	std::cout << "tRPM=" << fRPM << "\n";
	SetShooterRPM( fRPM );
}

//------------------------------------------------------------------------------
// Called frequently to adjust motor settings, using target setting and tracking
// error.

void ShooterController::Update()
{
	// Get a new encoder value.
	m_bGotGoodEncoderValue = true;
	m_fLastEncoderRPM = getRPM( m_pEncoder );	// the new value

	// Special check for zero target, to ensure motor speed really goes to zero.
	// (Make sure we turn it off, regardless of encoder value and status.)
	if (m_fTargetRPM < ZERO_RPM_BAND)
	{
		SetMotorOutputs( 0.0F );

		// Look for actual RPM to go to zero.
		if (m_bGotGoodEncoderValue)
			m_bIsReady = (m_fLastEncoderRPM < RPM_RANGE_BAND);
		else
			m_bIsReady = false;
	}
	else {
		// How far is the actual rotational speed from our target?
		// NOTE: Error calculation is in RPMs.
		TimeRPM fErrorRPM = { m_timer.Get() , m_fTargetRPM - m_fLastEncoderRPM };

		// Compute amount to adjust motor output as:
		//	ADJUSTMENT = (ERROR(P) * GAIN(P)) + (ERROR(D) * GAIN(D)) + ((ERROR(I) * GAIN(I))

		float adjustRPMs = fErrorRPM.RPM * GAIN_KP;

		// Add integral control error, which is sum of all errors since output target was set.
		m_fAccumulatedError += fErrorRPM.RPM * ( fErrorRPM.time - m_fPreviousError.time );
		adjustRPMs += m_fAccumulatedError * GAIN_KI;

		// Add derivative error, which is difference between this error and the previous cycle's error.
		adjustRPMs += (fErrorRPM.RPM - m_fPreviousError.RPM) / ( fErrorRPM.time - m_fPreviousError.time ) * GAIN_KD;

		// 'adjustRPMs' is now the total error adjustment.
		// After error adjustment, this is what we want to set the motor speed to
		// (converting from RPM to motor output).
		m_fCurrentRPMOutput += adjustRPMs;

		m_fPreviousError = fErrorRPM;

		// Convert this RPM value to a motor output value.
		float fNewMotorOutput = DetermineMotorSpeedFromRPM( m_fCurrentRPMOutput );

		SetMotorOutputs( fNewMotorOutput );

		// Is actual RPM within target range?
		m_bIsReady = (fErrorRPM.RPM > -RPM_RANGE_BAND) && (fErrorRPM.RPM < RPM_RANGE_BAND);
	}

}

//------------------------------------------------------------------------------
