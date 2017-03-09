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

class Robot: public frc::IterativeRobot {
public:
	void RobotInit() override {
		CommandBase::init();
		frc::SmartDashboard::PutString("AutoMode", "Center");
		frc::SmartDashboard::PutNumber("DriveTime", 3);
		frc::SmartDashboard::PutNumber("TurnTime",1);
	//	frc::SmartDashboard::PutNumber("RightAutoAngle", RIGHTTURNANGLE);
		//frc::SmartDashboard::PutNumber("LeftTurnAnlge", LEFTTURNANGLE);
		// chooser.AddDefault("Default Auto", new ExampleCommand());
		// chooser.AddObject("My Auto", new MyAutoCommand());
		//frc::SmartDashboard::PutData("Auto Modes", &chooser);
      //  CameraServer::GetInstance()->SetQuality(50);

        //the camera name (ex "cam0") can be found through the roborio web interface
       // CameraServer::GetInstance()->StartAutomaticCapture();
		//std::thread visionThread(VisionThread);
		//visionThread.detach();
		//disabledCommand.reset(new DisabledCommand());

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
//		std::string autoSelected = frc::SmartDashboard::GetString("Auto Selector", "Default");
//		if (autoSelected == "My Auto") {
//			autonomousCommand.reset(new Autonomous());
//			cout<<"Chose My Auto"<<endl;
//		}
//		else {
//			autonomousCommand.reset(new Autonomous());
//			cout<<"Chose default auto"<<endl;
//		}
//		CommandBase::AutonomousInit();
//
//		autonomousCommand.reset(new Autonomous());
//
//		if (autonomousCommand.get() != nullptr) {
//			autonomousCommand->Start();
//		}
	//	double rightAngle = frc::SmartDashboard::GetNumber("RightAngle", RIGHTTURNANGLE);
	//	double leftAngle = frc::SmartDashboard::GetNumber("LeftAngle", LEFTTURNANGLE);
		std::string autoSelected = frc::SmartDashboard::GetString("AutoMode", "Center");
		double driveTime = frc::SmartDashboard::GetNumber("DriveTime", 3);
		double turnTime = frc::SmartDashboard::GetNumber("TurnTime",1);
		CommandGroup *autonomous=new Autonomous();
		if (autoSelected == "Right") {
			autonomous->AddSequential(new DriveForTime(driveTime, 0.45));
			autonomous->AddSequential(new TurnForTime(turnTime, -0.45));
			//autonomous->AddSequential(new DriveStraight(DRIVEDISTANCE));
			//autonomous->AddSequential(new Turn(-0.27));
			//autonomous->AddSequential(new TurnToAngle(-RIGHTTURNANGLE));

			cout<<"Chose::Right Auto"<<endl;
		}
		else if(autoSelected == "Left"){
			autonomous->AddSequential(new DriveForTime(driveTime, 0.45));
			autonomous->AddSequential(new TurnForTime(turnTime, 0.45));
			//autonomous->AddSequential(new DriveStraight(DRIVEDISTANCE));
			//autonomous->AddSequential(new Turn(0.27));
			//autonomous->AddSequential(new TurnToAngle(LEFTTURNANGLE));
			cout<<"Chose::Left Auto"<<endl;
		}
		else {
			cout<<"Chose::Center Auto"<<endl;
			//autonomous->AddSequential(new DriveForTime(1, 0.4));
		}
		autonomous->AddSequential(new DriveToTarget());

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
