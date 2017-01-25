#ifndef DriveTrain_H
#define DriveTrain_H
#include "WPILib.h"
#include "Commands/Subsystem.h"
#include "CANTalon.h"
#include "ErrorBase.h"
#include "MotorSafety.h"
#include "MotorSafetyHelper.h"

using namespace frc;

class DriveTrain: public Subsystem, public MotorSafety {
private:
	// It's desirable that everything possible under private except
	// for methods that implement subsystem capabilities
	CANTalon frontLeft;
	CANTalon frontRight;
	CANTalon backLeft;
	CANTalon backRight;
	DoubleSolenoid *gearPneumatic;
	bool inlowgear=false;
	void InitDrive();
	void CustomArcade(float xAxis, float yAxis, float zAxis, bool squaredInputs);
	float coerce(float min, float max, float x);
public:
	DriveTrain();
	void Drive(float xAxis, float yAxis, float zAxis);
	void InitDefaultCommand();
	void SetLowGear();
	void SetHighGear();

	// required MotorSafety functions
	std::unique_ptr<MotorSafetyHelper> m_safetyHelper;
	void SetExpiration(double timeout) override;
	double GetExpiration() const override;
	bool IsAlive() const override;
	void StopMotor() override;
	bool IsSafetyEnabled() const override;
	void SetSafetyEnabled(bool enabled) override;
	void GetDescription(std::ostringstream& desc) const override;
};

#endif
