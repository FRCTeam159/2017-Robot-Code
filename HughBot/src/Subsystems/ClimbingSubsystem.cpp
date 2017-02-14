#include "Subsystems/ClimbingSubsystem.h"
#include "../RobotMap.h"


ClimbingSubsystem::ClimbingSubsystem() : Subsystem("ClimbingSubsystem"),climberMotor(CLIMBING_MOTOR) {
	climberMotor.GetOutputCurrent();
}

void ClimbingSubsystem::InitDefaultCommand() {
	// Set the default command for a subsystem here.
	// SetDefaultCommand(new MySpecialCommand());
	SetDefaultCommand(new Climb());
}

void ClimbingSubsystem::SetVoltage(double voltage) {
	climberMotor.Set(voltage);
}

bool ClimbingSubsystem::IsAtTop() { // not done, need to test and code this somehow
	return false;
}
// Put methods for controlling this subsystem
// here. Call these from Commands.
