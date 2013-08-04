#!/usr/bin/env python
import roslib; roslib.load_manifest('ardiunoNode')
import rospy
from std_msgs.msg import String
from ardiunoNode.msg import Sensors
from std_msgs.msg import Float64

def callback(data):
	rospy.loginfo(rospy.get_name() + "	X Acceleration: " + str(data.x))
	rospy.loginfo(rospy.get_name() + "	Y Acceleration: " + str(data.y))
	rospy.loginfo(rospy.get_name() + "	Z Acceleration: " + str(data.z))


def listener():
	rospy.init_node('listener', anonymous=True)
	rospy.Subscriber("arduinoReadings", Sensors, callback)
	rospy.spin()

if __name__ == '__main__':
	listener()
