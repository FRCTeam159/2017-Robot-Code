#include "UltrasonicSubsystem.h"
#include "../RobotMap.h"
#include "Commands/UltraUpdate.h"
using namespace frc;

UltrasonicSubsystem::UltrasonicSubsystem() : Subsystem("ExampleSubsystem"),
		port(9600, SerialPort::kOnboard, 8, SerialPort::kParity_None, SerialPort::kStopBits_One),
		dOutput(0){
	portEnabled=false;
	frc::SmartDashboard::PutNumber("UltraSonicDistance", 0);
}
/*
SerialPort::SerialPort(int baudRate, Port port, int dataBits,
                       SerialPort::Parity parity,
                       SerialPort::StopBits stopBits) {
*/

void UltrasonicSubsystem::InitDefaultCommand() {
		// Set the default command for a subsystem here.
		SetDefaultCommand(new UltraUpdate());
}

void UltrasonicSubsystem::Disable() {
	dOutput.Set(0);
	portEnabled=false;
}

void UltrasonicSubsystem::Enable() {
	dOutput.Set(1);
	portEnabled=true;
}

bool UltrasonicSubsystem::IsEnabled() {
	return portEnabled;
}

double UltrasonicSubsystem::GetDistance() {
	if(!IsEnabled()){
		cout<<"Ultrasonic port not enabled"<<endl;
		return 0.0;
	}
	char buffer [6]={0};
	int inChars = port.Read(buffer,5);
	int value = 999;
	if(inChars != 5){
		cout<<"unexpected input::"<<buffer<<endl;
		return 0;
	}
	sscanf(buffer, "R%d\n", & value);
	frc::SmartDashboard::PutNumber("UltraSonicDistance", value);
	return value;
}
// Put methods for controlling this subsystem
// here. Call these from Commands.
