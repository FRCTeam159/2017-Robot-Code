#ifndef DriveToTarget_H
#define DriveToTarget_H

#include "../CommandBase.h"

class DriveToTarget : public CommandBase, public PIDSource, public PIDOutput {
	PIDController pid;
	double distance;
	double angle;
	Vision::TargetInfo target;
	double GetDistance();
	double Clamp(double min,double max,double x);
public:
	DriveToTarget();
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
	void PIDWrite(double d);
	double PIDGet();
};

#endif  // DriveToTarget_H
