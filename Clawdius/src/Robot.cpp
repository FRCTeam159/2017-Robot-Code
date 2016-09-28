/* ----- Begin Official Style Guide -----
 *  Make spaces
 *  Put brackets on their own line
 *  Always include brackets, even if there's only one line of code
 *  Indent with tabs
 *  Use camelCase
 *  Use <object>-> instead of (*<object>).
 *  Never pass by reference (make pointers instead if you need them)
 */
#include "WPILib.h"
#define PI 3.14159265358579
#define COUNTSPERCYCLE 900
#define CIRCUMFERENCE 5.8333*PI
#define TICKDISTANCE CIRCUMFERENCE/COUNTSPERCYCLE
#define TICKSPERLEVEL 4000
#define SRXP 0.85
#define SRXI 0.004
#define SRXD 0.0192

class Robot: public IterativeRobot
{
private:
	LiveWindow *lw;
//	Talon *frontLeft;
//	Talon *frontRight;
//	Talon *backLeft;
//	Talon *backRight;
	CANTalon *liftMotor;
	CANTalon *frontLeft;
	CANTalon *frontRight;
	CANTalon *backLeft;
	CANTalon *backRight;
	Talon *leftRollers;
	Talon *rightRollers;
	Joystick *stick;
	Joystick *stick2;
	Gyro *gyro;
	DoubleSolenoid *claw;
	Relay *lights;
	bool *clawInput;
	bool *clawOutput;
	int desiredLevel;
	bool *pLiftInputUp;
	bool *pLiftOutputUp;
	bool *pLiftInputDown;
	Timer t;
	bool *pLiftOutputDown;
	float *totalError, *pError;
	float *liftTotalError, *liftPError;
	float *PRL, *PFR, *PRR, *PFL;  // P is for previous, F/R is for front/rear, L/R is for left/right
	float xDistance;
	float yDistance;
	float timestep;
	float xVelocity, yVelocity;
	bool *pInputLights, *pOutputLights;
	bool *pInputSpeed, *pOutputSpeed;  //Toggle pointers for adjusting the speed of the bot.
	bool *pInputLock, *pOutputLock;  //Toggle pointers for locking the robot's position on the field.
	int setValue = 0;
	int stepNumber;
	int speedMultiplier;  //Constant used to adjust the speed of movement.
	float gyroConstant = 0.0;

	void RobotInit()
	{
		lw = LiveWindow::GetInstance();
		frontLeft = new CANTalon(1);
		frontRight = new CANTalon(6);
		backLeft = new CANTalon(2);
		backRight = new CANTalon(4);
		liftMotor = new CANTalon(3);
		configTalon(liftMotor, false, .35, 0.00025, 0.01);//works fine (but way too fast) with .5 < p <.75
//		liftMotor->ConfigFwdLimitSwitchNormallyOpen(false);
//		liftMotor->ConfigRevLimitSwitchNormallyOpen(false);
		liftMotor->ConfigNeutralMode(CANSpeedController::NeutralMode::kNeutralMode_Brake);
//		liftMotor->ConfigLimitMode(CANSpeedController::kLimitMode_SwitchInputsOnly);
//		liftMotor->ConfigForwardLimit(20000);
//		liftMotor->ConfigReverseLimit(0);
		leftRollers= new Talon(0);
		rightRollers= new Talon (1);
		gyro = new Gyro(0);
		claw = new DoubleSolenoid(0, 2, 3);
		stick = new Joystick(0);
		stick2 = new Joystick(1);
		lights = new Relay(0);
		//start toggle pointers
		clawInput = new bool;
		*clawInput = false;
		clawOutput = new bool;
		*clawOutput = false;
		pLiftInputUp = new bool;
		*pLiftInputUp = false;
		pLiftOutputUp = new bool;
		*pLiftOutputUp = false;
		pLiftInputDown = new bool;
		*pLiftInputDown = false;
		pLiftOutputDown = new bool;
		*pLiftOutputDown = false;
		pInputLights = new bool;
		*pInputLights = false;
		pOutputLights = new bool;
		*pOutputLights = false;
		pInputSpeed = new bool;
		*pInputSpeed = false;
		pOutputSpeed = new bool;
		*pOutputSpeed = false;
		pOutputLock = new bool;
		*pOutputLock = false;
		pInputLock = new bool;
		*pInputLock = false;
		//end toggle pointers
		desiredLevel = 0;
		totalError = new float;
		liftTotalError = new float;
		liftPError = new float;
		pError = new float;
		PRL = new float;
		PRR = new float;
		PFL = new float;
		PFR = new float;
		*liftTotalError = 0;
		gyro->Reset();
		*totalError = 0;
		*pError = 0;
		(*PFL) = 0;
		(*PFR) = 0;
		(*PRL) = 0;
		(*PRR) = 0;
		xDistance = 0.0;
		yDistance = 0.0;
		timestep = 0.01;
		xVelocity = 0;
		yVelocity = 0;
	}

	void AutonomousInit()
	{
		stepNumber = 0;
		configTalon(frontLeft, false, SRXP * 10, 0, 0);
		configTalon(frontRight, false, SRXP * 10, 0, 0);
		configTalon(backLeft, false, SRXP * 10, 0, 0);
		configTalon(backRight, false, SRXP * 10, 0, 0);
		frontRight->SetPosition(0);
		frontLeft->SetPosition(0);
		backRight->SetPosition(0);
		backLeft->SetPosition(0);
		liftMotor->SetPosition(0);
		gyro->Reset();
		gyroConstant = 0.0;
		claw->Set(DoubleSolenoid::kReverse);
	}

	void AutonomousPeriodic()
	{
		static int stepNumber=1;

		if (stepNumber == 1)
				{
					static int increment = 0;
					/*Movement has been divided up into small duty cycles.
					  This maintains accuracy as the robot moves across the field.*/
					if(increment < 150)
					{
						backLeft->SetPosition(0);
						backRight->SetPosition(0);
						frontLeft->SetPosition(0);
						frontRight->SetPosition(0);

						backLeft->Set(70);
						backRight->Set(-70);
						frontLeft->Set(70);
						frontRight->Set(-70);

						increment++;
						printf("%i = increment", increment);
					}
					else if(increment>=150)
					{
						stepNumber=13;
					}
				}
//		else if (stepNumber==2)
//		{
//			backLeft->SetPosition(0);
//			backRight->SetPosition(0);
//			frontLeft->SetPosition(0);
//			frontRight->SetPosition(0);
//			backLeft->Set(0);
//			backRight->Set(0);
//			frontLeft->Set(0);
//			frontRight->Set(0);
//		}

//		//Step 0: Grab tote #1
//		if (stepNumber == 0)
//		{
//			printf("%i = stepNumber \n", stepNumber);
//
//			claw->Set(DoubleSolenoid::kReverse);  //Close claw.
//			stepNumber = 1;
//			Wait(0.25);
//		}
//		//Step 1: Lift tote stack above bin
//		else if (stepNumber == 1)
//		{
//			printf("%i = stepNumber \n", stepNumber);
//
//			liftMotor->Set(TICKSPERLEVEL * 3);
//			Wait(0.5);
//			stepNumber = 2;
//		}
//		//Step 2: Move to the right and lower arms on the way
//		else if (stepNumber == 2)
//		{
//			static int increment = 0;
//			/*Movement has been divided up into small duty cycles.
//			  This maintains accuracy as the robot moves across the field.*/
//			if(increment < 430)
//			{
//				backLeft->SetPosition(0);
//				backRight->SetPosition(0);
//				frontLeft->SetPosition(0);
//				frontRight->SetPosition(0);
//
//				backLeft->Set(-70);
//				backRight->Set(70);
//				frontLeft->Set(-70);
//				frontRight->Set(70);
//
//				increment++;
//				printf("%i = increment", increment);
//			}
//			//When the bot is approximately halfway to the next tote, lower the arms.
//			if (increment == 200)
//			{
//				liftMotor->Set(TICKSPERLEVEL * 1);
//			}
//			else if (increment > 429)
//			{
//				stepNumber = 3;
//			}
//		}
//		//Step #3: Release tote stack
//		else if (stepNumber == 3)
//		{
//			claw->Set(DoubleSolenoid::kForward);
//			Wait(.25);
//			stepNumber = 4;
//		}
//		//Step #4: Put arms in grabbing position
//		else if (stepNumber == 4)
//		{
//			liftMotor->Set(TICKSPERLEVEL * 0);
//			Wait(.25);
//			stepNumber = 5;
//		}
//		//Step #5: Grab tote
//		else if (stepNumber == 5)
//		{
//			claw->Set(DoubleSolenoid::kReverse);
//			Wait(.25);
//			stepNumber = 6;
//		}
//		//Step #6: Raise tote stack above bin
//		else if (stepNumber == 6)
//		{
//			liftMotor->Set(TICKSPERLEVEL * 3);
//			Wait(0.5);
//			stepNumber = 7;
//		}
//		//Step #7: Move to the right and lower arms on the way
//		else if (stepNumber == 7)
//		{
//			static int increment = 0;
//
//			if(increment < 430)
//			{
//				backLeft->SetPosition(0);
//				backRight->SetPosition(0);
//				frontLeft->SetPosition(0);
//				frontRight->SetPosition(0);
//
//				backLeft->Set(-70);
//				backRight->Set(70);
//				frontLeft->Set(-70);
//				frontRight->Set(70);
//
//				increment++;
//				printf("%i = increment", increment);
//			}
//			if (increment == 200)
//			{
//				liftMotor->Set(TICKSPERLEVEL * 1);
//			}
//			else if (increment > 429)
//			{
//				stepNumber = 8;
//			}
//		}
//		//Step #8: Release tote stack
//		else if (stepNumber == 8)
//		{
//			claw->Set(DoubleSolenoid::kForward);
//			Wait(.25);
//			stepNumber = 9;
//		}
//		//Step #9: Put arms in grabbing position
//		else if (stepNumber == 9)
//		{
//			liftMotor->Set(TICKSPERLEVEL * 0);
//			Wait(.25);
//			stepNumber = 10;
//		}
//		//Step #10: Grab tote
//		else if (stepNumber == 10)
//		{
//			claw->Set(DoubleSolenoid::kReverse);
//			Wait(.25);
//			stepNumber = 11;
//		}
//		//Step #11: Raise tote stack above bin
//		else if (stepNumber == 11)
//		{
//			liftMotor->Set(TICKSPERLEVEL * 3);
//			Wait(0.5);
//			stepNumber = 12;
//		}
//		//Step #12: Move backwards into the auto zone
//		else if (stepNumber == 12)
//		{
//			static int increment = 0;
//
//			if(increment < 225)
//			{
//				backLeft->SetPosition(0);
//				backRight->SetPosition(0);
//				frontLeft->SetPosition(0);
//				frontRight->SetPosition(0);
//
//				backLeft->Set(-70);
//				backRight->Set(-70);
//				frontLeft->Set(70);
//				frontRight->Set(70);
//
//				increment++;
//				printf("%i = increment", increment);
//			}
//			else if (increment > 224)
//			{
//				stepNumber = 13;
//			}
//		}
		//Step #13: Rotate 90 degrees so the robot and tote stack fits in the auto zone
		else if (stepNumber == 13)
		{
			printf("%i = stepNumber \n", stepNumber);

			configTalon(frontLeft, true, SRXP, SRXI, SRXD);
			configTalon(frontRight, true, SRXP, SRXI, SRXD);
			configTalon(backLeft, true, SRXP, SRXI, SRXD);
			configTalon(backRight, true, SRXP, SRXI, SRXD);

			//Put negative
			backLeft->Set(625);
			backRight->Set(625);
			frontLeft->Set(625);
			frontRight->Set(625);
			claw->Set(DoubleSolenoid::kReverse);  //Close claw.
			Wait(1.5);

			stepNumber = 14;
		}
		//Step #14: Release tote stack
		else if (stepNumber == 14)
		{
			claw->Set(DoubleSolenoid::kForward);  //Open claw.
			stepNumber = 15;
		}
		//If all steps have been completed, wait	` for autonomous period to end.
		else
		{
			mecDrive(0, 0, 0);
			Wait(.02);
		}
	}

	void TeleopInit()
	{
		configTalon(frontLeft, true, SRXP * 1.0, SRXI, SRXD);
		configTalon(frontRight, true, SRXP * 1.0, SRXI, SRXD);
		configTalon(backLeft, true, SRXP, SRXI, SRXD);
		configTalon(backRight, true, SRXP, SRXI, SRXD);
		t.Reset();
		t.Start();
		claw->Set(DoubleSolenoid::kReverse);
		liftMotor->SetPosition(10);
		setValue = 10;
		liftMotor->ClearIaccum();
		desiredLevel = 0;
		backLeft->SetPosition(0);
		backRight->SetPosition(0);
		frontLeft->SetPosition(0);
		frontRight->SetPosition(0);
		speedMultiplier = 0.25;
		gyro->Reset();
	}
	// Speed modifier set to 0.5 from 1.0
    // add toggle between reverse and off for relay 0, for lights
	void TeleopPeriodic()
	{
		float angle = 0;
		float xAxis = quadDeadBand(stick->GetX(), .2,.05) * -1.0;
		float yAxis = quadDeadBand(stick->GetY(), .2,.05) * -1.0;
		float zAxis = quadDeadBand(stick->GetZ(), .25, 0.3);  //joystick typically twists by about .3 while moving backwards
		if (xAxis == 0 && yAxis == 0)
		{
			backRight->ClearIaccum();
			backLeft->ClearIaccum();
			frontLeft->ClearIaccum();
			frontRight->ClearIaccum();
		}
		angle = properArcTan(xAxis, yAxis);
		float power = sqrt((xAxis * xAxis) + (yAxis * yAxis));

		if (power > 1)
		{
			power = 1;
		}
		mecDrive(angle, power, zAxis);
		if (toggle(stick->GetRawButton(7), pOutputLights, pInputLights))
		{
			lights->Set(Relay::kReverse);
		}
		else
		{
			lights->Set(Relay::kOff);
		}

		bool trigger = stick->GetRawButton(1);
		bool clawTrigger = toggle(trigger, clawOutput, clawInput);

		if (clawTrigger)  //If the clawTrigger button is pressed, close the pincers.
		{
			claw->Set(DoubleSolenoid::kReverse);  //Close claw.
		}
		else  //Otherwise, keep claws open.
		{
			claw->Set(DoubleSolenoid::kForward);  //Open claw.
		}

		int currentMotorPos = liftMotor->GetEncPosition();

		//If button 5 pressed and desiredLevel < 6, go up.
		if (toggle(stick->GetRawButton(5), pLiftOutputUp, pLiftInputUp) && desiredLevel < 7)
		{
			desiredLevel += 1;
			setValue = (TICKSPERLEVEL * desiredLevel) + 10;
			(*pLiftOutputUp) = false;
			liftMotor->ClearIaccum();
		}
		//If button 3 pressed and desiredLevel > 0, go down.
		else if (toggle(stick->GetRawButton(3), pLiftOutputDown, pLiftInputDown) && desiredLevel > -7)
		{
			desiredLevel -= 1;
			setValue = (TICKSPERLEVEL * desiredLevel) + 10;
			(*pLiftOutputDown) = false;
			liftMotor->ClearIaccum();
		}
		else if (stick->GetRawButton(4))
		{
			setValue -= 200;
		}
		else if(stick->GetRawButton(6))
		{
			setValue += 300;
		}
		liftMotor->Set(setValue);
		float currentError = liftMotor->GetClosedLoopError();
//		printf("LifterPosition=%i, desired=%i, error=%f \n", currentMotorPos, setValue, currentError);
	}

	void TestPeriodic()
	{
		lw->Run();
	}

	/** @params theta = angle of joystick in radians, power = scalar power of joystick, rotation = rotation of joystick
	 * We also have a gyro in here to make sure that our driving is field-centric rather than robot-centric.  We turn
	 * down the rotation as well.
	 */
	void mecDrive(float theta, float power, float rotation)
	{
		speedMultiplier = 1.0;  //Slow down the robot so obstinate drivers don't run at full speed on asphalt even though we tell them not to.
		float p = 0;  //gyroscope
//		float p = gyro->GetAngle();
//		printf("angle = %f\n", p);
//		float modTheta = properArcTan(cos(theta - p) * 1.2, sin(theta - p));
		p *= (PI / 180.0);  //Since the gyro gives values in degrees, we convert it to radians here so the math works out.
		float off = (gyro->GetRate() * PI / 180) - (rotation * 2.28);  //error in rotation in radians/sec
//		rotation = PIDLoop(.4, totalError, 0.000, 0, pError, off) * -1.0;//re-invert
		float diagonalA = ((1 / sqrt(2)) * (sin(theta - p) + cos(theta - p)));
		float diagonalB = ((1 / sqrt(2)) * (sin(theta - p) - cos(theta - p)));
		rotation *= .4;
		float scalar;  //intended to maximize motor output, while maintaining the same angle

		if (fabs(diagonalA) + fabs(rotation) >= fabs(diagonalB) + fabs(rotation))
		{
			scalar = (1 / (fabs(diagonalA) + fabs(rotation)));
		}
		else if(fabs(diagonalA) + fabs(rotation) == 0 && fabs(diagonalB) + fabs(rotation) == 0)
		{
			scalar = 0;
		}
		else
		{
			scalar = (1 / (fabs(diagonalB) + fabs(rotation)));
		}

		//Set motor outputs by first taking diagonalA/B input and multiplying by power, then adding or subtracting rotation.
		//After that we multiply by both the scalar input and .8, and then ramp the whole thing.
		float backRightInput = ramp(((diagonalB * power) - rotation) * scalar * -2500, PRR);
		float backLeftInput = ramp(((diagonalA * power) + rotation) * scalar * 2500, PRL);
		float frontRightInput = ramp(((diagonalA * power) - rotation) * scalar * -2500, PFR);
		float frontLeftInput = ramp(((diagonalB * power) + rotation) * scalar * 2500, PFL);

//		printf("%f = backLeftInput %f = backRightInput %f = frontRightInput %f = frontLeftInput \n", backLeftInput, backRightInput, frontRightInput, frontLeftInput);

		frontLeft->Set(frontLeftInput * speedMultiplier);
		frontRight->Set(frontRightInput * speedMultiplier);
		backLeft->Set(backLeftInput * speedMultiplier);
		backRight->Set(backRightInput * speedMultiplier);

//		int backLeftPos = backLeft->GetEncPosition();
//		int backRightPos = backRight->GetEncPosition();
//		int frontRightPos = frontRight->GetEncPosition();
//		int frontLeftPos = frontLeft->GetEncPosition();
//
//		int backLeftVel = backLeft->GetEncVel();
//		int backRightVel=backRight->GetEncVel();
//		int frontLeftVel=frontLeft->GetEncVel();
//		int frontRightVel=frontRight->GetEncVel();
//		printf("%i = backLeftVelocity %i = backRightVelocity %i = frontRightVelocity %i = frontLeftVelocity \n", backLeftVel, backRightVel, frontRightVel, frontLeftVel);
	}

	/** @params input = button input, loutput = last output, pinput = previous input
	 * loutput and pinput must both be false initially if you want toggle to return false when the program begins.
	 * You need two different pointers for loutput and pinput each time.
	 */
	bool toggle(bool input, bool* loutput, bool* pinput)
	{
		if (input && !(*pinput))
		{
			(*loutput) = !(*loutput);
			(*pinput) = input;
			return (*loutput);
		}
		else
		{
			(*pinput) = input;
			return (*loutput);
		}

	}

	/** @params input = joystick input, ignore = minimum allowed value
	 * The deadband trims all input values below the ignore value by taking the fabs value
	 * of the input and comparing it to ignore, so make sure ignore is a positive float.
	 */
	float deadband(float input, float ignore)
	{
		if (fabs(input) < ignore)
		{
			return 0;
		}
		else
		{
			return input;
		}
	}

	/** @params float proporational, float *totError, float integral, float derivative, float *pError, float off */
	float PIDLoop(float proportional, float *totError, float integral, float derivative, float *pError, float off)
	{
		float dError = (off - (*pError)) / timestep;
		(*totError) += off;
//		printf("totError=%f   off=%f \n", (*totError), off);
		return (proportional * off) + (integral * (*totError) + (derivative * dError));
	}

	float ramp(float input, float* previous)  // Ramping algorithm to reduce acceleration and protect gearboxes.
	{
		float toReturn = (0.6 * input) + (0.4 * (*previous));
		(*previous) = input;
		return toReturn;
	}

	float properArcTan(float xAxis, float yAxis)
	{
		float theta;
		if (xAxis != 0)
		{
			if(xAxis < 0)
			{
				theta = PI + atan(yAxis / xAxis);
			}
			if (xAxis > 0)
			{
				theta = atan(yAxis / xAxis);
			}
		}
		else
		{
			if (yAxis > 0)
			{
				theta = PI / 2;
			}
			else if (yAxis < 0)
			{
				theta = -PI / 2;
			}
			else
			{
				theta = 0;
			}
		}
		return theta;
	}

	/** @params This function gets our position using encoders. */
	void iteratePosition()
	{
		timestep = t.Get();
		t.Stop();
		t.Reset();
		t.Start();
		float relAngle = 0; // gyro->GetAngle() * PI / 180.0;
		xVelocity = ((frontLeft->GetEncVel() * cos((PI / 4) + relAngle)) + (backRight->GetEncVel() * cos((PI / 4) + relAngle))
			+ (backLeft->GetEncVel() * cos((3 * PI / 4) + relAngle)) + (frontRight->GetEncVel() * cos((3 * PI / 4) + relAngle)));
		yVelocity = ((frontLeft->GetEncVel() * sin((PI / 4) + relAngle)) + (backRight->GetEncVel() * sin((PI / 4) + relAngle))
			+ (backLeft->GetEncVel() * sin((3 * PI / 4) + relAngle)) + (frontRight->GetEncVel() * sin((3 * PI / 4) + relAngle)));
		xDistance += xVelocity * timestep;
		yDistance += yVelocity * timestep;
		int BRcount = backRight->GetEncPosition();
		int FRcount = frontRight->GetEncPosition();
		int BLcount = backLeft->GetEncPosition();
		int FLcount = frontLeft->GetEncPosition();
		printf("xVelocity=%f   yVelocity=%f \n", xVelocity, yVelocity);
		printf("xDistance=%f   yDistance=%f   BRcount=%i  FRcount=%i  BLcount=%i  FLcount=%i\n", xDistance, yDistance, BRcount, FRcount, BLcount, FLcount);
		if (stick->GetRawButton(8))
		{
			xVelocity = 0;
			yVelocity = 0;
			xDistance = 0;
			yDistance = 0;
		}
	}
	void configTalon(CANTalon *motor, bool vel, float P, float I, float D)
	{
		motor->SetPID(P,I,D);
		if (vel)
		{
			motor->SetControlMode(CANTalon::ControlMode::kSpeed);
		}
		else
		{
			motor->SetControlMode(CANTalon::ControlMode::kPosition);
		}
		motor->EnableControl();
	}
	float quadDeadBand(float input, float deadband, float minOutput)
	{
		float y = 0;
		if (fabs(input) > deadband)
		{
			if (input > 0)
			{
				y = (((1 - minOutput) * pow(input - deadband, 2)) / pow(1 - deadband, 2)) + minOutput;
			}
			else
			{
				y = (((-1 + minOutput) * pow(input + deadband, 2)) / pow(-1 + deadband, 2)) - minOutput;
			}
		}
		return y;
	}

};


START_ROBOT_CLASS(Robot);
