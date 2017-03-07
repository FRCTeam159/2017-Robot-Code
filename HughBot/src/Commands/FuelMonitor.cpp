#include "FuelMonitor.h"
#include "RobotMap.h"
#define FORWARDVOLTAGE .5
#define REVERSEVOLTAGE -0.5

FuelMonitor::FuelMonitor() {
	// Use Requires() here to declare subsystem dependencies
	// eg. Requires(Robot::chassis.get());
	Requires(fuelSubsystem.get());
}

// Called just before this Command runs the first time
void FuelMonitor::Initialize() {
	state = WAITFORBUTTON;
}

// Called repeatedly when this Command is scheduled to run
void FuelMonitor::Execute() {
	Joystick *stick = oi->GetJoystick();
	switch(state){
	default:
	case WAITFORBUTTON:
		if(stick->GetRawButton(FUELPUSHERBUTTON))
			state=WAITFORUPPERLIMIT;
		else
			fuelSubsystem->SetVoltage(0);
		break;
	case WAITFORUPPERLIMIT:
		if(fuelSubsystem->AtUpperLimit())
			state= WAITFORLOWERLIMIT;
		else
			fuelSubsystem->SetVoltage(FORWARDVOLTAGE);
		break;
	case WAITFORLOWERLIMIT:
		if(fuelSubsystem->AtLowerLimit())
			state= WAITFORBUTTON;
		else
			fuelSubsystem->SetVoltage(REVERSEVOLTAGE);
		break;
	}
}

// Make this return true when this Command no longer needs to run execute()
bool FuelMonitor::IsFinished() {
	return false;
}

// Called once after isFinished returns true
void FuelMonitor::End() {

}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void FuelMonitor::Interrupted() {

}
