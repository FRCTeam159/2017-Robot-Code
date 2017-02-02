#include <Subsystems/Cameras.h>
#include "RobotMap.h"

using namespace frc;

Cameras::Camera::Camera(llvm::StringRef name, int dev){
	camera = CameraServer::GetInstance()->StartAutomaticCapture(name,dev);
	sink=CameraServer::GetInstance()->GetVideo(name);
}

uint64_t Cameras::Camera::GrabFrame(cv::Mat mat) {
	return sink.GrabFrame(mat);
}

void Cameras::Camera::InitCamera(double e, double w, double b) {
	exposure=e;
	whitebalance=w;
	brightness=b;
	camera.SetBrightness(brightness);
	camera.SetExposureManual(exposure);
	camera.SetWhiteBalanceManual(whitebalance);
}

void Cameras::Camera::AdjustSettings(double e, double w, double b) {
	if(b!=brightness){
		cout<<"New Brightness> "<<b<<endl;
		brightness = b;
		camera.SetBrightness(brightness);
	}
	if(e!=exposure){
		cout<<"New exposure> "<<e<<endl;
		exposure = e;
		camera.SetExposureManual(exposure);
	}
	if(w!=whitebalance){
		cout<<"New White Balance> "<<w<<endl;
		whitebalance = w;
		camera.SetWhiteBalanceManual(w);
	}
}
Cameras::Cameras() : Subsystem("CamerasSubsystem"){
}

void Cameras::Init() {
	std::thread visionThread(AdjustThread);
	visionThread.detach();
}

void Cameras::AdjustThread() {
	Camera camera1("Logitech",0);
	Camera camera2("Microsoft",1);

	frc::SmartDashboard::PutNumber("LogitechBrightness", camera1.Brightness());
	frc::SmartDashboard::PutNumber("LogitechExposure", camera1.Exposure());
	frc::SmartDashboard::PutNumber("LogitechBalance", camera1.Balance());

	frc::SmartDashboard::PutNumber("MicrosoftBrightness", camera2.Brightness());
	frc::SmartDashboard::PutNumber("MicrosoftExposure", camera2.Exposure());
	frc::SmartDashboard::PutNumber("MicrosoftBalance", camera2.Balance());

	// Set the resolution
	camera1.SetResolution(320, 240);
	camera2.SetResolution(320, 240);
	camera1.SetFPS(5);
	camera2.SetFPS(5);

	camera1.InitCamera(2.0,2.0,1.0);
	camera2.InitCamera(50.0,5.0,10.0);

    cv::Mat mat;
    double b,e,w;
	while (true) {
		if(camera1.GrabFrame(mat)!=0){  // wait for a camera frame to be ready
			b = frc::SmartDashboard::GetNumber("LogitechBrightness",2);
			e = frc::SmartDashboard::GetNumber("LogitechExposure",2);
			w = frc::SmartDashboard::GetNumber("LogitechBalance",2);
			camera1.AdjustSettings(e,w,b);
		}
		if(camera2.GrabFrame(mat)!=0){
			b = frc::SmartDashboard::GetNumber("MicrosoftBrightness",2);
			e = frc::SmartDashboard::GetNumber("MicrosoftExposure",2);
			w = frc::SmartDashboard::GetNumber("MicrosoftBalance",2);
			camera2.AdjustSettings(e,w,b);
		}
		//Wait(0.2);
	}
}

// Put methods for controlling this subsystem
// here. Call these from Commands.
