#include "Vision.h"
#include "../RobotMap.h"
#include "Commands/VisionTest.h"

Vision::Vision() : Subsystem("ExampleSubsystem") {

}

void Vision::InitDefaultCommand() {
	// Set the default command for a subsystem here.
	SetDefaultCommand(new VisionTest());
}

// Put methods for controlling this subsystem
// here. Call these from Commands.
