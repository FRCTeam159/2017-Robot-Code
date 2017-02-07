#include "GearPlateToggle.h"

GearPlateToggle::GearPlateToggle() {
	Requires(gearSubsystem.get());
		std::cout << "new ToggleGearPlate::ToggleGearPlate()"<< std::endl;
}

// Called just before this Command runs the first time
void GearPlateToggle::Initialize() {
	if (isOpen){
		std::cout << "Closing Gear Plate"<< endl;
		gearSubsystem->Close();
	}
	else{
		std::cout << "Opening Gear Plate"<< endl;
		gearSubsystem->Open();
	}
	isOpen=!isOpen;

	// Called repeatedly when this Command is scheduled to run
};

// Called repeatedly when this Command is scheduled to run
void GearPlateToggle::Execute() {

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
