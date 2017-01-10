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
}

void DriveTrain::InitDefaultCommand()
{
	// Set the default command for a subsystem here.
	SetDefaultCommand(new DriveWithJoystick());
}
void DriveTrain::Drive(float xAxis, float yAxis, float zAxis)
{
	drive->MecanumDrive_Cartesian(xAxis, yAxis, zAxis);
}
// Put methods for controlling this subsystem
// here. Call these from Commands.
