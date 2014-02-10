#include "ros/ros.h"
#include "std_msgs/String.h"
#include "ElevatorFloorReader/Sensors.h"
#include "std_msgs/Int32.h"
#include <time.h>
#include <sstream>
#include <cmath>
#include <vector>

struct double_time{
	double magnitude;
	double time;
};

/*
 * A bucket of values
 */
std::vector<double> curve;
ros::Publisher pub;
double accumulatedDistance;
int currentFloor;
double x_0 = 0;
double v_0 = 0;
double a_0 = 0;

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


void callback(const ElevatorFloorReader::Sensors_ sen){
	
	static int prev_sign = 1;
	//invariant - the threshold will always be non-zero, therefore
	//sen.y need not cover the zero case.
	if(abs(sen.y) > threshold && prev_sign * sen.y > 0 )
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
		//Account for changes in positive 
		if(sen.y < 0){
			prev_sign = -1;
		}
		else{
			prev_sign = 1;
		}
	}
}

int main(int argc, char **argv){

	ros::init(argc,argv,"Talker");
	ros::NodeHandle n;
	//For an initial acceleration
	curve.push_back(a_0);
	ros::Subscriber sub = n.subscribe("subscriber_topic", 1, callback);
	pub = n.advertise<std_msgs::Int32>("current_floor",1000);
	return 0;

}

double double_integrate()
{
	double total = 0;
	double timeDifference = 0;

	std::vector<double> velocity_curve;
	velocity_curve.push_back(0.0);
	std::vector<double> displacement_curve;
	displacement_curve.push_back(0.0);

	//take the trapezoidal sum between points that essentially "integrates"
	for(int i = 0; i < curve.size() - 1; i++){
		velocity_curve.push_back( ((curve[i] + curve[i + 1])/2) * timeDifference );
	}

	for(int i = 0; i < curve.size() - 1; i++){
		displacement_curve.push_back( ((velocity_curve[i] + velocity_curve[i + 1])/2) * timeDifference );
	}
	
	total = std::accumulate(displacement_curve.begin(),displacement_curve.end(),0);

	curve.clear();

	return total;

}
