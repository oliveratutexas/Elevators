#include "ros/ros.h"
#include "std_msgs/String.h"
#include "ElevatorFloorReader/Sensors.h"
#include "std_msgs/Int32.h"
#include <time.h>
#include <sstream>
#include <cmath>
#include <vector>

/*
 * A bucket of values
 */
std::vector<double> curve;
ros::Publisher pub;
double accumulatedDistance;
int currentFloor;

/*
 *	We assume that the floors in the building have a uniform distance between them.
 */
double distanceBetweenFloors;

/*
 *  +/- the deviation from "1g" that I don't expect the values from the ardiuno to exceed.
 */
double threshold = 0.5;

/*
 * true: 	force of gravity is greater than 1g + threshold
 * false: 	force of gravity is less than 1g + threshold
 */
bool direction;

class Value_TimeStamp {
    double x;
    time_t stamp;
};

void callback(const ElevatorFloorReader::Sensors_ sen){

	if(abs(sen.y) > threshold)
	{
		curve.push_back(sen.y);
	}
	else
	{
		//If there's something in the array
		if(curve.size()){

			accumulatedDistance += double_integrate();

			//went up a floor
			if(accumulatedDistance >= distanceBetweenFloors){
				currentFloor++;
				accumulatedDistance = 0;
			}

			//went down a floor
			if(accumulatedDistance <= -1 * distanceBetweenFloors ){
				currentFloor--;
				accumulatedDistance = 0;
			}


		}
	}
}

int main(int argc, char **argv){

	ros::init(argc,argv,"Talker");
	ros::NodeHandle n;
	curve.push_back(100.00);
	ros::Subscriber sub = n.subscribe("subscriber_topic", 1, callback);
	pub = n.advertise<std_msgs::Int32>("current_floor",1000);
	return 0;

}

double double_integrate()
{
	double total = 0;
	double timeDifference = 0;
	double overallTimeDifference = 0;

	//take the trapezoidal sum between points that essentially "integrates"
	for(int i = 0; i < curve.size() - 1; i++){
		total += ((curve[i] + curve[i + 1])/2) * timeDifference;
	}

	total = total * overallTimeDifference;

	curve.clear();

	return total;

}
