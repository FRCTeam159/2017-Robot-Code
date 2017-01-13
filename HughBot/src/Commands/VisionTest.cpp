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
	camera = CameraServer::GetInstance()->StartAutomaticCapture();
	// Set the resolution
	camera.SetResolution(320, 240);

	// Get a CvSink. This will capture Mats from the Camera
	cvSink = CameraServer::GetInstance()->GetVideo();
	// Setup a CvSource. This will send images back to the Dashboard
	outputStream = CameraServer::GetInstance()->
			PutVideo("Rectangle", 320, 240);


}

// Called repeatedly when this Command is scheduled to run
void VisionTest::Execute() {
	if (cvSink.GrabFrame(mat) == 0) {
		// Send the output the error.
		outputStream.NotifyError(cvSink.GetError());
		// skip the rest of the current iteration
		error=true;
				return;
	}
	//cout<<"VisionTestRan"<<endl;
	// Mats are very memory expensive. Let's reuse this Mat.
	rectangle(mat, cv::Point(10, 10), cv::Point(100, 100),
			cv::Scalar(255, 0, 0), 10);
	visionSubsystem->Process(mat);
	// Give the output stream a new image to display
	outputStream.PutFrame(mat);
}

// Make this return true when this Command no longer needs to run execute()
bool VisionTest::IsFinished() {
	return error;
}

// Called once after isFinished returns true
void VisionTest::End() {

}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void VisionTest::Interrupted() {

}
