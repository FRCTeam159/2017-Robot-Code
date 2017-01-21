#ifndef Vision_H
#define Vision_H

#include <Commands/Subsystem.h>
#include <networktables/NetworkTable.h>

class Vision : public Subsystem {
private:
	static void VisionThread();
	std::shared_ptr<NetworkTable> table;
	double brightness=2;
public:
	Vision();
	void InitDefaultCommand();
	void Process();
	void Init();
};

#endif  // Vision_H
