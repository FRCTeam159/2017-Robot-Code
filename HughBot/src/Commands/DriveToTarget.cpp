#include "Commands/DriveToTarget.h"

#define ADJUST_TIMEOUT 0.5
#define MAX_ANGLE_ERROR 0.5
#define DEFAULT_P 0.07
#define DEFAULT_I 0.0002
#define D 0
#define SCALE 0.1
#define MIN_DISTANCE 12
#define DRIVE_TIMEOUT 0.5
#define MAX_SPEED 0.5
#define MIN_CURRENT .1
DriveToTarget::DriveToTarget() : CommandBase("DriveToTarget"),
    pid(DEFAULT_P,DEFAULT_I,D,this,this)
{
	Requires(driveTrain.get());
    std::cout << "new DriveToTarget"<< std::endl;
}


// Called just before this Command runs the first time
void DriveToTarget::Initialize() {
	distance=visionSubsystem->GetTargetDistance();
	driveTrain->EnableDrive();
	SetTimeout(DRIVE_TIMEOUT*distance+1);
    int ntargets = visionSubsystem->GetNumTargets();
    if (ntargets>0){
       std::cout << "DriveToTarget Started ..." << std::endl;
      	pid.Reset();
		pid.SetSetpoint(MIN_DISTANCE);
		double I = visionSubsystem->GetI();
		double P = visionSubsystem->GetP();
		pid.SetPID(P,I,0);
		pid.Enable();
    }
    else{
    	std::cout << "DriveToTarget Error(no targets) cancelling ..." << std::endl;
    	End();
    }
}


// Called repeatedly when this Command is scheduled to run
void DriveToTarget::Execute() {

}

// Make this return true when this Command no longer needs to run execute()
bool DriveToTarget::IsFinished() {
	/*if(IsTimedOut()){
		std::cout << "DriveToTarget Error:  Timeout expired"<<std::endl;
		return true;
	}*/
	visionSubsystem->GetTargetInfo(target);
#define MAX_DIST_ERR 2
	double d=GetDistance();
	double err=d-MIN_DISTANCE;
	int ntargets=visionSubsystem->GetNumTargets();
	if(ntargets==0 || err<MAX_DIST_ERR)
		return true;
	return false;
}

// Called once after isFinished returns true
void DriveToTarget::End() {
	pid.Disable();
	driveTrain->DisableDrive();
	gearSubsystem->Open();
    std::cout << "DriveToTarget End" << std::endl;

}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void DriveToTarget::Interrupted() {
	End();
}

double DriveToTarget::PIDGet() {
	return visionSubsystem->GetTargetDistance();
}
double DriveToTarget::GetDistance() {
	double d1=visionSubsystem->GetTargetDistance();
#ifdef ULTRASONIC
	double d2=ultrasonicSubsystem->GetDistance();
	double d=d1;
	if(d1<20)
		d=d2;
	return d;
#else
    return d1;
#endif
}
void DriveToTarget::PIDWrite(double err) {
	double d=GetDistance();
	int n=visionSubsystem->GetNumTargets();
	//err += MIN_CURRENT;
	double df=(d-MIN_DISTANCE)/(distance-MIN_DISTANCE); // fraction of starting distance remaining
	//double afact=(1-df)+0.1; // bias angle correction towards end of travel
	double a=-0.01*df*visionSubsystem->GetTargetAngle();
	// double a=-0.1*df*pow(afact,2.0)*visionSubsystem->GetTargetAngle();
	if(n==0)
		a=0;
    double e=-0.5*err;
	double m1=e-a;
	double m2=e+a;
	double mx=m1>m2?m1:m2;
	double scale=mx>1?1/mx:1;
	double l=m2*scale;
	double r=m1*scale;
	//l=Clamp(-0.25,0.25,l);
	//r=Clamp(-0.25,0.25,r);
#define DEBUG_COMMAND
#ifdef DEBUG_COMMAND
	cout<<" dist:"<<d <<" a:"<<a<<" e:"<<e<<" l:"<<l<<" r:"<<r<<endl;
#endif
	driveTrain->TankDrive(l,r);
}

double DriveToTarget::Clamp(double min,double max, double x) {
	 x=x>max? max: x;
	 x=x<min? min: x;
	 return x;
}
