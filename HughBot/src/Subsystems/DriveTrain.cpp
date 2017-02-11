#include "DriveTrain.h"
#include "RobotMap.h"
#include "Commands/DriveWithJoystick.h"
#include "WPILib.h"

#define P 0.1
#define I 0.0
#define D 0.0
#define SIGN(x) ((x) >= 0? 1:-1)


DriveTrain::DriveTrain() : Subsystem("DriveTrain"),
		frontLeft(FRONTLEFT),   // slave  1
		frontRight(FRONTRIGHT), // master 4
		backLeft(BACKLEFT),     // master 2
		backRight(BACKRIGHT)    // slave  3
{
	InitDrive();

	backLeft.SetInverted(false);
	frontRight.ConfigLimitMode(CANSpeedController::kLimitMode_SrxDisableSwitchInputs);
	backLeft.ConfigLimitMode(CANSpeedController::kLimitMode_SrxDisableSwitchInputs);

	frontRight.SetControlMode(CANTalon::kPercentVbus);
	backLeft.SetControlMode(CANTalon::kPercentVbus);
/*
	frontRight.SetControlMode(CANTalon::kSpeed);
	backLeft.SetControlMode(CANTalon::kSpeed);

	frontRight.SetPID(P,I,D);
	backLeft.SetPID(P,I,D);

	frontRight.SetFeedbackDevice(CANTalon::QuadEncoder);
	backLeft.SetFeedbackDevice(CANTalon::QuadEncoder);

	frontRight.ConfigEncoderCodesPerRev(900);
	backLeft.ConfigEncoderCodesPerRev(900);
*/
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
//Squaring inputs in order to create a smoother acceleration curve
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
				right = SIGN(yAxis) * (fabs(yAxis) - fabs(xAxis));
			}

			else {
				right = yAxis;
				left = SIGN(yAxis) * (fabs(yAxis) - fabs(yAxis));
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

void DriveTrain::DisableDrive() {
	backRight.Disable();
	backLeft.Disable();
	frontRight.Disable();
	frontLeft.Disable();
}

void DriveTrain::EnableDrive() {
	backRight.Enable();
	backLeft.Enable();
	frontRight.Enable();
	frontLeft.Enable();
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
void DriveTrain::TankDrive(float left, float right) { //Autonomous drive train call
	left*=0.1;
	right*=0.1;
	frontLeft.Set(BACKLEFT);
	backLeft.Set(left);
	frontRight.Set(-right);
	backRight.Set(FRONTRIGHT);

	m_safetyHelper->Feed();
}
void DriveTrain::Enable() {
	//backRight.Enable();
	//frontLeft.Enable();
	frontRight.Enable();
	backLeft.Enable();
}

