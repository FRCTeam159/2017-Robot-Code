#ifndef DriveTrain_H
#define DriveTrain_H
#include "WPILib.h"
#include "Commands/Subsystem.h"
#include "CANTalon.h"

class DriveTrain: public frc::Subsystem
{
private:
	// It's desirable that everything possible under private except
	// for methods that implement subsystem capabilities
	CANTalon frontLeft;
	CANTalon frontRight;
	CANTalon backLeft;
	CANTalon backRight;
	RobotDrive *drive;
public:
	DriveTrain();
	void InitDefaultCommand();
	float quadDeadband(float minThreshold, float minOutput, float input);
	void Drive(float xAxis, float yAxis, float zAxis);
};

#endif
