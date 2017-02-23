#include "Autonomous.h"
#include "VisionUpdate.h"
#include "Commands/DriveToTarget.h"
#include "Commands/DriveStraight.h"
#include "Commands/Delay.h"
//Autonomous::Autonomous() {
//
//	AddParallel(new VisionUpdate());
//	AddSequential(new Delay(0.5));
//	AddSequential(new DriveToTarget());
//}
Autonomous::Autonomous() {

	AddParallel(new VisionUpdate());
	AddSequential(new Delay(0.5));
	//AddSequential(new DriveStraight(66));
}
