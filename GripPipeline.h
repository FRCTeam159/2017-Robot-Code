#pragma once
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/contrib/contrib.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <vector>
#include <string>
#include <math.h>

namespace grip {

/**
* A representation of the different types of blurs that can be used.
*
*/
enum BlurType {
	BOX, GAUSSIAN, MEDIAN, BILATERAL
};
/**
* GripPipeline class.
* 
* An OpenCV pipeline generated by GRIP.
*/
class GripPipeline {
	private:
		cv::Mat resizeImageOutput;
		cv::Mat blurOutput;
		cv::Mat rgbThresholdOutput;
		cv::Mat source0;
		std::vector<std::vector<cv::Point> > findContoursOutput;
		std::vector<std::vector<cv::Point> > convexHullsOutput;
		std::vector<std::vector<cv::Point> > filterContoursOutput;
		void resizeImage(cv::Mat &, double , double , int , cv::Mat &);
		void blur(cv::Mat &, BlurType &, double , cv::Mat &);
		void rgbThreshold(cv::Mat &, double [], double [], double [], cv::Mat &);
		void findContours(cv::Mat &, bool , std::vector<std::vector<cv::Point> > &);
		void convexHulls(std::vector<std::vector<cv::Point> > &, std::vector<std::vector<cv::Point> > &);
		void filterContours(std::vector<std::vector<cv::Point> > &, double , double , double , double , double , double , double [], double , double , double , double , std::vector<std::vector<cv::Point> > &);

	public:
		GripPipeline();
		void process(cv::Mat source0);
		cv::Mat* getresizeImageOutput();
		cv::Mat* getblurOutput();
		cv::Mat* getrgbThresholdOutput();
		std::vector<std::vector<cv::Point> >* getfindContoursOutput();
		std::vector<std::vector<cv::Point> >* getconvexHullsOutput();
		std::vector<std::vector<cv::Point> >* getfilterContoursOutput();
		void setsource0(cv::Mat &source0);
};


} // end namespace grip

