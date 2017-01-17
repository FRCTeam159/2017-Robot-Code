#ifndef AdjustCamera_H
#define AdjustCamera_H

#include "../CommandBase.h"

class AdjustCamera : public CommandBase {
public:
	AdjustCamera();
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};

#endif  // AdjustCamera_H
