#ifndef Fuel_H
#define Fuel_H
#include "CANTALON.h"
#include <Commands/Subsystem.h>
#include "CANTalon.h"

class FuelSubsystem : public Subsystem {
private:
	CANTalon fuelPusherMotor;
	// It's desirable that everything possible under private except
	// for methods that implement subsystem capabilities

public:
	FuelSubsystem();
	void InitDefaultCommand();
	void SetVoltage(double);
	bool AtUpperLimit();
	bool AtLowerLimit();
};

#endif
