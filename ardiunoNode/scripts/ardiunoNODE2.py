#!/usr/bin/env python
import roslib; roslib.load_manifest('ardiunoNode')
import rospy

import re #yeah, we're using REGEX.
import serial

from std_msgs.msg import String		#just in case we need to publish dummy messages
from std_msgs.msg import Float32	#for the float values
from std_msgs.msg import Int32		#for the int values
from ardiunoNode.msg import Sensors #look this up
''' new stuff'''
import time, struct
from threading import Thread


'''
SerialMonitor operates as a separate thread that 
receives incoming light level data from the Arduino Mega.  Since
light level messages are transmitted at 10Hz, this
thread loops at 20Hz.
'''
'''The REGEXes for recognizing an integer or recognizing a float'''
intRE = re.compile( " -?[0-9]+ ?") #be sure to note the option of a space before and the
									#necessity of a space afterwards
floatRE = re.compile("-?[0-9]+\.[0-9]+")

class SerialMonitor(Thread): # SerialMonitor extends Thread
  ''' 
  The constructor of SerialMonitor.

  Parameters:
   - ser: The serial port object
   - pub: The object through which LightLevel messages may be published
  '''
  def __init__(self, ser, pub):
	Thread.__init__(self, name="SerialMonitor") # Call superclass constructor
	self.ser = ser
	print ser.inWaiting()
	self.seqno = 0
	self.pub = pub

  def run(self):
	rospy.loginfo(rospy.get_name() + " SerialMonitor: Thread starting.")
	while not rospy.is_shutdown():		
		if(self.ser.inWaiting() > 0):
			print "serial buffer"
			msgSensors = Sensors()
			line = ser.readline()
			rospy.loginfo(line)
			threeFloats = floatRE.findall(line)
			rospy.loginfo(threeFloats)
			if len(threeFloats) > 2 :
			#	if type(threeFloats[0]) == 'str':
				msgSensors.x = float(threeFloats[0])
			#	else:
			#		msgSensors.x = 2.0
			#	if type(threeFloats[1]) == 'str':
				msgSensors.y = float(threeFloats[1])
			#	else:
			#		msgSensors.y = 2.0
			#	if type(threeFloats[2]) == 'str':
				msgSensors.z = float(threeFloats[2])
			#	else:
			#		msgSensors.z = 2.0
				print type(threeFloats[0])
			else:
				msgSensors.x = -1.0
				msgSensors.y = -1.0
				msgSensors.z = -1.0
			
			#rospy.loginfo(str(threeFloats[0]))
			pub.publish(msgSensors)
			"""
			mockData = Sensors()
			mockData.x = 0.0
			mockData.y = 0.0
			mockData.z = 0.0
			"""
			
			#pub.publish(mockData)

	self.seqno += 1
                    
            
# Publish the AmbientLight message
        time.sleep(0.05)  # cycle at 20Hz

def callback(cmd):
  ''' The loginfo command might be useful, but this is commented because it's specific to that program.'''
  #data = [cmd.cmd]
  #bytes = struct.pack("<b", *data)
  #numTX = ser.write(bytes)
  #rospy.loginfo(rospy.get_name() + " - Sent command (%i bytes)", numTX)
  # Format chars: http://docs.python.org/library/struct.html#format-characters

                        
if __name__ == '__main__':
	#the port randomly changes w/ the usb, so, sadly, this is necessary.
	rospy.init_node('talker')
	pub = rospy.Publisher('arduinoReadings', Sensors)
	for x in range(0,10):
		port = '/dev/ttyACM' + str(x)
		baud = 115200
		rospy.loginfo("Serial(USB) port = %s", port)
		rospy.loginfo("Serial baud = %i", baud)
		#just to create a variable
		try:
			ser = serial.Serial(port,baud)
			break
		except:
			if x is not 9:
				rospy.logerr("Unable to open serial port" + str(x) + "\nTrying next port.")
			else:
				rospy.logerr("\n\nIt seems as if the ardiuno is unplugged.\n\n")
				time.sleep(0.05)
				print sys.exec_info()[0]
				sys.exit()
		rospy.loginfo("Waiting 2s for Arduino to initialize...")
  		time.sleep(2)
	if not ser:
		rospy.logerr("Unable to open serial port\nExiting.")
		sys.exit()	
	#what does this do?
	print ser.inWaiting()
	#ser.flushInput()
	print "after flush" + str(ser.inWaiting())

    # Create and start a serial monitor thread.
    # This is for receiving light level information.
	smThread = SerialMonitor(ser, pub)
	smThread.start()
        """
        while not rospy.is_shutdown():
        	data = Sensors(0.0, 0.0, 0.0)
                print "hello: " + str(data)
                pub.publish(data)
                rospy.sleep(1.0)
        """
	rospy.spin()
