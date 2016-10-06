#include "WPILib.h"
#define LIFTER_UP_SPEED 0.5
#define LIFTER_DOWN_SPEED -0.5
#define LIFTER_UP_BUTTON 5
#define LIFTER_DOWN_BUTTON 6
#define FLYWHEEL_BUTTON 2
#define SHOOTER_BUTTON 1

class Robot: public IterativeRobot {
private:
	Joystick *stick;
	CANTalon *frontRight;
	CANTalon *frontLeft;
	CANTalon *rearLeft;
	CANTalon *rearRight;
	CANTalon *liftMotor;
	CANTalon *shooterMotor;
	DoubleSolenoid *shooterPneumatic;

	RobotDrive *robotDrive;
	bool *wheelOutput;
	bool *wheelPInput;

	LiveWindow *lw = LiveWindow::GetInstance();
	SendableChooser *chooser;
	const std::string autoNameDefault = "Default";
	const std::string autoNameCustom = "My Auto";
	std::string autoSelected;

	void RobotInit() {
		stick = new Joystick(0);
		shooterPneumatic = new DoubleSolenoid(7,0,1);
		frontLeft = new CANTalon(1);
		frontRight = new CANTalon(4);
		rearLeft = new CANTalon(2);
		rearRight = new CANTalon(3);
		liftMotor = new CANTalon(5);
		shooterMotor = new CANTalon(6);

		chooser = new SendableChooser();
		chooser->AddDefault(autoNameDefault, (void*) &autoNameDefault);
		chooser->AddObject(autoNameCustom, (void*) &autoNameCustom);
		SmartDashboard::PutData("Auto Modes", chooser);
		robotDrive = new RobotDrive(frontLeft, rearLeft, frontRight, rearRight);
		robotDrive->SetInvertedMotor(RobotDrive::kFrontRightMotor, true);
		robotDrive->SetInvertedMotor(RobotDrive::kRearRightMotor, true);
		wheelOutput = new bool;
		wheelPInput = new bool;
	}

	void AutonomousInit() {
	}

	void AutonomousPeriodic() {
	}

	void TeleopInit() {
		*wheelPInput = false;
		*wheelOutput = false;
	}

	void TeleopPeriodic() {
		DriveWithJoystick();
		DoLifter();
		DoShooter ();
	}

	void TestPeriodic() {
		lw->Run();
	}
	void DriveWithJoystick() {
		// Get axis values
		float yAxis = stick-> GetY();
		float xAxis = stick-> GetX();
		float zAxis = stick-> GetZ();
		// Run axis values through deadband
		yAxis = quadDeadband(.6, .3, yAxis);
		xAxis = quadDeadband(.6, .3, xAxis);
		zAxis = quadDeadband(.6, .3, zAxis);
		robotDrive->MecanumDrive_Cartesian(xAxis, yAxis, zAxis);
	}
	void DoShooter ()
	{
		if (toggle(stick->GetRawButton(FLYWHEEL_BUTTON), wheelOutput, wheelPInput))
			shooterMotor->Set(-1);
		else
			shooterMotor->Set(0);

		if (stick->GetRawButton(SHOOTER_BUTTON))
			shooterPneumatic->Set(DoubleSolenoid::kReverse);
		else
			shooterPneumatic->Set(DoubleSolenoid::kForward);
	}
	void DoLifter() {
		if (stick->GetRawButton(LIFTER_UP_BUTTON)) {
			liftMotor->Set(LIFTER_UP_SPEED);
		}
		else if (stick->GetRawButton(LIFTER_DOWN_BUTTON)) {
			liftMotor->Set(LIFTER_DOWN_SPEED);
		} else {
			liftMotor->Set(0);
		}
	}

	bool toggle(bool input, bool* loutput, bool* pinput) {
		if (input && !(*pinput)) {
			(*loutput) = !(*loutput);
			(*pinput) = input;
			return (*loutput);
		} else {
			(*pinput) = input;
			return (*loutput);
		}
	}
	float quadDeadband(float minThreshold, float minOutput, float input) {
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
};

START_ROBOT_CLASS(Robot)
