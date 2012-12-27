cmake_minimum_required(VERSION 2.8)

project(Leonidas)

include_directories( "${WIND_BASE}/target/h/WPILib" )

add_subdirectory(SFMLNetwork)
add_subdirectory(SFMLSystem)

set(SRC
    AutonomousModes/Autonomous.cpp
    Kinect/DriveKinect.cpp
    Kinect/TurretKinect.cpp
    Kinect/KinectBase.cpp

    OurRobot.cpp
    OperatorControl.cpp
    Disabled.cpp
    ButtonTracker.cpp
    DriverStationDisplay.cpp
    LockSolenoid.cpp
    Settings.cpp
    ShooterController.cpp
)

add_executable(FRC_UserProgram.out ${SRC})

target_link_libraries(FRC_UserProgram.out SFMLNetwork SFMLSystem)
