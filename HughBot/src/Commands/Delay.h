#ifndef Delay_H
#define Delay_H

#include "../CommandBase.h"

class Delay : public CommandBase {
	double timeout;
public:
	Delay(double seconds);
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};

#endif  // Delay_H
