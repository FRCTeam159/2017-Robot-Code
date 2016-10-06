#include "WPILib.h"

class Robot: public IterativeRobot
{
private:
	Joystick *stick;
	CANTalon *frontRight;
	CANTalon *frontLeft;
	CANTalon *rearLeft;
	CANTalon *rearRight;
	CANTalon *liftMotor;
	LiveWindow *lw;
	float *routput;
	float *loutput;
	bool *wheelOutput;
	bool *wheelPInput;
	CANTalon *shooterMotor;
	RobotDrive *robotDrive;
	DoubleSolenoid *shooterPneumatic;
	SendableChooser *chooser;
	const std::string autoNameDefault = "Default";
	const std::string autoNameCustom = "My Auto";
	std::string autoSelected;

	void RobotInit()
	{
		stick = new Joystick(0);
		shooterPneumatic = new DoubleSolenoid(7,0,1);
		frontLeft = new CANTalon(1);
		frontRight = new CANTalon(4);
		rearLeft = new CANTalon(2);
		rearRight = new CANTalon(3);
		liftMotor = new CANTalon(5);
		shooterMotor = new CANTalon(6);
		chooser = new SendableChooser();
		chooser->AddDefault(autoNameDefault, (void*)&autoNameDefault);
		chooser->AddObject(autoNameCustom, (void*)&autoNameCustom);
		SmartDashboard::PutData("Auto Modes", chooser);
		robotDrive = new RobotDrive(frontLeft, rearLeft, frontRight, rearRight);
		robotDrive->SetInvertedMotor(RobotDrive::kFrontRightMotor,true);
		robotDrive->SetInvertedMotor(RobotDrive::kRearRightMotor,true);
		wheelOutput = new bool;
		wheelPInput = new bool;

		lw = LiveWindow::GetInstance();
	}


	/**
	 * This autonomous (along with the chooser code above) shows how to select between different autonomous modes
	 * using the dashboard. The sendable chooser code works with the Java SmartDashboard. If you prefer the LabVIEW
	 * Dashboard, remove all of the chooser code and uncomment the GetString line to get the auto name from the text box
	 * below the Gyro
	 *
	 * You can add additional auto modes by adding additional comparisons to the if-else structure below with additional strings.
	 * If using the SendableChooser make sure to add them to the chooser code above as well.
	 */
	void AutonomousInit()
	{
		autoSelected = *((std::string*)chooser->GetSelected());
		//std::string autoSelected = SmartDashboard::GetString("Auto Selector", autoNameDefault);
		std::cout << "Auto selected: " << autoSelected << std::endl;

		if(autoSelected == autoNameCustom){
			//Custom Auto goes here
		} else {
			//Default Auto goes here
		}
	}

	void AutonomousPeriodic()
	{
		if(autoSelected == autoNameCustom){
			//Custom Auto goes here
		} else {
			//Default Auto goes here
		}
	}

	void TeleopInit()
	{
		*wheelPInput = false;
		*wheelOutput = false;
	}

	void TeleopPeriodic()
	{
		DriveWithJoystick ();
	DoShooter ();
	DoLifter ();

	}

	void TestPeriodic()
	{
		lw->Run();
	}
	void DriveWithJoystick ()
	{
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
		if (toggle(stick->GetRawButton(2), wheelOutput, wheelPInput))
			shooterMotor->Set(-1);
		else
			shooterMotor->Set(0);

		if (stick->GetRawButton(1))
			shooterPneumatic->Set(DoubleSolenoid::kReverse);
		else
			shooterPneumatic->Set(DoubleSolenoid::kForward);
	}
	void DoLifter ()
	{
		if (stick->GetRawButton(3)) {
			liftMotor->Set(.5);
		//	printf("Flipper up button pressed\n");
		}

		else if (stick->GetRawButton(4)) {
			liftMotor->Set(-.5);
		//	printf("Flipper down button pressed\n");
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
