#ifndef GearPlateToggle_H
#define GearPlateToggle_H

#include "../CommandBase.h"

class GearPlateToggle : public CommandBase {
	bool isOpen=false;
public:
	GearPlateToggle();
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};

#endif  // GearPlateToggle_H
