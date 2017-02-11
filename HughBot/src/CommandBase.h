#ifndef COMMAND_BASE_H
#define COMMAND_BASE_H

#include <memory>
#include <string>

#include <Commands/Command.h>
#include <Subsystems/FuelSubsystem.h>

#include "OI.h"
#include "Subsystems/DriveTrain.h"
#include "Subsystems/UltrasonicSubsystem.h"
#include "Subsystems/GearSubsystem.h"
#include "Subsystems/Vision.h"
using namespace std;

/**
 * The base for all commands. All atomic commands should subclass CommandBase.
 * CommandBase stores creates and stores each control system. To access a
 * subsystem elsewhere in your code in your code use
 * CommandBase::exampleSubsystem
 */
class CommandBase: public frc::Command {
public:
	CommandBase(const std::string& name);
	CommandBase() = default;
	static void init();
	// Create a single static instance of all of your subsystems
	static std::shared_ptr<DriveTrain> driveTrain;
	static std::shared_ptr<GearSubsystem> gearSubsystem;
	static std::shared_ptr<Vision> visionSubsystem;
	static std::shared_ptr<FuelSubsystem> fuelSubsystem;
	static std::shared_ptr<UltrasonicSubsystem> ultrasonicSubsystem;
	static std::shared_ptr<FuelSubsystem> fuelSubsystem;
	static void Enable();
	static void Disable();
	static void AutonomousInit();

	//static std::unique_ptr<ExampleSubsystem> exampleSubsystem;
	static std::unique_ptr<OI> oi;
};

#endif  // COMMAND_BASE_H
