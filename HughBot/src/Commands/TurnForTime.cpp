#include "TurnForTime.h"

TurnForTime::TurnForTime(double t, double s) {
	Requires(driveTrain.get());
	// Use Requires() here to declare subsystem dependencies
	// eg. Requires(Robot::chassis.get());
	time = t;
	speed = s;
}

// Called just before this Command runs the first time
void TurnForTime::Initialize() {
	driveTrain->EnableDrive();
	targetTime = Timer::GetFPGATimestamp() + time;
	std::cout << "TurnForTime Started .."<< std::endl;
}

// Called repeatedly when this Command is scheduled to run
void TurnForTime::Execute() {
	driveTrain->TankDrive(speed, -speed);
}

// Make this return true when this Command no longer needs to run execute()
bool TurnForTime::IsFinished() {
	currentTime = Timer::GetFPGATimestamp();
		if(currentTime >= targetTime)
			return true;
		return false;
}

// Called once after isFinished returns true
void TurnForTime::End() {
	driveTrain->DisableDrive();
	std::cout << "TurnForTime End"<< std::endl;
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void TurnForTime::Interrupted() {
	End();
}
