#include "TurnToAngle.h"


#define P 0.01
#define I 0.0001
#define D 0.0

#define WIDTH 26 // horizontal distance between wheels (side-to-side)
#define LENGTH 8.5 // vertical distance between center wheels only (i.e the wheels with encoders)
#define USE_GYRO

TurnToAngle::TurnToAngle(double a) : CommandBase("DriveStraight"),
	pid(P,I,D,this,this)
{
	Requires(driveTrain.get());
	angle = a;
	// radius of travel circle = 1/2 diagonal of rectangle containing center wheels
  	radius = 0.5*sqrt(WIDTH*WIDTH+LENGTH*LENGTH);
	std::cout << "new TurnToAngle("<<a<<")"<< std::endl;
}

// Called just before this Command runs the first time
void TurnToAngle::Initialize() {
	//double d = driveTrain->GetDistance();
#ifdef USE_GYRO
	double a=angle;
#else
	double d = driveTrain->GetDistance();
	driveTrain->Reset();
	driveTrain->SetDistance(d);
	double a = angle*2.0*M_PI*radius/360;
#endif
	driveTrain->Reset();
	//driveTrain->SetDistance(d);
  	pid.Reset();
  	// arc length (wheel travel distance) given radius and turn angle
  	pid.SetSetpoint(a);
	pid.SetAbsoluteTolerance(5);
	pid.Enable();
	driveTrain->Enable();
	std::cout << "TurnToAngle Started: "<< a <<std::endl;
}

// Called repeatedly when this Command is scheduled to run
void TurnToAngle::Execute() {
   // nothing to do: action is in PID control loop
}

// Make this return true when this Command no longer needs to run execute()
bool TurnToAngle::IsFinished() {
	return pid.OnTarget();
}

// Called once after isFinished returns true
void TurnToAngle::End() {
	pid.Disable();
    std::cout << "TurnToAngle End" << std::endl;
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void TurnToAngle::Interrupted() {
	End();
}
double TurnToAngle::PIDGet() {
#ifdef USE_GYRO
	double a=driveTrain->GetHeading();
#ifdef DEBUG_COMMAND
	std::cout << "TurnToAngle::PIDGet:" << a << std::endl;
#endif

	return a;
#else
	double l = driveTrain->GetLeftDistance();
	double r = driveTrain->GetRightDistance();
	double d = 0.5 * (fabs(l) + fabs(r)); // average wheel travel distance

	d = angle < 0 ? -d : d;
	double a = d * 360 / M_PI / radius / 2;

	driveTrain->SetAngle(a);
#ifdef DEBUG_COMMAND
	std::cout << "TurnToAngle::PIDGet:" << l << "," << r << ","<<d<<","<<a << std::endl;
#endif
	return d;
#endif
}

#define MAXSPEED 0.5
void TurnToAngle::PIDWrite(double a) {

    std::cout << "TurnToAngle::PIDWrite("<<a<<")"<< std::endl;
    if (fabs(a) > MAXSPEED) {
    	if (a>0) 	a = MAXSPEED;
    	else a = -MAXSPEED;
    }

	driveTrain->TankDrive(-a,a);
}
