#ifndef Vision_H
#define Vision_H
#include "Subsystems/GripPipeline.h"

#include <Commands/Subsystem.h>
#include <CameraServer.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core/types.hpp>
using namespace grip;

class Vision : public Subsystem {
private:
	static cs::UsbCamera camera1;
	static cs::UsbCamera camera2;
	static cs::CvSink cvSink;
	static cs::CvSource outputStream;


	static llvm::ArrayRef<double>  hsvThresholdHue;
	static llvm::ArrayRef<double>  hsvThresholdSaturation;
	static llvm::ArrayRef<double>  hsvThresholdValue;
	static void VisionThread();
	std::shared_ptr<NetworkTable> table;
	cv::Point top=cv::Point(10, 10);
	cv::Point bot=cv::Point(20, 20);


public:
	Vision();
	void InitDefaultCommand();
	void Process();
	void Init();
	static void AdjustCamera(double exposure, double balance, double brightness);
	double GetDistance();
	double GetDirection();
};

#endif  // Vision_H
