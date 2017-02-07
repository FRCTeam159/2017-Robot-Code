#include "Fuel.h"
#include "../RobotMap.h"

Fuel::Fuel() : Subsystem("Fuel"), FuelPusherMotor(FUELMOTOR) {

}

void Fuel::InitDefaultCommand() {
	// Set the default command for a subsystem here.
	// SetDefaultCommand(new MySpecialCommand());
}

// Put methods for controlling this subsystem
// here. Call these from Commands.
