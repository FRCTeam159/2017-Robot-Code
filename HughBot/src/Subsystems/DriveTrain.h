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
	DoubleSolenoid *gearPneumatic;
	bool inlowgear=false;

	void CustomArcade(float xAxis, float yAxis, float zAxis);
	float coerce(float min, float max, float x);
public:
	DriveTrain();
	void Drive(float xAxis, float yAxis, float zAxis);
	void InitDefaultCommand();
	//void Drive(float xAxis, float yAxis, float zAxis);
	//void ArcadeDrive(Joystick*);
	void SetLowGear();
	void SetHighGear();
};

#endif
