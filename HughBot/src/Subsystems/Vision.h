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
	cs::CvSink cvSink;
	cs::CvSource outputStream;
	GripPipeline gp;
	bool error=false;

public:
	Vision();
	void InitDefaultCommand();
	void Process();
	void Init();
};

#endif  // Vision_H
