#ifndef VisionTest_H
#define VisionTest_H

#include "../CommandBase.h"

class VisionTest : public CommandBase {
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
