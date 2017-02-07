#ifndef Fuel_H
#define Fuel_H

#include <Commands/Subsystem.h>
#include "CANTalon.h"

class Fuel : public Subsystem {
private:
	CANTalon FuelPusherMotor;
	// It's desirable that everything possible under private except
	// for methods that implement subsystem capabilities

public:
	Fuel();
	void InitDefaultCommand();
};

#endif  // Fuel_H
