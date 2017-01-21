#include "DriveTrain.h"
#include "RobotMap.h"
#include "Commands/DriveWithJoystick.h"
#include "WPILib.h"

DriveTrain::DriveTrain() :
		frc::Subsystem("DriveTrain"),
		frontLeft(FRONTLEFT),
		frontRight(FRONTRIGHT),
		backLeft(BACKLEFT),
		backRight(BACKRIGHT)
{
	drive = new RobotDrive(&frontLeft, &backLeft, &frontRight, &backRight);
	drive->SetInvertedMotor(RobotDrive::kFrontLeftMotor,true);
	drive->SetInvertedMotor(RobotDrive::kRearLeftMotor,true);
	drive->SetExpiration(0.5);
}
void DriveTrain::InitDefaultCommand()
{
	// Set the default command for a subsystem here.
	SetDefaultCommand(new DriveWithJoystick());
}
void DriveTrain::Drive(float xAxis, float yAxis, float zAxis)
{
 CustomArcade(xAxis, yAxis, zAxis);
}
// Put methods for controlling this subsystem
// here. Call these from Commands.
void DriveTrain::CustomArcade(float xAxis, float yAxis, float zAxis)
	{
		float left=0;
		float right=0;  // Left and right motor power values

		// Get axis values
		//float yAxis = 1 * ((*stick).GetY());
		//float xAxis = -1 * ((*stick).GetX());
		//float zAxis = (*stick).GetZ();

		// Turning logic
		if (zAxis != 0)
		{
			left = zAxis;
			right = -zAxis;
		}

		else
		{
			if (xAxis != 0)
			{
				if (xAxis > 0)
				{
					left = yAxis;
					right = (fabs(yAxis) / yAxis) * (fabs(yAxis) - fabs(xAxis));
				}

				else
				{
					right=yAxis;
					left = (fabs(yAxis) / yAxis) * (fabs(yAxis) - fabs(xAxis));
				}
			}

			else
			{
				left = yAxis;
				right = yAxis;
			}
		}

		// Ramp values up

		// Make sure values are between -1 and 1
		left = coerce(-1,1,left);
		right = coerce(-1,1,right);
		frontLeft.Set(left);
		backLeft.Set(left);
		frontRight.Set(-1*right);
		backRight.Set(-1*right);
	}

float DriveTrain::coerce(float min, float max, float x)
	{
		if (x < min)
		{
			x = min;
		}

		else if (x > max)
		{
			x = max;
		}

		return x;
	}

