#include "DriveTrain.h"
#include "RobotMap.h"
#include "Commands/DriveWithJoystick.h"
#include "WPILib.h"
DriveTrain::DriveTrain() : Subsystem("DriveTrain"),
		frontLeft(FRONTLEFT),   // slave  1
		frontRight(FRONTRIGHT), // master 4
		backLeft(BACKLEFT),     // master 2
		backRight(BACKRIGHT)    // slave  3
{
	InitDrive();
	//drive = new RobotDrive(&frontLeft, &backLeft, &frontRight, &backRight);
	//drive->SetInvertedMotor(DriveTrain::kFrontLeftMotor,true);

	backLeft.SetInverted(true);
	//drive->SetInvertedMotor(RobotDrive::kRearLeftMotor,false);

	//frontRight.SetFeedbackDevice(CANTalon::QuadEncoder);
	//backLeft.SetFeedbackDevice(CANTalon::QuadEncoder);
	//frontRightSetExpiration.SetControlMode(CANTalon::kSpeed);
	//backLeft.SetControlMode(CANTalon::kSpeed);
	frontLeft.SetControlMode(CANTalon::kFollower);
	backRight.SetControlMode(CANTalon::kFollower);
	backRight.EnableControl();
	frontLeft.EnableControl();

	gearPneumatic = new DoubleSolenoid(GEARSHIFTID,0,1);
	SetLowGear();

	SetExpiration(0.2);
}
void DriveTrain::InitDefaultCommand()
{
	// Set the default command for a subsystem here.
	SetDefaultCommand(new DriveWithJoystick());
}
void DriveTrain::Drive(float xAxis, float yAxis, float zAxis)
{
	CustomArcade(xAxis, yAxis, zAxis, true);
}
// Put methods for controlling this subsystem
// here. Call these from Commands.
void DriveTrain::CustomArcade(float xAxis, float yAxis, float zAxis, bool squaredInputs) {

	  if (squaredInputs) {
	    if (yAxis >= 0) {
	      yAxis = (yAxis * yAxis);
	    }else {
	      yAxis = -(yAxis * yAxis);
	    }
	    if (xAxis >= 0) {
	      xAxis = (xAxis * xAxis);
	    }else {
	      xAxis = -(xAxis * xAxis);
	    }
	    if (zAxis >= 0){
	    	zAxis = (zAxis * zAxis);
	    }else {
	    	zAxis = -(zAxis * zAxis);
	    }
	  }
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
	m_safetyHelper->Feed();
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
void DriveTrain::InitDrive() {
	m_safetyHelper = std::make_unique<MotorSafetyHelper>(this);
	m_safetyHelper->SetSafetyEnabled(true);
}

void DriveTrain::SetExpiration(double timeout) {
  m_safetyHelper->SetExpiration(timeout);
}

double DriveTrain::GetExpiration() const {
  return m_safetyHelper->GetExpiration();
}

bool DriveTrain::IsAlive() const { return m_safetyHelper->IsAlive(); }

bool DriveTrain::IsSafetyEnabled() const {
  return m_safetyHelper->IsSafetyEnabled();
}

void DriveTrain::SetSafetyEnabled(bool enabled) {
  m_safetyHelper->SetSafetyEnabled(enabled);
}

void DriveTrain::GetDescription(std::ostringstream& desc) const {
  desc << "DriveTrain";
}

void DriveTrain::StopMotor() {
  backRight.StopMotor();
  backLeft.StopMotor();
  frontRight.StopMotor();
  frontLeft.StopMotor();
  m_safetyHelper->Feed();
}

