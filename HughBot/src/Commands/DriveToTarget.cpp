#include "Commands/DriveToTarget.h"
#define ADJUST_TIMEOUT 0.5
#define MAX_ANGLE_ERROR 0.5
#define P 0.5
#define I 0.01
#define D 0
#define SCALE 0.1
#define MIN_DISTANCE 10
#define DRIVE_TIMEOUT 0.2

DriveToTarget::DriveToTarget() : CommandBase("DriveToTarget"),
    pid(P,I,D,this,this)
{
	Requires(driveTrain.get());
    std::cout << "new DriveToTarget"<< std::endl;
}

// Called just before this Command runs the first time
void DriveToTarget::Initialize() {
	distance=visionSubsystem->GetTargetDistance();

	SetTimeout(DRIVE_TIMEOUT*distance+1);
    int ntargets = visionSubsystem->GetNumTargets();
    if (ntargets>0){
       std::cout << "DriveToTarget Started ..." << std::endl;
      	pid.Reset();
		pid.SetSetpoint(MIN_DISTANCE);
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
	if(IsTimedOut()){
		std::cout << "DriveToTarget Error:  Timeout expired"<<std::endl;
		return true;
	}
	visionSubsystem->GetTargetInfo(target);

	double d=GetDistance();
	int ntargets=visionSubsystem->GetNumTargets();
	if(ntargets==0 || d<=MIN_DISTANCE)
		return true;
	return false;
}

// Called once after isFinished returns true
void DriveToTarget::End() {
	pid.Disable();
	driveTrain->DisableDrive();
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
	double d2=ultrasonicSubsystem->GetDistance();
	double d=d1;
	if(d2>5 && d1<20)
		d=d2;
    return d1;
}
void DriveToTarget::PIDWrite(double err) {
	double d=GetDistance();

	double df=(d-MIN_DISTANCE)/(distance-MIN_DISTANCE); // fraction of starting distance remaining
	double afact=(1-df)+0.2; // bias angle correction towards end of travel
	double a=-0.1*pow(afact,4.0)*visionSubsystem->GetTargetAngle();
    double e=-err;
	double m1=e+a;
	double m2=e-a;
	double mx=m1>m2?m1:m2;
	double scale=mx>1?1/mx:1;
	double l=m2*scale;
	double r=m1*scale;
#ifdef DEBUG_COMMAND
	cout<<" dist:"<<d <<" a:"<<a<<" e:"<<e<<" l:"<<l<<" r:"<<r<<endl;
#endif
	driveTrain->TankDrive(l,r);
}
