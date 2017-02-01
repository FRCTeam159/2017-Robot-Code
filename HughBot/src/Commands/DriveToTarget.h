#ifndef DriveToTarget_H
#define DriveToTarget_H

#include "../CommandBase.h"

class DriveToTarget : public CommandBase {
public:
	DriveToTarget();
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};

#endif  // DriveToTarget_H
