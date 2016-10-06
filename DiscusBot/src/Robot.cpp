#include "WPILib.h"
#define LIFTER_UP_SPEED 0.2
#define LIFTER_DOWN_SPEED -0.2
#define LIFTER_UP_BUTTON 5
#define LIFTER_DOWN_BUTTON 6

class Robot: public IterativeRobot {
private:
	Joystick *stick;
	CANTalon *frontRight;
	CANTalon *frontLeft;
	CANTalon *rearLeft;
	CANTalon *rearRight;
	CANTalon *liftMotor;
	RobotDrive *robotDrive;
	LiveWindow *lw = LiveWindow::GetInstance();
	SendableChooser *chooser;
	const std::string autoNameDefault = "Default";
	const std::string autoNameCustom = "My Auto";
	std::string autoSelected;

	void RobotInit() {
		stick = new Joystick(0);
		frontLeft = new CANTalon(1);
		frontRight = new CANTalon(4);
		rearLeft = new CANTalon(2);
		rearRight = new CANTalon(3);
		liftMotor = new CANTalon(5);
		chooser = new SendableChooser();
		chooser->AddDefault(autoNameDefault, (void*) &autoNameDefault);
		chooser->AddObject(autoNameCustom, (void*) &autoNameCustom);
		SmartDashboard::PutData("Auto Modes", chooser);
		robotDrive = new RobotDrive(frontLeft, rearLeft, frontRight, rearRight);
		robotDrive->SetInvertedMotor(RobotDrive::kFrontRightMotor, true);
		robotDrive->SetInvertedMotor(RobotDrive::kRearRightMotor, true);
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
	void AutonomousInit() {
		autoSelected = *((std::string*) chooser->GetSelected());
		//std::string autoSelected = SmartDashboard::GetString("Auto Selector", autoNameDefault);
		std::cout << "Auto selected: " << autoSelected << std::endl;

		if (autoSelected == autoNameCustom) {
			//Custom Auto goes here
		} else {
			//Default Auto goes here
		}
	}

	void AutonomousPeriodic() {
		if (autoSelected == autoNameCustom) {
			//Custom Auto goes here
		} else {
			//Default Auto goes here
		}
	}

	void TeleopInit() {

	}

	void TeleopPeriodic() {
		DriveWithJoystick();
		DoLifter();
	}

	void TestPeriodic() {
		lw->Run();
	}
	void DriveWithJoystick() {
		robotDrive->MecanumDrive_Cartesian(stick->GetX(), stick->GetY(),
				stick->GetZ());
	}
	void DoLifter() {
		if (stick->GetRawButton(LIFTER_UP_BUTTON)) {
			liftMotor->Set(LIFTER_UP_SPEED);
			printf("Lifter up button pressed\n");
		}

		else if (stick->GetRawButton(LIFTER_DOWN_BUTTON)) {
			liftMotor->Set(LIFTER_DOWN_SPEED);
			printf("Lifter down button pressed\n");
		} else {
			liftMotor->Set(0);
		}
	}
};

START_ROBOT_CLASS(Robot)
