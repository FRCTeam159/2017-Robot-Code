#include "Vision.h"
#include "../RobotMap.h"
#include "Commands/VisionTest.h"
#include "Subsystems/GripPipeline.h"

Vision::Vision() : Subsystem("ExampleSubsystem"), gp(){

}

void Vision::InitDefaultCommand() {
	// Set the default command for a subsystem here.
	SetDefaultCommand(new VisionTest());
}

void Vision::Process(cv::Mat mat) {
	gp.process(mat);
}
// Put methods for controlling this subsystem
// here. Call these from Commands.
