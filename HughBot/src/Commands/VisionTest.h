#ifndef VisionTest_H
#define VisionTest_H

#include "../CommandBase.h"
#include <CameraServer.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core/types.hpp>

class VisionTest : public CommandBase {
	cs::UsbCamera camera;
	cs::CvSink cvSink;
	cs::CvSource outputStream;
	cv::Mat mat;
	bool error=false;
public:
	VisionTest();
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
	void InitDefaultCommand();
};

#endif  // VisionTest_H
