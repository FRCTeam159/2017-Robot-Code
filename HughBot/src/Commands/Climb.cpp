#include "Climb.h"

#define VOLTAGE 1
//#define MOTORS_ENABLED // so we can get print statements but not mess with motors

Climb::Climb() {
	// Use Requires() here to declare subsystem dependencies
	// eg. Requires(Robot::chassis.get());
	Requires(climbingSubsystem.get());
}

// Called just before this Command runs the first time
void Climb::Initialize() {

}

// Called repeatedly when this Command is scheduled to run
void Climb::Execute() {
	Joystick *stick = oi->GetJoystick();

	if(stick->GetRawButton(CLIMBING_MOTOR) && !climbingSubsystem->IsAtTop()){
		std::cout << "ClimbingSubsystem: Pressing button and not at top."<<endl;
	#ifdef MOTORS_ENABLED
		climbingSubsystem->SetVoltage(VOLTAGE);
	#endif
	} else if (!stick->GetRawButton(CLIMBING_MOTOR) || climbingSubsystem->IsAtTop()) {
		std::cout << "ClimbingSubsystem: Not pressing button or we are at the top."<<endl;
	#ifdef MOTORS_ENABLED
		climbingSubsystem->SetVoltage(0);
	#endif
	}

}

// Make this return true when this Command no longer needs to run execute()
bool Climb::IsFinished() {
	return false;
}

// Called once after isFinished returns true
void Climb::End() {

}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void Climb::Interrupted() {

}
