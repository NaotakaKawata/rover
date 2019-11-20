#!/usr/bin/env python

from __future__ import print_function

import rospy

from std_msgs.msg import Float64
from std_msgs.msg import String
import sys, select, termios, tty

msg = """
Reading from the keyboard !
---------------------------
Moving around:
   X    i    X
   j    k    l
   X    ,    X

CTRL-C to quit
"""

moveBindings = {
        'i':'f',
        'j':'l',
        'l':'r',
        ',':'b',
    }

speedBindings={
        'q':(1.1,1.1),
        'z':(.9,.9),
    }

def getKey():
    tty.setraw(sys.stdin.fileno())
    key = sys.stdin.read(1)
    termios.tcsetattr(sys.stdin, termios.TCSADRAIN, settings)
    return key


def vels(speed):
    return "currently:\tspeed %s " % (speed)

if __name__=="__main__":
    settings = termios.tcgetattr(sys.stdin)

    rospy.init_node('vel_Publisher')
    pub_command = rospy.Publisher("/command", String, queue_size=1)

    speed = 1.0
    com = ''

    try:
        print(msg)
        print(vels(speed))
        while(1):
            key = getKey()
            if key in moveBindings.keys():
                com = moveBindings[key]
            elif key in speedBindings.keys():
                speed = speed * speedBindings[key][0]

                print(vels(speed))
                if (status == 14):
                    print(msg)
                status = (status + 1) % 15
            else:
                com = ''
                if (key == '\x03'):
                    break

	    pub_command.publish(com)
    except Exception as e:
        print(e)

    finally:
        com = ''

	pubcommand.publish(com)
        termios.tcsetattr(sys.stdin, termios.TCSADRAIN, settings)
