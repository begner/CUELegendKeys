#pragma once

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs.hpp>

#include <vector>
#include <map>

using namespace std;

// Look at:
// http://stackoverflow.com/questions/23842940/clustering-image-segments-in-opencv

class DBScan
{
private:
	
	int C;
	double eps;
	int mnpts;
	double* dp;
	//memoization table in case of complex dist functions
	#define DP(i,j) dp[(data.size()*i)+j]

public:
	
	DBScan(vector<cv::Rect>& _data, double _eps, int _mnpts);
	~DBScan();

	std::map<int, int> labels;
	vector<cv::Rect>& data;

	bool DBScan::isVisited(int i);
	void run();
	void expandCluster(int p, vector<int> neighbours);
	vector<int> regionQuery(int p);
	double dist2d(cv::Point2d a, cv::Point2d b);
	double distanceFunc(int ai, int bi);
	vector<vector<cv::Rect>> getGroups();
};

