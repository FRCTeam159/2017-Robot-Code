#include "Climb.h"
#include "RobotMap.h"

#define VOLTAGE 0.1

Climb::Climb() {
	// Use Requires() here to declare subsystem dependencies
	// eg. Requires(Robot::chassis.get());
	Requires(climbingSubsystem.get());
}

// Called just before this Command runs the first time
void Climb::Initialize() {
	frc::SmartDashboard::PutNumber("climberCurrent", 0);
	state = WAITFORTOP;
}

// Called repeatedly when this Command is scheduled to run
void Climb::Execute() {
	double climbingCurrent = climbingSubsystem->GetCurrent();
	Joystick *stick = oi->GetJoystick();
	frc::SmartDashboard::PutNumber("climberCurrent", climbingCurrent);
	std::cout << "ClimbingSubsystem: Current is " << climbingCurrent << endl;

	if(climbingSubsystem->IsAtTop()){
		state = ISATTOP;
	}

	if(stick->GetRawButton(CLIMBERBUTTON) && state == WAITFORTOP){
		//std::cout << "ClimbingSubsystem: Pressing button and not at top."<<endl;
		climbingSubsystem->ClimberClimb();
	} else if (!stick->GetRawButton(CLIMBERBUTTON) || state == ISATTOP){
		climbingSubsystem->Stop();
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
