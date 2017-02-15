#include "Climb.h"
#include "RobotMap.h"

#define VOLTAGE 0.1
#define MOTORS_ENABLED // so we can get print statements but not mess with motors

Climb::Climb() {
	// Use Requires() here to declare subsystem dependencies
	// eg. Requires(Robot::chassis.get());
	Requires(climbingSubsystem.get());
}

// Called just before this Command runs the first time
void Climb::Initialize() {
	frc::SmartDashboard::PutNumber("climberCurrent", 0);
}

// Called repeatedly when this Command is scheduled to run
void Climb::Execute() {
	double climbingCurrent = climbingSubsystem->GetCurrent();
	Joystick *stick = oi->GetJoystick();
	frc::SmartDashboard::PutNumber("climberCurrent", climbingCurrent);
	std::cout << "ClimbingSubsystem: Current is " << climbingCurrent << endl;

	if(stick->GetRawButton(CLIMBERBUTTON) && !climbingSubsystem->IsAtTop()){
		//std::cout << "ClimbingSubsystem: Pressing button and not at top."<<endl;
	#ifdef MOTORS_ENABLED
		climbingSubsystem->ClimberClimb();
	#endif
	} else {
		//std::cout << "ClimbingSubsystem: Not pressing button or we are at the top."<<endl;
	#ifdef MOTORS_ENABLED
		climbingSubsystem->Stop();
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
