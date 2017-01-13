#ifndef Vision_H
#define Vision_H
#include "Subsystems/GripPipeline.h"

#include <Commands/Subsystem.h>
using namespace grip;

class Vision : public Subsystem {
private:
	GripPipeline gp;


public:
	Vision();
	void InitDefaultCommand();
	void Process( cv::Mat);
};

#endif  // Vision_H
