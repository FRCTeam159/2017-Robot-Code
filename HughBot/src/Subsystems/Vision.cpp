#include <Commands/VisionUpdate.h>
#include "Vision.h"
#include "../RobotMap.h"
#include "Subsystems/GripPipeline.h"
#include "llvm/ArrayRef.h"
#include "llvm/StringRef.h"

#define RPD(x) (x)*2*M_PI/360
#define IMAGE_WIDTH 320
#define IMAGE_HEIGHT 240
#define FOV 60.0

using namespace frc;

llvm::ArrayRef<double>  Vision::hsvThresholdHue = {70, 110};
llvm::ArrayRef<double>  Vision::hsvThresholdSaturation = {100, 255};
llvm::ArrayRef<double>  Vision::hsvThresholdValue = {100, 200};
cs::UsbCamera Vision::camera1;
cs::UsbCamera Vision::camera2;
cs::CvSink Vision::cvSink;
cs::CvSource Vision::outputStream;

static double brightness = 2;
static double exposure = 2;
static bool showColorThreshold = false;
static double whiteBalance = 2;
static double driverCameraExposure = 0;
static double driverCameraBalance = 0;
static 	GripPipeline gp;

Vision::Vision() : Subsystem("VisionSubsystem") {
	SetCameraInfo(IMAGE_WIDTH,IMAGE_HEIGHT,FOV);
}

void Vision::InitDefaultCommand() {
	// Set the default command for a subsystem here.
	SetDefaultCommand(new VisionUpdate());
}

void Vision::Init() {
	table=NetworkTable::GetTable("datatable");

	camera1 = CameraServer::GetInstance()->StartAutomaticCapture("Logitech",0);
	camera2 = CameraServer::GetInstance()->StartAutomaticCapture("DriverCam",1);

	frc::SmartDashboard::PutNumber("CameraBrightness", 2);
	frc::SmartDashboard::PutNumber("CameraExposure", exposure);
	frc::SmartDashboard::PutNumber("TargetDistance", 0);
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
	camera1.SetFPS(5);


	//camera.SetPixelFormat(cs::VideoMode::PixelFormat::kBGR);

	// Get a CvSink. This will capture Mats from the Camera
	//cvSink = CameraServer::GetInstance()->GetVideo();
	// Setup a CvSource. This will send images back to the Dashboard
	//outputStream = CameraServer::GetInstance()->PutVideo("Rectangle", 320, 240);
	//CameraSettings(0,0,frc::SmartDashboard::GetNumber("CameraBrightness",2));

	std::thread visionThread(VisionThread);
	visionThread.detach();
}

#define SHOW_COLOR_THRESHOLD

void Vision::Process() {
	// test receiving data from image processing thread

	top.x=table->GetNumber("TopLeftX", 10);
	top.y=table->GetNumber("TopLeftY", 10);
	bot.x=table->GetNumber("BotRightX",20);
	bot.y=table->GetNumber("BotRightY",20);
	frc::SmartDashboard::PutNumber("TargetDistance", GetDistance());
	//cout<<"GetDistance: "<<GetDistance()<<endl;
	//cout<<"TL:"<<top<<" BR:"<<bot<<endl;
}


void Vision::VisionThread(){

	std::shared_ptr<NetworkTable> table2=NetworkTable::GetTable("datatable");


	// Get a CvSink. This will capture Mats from the Camera
	cvSink = CameraServer::GetInstance()->GetVideo("Logitech");
	// Setup a CvSource. This will send images back to the Dashboard
	outputStream = CameraServer::GetInstance()->PutVideo("Rectangle", 320, 240);

	cv::Mat mat;

	cv::Point tl=cv::Point(10, 10);
	cv::Point br=cv::Point(20, 20);


	while(true){

		double val = frc::SmartDashboard::GetNumber("CameraBrightness",2);
		double exp = frc::SmartDashboard::GetNumber("CameraExposure",1);
		double bal = frc::SmartDashboard::GetNumber("CameraBalance",2);

		double DriverCameraExposure = frc::SmartDashboard::GetNumber("DriverCameraExposure",0);
		double DriverCameraBalance = frc::SmartDashboard::GetNumber("DriverBalance",0);

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

		showColorThreshold = frc::SmartDashboard::GetBoolean("showColorThreshold", false);

		// Tell the CvSink to grab a frame from the camera and put it
		// in the source mat.  If there is an error notify the output.
		if (cvSink.GrabFrame(mat) == 0) {
			// Send the output the error.
			//outputStream.NotifyError(cvSink.GetError());
			// skip the rest of the current iteration
			continue;
		}

		hsvThresholdHue={SmartDashboard::GetNumber("HueMin",70),SmartDashboard::GetNumber("HueMax", 100)};
		hsvThresholdValue={SmartDashboard::GetNumber("ValueMin", 100),SmartDashboard::GetNumber("ValueMax",255)};
		hsvThresholdSaturation={SmartDashboard::GetNumber("SaturationMin", 100),SmartDashboard::GetNumber("SaturationMax",255)};

		gp.setHSVThresholdHue(hsvThresholdHue);
		gp.setHSVThresholdValue(hsvThresholdValue);
		gp.setHSVThresholdSaturation(hsvThresholdSaturation);

		gp.process(mat);

		if(showColorThreshold){
			cv::Mat* mat2=gp.getColorThresholdOutput();
			mat2->copyTo(mat);
		}

		int minx = 1000, maxx = 0, miny = 1000, maxy = 0;
		std::vector<cv::Rect> rects= *gp.getRectangles();

		bool showGoodRects = frc::SmartDashboard::GetBoolean("showGoodRects", true);

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

		tl=cv::Point(minx, miny);
		br=cv::Point(maxx, maxy);

		rectangle(mat, cv::Point(minx, miny), cv::Point(maxx, maxy), cv::Scalar(255, 255, 255), 1);

		outputStream.PutFrame(mat);
		table2->PutNumber("TopLeftX", tl.x);
		table2->PutNumber("TopLeftY", tl.y);
		table2->PutNumber("BotRightX", br.x);
		table2->PutNumber("BotRightY", br.y);
	}
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

double Vision::GetDistance() {
	//double camerafactor = 1/(2*tan(RPD(f/2.0)));
	double targetwidth = 10.25; // width of tape to outside, in inches
	int closestwidth = bot.x - top.x;
	double dw = cameraInfo.fovFactor*cameraInfo.screenWidth*targetwidth/closestwidth;
	return dw;
}

double Vision::GetDirection() {
	return 0;
}

void Vision::SetCameraInfo(int width, int height, double fov) {
	cameraInfo.screenWidth = width;
	cameraInfo.screenHeight = height;
	cameraInfo.fov = fov;
	cameraInfo.fovFactor = 1/(2*tan(RPD(fov/2.0)));
	cout<<"fovFactor: "<<cameraInfo.fovFactor<<endl;
}
// Put methods for controlling this subsystem
// here. Call these from Commands.
