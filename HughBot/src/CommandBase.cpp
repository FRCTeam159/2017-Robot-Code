#include "CommandBase.h"
#include "Subsystems/DriveTrain.h"
#include <Commands/Scheduler.h>


// Initialize a single static instance of all of your subsystems. The following
// line should be repeated for each subsystem in the project.
shared_ptr<DriveTrain> CommandBase::driveTrain;
shared_ptr<GearSubsystem> CommandBase::gearSubsystem;
shared_ptr<Vision> CommandBase::visionSubsystem;
shared_ptr<UltrasonicSubsystem>CommandBase::ultrasonicSubsystem;
shared_ptr<ClimbingSubsystem> CommandBase::climbingSubsystem;
shared_ptr<FuelSubsystem> CommandBase::fuelSubsystem;



std::unique_ptr<OI> CommandBase::oi = std::make_unique<OI>();

CommandBase::CommandBase(const std::string &name) :
		frc::Command(name) {
}

void CommandBase::init()
{
	// Create a single static instance of all of your subsystems. The following
	// line should be repeated for each subsystem in the project.
	visionSubsystem.reset(new Vision());

	visionSubsystem->Init();
	visionSubsystem->SetRingLight(true);

	driveTrain.reset(new DriveTrain());
	gearSubsystem.reset(new GearSubsystem());
	ultrasonicSubsystem.reset(new UltrasonicSubsystem());
	fuelSubsystem.reset(new FuelSubsystem());
	climbingSubsystem.reset(new ClimbingSubsystem());
	oi.reset(new OI());
}

void CommandBase::Enable() {
	visionSubsystem->SetRingLight(true);
	driveTrain->EnableDrive();
}

void CommandBase::Disable() {
	//visionSubsystem->SetRingLight(false);
	driveTrain->DisableDrive();
}
void CommandBase::AutonomousInit() {

	driveTrain->Enable();
	ultrasonicSubsystem->Enable();
}
