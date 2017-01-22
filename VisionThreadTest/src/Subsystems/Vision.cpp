#include "Subsystems/Vision.h"
#include "RobotMap.h"
#include "Commands/VisionTest.h"
#include "llvm/ArrayRef.h"
#include "llvm/StringRef.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core/types.hpp>
#include <CameraServer.h>
using namespace frc;

Vision::Vision() : Subsystem("Vision"){
}

void Vision::InitDefaultCommand() {
	// Set the default command for a subsystem here.
	SetDefaultCommand(new VisionTest());
}

void Vision::Init() {
	table=NetworkTable::GetTable("datatable");
	frc::SmartDashboard::PutNumber("CameraBrightness", brightness);

	// We need to run our vision program in a separate Thread.
	// If not, our robot program will not run
	std::thread visionThread(VisionThread);
	visionThread.detach();

}

void Vision::Process() {
	// test receiving data from image processing thread
	double val=table->GetNumber("brightness",1);
	if(val!=brightness){
		brightness=val;
		std::cout << "brightness="<<brightness<<std::endl;
	}
}

void Vision::VisionThread() {
	static double brightness=2.0;
	std::shared_ptr<NetworkTable> table=NetworkTable::GetTable("datatable");

	// Get the USB camera from CameraServer
	cs::UsbCamera camera = CameraServer::GetInstance()->StartAutomaticCapture();
	// Set the resolution
	camera.SetResolution(320, 240);
	camera.SetFPS(10);

	// Get a CvSink. This will capture Mats from the Camera
	cs::CvSink cvSink = CameraServer::GetInstance()->GetVideo();
	// Setup a CvSource. This will send images back to the Dashboard
	cs::CvSource outputStream = CameraServer::GetInstance()->PutVideo("Rectangle", 320, 240);

	// Mats are very memory expensive. Lets reuse this Mat.
	cv::Mat mat;

	camera.SetBrightness(brightness);
	while (true) {
		// Tell the CvSink to grab a frame from the camera and put it
		// in the source mat.  If there is an error notify the output.
		if (cvSink.GrabFrame(mat) == 0) {
			// Send the output the error.
			outputStream.NotifyError(cvSink.GetError());
			// skip the rest of the current iteration
			continue;
		}
		// Put a rectangle on the image
		rectangle(mat, cv::Point(10, 10), cv::Point(200, 200),cv::Scalar(255, 255, 255), 5);
		// Give the output stream a new image to display
		outputStream.PutFrame(mat);
		double val = frc::SmartDashboard::GetNumber("CameraBrightness",2);
		table->PutNumber("brightness",val); // test passing data to robot program
		if(val!=brightness){
			//cout<<"New Brightness> "<<b<<endl;
			brightness = val;
			camera.SetBrightness(brightness);
		}
		Wait(0.05);
	}
}
// Put methods for controlling this subsystem
// here. Call these from Commands.
