#ifndef Vision_H
#define Vision_H

#include <Commands/Subsystem.h>
#include <networktables/NetworkTable.h>
#include "Subsystems/GripPipeline.h"
#include <CameraServer.h>


class Vision : public Subsystem {
private:
	static void VisionThread();
	std::shared_ptr<NetworkTable> table;

public:
	Vision();
	void InitDefaultCommand();
	void Process();
	void Init();
};

#endif  // Vision_H
