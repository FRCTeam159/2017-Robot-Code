#include <Commands/VisionUpdate.h>
#include "Vision.h"
#include "../RobotMap.h"
#include "Subsystems/GripPipeline.h"
#include "llvm/ArrayRef.h"
#include "llvm/StringRef.h"

using namespace frc;

Vision::Vision() :
	Subsystem("ExampleSubsystem"), gp() {
}

void Vision::InitDefaultCommand() {
	// Set the default command for a subsystem here.
	SetDefaultCommand(new VisionUpdate());
}

void Vision::Init() {
	camera1 = CameraServer::GetInstance()->StartAutomaticCapture(0);
	camera2 = CameraServer::GetInstance()->StartAutomaticCapture("DriverCam",1);

	CameraSettings(exposure, 0, brightness);
	frc::SmartDashboard::PutNumber("CameraBrightness", camera1.GetBrightness());
	frc::SmartDashboard::PutNumber("CameraExposure", exposure);
	frc::SmartDashboard::PutNumber("CameraBalance", whiteBalance);
	frc::SmartDashboard::PutBoolean("showColorThreshold", false);
	frc::SmartDashboard::PutNumber("HueMax", hsvThresholdHue[1]);
	frc::SmartDashboard::PutNumber("HueMin", hsvThresholdHue[0]);
	//frc::SmartDashboard::PutNumberArray("hue", hsvThresholdHue);
	frc::SmartDashboard::PutNumber("SaturationMax", hsvThresholdSaturation[1]);
	frc::SmartDashboard::PutNumber("SaturationMin", hsvThresholdSaturation[0]);
	frc::SmartDashboard::PutNumber("ValueMax", hsvThresholdValue[1]);
	frc::SmartDashboard::PutNumber("ValueMin", hsvThresholdValue[0]);
	frc::SmartDashboard::PutNumber("Rectangles", 0);
	frc::SmartDashboard::PutBoolean("showGoodRects", false);
	frc::SmartDashboard::PutNumber("DriverCameraExposure",0);
	frc::SmartDashboard::PutNumber("DriverBalance",0);

	// Set the resolution
	camera1.SetResolution(320, 240);
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
	double val = frc::SmartDashboard::GetNumber("CameraBrightness",2);
	double exp = frc::SmartDashboard::GetNumber("CameraExposure",1);
	double bal = frc::SmartDashboard::GetNumber("CameraBalance",2);
	showColorThreshold = frc::SmartDashboard::GetBoolean("showColorThreshold", false);
	hsvThresholdHue={SmartDashboard::GetNumber("HueMin",70),SmartDashboard::GetNumber("HueMax", 100)};
	hsvThresholdValue={SmartDashboard::GetNumber("ValueMin", 100),SmartDashboard::GetNumber("ValueMax",255)};
	hsvThresholdSaturation={SmartDashboard::GetNumber("SaturationMin", 100),SmartDashboard::GetNumber("SaturationMax",255)};
	double DriverCameraExposure = frc::SmartDashboard::GetNumber("DriverCameraExposure",0);
	double DriverCameraBalance = frc::SmartDashboard::GetNumber("DriverBalance",0);
	gp.setHSVThresholdHue(hsvThresholdHue);
	gp.setHSVThresholdValue(hsvThresholdValue);
	gp.setHSVThresholdSaturation(hsvThresholdSaturation);
	AdjustCamera(exp,bal,val);
	if(driverCameraBalance != DriverCameraBalance){
		if(DriverCameraBalance < 1){
			camera2.SetWhiteBalanceAuto();
		}else{
			camera2.SetWhiteBalanceManual(DriverCameraBalance);
		}
		driverCameraBalance = DriverCameraBalance;
	}
	if(driverCameraExposure != DriverCameraExposure){
		if(DriverCameraExposure < 1){
			camera2.SetExposureAuto();
		}else{
			camera2.SetExposureManual(DriverCameraExposure);
		}
		driverCameraExposure = DriverCameraExposure;
	}


	gp.process(mat);
	if(showColorThreshold){
		//cout<<"Show color threshold is true"<<endl;
	cv::Mat* mat2=gp.getColorThresholdOutput();
	mat2->copyTo(mat);
	}

	//cv::Mat* mat2=gp.getblurOutput();
	int minx = 1000, maxx = 0, miny = 1000, maxy = 0;
#ifdef SHOWCONTOURS
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
#else
	std::vector<cv::Rect> rects= *gp.getRectangles();

	bool showGoodRects = frc::SmartDashboard::GetBoolean("showGoodRects", true);
	//cout<<"goodRects="<<showGoodRects<<endl;
	//cout<<"number of rectangles="<<rects.size()<<endl;
	std::vector<cv::Rect> rectsPointer=rects;
	if (showGoodRects){
		std::vector<cv::Rect> goodrects;
		int goodFactor=5;
		for (unsigned int i = 0; i < rects.size(); i++) {
			int score = 0;
			cv::Rect Rect1=rects [i];
			int cx1=0.5*(Rect1.tl().x+Rect1.br().x);
			int cy1=0.5*(Rect1.tl().y+Rect1.br().y);
			double w1=rects[i].width;
			for (unsigned int j = 0; j < rects.size(); j++) {
				if (i==j)
					continue;
				cv::Rect Rect2=rects [j];
				int cx2=0.5*(Rect2.tl().x+Rect2.br().x);
				int cy2=0.5*(Rect2.tl().y+Rect2.br().y);
				int dx=cx1-cx2;
				int dy=cy1-cy2;
				int d=sqrt(dx*dx+dy*dy);
				double r=d/w1;
				if (r<goodFactor)
					score++;
			}
			if (score>0)
				goodrects.push_back(Rect1);
		}
		rectsPointer=goodrects;
	}
	frc::SmartDashboard::PutNumber("Rectangles",rectsPointer.size());
	for (unsigned int i = 0; i < rectsPointer.size(); i++) {
		cv::Rect r= rectsPointer[i];
		cv::Point p= r.tl();
		minx = p.x < minx ? p.x : minx;
		miny = p.y < miny ? p.y : miny;
		p= r.br();
		maxy = p.y > maxy ? p.y : maxy;
		maxx = p.x > maxx ? p.x : maxx;
		rectangle(mat, r.tl(), r.br(), cv::Scalar(0, 255, 255), 1);
	}
#endif
	rectangle(mat, cv::Point(minx, miny), cv::Point(maxx, maxy), cv::Scalar(255, 255, 255), 1);
	//drawMarker(mat, c, cv::Scalar(0, 0, 255),0, 10, 2,8);
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
	camera1.SetBrightness(brightness);
	camera1.SetExposureManual(exposure);
	camera1.SetWhiteBalanceManual(balance);
	//e=camera.GetBrightness();
	//cout <<"new brightness:"<<e<< endl;
}

void Vision::AdjustCamera(double e, double bal, double b) {
	if(b!=brightness){
		cout<<"New Brightness> "<<b<<endl;
		brightness = b;
		camera1.SetBrightness(brightness);
		//CameraSettings(0,0,brightness);
	}
	if(e!=exposure){
		cout<<"New exposure> "<<e<<endl;
		exposure = e;
		camera1.SetExposureManual(exposure);
	}
	if(bal!=whiteBalance){
		cout<<"New White Balance> "<<bal<<endl;
		whiteBalance = bal;
		camera1.SetWhiteBalanceManual(bal);
	}
}
// Put methods for controlling this subsystem
// here. Call these from Commands.
