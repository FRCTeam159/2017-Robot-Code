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

void Vision::Process() {
	cv::Mat mat;
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
	// Give the output stream a new image to display
	gp.process(mat);
	cv::Mat* mat2=gp.getrgbThresholdOutput();
	//cv::Mat* mat2=gp.getblurOutput();
	mat2->copyTo(mat);
	outputStream.PutFrame(mat);
}

void Vision::Init() {
	camera = CameraServer::GetInstance()->StartAutomaticCapture();
	// Set the resolution
	camera.SetResolution(320, 240);
	// Get a CvSink. This will capture Mats from the Camera
	cvSink = CameraServer::GetInstance()->GetVideo();
	// Setup a CvSource. This will send images back to the Dashboard
	outputStream = CameraServer::GetInstance()->
			PutVideo("Rectangle", 320, 240);
}
// Put methods for controlling this subsystem
// here. Call these from Commands.
