#include "GearSubsystem.h"
#include "../RobotMap.h"
#include "Commands/GearPlateToggle.h"

GearSubsystem::GearSubsystem() : Subsystem("GearSubsystem"),
gearPiston(0,2,3) {
	frc::SmartDashboard::PutBoolean("GearPlateOpen", isOpen);
}

void GearSubsystem::InitDefaultCommand() {
	// Set the default command for a subsystem here.
	SetDefaultCommand(new GearPlateToggle());
}

void GearSubsystem::Open() {
	gearPiston.Set(DoubleSolenoid::kReverse);
	isOpen=true;
	frc::SmartDashboard::PutBoolean("GearPlateOpen", isOpen);
}

void GearSubsystem::Close() {
	gearPiston.Set(DoubleSolenoid::kForward);
	isOpen=false;
	frc::SmartDashboard::PutBoolean("GearPlateOpen", isOpen);
}

// Put methods for controlling this subsystem
// here. Call these from Commands.
