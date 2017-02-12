#include "GearPlateToggle.h"
#include "RobotMap.h"

GearPlateToggle::GearPlateToggle() {
	Requires(gearSubsystem.get());
		std::cout << "new ToggleGearPlate::ToggleGearPlate()"<< std::endl;
}

// Called just before this Command runs the first time
void GearPlateToggle::Initialize() {
	state = WAIT_FOR_TRIGGER_PUSH;

};

// Called repeatedly when this Command is scheduled to run
void GearPlateToggle::Execute() {
	Joystick *stick = oi->GetJoystick();

	if(state == WAIT_FOR_TRIGGER_PUSH){
		if(stick->GetRawButton(GEARTOGGLEBUTTON)){
			gearSubsystem->Open();
			state = WAIT_FOR_TRIGGER_RELEASE;
		}
	} else {
		if(!stick->GetRawButton(GEARTOGGLEBUTTON)){
			gearSubsystem->Close();
			state = WAIT_FOR_TRIGGER_PUSH;
		}
	}
}

// Make this return true when this Command no longer needs to run execute()
bool GearPlateToggle::IsFinished() {
	return false;
}

// Called once after isFinished returns true
void GearPlateToggle::End() {

}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void GearPlateToggle::Interrupted() {

}
