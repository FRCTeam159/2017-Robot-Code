#include "DriveTrain.h"
#include "RobotMap.h"
#include "Commands/DriveWithJoystick.h"
#include "WPILib.h"

DriveTrain::DriveTrain() :
		frc::Subsystem("DriveTrain"),
		frontLeft(FRONTLEFT), // slave 1
		frontRight(FRONTRIGHT), //4
		backLeft(BACKLEFT), //2
		backRight(BACKRIGHT) // slave 3
{
	drive = new RobotDrive(&frontLeft, &backLeft, &frontRight, &backRight);
	//drive->SetInvertedMotor(RobotDrive::kFrontLeftMotor,true);
	drive->SetInvertedMotor(RobotDrive::kRearLeftMotor,false);
	//frontRight.SetFeedbackDevice(CANTalon::QuadEncoder);
	//backLeft.SetFeedbackDevice(CANTalon::QuadEncoder);
	//drive->SetExpiration(0.1);
	//frontRight.SetControlMode(CANTalon::kSpeed);
	//backLeft.SetControlMode(CANTalon::kSpeed);
	frontLeft.SetControlMode(CANTalon::kFollower);
	backRight.SetControlMode(CANTalon::kFollower);
	backRight.EnableControl();
	frontLeft.EnableControl();



	gearPneumatic = new DoubleSolenoid(0,0,1);
	SetLowGear();

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
void DriveTrain::CustomArcade(float xAxis, float yAxis, float zAxis) {
	float left = 0;
	float right = 0;  // Left and right motor power values
	// Turning logic
	if (zAxis != 0) {
		left = zAxis;
		right = -zAxis;
	}
	else {
		if (xAxis != 0) {
			if (xAxis > 0) {
				left = yAxis;
				right = (fabs(yAxis) / yAxis) * (fabs(yAxis) - fabs(xAxis));
			}

			else {
				right = yAxis;
				left = (fabs(yAxis) / yAxis) * (fabs(yAxis) - fabs(xAxis));
			}
		}
		else {
			left = yAxis;
			right = yAxis;
		}
	}

	// Ramp values up

	// Make sure values are between -1 and 1
	left = coerce(-1, 1, left);
	right = coerce(-1, 1, right);
	frontLeft.Set(BACKLEFT);
	backLeft.Set(left);
	frontRight.Set(-right);
	backRight.Set(FRONTRIGHT);
}

float DriveTrain::coerce(float min, float max, float x) {
	if (x < min) {
		x = min;
	}

	else if (x > max) {
		x = max;
	}
	return x;
}

void DriveTrain::SetLowGear() {
	if(!inlowgear){
		gearPneumatic->Set(DoubleSolenoid::kReverse);
		cout << "Setting Low Gear"<<endl;
		inlowgear=true;
	}
}

void DriveTrain::SetHighGear() {
	if(inlowgear){
		gearPneumatic->Set(DoubleSolenoid::kForward);
		cout << "Setting High Gear"<<endl;
		inlowgear=false;
	}
}
