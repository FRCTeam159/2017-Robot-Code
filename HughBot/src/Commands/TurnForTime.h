#ifndef TurnForTime_H
#define TurnForTime_H

#include "../CommandBase.h"

class TurnForTime : public CommandBase {
private:
	double time;
	double speed;
	double targetTime;
	double currentTime;
public:
	TurnForTime(double t, double s);
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};

#endif  // TurnForTime_H
