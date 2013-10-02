#include "ros/ros.h"
#include "std_msgs/String.h"

#include <sstream>
#include <cmath>
#include <vector>

std::vector<double> curve;
ros::Publisher pub;

double runningDistance;
int currentFloor;
double distanceBetweenFloors;

void callback(const /**/){

	if(/*The value that I get is bigger than my threshold*/)
	{
		curve.push_ back(/*The value that I get*/);

	}
	else
	{
		if(curve.size()){

			runningDistance += double_integrate();

			if(currentFloor != Math.floor){

			}

		}
		else{

		}
	}

}

int main(int argc, char **argv){

	ros::init(argc,argv,"Talker");
	ros::NodeHandle n;
	curve.push_back(100.00);
	ros::Subscriber sub = n.subscribe("subscriber_topic", 1, callback);
	return 0;

}

double double_integrate()
{
	double total = 0;
	double timeDifference = 0;
	double overallTimeDifference = 0;

	for(int i = 0; i < curve.size() - 1; i++){
		total += ((curve[i] + curve[i + 1])/2) * timeDifference;
	}

	total = total * overallTimeDifference;

	curve.clear();

	return total;

}
