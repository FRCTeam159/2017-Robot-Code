#include <Subsystems/FuelSubsystem.h>
#include "../RobotMap.h"
#include "Commands/FuelMonitor.h"


FuelSubsystem::FuelSubsystem() : Subsystem("Fuel"), fuelPusherMotor(FUELMOTOR) {
	fuelPusherMotor.ConfigLimitMode(CANTalon::kLimitMode_SwitchInputsOnly);
	fuelPusherMotor.ConfigRevLimitSwitchNormallyOpen(false);
	fuelPusherMotor.ConfigFwdLimitSwitchNormallyOpen(false);
}

void FuelSubsystem::InitDefaultCommand() {
	// Set the default command for a subsystem here.
	SetDefaultCommand(new FuelMonitor());
}

void FuelSubsystem::SetVoltage(double value) {
	fuelPusherMotor.Set(value);
}

bool FuelSubsystem::AtUpperLimit() {
	return fuelPusherMotor.IsFwdLimitSwitchClosed();
}

bool FuelSubsystem::AtLowerLimit() {
	return fuelPusherMotor.IsRevLimitSwitchClosed();
}
// Put methods for controlling this subsystem
// here. Call these from Commands.
