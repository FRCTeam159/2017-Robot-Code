#include "Vision.h"
#include "../RobotMap.h"
#include "Commands/VisionTest.h"
#include "Subsystems/GripPipeline.h"

Vision::Vision() :
		Subsystem("ExampleSubsystem"), gp() {

}

void Vision::InitDefaultCommand() {
	// Set the default command for a subsystem here.
	SetDefaultCommand(new VisionTest());
}

//#define SHOW_RGB_THRESHOLD

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
	gp.process(mat);
#ifdef SHOW_RGB_THRESHOLD
	cv::Mat* mat2=gp.getrgbThresholdOutput();
	mat2->copyTo(mat);
#endif
	//cv::Mat* mat2=gp.getblurOutput();
	int minx = 1000, maxx = 0, miny = 1000, maxy = 0;
	std::vector<std::vector<cv::Point> > points = *gp.getfilterContoursOutput();
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
}

void Vision::Init() {
	camera = CameraServer::GetInstance()->StartAutomaticCapture();
	// Set the resolution
	camera.SetResolution(320, 240);
	//camera.SetFPS(1);

	//camera.SetPixelFormat(cs::VideoMode::PixelFormat::kBGR);

	// Get a CvSink. This will capture Mats from the Camera
	cvSink = CameraServer::GetInstance()->GetVideo();
	// Setup a CvSource. This will send images back to the Dashboard
	outputStream = CameraServer::GetInstance()->PutVideo("Rectangle", 320, 240);
}
// Put methods for controlling this subsystem
// here. Call these from Commands.
