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
	cs::UsbCamera camera;
	cs::UsbCamera camera2;
	cs::CvSink cvSink;
	cs::CvSource outputStream;
	GripPipeline gp;
	bool error=false;
	double brightness = 10;
	double exposure = 10;

public:
	Vision();
	void InitDefaultCommand();
	void Process();
	void Init();
	void CameraSettings(double exposure, double balance, double brightness);
	void AdjustCamera(double exposure, double balance, double brightness);

};

#endif  // Vision_H
