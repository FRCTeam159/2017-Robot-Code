#ifndef GearPlateToggle_H
#define GearPlateToggle_H

#include "../CommandBase.h"

class GearPlateToggle : public CommandBase {
	enum{
		WAIT_FOR_TRIGGER_PUSH,
		WAIT_FOR_TRIGGER_RELEASE
	};

	int state = WAIT_FOR_TRIGGER_PUSH;

public:
	GearPlateToggle();
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};

#endif  // GearPlateToggle_H
