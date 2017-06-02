// Copyright (c) 2012-2017 FRC Team 3512. All Rights Reserved.

#include "Robot.hpp"

#include <sys/times.h>

#include <cstring>
#include <string>
#include <thread>

#include "ButtonTracker.hpp"
#include "DriverStationDisplay.hpp"

int gettimeofday(struct timeval* tv_ptr, void* ptr);

float ScaleZ(Joystick& stick) {
    // CONSTANT^-1 is step value (now 1/500)
    return floorf(500.f * (1.f - stick.GetZ()) / 2.f) / 500.f;
}

Robot::Robot() : Settings("/ni-rt/system/RobotSettings.txt") {
    struct timeval rawTime;

    /* Store the current time into startTime as the fixed starting point
     * for our graph.
     */
    gettimeofday(&rawTime, nullptr);
    startTime = rawTime.tv_usec / 1000 + rawTime.tv_sec * 1000;
    lastTime = startTime;

    driverStation = DriverStationDisplay::getInstance(
        std::atoi(Settings::getValueFor("DS_Port").c_str()));

    autonModes.addMethod("Shoot", &Robot::AutonShoot, this);
    autonModes.addMethod("Bridge", &Robot::AutonBridge, this);
    autonModes.addMethod("Feed", &Robot::AutonFeed, this);

    // let motors run for up to 1 second uncontrolled before shutting them down
    mainDrive.SetExpiration(1.f);

    shooter.setPID(std::atof(getValueFor("PID_P").c_str()),
                   std::atof(getValueFor("PID_I").c_str()),
                   std::atof(getValueFor("PID_D").c_str()));
    shooter.stop();
}

Robot::~Robot() {
    DriverStationDisplay::freeInstance();
    driverStation = nullptr;
}

void Robot::Autonomous() {
    mainCompressor.Start();
    autoTime.Reset();
    autoTime.Start();

    autonModes.execAutonomous(autonMode);

    autoTime.Stop();
}

void Robot::OperatorControl() {
    mainCompressor.Start();

    isAutoAiming = false;

    bridgeArm.Set(LockSolenoid::Retracted);

    ButtonTracker driveStick1Buttons(1);
    ButtonTracker driveStick2Buttons(2);
    ButtonTracker turretStickButtons(3);

    shooter.start();

    while (IsEnabled() && IsOperatorControl()) {
        DS_PrintOut();

        // update "new" value of joystick buttons
        driveStick1Buttons.updateButtons();
        driveStick2Buttons.updateButtons();
        turretStickButtons.updateButtons();

        /* ===== AIM ===== */
        if (std::fabs(turretStick.GetX()) > 0.06) {  // manual turret movement
            // Squaring gives finer control with a smaller joystick movement
            // while retaining max speed
            rotateMotor.Set(
                -(turretStick.GetX() * std::fabs(turretStick.GetX())));
        } else if (isAutoAiming) {  // let autoAiming take over if activated and
                                    // user isn't rotating turret
            if (std::fabs(turretKinect.getPixelOffset()) <
                TurretKinect::pxlDeadband) {  // deadband so shooter won't
                                              // jitter
                rotateMotor.Set(0);
            } else {
                /* Set(x) = x / 320
                 * -320 <= x <= 320 therefore Set( -1 <= x / 320 <= 1 )
                 * smooth tracking because as the turret aims closer to the
                 * target, the motor value gets smaller
                 */
                rotateMotor.Set(
                    static_cast<float>(turretKinect.getPixelOffset()) / 320.f);
            }
        } else {
            rotateMotor.Set(0);
        }
        /* =============== */

        /* ===================== AutoAim ===================== */
        if (turretStickButtons.releasedButton(3)) {
            isAutoAiming = !isAutoAiming;  // toggles autoAim
        }
        /* =================================================== */

        /* ================= Target Selection ================ */
        // selecting target to left of current target
        if (turretStickButtons.releasedButton(4)) {
            turretKinect.setTargetSelect(-1);

            turretKinect.send();
        }

        // selecting target to right of current target
        if (turretStickButtons.releasedButton(5)) {
            turretKinect.setTargetSelect(1);

            turretKinect.send();
        }
        /* =================================================== */

        /* ============== Toggle Shooter Motors ============== */
        // turns shooter on/off
        if (turretStickButtons.releasedButton(1)) {  // if released trigger
            if (shooter.isShooting()) {
                shooter.stop();
            } else {
                shooter.start();
            }
        }

        shooter.setScale(ScaleZ(turretStick));

        // toggle manual RPM setting vs setting with encoder input
        if (turretStickButtons.releasedButton(2)) {
            if (shooter.getControlMode() == Shooter::Manual) {
                shooter.setControlMode(Shooter::PID);
            } else {
                shooter.setControlMode(Shooter::Manual);
            }
        }
        /* =================================================== */

        /* ===== Ball Intake/Conveyor ===== */
        if (turretStick.GetRawButton(6)) {
            lift.Set(Relay::kForward);  // move lift up
        } else if (turretStick.GetRawButton(7)) {
            lift.Set(Relay::kReverse);  // move lift down
        } else {
            lift.Set(Relay::kOff);  // turn off lift
        }
        /* ================================ */

        /* ===== Bridge Arm ===== */
        if (driveStick2Buttons.releasedButton(1)) {
            LockSolenoid::Status armState = bridgeArm.Get();

            /* Toggles state of bridge arm between Retracted and Deployed
             * (Doesn't toggle if the arm is transitioning)
             */
            if (armState == LockSolenoid::Retracted) {
                bridgeArm.Set(LockSolenoid::Deployed);
            } else if (armState == LockSolenoid::Deployed) {
                bridgeArm.Set(LockSolenoid::Retracted);
            }
        }

        // Makes sure bridge arm completed transitioning between states
        bridgeArm.Update();
        /* ====================== */

        // Reset PID constants
        if (turretStickButtons.releasedButton(8)) {
            Settings::update();

            shooter.setPID(std::atof(getValueFor("PID_P").c_str()),
                           std::atof(getValueFor("PID_I").c_str()),
                           std::atof(getValueFor("PID_D").c_str()));
        }

        // move robot based on two joystick inputs
        mainDrive.ArcadeDrive(ScaleZ(driveStick1) * driveStick1.GetY(),
                              ScaleZ(driveStick2) * driveStick2.GetX(), false);

        Wait(0.1);
    }

    shooter.stop();
}

void Robot::Disabled() {
    mainCompressor.Stop();
    bridgeArm.Set(LockSolenoid::Retracted);

    ButtonTracker driveStick1Buttons(1);
    ButtonTracker driveStick2Buttons(2);
    ButtonTracker turretStickButtons(3);

    while (IsDisabled()) {
        DS_PrintOut();

        // update "new" value of joystick buttons
        driveStick1Buttons.updateButtons();
        driveStick2Buttons.updateButtons();
        turretStickButtons.updateButtons();

        /* ===================== AutoAim ===================== */
        if (turretStickButtons.releasedButton(3)) {
            isAutoAiming = !isAutoAiming;  // toggles autoAim
        }
        /* =================================================== */

        /* ============== Toggle Shooter Motors ============== */
        // turns shooter on/off
        if (turretStickButtons.releasedButton(1)) {  // if released trigger
            if (shooter.isShooting()) {
                shooter.stop();
            } else {
                shooter.start();
            }
        }

        // toggle manual RPM setting vs setting with encoder input
        if (turretStickButtons.releasedButton(2)) {
            if (shooter.getControlMode() == Shooter::Manual) {
                shooter.setControlMode(Shooter::PID);
            } else {
                shooter.setControlMode(Shooter::Manual);
            }
        }
        /* =================================================== */

        bridgeArm.Update();

        // Reset PID constants
        if (turretStickButtons.releasedButton(8)) {
            Settings::update();

            shooter.setPID(std::atof(getValueFor("PID_P").c_str()),
                           std::atof(getValueFor("PID_I").c_str()),
                           std::atof(getValueFor("PID_D").c_str()));
        }

        std::this_thread::sleep_for(100ms);
    }
}

void Robot::DS_PrintOut() {
    struct timeval rawTime;
    uint32_t currentTime;

    gettimeofday(&rawTime, nullptr);
    currentTime = rawTime.tv_usec / 1000 + rawTime.tv_sec * 1000;

    if (currentTime - lastTime > 5) {
        // pidGraph.graphData( currentTime - startTime , 5000.f , "PID0" );
        pidGraph.graphData(currentTime - startTime, shooter.getRPM(), "PID0");
        pidGraph.graphData(currentTime - startTime, shooter.getTargetRPM(),
                           "PID1");

        lastTime = currentTime;
    }

    /* ===== Print to Driver Station LCD =====
     * Packs the following variables:
     *
     * std::string: type of data (either "display" or "autonList")
     * uint32_t: drive1 ScaleZ
     * uint32_t: drive2 ScaleZ
     * uint32_t: turret ScaleZ
     * bool: drivetrain is in low gear
     * uint8_t: is hammer mechanism deployed
     * uint32_t: shooter RPM
     * bool: shooter RPM control is manual
     * bool: isShooting
     * bool: isAutoAiming
     * bool: turret is locked on
     * uint8_t: Kinect is online
     * uint32_t: distance to target
     */

    // floats don't work so " * 100000" saves some precision in a UINT

    driverStation->clear();

    *driverStation << static_cast<std::string>("display");

    *driverStation << static_cast<uint32_t>(ScaleZ(driveStick1) * 100000.f);

    *driverStation << static_cast<uint32_t>(ScaleZ(driveStick2) * 100000.f);

    *driverStation << static_cast<uint32_t>(ScaleZ(turretStick) * 100000.f);

    *driverStation << static_cast<bool>(false);

    *driverStation << static_cast<uint8_t>(bridgeArm.Get());

    *driverStation << static_cast<uint32_t>(shooter.getRPM() * 100000.f);

    *driverStation << static_cast<bool>(shooter.getControlMode() ==
                                        Shooter::Manual);

    *driverStation << shooter.isShooting();

    *driverStation << isAutoAiming;

    *driverStation << static_cast<bool>(
        std::fabs(turretKinect.getPixelOffset()) < TurretKinect::pxlDeadband &&
        turretKinect.getOnlineStatus() == sf::Socket::Done);

    *driverStation << static_cast<uint8_t>(turretKinect.getOnlineStatus());

    *driverStation << turretKinect.getDistance();

    driverStation->sendToDS();

    const std::string& command = driverStation->receiveFromDS(&autonMode);

    // If the DS just connected, send it a list of available autonomous modes
    if (std::strcmp(command.c_str(), "connect\r\n") == 0) {
        driverStation->clear();

        *driverStation << static_cast<std::string>("autonList");

        for (uint32_t i = 0; i < autonModes.size(); i++) {
            *driverStation << autonModes.name(i);
        }

        driverStation->sendToDS();
    }
    /* ====================================== */
}

START_ROBOT_CLASS(Robot);
