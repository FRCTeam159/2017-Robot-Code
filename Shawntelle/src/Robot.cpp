#include "WPILib.h"

class Robot: public IterativeRobot
{
private:
	float *timer;

	LiveWindow *lw;
	float *routput;
	float *loutput;

	bool *wheelOutput;
	bool *wheelPInput;
	Victor *wheelMotor;
	Victor *angleMotor;
	Talon *testMotor;
	Joystick *stick;

	Talon *FrontLeft;
	Talon *FrontRight;
	Talon *BackLeft;

	Talon *BackRight;
	RobotDrive *drive;

	DoubleSolenoid *shooterPneumatic;


	void RobotInit()

	{
		stick = new Joystick(1);
		wheelMotor = new Victor(6);
		angleMotor = new Victor(5);
		shooterPneumatic = new DoubleSolenoid(0,0,1);

		//These are the right motor configurations. Do not trust the writing on the tape!!!
		FrontLeft = new Talon(4);
		FrontRight = new Talon(1);
		BackLeft = new Talon(5);
		BackRight = new Talon(2);
		drive = new RobotDrive((*FrontLeft),(*FrontRight),(*BackLeft),(*BackRight));
		drive->SetInvertedMotor(RobotDrive::kFrontLeftMotor,false);
		drive->SetInvertedMotor(RobotDrive::kFrontRightMotor,true);
		drive->SetInvertedMotor(RobotDrive::kRearRightMotor,true);
		drive->SetInvertedMotor(RobotDrive::kRearLeftMotor,false);
		//These are the right motor configurations. Do not trust the writing on the tape!!!

		routput=new float;
		(*routput) = 0;
		loutput=new float;
		(*loutput)=0;
		lw = LiveWindow::GetInstance();
	}

	void AutonomousInit()
	{
		timer=new float;
		(*timer) = 0;
	}

	void AutonomousPeriodic()
	{
		/*if((*timer)<6){
		drive->TankDrive(.5, .5, false);  // Move forward for
		Wait(2.0);
		(*timer)+=1;// 2 seconds
		}
		else drive->TankDrive(0.0, 0.0, false);*/
	}

	void TeleopInit()
	{
		wheelPInput = new bool;
		(*wheelPInput) = false;
		wheelOutput = new bool;
		(*wheelOutput) = false;
	}

	void TeleopPeriodic()
	{
		customArcade();

		if (toggle(stick->GetRawButton(2), wheelOutput, wheelPInput))
			testMotor->SetSpeed(-1);
		else
			testMotor->SetSpeed(0);

		if(stick->GetRawButton(1))
			shooterPneumatic->Set(DoubleSolenoid::kReverse);
		else
			shooterPneumatic->Set(DoubleSolenoid::kForward);

		if(stick->GetRawButton(5))
		{
			angleMotor->SetSpeed(.25);
		}
		if(stick->GetRawButton(6))
		{
			angleMotor->SetSpeed(-.25);
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
		if ( abs(input) > 0.05 )
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
		float yAxis = -1 * ((*stick).GetY());
		float xAxis = (*stick).GetX();
		float zAxis = (*stick).GetZ();
		// Run axis values through deadband
		yAxis = quadDeadband(.6 , .3 , yAxis);
		xAxis = quadDeadband(.6 , .3 , xAxis);
		zAxis = quadDeadband(.6 , .3 , zAxis);

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
					right = (abs(yAxis) / yAxis) * (abs(yAxis) - abs(xAxis));
				}

				else
				{
					right=yAxis;
					left = (abs(yAxis) / yAxis) * (abs(yAxis) - abs(xAxis));
				}
			}

			else
			{
				left = yAxis;
				right = yAxis;
			}
		}

		// Ramp values up
		left = ramping(left, loutput);
		right = ramping(right, routput);

		// Make sure values are between -1 and 1
		left = coerce(-1,1,left);
		right = coerce(-1,1,right);
		drive->TankDrive(left, right, false);  // Output left and right power values to motors
	}
};

START_ROBOT_CLASS(Robot);
