//=============================================================================
//File Name: PhotoEncoder.cpp
//Description:
//Author: FRC Team 3512, Spartatroniks
//=============================================================================

#include "PhotoEncoder.hpp"
#include <Timer.h>

PhotoEncoder::PhotoEncoder( UINT32 channel , UINT32 teeth , float gearRatio ) : m_counter( channel ) ,
m_rpmAverager( 2 ) ,
m_gearRatio( gearRatio ) ,
m_teeth( teeth ) ,
m_sampleRate( 4.f ) ,
m_isRunning( true ) {
    pthread_mutex_init( &m_dataMutex , NULL );
    pthread_create( &m_sampleThread , NULL , &PhotoEncoder::threadFunc , this );
}

PhotoEncoder::~PhotoEncoder() {
    m_isRunning = false;

    pthread_join( m_sampleThread , NULL );
    pthread_mutex_destroy( &m_dataMutex );
}

void PhotoEncoder::start() {
    m_counter.Start();
}

void PhotoEncoder::stop() {
    m_counter.Stop();
}

void PhotoEncoder::setTeethPerRevolution( UINT32 teeth ) {
    m_teeth = teeth;
}

void PhotoEncoder::setGearRatio( float ratio ) {
    m_gearRatio = ratio;
}

void PhotoEncoder::setAverageSize( UINT32 size ) {
    m_rpmAverager.setSize( size );
}

float PhotoEncoder::getRPM() {
    return m_rpmAverager.getAverage();
}

bool PhotoEncoder::isIncreasing() {
    std::list<float>::iterator index = m_rpmList.end();
    index--;

    // True if old value is less than new value
    return *m_rpmList.begin() < *index;
}

void PhotoEncoder::setSampleRate( UINT32 sampleRate ) {
    pthread_mutex_lock( &m_dataMutex );

    m_sampleRate = sampleRate;

    pthread_mutex_unlock( &m_dataMutex );
}

void* PhotoEncoder::threadFunc( void* object ) {
    PhotoEncoder* encoderObj = static_cast<PhotoEncoder*>( object );

    /* Derivation of RPM:
     * period = seconds per tick of gear
     * 1 / period = ticks per second of gear
     * 60 / period = ticks per minute of gear
     * 56 ticks per revolution therefore: 60 / ( 56 * period ) = revolutions per minute of gear
     * 4 * 60 / ( 56 * period ) = revolutions per minute of shooter wheel
     *
     * shooterRPM = 4.f * 60.f / ( 56.f * shooterEncoder.GetPeriod() );
     */

    // Safe copy of sample rate at which to collect data
    float sampleRate = 0.f;

    float rpmValue = 0.f;

    while ( encoderObj->m_isRunning ) {
        pthread_mutex_lock( &encoderObj->m_dataMutex );

        rpmValue = encoderObj->m_gearRatio * 60.f /
                ( encoderObj->m_teeth * encoderObj->m_counter.GetPeriod() );

        // Add RPM value to rolling average filter
        encoderObj->m_rpmAverager.addValue( rpmValue );

        // Add RPM value to RPM list
        encoderObj->m_rpmList.push_back( encoderObj->m_rpmAverager.getAverage() );
        while ( encoderObj->m_rpmList.size() > 5 ) {
            encoderObj->m_rpmList.pop_front();
        }

        sampleRate = encoderObj->m_sampleRate;

        pthread_mutex_unlock( &encoderObj->m_dataMutex );

        Wait( 1.f / sampleRate );
    }

    return NULL;
}
