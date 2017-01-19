#include "Vision.h"
#include "../RobotMap.h"
#include "Commands/VisionTest.h"
#include "Subsystems/GripPipeline.h"
#include "llvm/ArrayRef.h"
#include "llvm/StringRef.h"

Vision::Vision() :
		Subsystem("ExampleSubsystem"), gp() {
}

void Vision::InitDefaultCommand() {
	// Set the default command for a subsystem here.
	SetDefaultCommand(new VisionTest());
}

void Vision::Init() {
	camera = CameraServer::GetInstance()->StartAutomaticCapture();
	camera2 = CameraServer::GetInstance()->StartAutomaticCapture("DriverCam",1);

	CameraSettings(exposure, 0, brightness);
	frc::SmartDashboard::PutNumber("CameraBrightness", camera.GetBrightness());
	frc::SmartDashboard::PutNumber("CameraExposure", exposure);
	frc::SmartDashboard::PutBoolean("showColorThreshold", false);
	frc::SmartDashboard::PutNumber("HueMax", hsvThresholdHue[1]);
	frc::SmartDashboard::PutNumber("HueMin", hsvThresholdHue[0]);
	//frc::SmartDashboard::PutNumberArray("hue", hsvThresholdHue);


	// Set the resolution
	camera.SetResolution(320, 240);
	camera2.SetResolution(320, 240);
	//camera.SetFPS(1);

	//camera.SetPixelFormat(cs::VideoMode::PixelFormat::kBGR);

	// Get a CvSink. This will capture Mats from the Camera
	cvSink = CameraServer::GetInstance()->GetVideo();
	// Setup a CvSource. This will send images back to the Dashboard
	outputStream = CameraServer::GetInstance()->PutVideo("Rectangle", 320, 240);
	//CameraSettings(0,0,frc::SmartDashboard::GetNumber("CameraBrightness",2));
}

#define SHOW_COLOR_THRESHOLD

void Vision::Process() {

	cv::Mat mat;
	//return;
	int status = cvSink.GrabFrame(mat);
	//std::cout<<"status="<<status<<std::endl;
	//std::cout<<"frame capture error"<<std::endl;

	if (status == 0) {
		// Send the output the error.
		outputStream.NotifyError(cvSink.GetError());
		// skip the rest of the current iteration
		error = true;
		return;
	}
	//cout<<"VisionTestRan"<<endl;
	double val = frc::SmartDashboard::GetNumber("CameraBrightness",0);
	double exp = frc::SmartDashboard::GetNumber("CameraExposure",10);
	showColorThreshold = frc::SmartDashboard::GetBoolean("showColorThreshold", false);
	double hMin = frc::SmartDashboard::GetNumber("HueMin",0);
	double hMax = frc::SmartDashboard::GetNumber("HueMax", 100);
	llvm::ArrayRef<double> test={hMin,hMax};
	if((hMin != hsvThresholdHue[0]) || (hMax != hsvThresholdHue[1])){
		cout<<"hue changed"<<endl;
		//llvm::ArrayRef<double> temp = gp.getHSVHue();
		hsvThresholdHue=test;
		//hsvThresholdHue.[0] = hMin;
		//hsvThresholdHue[1] = hMax;
		gp.setHSVThresholdHue(hsvThresholdHue);
	}
	AdjustCamera(exp,0,val);

	gp.process(mat);
	if(showColorThreshold){
		//cout<<"Show color threshold is true"<<endl;
	cv::Mat* mat2=gp.getColorThresholdOutput();
	mat2->copyTo(mat);
	}

	//cv::Mat* mat2=gp.getblurOutput();
	int minx = 1000, maxx = 0, miny = 1000, maxy = 0;
	std::vector<std::vector<cv::Point> > points = *gp.getResultVector();
	//cout << "points size:" << points.size() << endl;
	for (unsigned int i = 0; i < points.size(); i++) {
		std::vector<cv::Point> pv = points[i];
		for (unsigned int j = 0; j < pv.size(); j++) {
			cv::Point p = pv[j];
			minx = p.x < minx ? p.x : minx;
			maxx = p.x > maxx ? p.x : maxx;
			miny = p.y < miny ? p.y : miny;
			maxy = p.y > maxy ? p.y : maxy;
			//cout<<p.x<<" "<<p.y;
			//cout<<endl;
		}
	}
	cv::Point c(0.5*(maxx+minx),0.5*(maxy+miny));
	rectangle(mat, cv::Point(minx, miny), cv::Point(maxx, maxy), cv::Scalar(255, 0, 0), 1);
	drawMarker(mat, c, cv::Scalar(0, 0, 255),0, 10, 2,8);
	// Mats are very memory expensive. Let's reuse this Mat.
	//rectangle(mat, cv::Point(10, 10), cv::Point(100, 100),
	//		cv::Scalar(255, 0, 0), 1);
	// Give the output stream a new image to display

	outputStream.PutFrame(mat);

	//double val = frc::SmartDashboard::GetNumber("CameraBrightness",2);
	//cout<<"CameraBrightness = "<<val<<endl;

}

void Vision::CameraSettings(double exposure, double balance, double brightness)
{
	//float e=camera.GetBrightness();
	//cout <<"original brightness:"<<e<< endl;
	camera.SetBrightness(brightness);
	camera.SetExposureManual(exposure);
	//e=camera.GetBrightness();
	//cout <<"new brightness:"<<e<< endl;
}

void Vision::AdjustCamera(double e, double balance, double b) {
	if(b!=brightness){
		cout<<"New Brightness> "<<b<<endl;
		brightness = b;
		camera.SetBrightness(brightness);
		//CameraSettings(0,0,brightness);
	}
	if(e!=exposure){
		cout<<"New exposure> "<<e<<endl;
		exposure = e;
		camera.SetExposureManual(exposure);
	}
}
// Put methods for controlling this subsystem
// here. Call these from Commands.
