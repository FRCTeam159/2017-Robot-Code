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
public:
	struct CameraInfo {
		int screenWidth;
		int screenHeight;
		double fov; // Field of veiw
		double fovFactor;
		double HorizontalOffset;
	};

	CameraInfo cameraInfo;

	struct TargetInfo {
		cv::Point Center;
		double Distance;
		double HorizontalOffset;
		double HorizontalAngle;
		double Height;
		double Width;
		double ActualWidth;
		double ActualHeight;
		int numrects;
	};
private:
	Relay ringLight;

	static cs::UsbCamera camera1;
	static cs::UsbCamera camera2;
	static cs::CvSink cvSink;
	static cs::CvSource outputStream;

	void CalcTargetInfo(cv::Point top, cv::Point bottom);


	static llvm::ArrayRef<double>  hsvThresholdHue;
	static llvm::ArrayRef<double>  hsvThresholdSaturation;
	static llvm::ArrayRef<double>  hsvThresholdValue;
	static void VisionThread();
	std::shared_ptr<NetworkTable> table;

	static std::vector<cv::Rect> GoodRects(std::vector<cv::Rect> r);

public:
	Vision();
	TargetInfo targetInfo;
	void InitDefaultCommand();
	void Process();
	void Init();
	double GetTargetDistance();
	double GetTargetAngle();
	int GetNumTargets();
	void SetCameraInfo(int width, int height, double fov, double hoff);
	void CalcTargetInfo(int n,cv::Point top, cv::Point bottom, TargetInfo &info);
	void GetTargetInfo(TargetInfo &info);
	void PublishTargetInfo(TargetInfo &info);
	void SetRingLight(bool state);
};

#endif  // Vision_H
