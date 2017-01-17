#include "AdjustCamera.h"

AdjustCamera::AdjustCamera() {
	Requires(visionSubsystem.get());
	// Use Requires() here to declare subsystem dependencies
	// eg. Requires(Robot::chassis.get());
}

// Called just before this Command runs the first time
void AdjustCamera::Initialize() {
	cout<<"AdjustCamera::Initialize"<<endl;
}

// Called repeatedly when this Command is scheduled to run
void AdjustCamera::Execute() {
	double val = frc::SmartDashboard::GetNumber("CameraBrightness",2);
	visionSubsystem->AdjustCamera(0,0,val);
}

// Make this return true when this Command no longer needs to run execute()
bool AdjustCamera::IsFinished() {
	return false;
}

// Called once after isFinished returns true
void AdjustCamera::End() {
	cout<<"AdjustCamera::End"<<endl;
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void AdjustCamera::Interrupted() {

}
