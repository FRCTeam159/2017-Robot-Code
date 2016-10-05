#include "WPILib.h"
//
// ExampleDiscusBot
// - bare bones mentor example robot code
// features and requirements:
// 1) drive drain
//   - type: mechanum drive
//   - implementation
//     * uses built-in wpi RobotDrive function
//     * no embellishments (deadband etc.)
// 2) elevator-lifter
//   - vertical elevator (adapted from Clawdius robot)
//   - modifications
//     * no encoders or limit switches
//     * simple up-down control using separate buttons with fixed power values(hold to activate)
//     * uses similar code to "flapper" control in "newSherise" project
// 3) frisbe shooter
//   - shooter adapted from "Shawntelle" robot
//   - implementation
//      * a function to turn on flywheels (toggles on/off)
//      * a function to fire frisbe (activate piston)

#define FRONT_LEFT_MOTOR 1
#define FRONT_RIGHT_MOTOR 2
#define BACK_LEFT_MOTOR 3
#define BACK_RIGHT_MOTOR 4
#define LIFTER_MOTOR 5
#define FLYWHEEL_MOTOR 6

#define LIFTER_UP_BUTTON 5
#define LIFTER_DOWN_BUTTON 6
#define FLYWHEEL_BUTTON 2
#define SHOOTER_BUTTON 1

#define LIFTER_UP_SPEED .2
#define LIFTER_DOWN_SPEED -.2

class Robot: public IterativeRobot
{
private:
	CANTalon *frontLeft;
	CANTalon *frontRight;
	CANTalon *backLeft;
	CANTalon *backRight;
	CANTalon *lifter;
	CANTalon *flywheel;

	RobotDrive *drive;
	Joystick *stick;
	DoubleSolenoid *shooterPneumatic;

public:
	void RobotInit()
	{
		frontLeft = new CANTalon(FRONT_LEFT_MOTOR);
		frontRight = new CANTalon(FRONT_RIGHT_MOTOR);
		backLeft = new CANTalon(BACK_LEFT_MOTOR);
		backRight = new CANTalon(BACK_RIGHT_MOTOR);
		lifter = new CANTalon(LIFTER_MOTOR);
		flywheel = new CANTalon(FLYWHEEL_MOTOR);

		drive=new RobotDrive(frontLeft,frontRight,backLeft,backRight);

		stick = new Joystick(1);

		shooterPneumatic = new DoubleSolenoid(0,0,1);

	}

	void AutonomousInit()
	{
		std::cout<<"Autonomous mode not implemented"<<std::endl;
	}

	void AutonomousPeriodic()
	{
	}

	void TeleopInit()
	{
		std::cout<<"Starting Teleop"<<std::endl;
	}

	void TeleopPeriodic()
	{
		driveWithJoystick();
		checkLifter();
		checkShooter();
	}

	void TestPeriodic()
	{
	}

	// driveWithJoystick()
	// - mechanem drive with joystick
	// - using built-in wpi RobotDrive function
	void driveWithJoystick(){
		double x=0,y=0,z=0,g=0;
		x=-stick->GetX();
		y=-stick->GetY();
		z=-stick->GetZ();
		drive->MecanumDrive_Cartesian(x, y, z, g);
	}
	// checkLifter()
	// - monitor lifter up and down buttons
	void checkLifter(){
		if (stick->GetRawButton(LIFTER_UP_BUTTON))
			lifter->Set(LIFTER_UP_SPEED);
		else if (stick->GetRawButton(LIFTER_DOWN_BUTTON))
			lifter->Set(LIFTER_DOWN_SPEED);
		else
			lifter->Set(0);
	}
	// checkShooter()
	// - monitor shooter buttons
	void checkShooter(){
		// toggle flywheel on/off
		static bool wheelOutput=false;
		static bool wheelPInput=false;
		if (toggle(stick->GetRawButton(FLYWHEEL_BUTTON), &wheelOutput, &wheelPInput))
			flywheel->Set(-1);
		else
			flywheel->Set(0);
		// fire frisbe if shooter button is pressed
		if(stick->GetRawButton(SHOOTER_BUTTON))
			shooterPneumatic->Set(DoubleSolenoid::kReverse);
		else
			shooterPneumatic->Set(DoubleSolenoid::kForward);
	}
	// utility function to toggle the state of a button
	bool toggle(bool input, bool* loutput, bool* pinput)
	{
		if ((input ^ (*pinput)) && input)
			(*loutput) = !(*loutput);
		(*pinput) = input;
		return (*loutput);
	}
};

START_ROBOT_CLASS(Robot)
