#ifndef GearSubsystem_H
#define GearSubsystem_H
#include "WPILib.h"
#include "Commands/Subsystem.h"

#include <Commands/Subsystem.h>

class GearSubsystem : public Subsystem {
private:
	DoubleSolenoid gearPiston;
		bool isOpen=false;
	// It's desirable that everything possible under private except
	// for methods that implement subsystem capabilities

public:
	GearSubsystem();
	void InitDefaultCommand();

	void Open();
	void Close();
	bool IsOpen() { return isOpen;}

};

#endif
