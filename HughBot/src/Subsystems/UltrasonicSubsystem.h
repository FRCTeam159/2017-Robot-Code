#ifndef UltrasonicSubsystem_H
#define UltrasonicSubsystem_H

#include <Commands/Subsystem.h>
#include "SerialPort.h"
#include "DigitalOutput.h"
#include "I2C.h"

class UltrasonicSubsystem : public Subsystem {
private:
	bool portEnabled;
	double distance = 0;
	SerialPort port;
	DigitalOutput dOutput;

public:
	UltrasonicSubsystem();
	void InitDefaultCommand();
	void Disable();
	void Enable();
	bool IsEnabled();
	double GetDistance();
};

#endif  // UltrasonicSubsystem_H
