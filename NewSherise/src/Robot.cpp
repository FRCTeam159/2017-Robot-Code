#include "WPILib.h"
#include "Toggler.h"
#define FLIPPER_SPEED .5
#define FLIPPER_UP_BUTTON 5
#define FLIPPER_DOWN_BUTTON 6
#define FLIPPER_UP_SPEED .2
#define FLIPPER_DOWN_SPEED -.2
#define SHOOTER_UP_SPEED 1
#define SHOOTER_DOWN_SPEED -1
#define SHOOTER_BUTTON 3
#define INTAKE_BUTTON 4

class Robot: public IterativeRobot
{
private:
	LiveWindow *lw;
	Joystick *stick;
	Talon *testMotor;
	Talon *leftMotor;
	Talon *rightMotor;
	Talon *intakeMotor;
	Talon *shootMotor;
	Talon *leftrearMotor;
	Talon *rightrearMotor;
	Toggler *intakeToggle;
	Talon *Flipper;

	void RobotInit()

	{
		stick = new Joystick(0);
		lw = LiveWindow::GetInstance();
		leftrearMotor= new Talon(2);
		Flipper = new Talon (6);
		leftMotor= new Talon(5);
		rightMotor= new Talon(1);
		rightrearMotor = new Talon(0);
		intakeMotor=new Talon(3);
		shootMotor= new Talon(4);
		intakeToggle=new Toggler();
	}

	void AutonomousInit()
	{
	}

	void AutonomousPeriodic()
	{

	}

	void TeleopInit()
	{

	}

	void TeleopPeriodic()
	{
		/*intakeToggle->Toggle(stick->GetRawButton(2));
		if(intakeToggle->previousOutput)
		{
			intakeMotor->Set(1);
		}
		else
		{
			intakeMotor->Set(0);
		}*/
		customArcade();
		if(stick->GetRawButton(SHOOTER_BUTTON))
		{
			intakeMotor->Set(SHOOTER_UP_SPEED);
		}
		else
		{
			intakeMotor->Set(0);
		}
		if(stick->GetRawButton(INTAKE_BUTTON))
		{
			shootMotor->Set(1);
			printf ("Shooter down button pressed\n");
		}
		else
		{
			shootMotor->Set(0);
		}
		if(stick->GetRawButton(FLIPPER_UP_BUTTON))
		{
			Flipper->Set(FLIPPER_UP_SPEED);
			printf ("Flipper up button pressed\n");
		}

		else if(stick->GetRawButton(FLIPPER_DOWN_BUTTON))
		{
			Flipper->Set(FLIPPER_DOWN_SPEED);
			printf ("Flipper down button pressed\n");
		}
		else
		{
			Flipper->Set(0);
		}
	}

	void TestPeriodic()
	{
		lw->Run();
	}

	float quadDeadband(float minThreshold, float minOutput, float input)
	{
		if (input > minThreshold)
		{
			return (((( 1 - minOutput)  / ((1 - minThreshold) * (1 - minThreshold))) *
					((input - minThreshold) * (input - minThreshold))) + minOutput);
		}
		else
		{
			if ( input < ( -1 * minThreshold ) )
			{
				return (((minOutput - 1) / ((minThreshold - 1) * (minThreshold - 1))) *
						((minThreshold + input) * (minThreshold + input) ) ) - minOutput;
			}

			else
			{
				return 0;
			}
		}
	}

	float ramping(float input, float* output)
	{
		if ( fabs(input) > 0.05 )
		{
			(*output) = ( ( .4 * (*output) ) + ( .6 * input ) );
			return (*output);
		}

		else
		{
			return 0;
		}
	}

	float coerce(float min, float max, float x)
	{
		if (x < min)
		{
			x = min;
		}

		else if (x > max)
		{
			x = max;
		}

		return x;
	}

	bool toggle(bool input, bool* loutput, bool* pinput)
	{
		if ( ( input ^ (*pinput) ) && input )
		{
			(*loutput) = !(*loutput);
		}

		(*pinput) = input;
		return (*loutput);
	}

	void customArcade()
	{
		float left=0;
		float right=0;  // Left and right motor power values

		// Get axis values
		float yAxis = -0.75* ((*stick).GetY());
		float xAxis = 0.75* ((*stick).GetX());
		float zAxis = .75*(*stick).GetZ();
		// Run axis values through deadband
		yAxis = quadDeadband(.4 , .3 , yAxis);
		xAxis = quadDeadband(.4 , .3 , xAxis);
		zAxis = quadDeadband(.4 , .3 , zAxis);

		// Turning logic
		if (zAxis != 0)
		{
			left = zAxis;
			right = -zAxis;
		}

		else
		{
			if (xAxis != 0)
			{
				if (xAxis > 0)
				{
					left = yAxis;
					right = (fabs(yAxis) / yAxis) * (fabs(yAxis) - fabs(xAxis));
				}

				else
				{
					right=yAxis;
					left = (fabs(yAxis) / yAxis) * (fabs(yAxis) - fabs(xAxis));
				}
			}

			else
			{
				left = yAxis;
				right = yAxis;
			}
		}

		// Ramp values up

		// Make sure values are between -1 and 1
		left = coerce(-1,1,left);
		right = coerce(-1,1,right);
		leftMotor->Set(left);
		leftrearMotor->Set(left);
		rightMotor->Set(-1*right);
		rightrearMotor->Set(-1*right);
	}
	void setFlipper (double value){

	}
};

START_ROBOT_CLASS(Robot);
