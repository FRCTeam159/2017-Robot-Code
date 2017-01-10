#ifndef Vision_H
#define Vision_H

#include <Commands/Subsystem.h>

class Vision : public Subsystem {
private:
	// It's desirable that everything possible under private except
	// for methods that implement subsystem capabilities

public:
	Vision();
	void InitDefaultCommand();
};

#endif  // Vision_H
