#include "OI.h"
#include "RobotMap.h"
#include <Commands/GearPlateToggle.h>
#include <WPILib.h>

OI::OI() {
	stick = new Joystick(STICK);
	// Process operator interface input here.
	JoystickButton* d_up = new JoystickButton(stick, GEARTOGGLEBUTTON);
		d_up->WhenPressed(new GearPlateToggle());
}

Joystick* OI::GetJoystick() {
	return stick;
}
