#include <memory>

#include <Commands/Command.h>
#include <Commands/Scheduler.h>
#include <IterativeRobot.h>
#include <LiveWindow/LiveWindow.h>
#include <SmartDashboard/SendableChooser.h>
#include <SmartDashboard/SmartDashboard.h>
#include "CommandBase.h"
#include "Commands/Autonomous.h"
#include "Commands/DriveForTime.h"
#include "Commands/TurnToAngle.h"
#include "Commands/DriveToTarget.h"
#include "Commands/TurnForTime.h"

#include <thread>
#include <CameraServer.h>
#include <IterativeRobot.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core/types.hpp>


#define RIGHTTURNANGLE 55
#define LEFTTURNANGLE 45

#define DRIVE_TIME 3.0
#define TURN_TIME 1.0
#define TUNE_AUTO

class Robot: public frc::IterativeRobot {
	double rightDrive=0.45;
	double rightTurn=0.4;
	double leftDrive=0.45;
	double leftTurn=0.25;

public:
	void RobotInit() override {
		CommandBase::init();
		frc::SmartDashboard::PutString("AutoMode", "Center");
#ifdef TUNE_AUTO
		frc::SmartDashboard::PutNumber("leftDrive", leftDrive);
		frc::SmartDashboard::PutNumber("leftTurn",leftTurn);
		frc::SmartDashboard::PutNumber("rightDrive", rightDrive);
		frc::SmartDashboard::PutNumber("rightTurn",rightTurn);
#endif
	}

	/**
	 * This function is called once each time the robot enters Disabled mode.
	 * You can use it to reset any subsystem information you want to clear when
	 * the robot is disabled.
	 */
	void DisabledInit() override {
		CommandBase::Disable();
	}

	void DisabledPeriodic() override {
		frc::Scheduler::GetInstance()->Run();
	}

	/**
	 * This autonomous (along with the chooser code above) shows how to select
	 * between different autonomous modes using the dashboard. The sendable
	 * chooser code works with the Java SmartDashboard. If you prefer the
	 * LabVIEW Dashboard, remove all of the chooser code and uncomment the
	 * GetString code to get the auto name from the text box below the Gyro.
	 *
	 * You can add additional auto modes by adding additional commands to the
	 * chooser code above (like the commented example) or additional comparisons
	 * to the if-else structure below with additional strings & commands.
	 */
	void AutonomousInit() override {
		std::string autoSelected = frc::SmartDashboard::GetString("AutoMode", "Center");
#ifdef TUNE_AUTO
		leftDrive = frc::SmartDashboard::GetNumber("leftDrive", 0.5);
		leftTurn = frc::SmartDashboard::GetNumber("leftTurn",0.32);
		rightDrive = frc::SmartDashboard::GetNumber("rightDrive", 0.5);
		rightTurn = frc::SmartDashboard::GetNumber("rightTurn",0.32);
#endif
		CommandGroup *autonomous=new Autonomous();
		if (autoSelected == "Right") {
			// practice-bot: leftDrive=0.45 turnVoltage=0.32
#ifdef TUNE_AUTO
			autonomous->AddSequential(new DriveForTime(DRIVE_TIME,rightDrive));
			autonomous->AddSequential(new TurnForTime(TURN_TIME, rightTurn));
#else
			autonomous->AddSequential(new DriveForTime(DRIVE_TIME,0.45));
			autonomous->AddSequential(new TurnForTime(TURN_TIME, 0.4));
#endif
			cout<<"Chose::Right Auto"<<endl;
		}
		else if(autoSelected == "Left"){
			// practice-bot: leftDrive=0.45 turnVoltage=0.25
#ifdef TUNE_AUTO
			autonomous->AddSequential(new DriveForTime(DRIVE_TIME,leftDrive));
			autonomous->AddSequential(new TurnForTime(TURN_TIME, -leftTurn));
#else
			autonomous->AddSequential(new DriveForTime(DRIVE_TIME,0.45));
			autonomous->AddSequential(new TurnForTime(TURN_TIME, -0.25));
#endif
			//autonomous->AddSequential(new DriveStraight(DRIVEDISTANCE));
			//autonomous->AddSequential(new TurnToAngle(LEFTTURNANGLE));
			cout<<"Chose::Left Auto"<<endl;
		}
		else {
			cout<<"Chose::Center Auto"<<endl;
			autonomous->AddSequential(new DriveForTime(1.5, 0.4));
		}
		autonomous->AddSequential(new DriveToTarget());
		autonomous->AddSequential(new DriveForTime(1, 0.25));
		autonomous->AddSequential(new DriveForTime(1.5, -0.2));

		autonomousCommand.reset(autonomous);

		CommandBase::AutonomousInit();

		if (autonomousCommand.get() != nullptr) {
			autonomousCommand->Start();
		}

	}

	void AutonomousPeriodic() override {
		frc::Scheduler::GetInstance()->Run();
	}

	void TeleopInit() override {
		// This makes sure that the autonomous stops running when
		// teleop starts running. If you want the autonomous to
		// continue until interrupted by another command, remove
		// this line or comment it out.
		if (autonomousCommand != nullptr) {
			autonomousCommand->Cancel();
		}
		CommandBase::Enable();
	}

	void TeleopPeriodic() override {
		frc::Scheduler::GetInstance()->Run();
	}

	void TestInit(){
		cout<<"TestInit"<<endl;
	}

	void TestPeriodic() override {
		frc::LiveWindow::GetInstance()->Run();
	}

private:
	std::unique_ptr<frc::Command> autonomousCommand;
	frc::SendableChooser<frc::Command*> chooser;
};

START_ROBOT_CLASS(Robot)
