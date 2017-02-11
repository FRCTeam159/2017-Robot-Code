#ifndef FuelMonitor_H
#define FuelMonitor_H

#include "../CommandBase.h"

class FuelMonitor : public CommandBase {
public:
	FuelMonitor();
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};

#endif  // FuelMonitor_H
