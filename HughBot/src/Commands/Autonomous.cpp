#include "Autonomous.h"
#include "VisionUpdate.h"
#include "Commands/DriveToTarget.h"
Autonomous::Autonomous() {

	AddParallel(new VisionUpdate());
	AddSequential(new DriveToTarget());
}
