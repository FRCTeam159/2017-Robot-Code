#include "Subsystems/ClimbingSubsystem.h"
#include "../RobotMap.h"


ClimbingSubsystem::ClimbingSubsystem() : Subsystem("ClimbingSubsystem"),climberMotor(CLIMBING_MOTOR) {
	climberMotor.GetOutputCurrent();
}

void ClimbingSubsystem::InitDefaultCommand() {
	// Set the default command for a subsystem here.
	// SetDefaultCommand(new MySpecialCommand());
}

// Put methods for controlling this subsystem
// here. Call these from Commands.
