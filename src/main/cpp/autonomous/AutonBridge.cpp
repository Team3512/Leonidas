// Copyright (c) 2012-2020 FRC Team 3512. All Rights Reserved.

#include "Robot.hpp"

enum class State { kInit, kMoveFoward, kSlowDown, kIdle };
static State state;

void Robot::AutonBridgeInit() { state = State::kInit; }

// Drives to Coopertition bridge and tips it to our side
void Robot::AutonBridgePeriodic() {
    switch (state) {
        case State::kInit:
            bridgeArm.Set(LockSolenoid::State::kDeployed);
            state = State::kMoveFoward;
            break;
        case State::kMoveFoward:
            bridgeArm.Update();
            drivetrain.TankDrive(1.0, 1.0);

            if (autonTimer.Get() >= 1_s) {
                state = State::kSlowDown;
            }
            break;
        case State::kSlowDown:
            drivetrain.TankDrive(0.5, 0.5);

            if (autonTimer.Get() >= 2_s) {
                drivetrain.TankDrive(0.0, 0.0);
                state = State::kIdle;
            }
            break;
        case State::kIdle:
            break;
    }
}
