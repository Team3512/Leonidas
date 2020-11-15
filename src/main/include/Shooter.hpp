// Copyright (c) 2020 FRC Team 3512. All Rights Reserved.

#pragma once

#include <frc/Encoder.h>
#include <frc/Victor.h>
#include <frc/controller/PIDController.h>
#include <units/units.h>

class Shooter {
public:
    Shooter();

    /**
     * Enables shooter controller.
     */
    void Enable();

    /**
     * Disables shooter controller.
     */
    void Disable();

    /**
     * Returns true if the shooter controller is enabled.
     */
    bool IsEnabled() const;

    /**
     * Sets the flywheel's reference velocity.
     */
    void SetReference(units::revolutions_per_minute_t angularVelocity);

    /**
     * Returns true if the controller has reached the reference.
     */
    bool AtReference() const;

    /**
     * Updates motor outputs with controller output.
     */
    void Update();

private:
    static constexpr auto kMaxSpeed = 4000_rpm;

    frc::Victor m_motor1{7};
    frc::Victor m_motor2{6};
    frc::Encoder m_encoder{0, 1};
    frc2::PIDController m_controller{0.003, 0.0, 0.0, 20_ms};
    bool m_enabled = false;
};
