#ifndef Vision_H
#define Vision_H

#include <Commands/Subsystem.h>
#include <CameraServer.h>
#include "llvm/ArrayRef.h"
#include "llvm/StringRef.h"
#include <opencv2/core/types.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>

class Cameras : public Subsystem {
	class Camera {
		double brightness=2;
		double whitebalance=2;
		double exposure=2;
		cs::UsbCamera camera;
		cs::CvSink sink;
	public:
		Camera(llvm::StringRef name, int dev);

		void InitCamera(double e, double w, double b);

		double Exposure() { return exposure;}
		double Balance() { return whitebalance;}
		double Brightness() { return brightness;}
		void SetResolution(int w, int h) { camera.SetResolution(w,h);}
		void SetFPS(int s) { camera.SetFPS(s);}
		void AdjustSettings(double e, double w, double b);
		uint64_t GrabFrame(cv::Mat);
	};
	static void AdjustThread();

public:
	Cameras();
	void Init();
};

#endif  // Vision_H
