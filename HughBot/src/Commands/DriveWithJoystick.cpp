#include "DriveWithJoystick.h"
#include "Subsystems/DriveTrain.h"

DriveWithJoystick::DriveWithJoystick()
{
	// Use Requires() here to declare subsystem dependencies
	Requires(drivetrain.get());
}

// Called just before this Command runs the first time
void DriveWithJoystick::Initialize()
{
	std::cout << "DriveWithJoystick::Initialize()" << std::endl;
}

// Called repeatedly when this Command is scheduled to run
void DriveWithJoystick::Execute()
{
	// Get axis values
	Joystick *stick = oi->GetJoystick();
	float yAxis = stick-> GetY();
	float xAxis = stick-> GetX();
	float zAxis = stick-> GetZ();
	// Run axis values through deadband
	yAxis = quadDeadband(.6, .3, yAxis);
	xAxis = quadDeadband(.6, .3, xAxis);
	zAxis = quadDeadband(.6, .3, zAxis);
	drivetrain.get()->Drive(xAxis, yAxis, zAxis);
}

// Make this return true when this Command no longer needs to run execute()
bool DriveWithJoystick::IsFinished()
{
	return false;
}

// Called once after isFinished returns true
void DriveWithJoystick::End()
{
	std::cout << "DriveWithJoystick Finished" << std::endl;
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void DriveWithJoystick::Interrupted()
{
	End();
}
float DriveWithJoystick::quadDeadband(float minThreshold, float minOutput, float input)
{
	if (input > minThreshold) {
		return ((((1 - minOutput)
				/ ((1 - minThreshold) * (1 - minThreshold)))
				* ((input - minThreshold) * (input - minThreshold)))
				+ minOutput);
	} else {
		if (input < (-1 * minThreshold)) {
			return (((minOutput - 1)
					/ ((minThreshold - 1) * (minThreshold - 1)))
					* ((minThreshold + input) * (minThreshold + input)))
					- minOutput;
		}

		else {
			return 0;
		}
	}
}
