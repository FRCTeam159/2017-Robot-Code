#include <memory>

#include <Commands/Command.h>
#include <Commands/Scheduler.h>
#include <IterativeRobot.h>
#include <LiveWindow/LiveWindow.h>
#include <SmartDashboard/SendableChooser.h>
#include <SmartDashboard/SmartDashboard.h>
#include "CommandBase.h"
#include "Commands/DisabledCommand.h"
#include "Commands/AdjustCamera.h"

#include "Commands/Autonomous.h"

#include <thread>
#include <CameraServer.h>
#include <Commands/DisabledCommand.h>
#include <IterativeRobot.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core/types.hpp>

class Robot: public frc::IterativeRobot {
	static void VisionThread() {
			// Get the USB camera from CameraServer
			cs::UsbCamera camera = CameraServer::GetInstance()->StartAutomaticCapture();
			// Set the resolution
			camera.SetResolution(320, 240);

			// Get a CvSink. This will capture Mats from the Camera
			cs::CvSink cvSink = CameraServer::GetInstance()->GetVideo();
			// Setup a CvSource. This will send images back to the Dashboard
			cs::CvSource outputStream = CameraServer::GetInstance()->
					PutVideo("Rectangle", 320, 240);

			// Mats are very memory expensive. Lets reuse this Mat.
			cv::Mat mat;
			int startposition=10;
			while (true) {
				// Tell the CvSink to grab a frame from the camera and put it
				// in the source mat.  If there is an error notify the output.
				if (cvSink.GrabFrame(mat) == 0) {
					// Send the output the error.
					outputStream.NotifyError(cvSink.GetError());
					// skip the rest of the current iteration
					continue;
				}
				// Put a rectangle on the image
				int width=100;
				int linewidth=5;

				rectangle(mat, cv::Point(startposition, startposition), cv::Point(startposition+width, startposition+width),
						cv::Scalar(255, 0, 0), linewidth);
				// Give the output stream a new image to display
				outputStream.PutFrame(mat);
				startposition++;
				if (startposition>20)
					startposition=10;
			}
		}

public:
	void RobotInit() override {
		CommandBase::init();
		// chooser.AddDefault("Default Auto", new ExampleCommand());
		// chooser.AddObject("My Auto", new MyAutoCommand());
		frc::SmartDashboard::PutData("Auto Modes", &chooser);
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
		std::string autoSelected = frc::SmartDashboard::GetString("Auto Selector", "Default");
		if (autoSelected == "My Auto") {
			autonomousCommand.reset(new Autonomous());
			cout<<"Chose My Auto"<<endl;
		}
		else {
			autonomousCommand.reset(new Autonomous());
			cout<<"Chose default auto"<<endl;
		}

		//autonomousCommand.reset(chooser.GetSelected());

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
	}

	void TeleopPeriodic() override {
		frc::Scheduler::GetInstance()->Run();
	}

	void TestInit(){
		disabledCommand.reset(new DisabledCommand());
		disabledCommand->Start();
		cout<<"TestInit"<<endl;
	}

	void TestPeriodic() override {
		frc::LiveWindow::GetInstance()->Run();
	}

private:
	std::unique_ptr<frc::Command> autonomousCommand;
	frc::SendableChooser<frc::Command*> chooser;
	std::unique_ptr<frc::Command> disabledCommand;
};

START_ROBOT_CLASS(Robot)
