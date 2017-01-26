#include <Commands/VisionUpdate.h>
#include "Subsystems/Vision.h"
#include "RobotMap.h"
#include "llvm/ArrayRef.h"
#include "llvm/StringRef.h"

using namespace frc;
using namespace grip;

static llvm::ArrayRef<double>  hsvThresholdHue = {70, 110};
static llvm::ArrayRef<double>  hsvThresholdSaturation = {180, 255};
static llvm::ArrayRef<double>  hsvThresholdValue = {70, 200};
static cs::UsbCamera camera1;
static cs::UsbCamera camera2;
static cs::CvSink cvSink;
static cs::CvSource outputStream;

cv::Point tl=cv::Point(10, 10);
cv::Point br=cv::Point(20, 20);

Vision::Vision() : Subsystem("Vision"){
}

void Vision::InitDefaultCommand() {
	// Set the default command for a subsystem here.
	SetDefaultCommand(new VisionUpdate());
}

void Vision::Init() {
	table=NetworkTable::GetTable("datatable");
	frc::SmartDashboard::PutNumber("TopLeftX", tl.x);
	frc::SmartDashboard::PutNumber("TopLeftY", tl.y);
	frc::SmartDashboard::PutNumber("BotRightX", br.x);
	frc::SmartDashboard::PutNumber("BotRightY", br.y);
	// Get the USB camera from CameraServer

	camera1 = CameraServer::GetInstance()->StartAutomaticCapture();
	// Set the resolution
	camera1.SetResolution(320, 240);
	camera1.SetFPS(10);

	// We need to run our vision program in a separate Thread.
	// If not, our robot program will not run
	std::thread visionThread(VisionThread);
	visionThread.detach();
}

void Vision::Process() {
	// test receiving data from image processing thread
	cv::Point top=cv::Point(10, 10);
	cv::Point bot=cv::Point(20, 20);

	top.x=table->GetNumber("TopLeftX", 10);
	top.y=table->GetNumber("TopLeftY", 10);
	bot.x=table->GetNumber("BotRightX",20);
	bot.x=table->GetNumber("BotRightY",20);
	cout<<"TL:"<<top<<" BR:"<<bot<<endl;
}

static bool showColorThreshold=false;

void Vision::VisionThread() {
	static double brightness=2.0;
	GripPipeline gp;
	std::shared_ptr<NetworkTable> table=NetworkTable::GetTable("datatable");

	// Get a CvSink. This will capture Mats from the Camera
	cvSink = CameraServer::GetInstance()->GetVideo();
	// Setup a CvSource. This will send images back to the Dashboard
	outputStream = CameraServer::GetInstance()->PutVideo("Rectangle", 320, 240);

	gp.setHSVThresholdHue(hsvThresholdHue);
	gp.setHSVThresholdValue(hsvThresholdValue);
	gp.setHSVThresholdSaturation(hsvThresholdSaturation);

	// Mats are very memory expensive. Lets reuse this Mat.
	cv::Mat mat;

	camera1.SetBrightness(brightness);
	while (true) {
		// Tell the CvSink to grab a frame from the camera and put it
		// in the source mat.  If there is an error notify the output.
		if (cvSink.GrabFrame(mat) == 0) {
			// Send the output the error.
			outputStream.NotifyError(cvSink.GetError());
			// skip the rest of the current iteration
			continue;
		}

		gp.process(mat);

		if(showColorThreshold){
			cv::Mat* mat2=gp.getColorThresholdOutput();
			mat2->copyTo(mat);
		}

		std::vector<cv::Rect> rects= *gp.getRectangles();
		int minx = 1000, maxx = 0, miny = 1000, maxy = 0;

		for (unsigned int i = 0; i < rects.size(); i++) {
			cv::Rect r= rects [i];
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

		// Put a rectangle on the image
		rectangle(mat, tl, br,cv::Scalar(255, 255, 255), 5);
		// Give the output stream a new image to display
		outputStream.PutFrame(mat);
		table->PutNumber("TopLeftX", tl.x);
		table->PutNumber("TopLeftY", tl.y);
		table->PutNumber("BotRightX", br.x);
		table->PutNumber("BotRightY", br.y);

		//Wait(0.1);
	}
}
// Put methods for controlling this subsystem
// here. Call these from Commands.
