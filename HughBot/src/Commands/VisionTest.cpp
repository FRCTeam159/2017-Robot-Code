#include "VisionTest.h"
#include <CameraServer.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core/types.hpp>
#include "Subsystems/GripPipeline.h"

VisionTest::VisionTest() {
	// Use Requires() here to declare subsystem dependencies
	// eg. Requires(Robot::chassis.get());
	Requires(visionSubsystem.get());
}

// Called just before this Command runs the first time
void VisionTest::Initialize() {
	// Get the USB camera from CameraServer


}

// Called repeatedly when this Command is scheduled to run
void VisionTest::Execute() {
	visionSubsystem->Process();
}

// Make this return true when this Command no longer needs to run execute()
bool VisionTest::IsFinished() {
	return false;
}

// Called once after isFinished returns true
void VisionTest::End() {

}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void VisionTest::Interrupted() {

}
