#include <thread>
#include <Commands/Command.h>
#include <Commands/Scheduler.h>

#include <IterativeRobot.h>
#include "Timer.h"
#include "CommandBase.h"

//#include "llvm/ArrayRef.h"
//#include "llvm/StringRef.h"


/**
 * This is a demo program showing the use of OpenCV to do vision processing. The
 * image is acquired from the USB camera, then a rectangle is put on the image and
 * sent to the dashboard. OpenCV has many methods for different types of
 * processing.
 */
class Robot: public frc::IterativeRobot {
private:


	void RobotInit() {
		CommandBase::init();
	}
	void TeleopInit() override {
	}

	void TeleopPeriodic() override {
		frc::Scheduler::GetInstance()->Run();
	}

};

START_ROBOT_CLASS(Robot)
