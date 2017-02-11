#include <Subsystems/FuelSubsystem.h>
#include "../RobotMap.h"


FuelSubsystem::FuelSubsystem() : Subsystem("Fuel"), fuelPusherMotor(FUELMOTOR) {
	fuelPusherMotor.ConfigLimitMode(CANTalon::kLimitMode_SwitchInputsOnly);
	fuelPusherMotor.ConfigRevLimitSwitchNormallyOpen(false);
	fuelPusherMotor.ConfigFwdLimitSwitchNormallyOpen(false);
}

void FuelSubsystem::InitDefaultCommand() {
	// Set the default command for a subsystem here.
	// SetDefaultCommand(new MySpecialCommand());
}

// Put methods for controlling this subsystem
// here. Call these from Commands.
