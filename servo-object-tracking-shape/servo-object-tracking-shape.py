#!/usr/bin/python3

"""
Find geometric shaped objects in the image using houghCircles().

Reference: 
- https://www.pyimagesearch.com/2014/07/21/detecting-circles-images-using-opencv-hough-circles/

YunFei Robotics Labrotary
Twitter: @yfrobotics
Website: https://www.yfrl.org
"""

import cv2
import numpy as np
import serial
import time

CAMERA_DEVICE_ID = 0
IMAGE_WIDTH = 640
IMAGE_HEIGHT = 480


def isset(v):
    try:
        type (eval(v))
    except:
        return 0
    else:
        return 1


lost = True

if __name__ == "__main__":
    try:
        # open serial
        ser = serial.Serial('/dev/ttyACM0')
        time.sleep(1)

        # create video capture
        cap = cv2.VideoCapture(CAMERA_DEVICE_ID)
        
        # set the fps
        cap.set(cv2.CAP_PROP_FPS, 30)

        # set resolution to 320x240 to reduce latency 
        cap.set(3, IMAGE_WIDTH)
        cap.set(4, IMAGE_HEIGHT)

        while True:
            # Read the frames frome a camera
            _, frame = cap.read()
            frame = cv2.blur(frame,(3,3))

            # Or get it from a JPEG
            # frame = cv2.imread('frame0010.jpg', 1)
        
            # convert the image into gray color
            output = frame.copy()
            gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

            # detect circles in the image
            circles = cv2.HoughCircles(gray, cv2.HOUGH_GRADIENT, 1.2, 100)
            
            # ensure at least some circles were found
            if circles is not None:
                # convert the (x, y) coordinates and radius of the circles to integers
                circles = np.round(circles[0, :]).astype("int")

                lost = False
                
                # loop over the (x, y) coordinates and radius of the circles
                for (x, y, r) in circles:
                    # draw the circle in the output image, then draw a rectangle
                    # corresponding to the center of the circle
                    cv2.circle(output, (x, y), r, (0, 255, 0), 4)
                    cv2.rectangle(output, (x - 5, y - 5), (x + 5, y + 5), (0, 128, 255), -1)
                    # control the servos
                    if (x > IMAGE_WIDTH/2 - 20 and x < IMAGE_WIDTH/2 + 20 and y > IMAGE_HEIGHT/2 - 20 and y < IMAGE_HEIGHT/2 + 20):
                      ser.write(b'0:0\n')
                    elif (x > IMAGE_WIDTH/2 and y > IMAGE_HEIGHT/2):
                      ser.write(b'-2:2\n')
                    elif (x > IMAGE_WIDTH/2 and y < IMAGE_HEIGHT/2):
                      ser.write(b'-2:-2\n')
                    elif (x < IMAGE_WIDTH/2 and y > IMAGE_HEIGHT/2):
                      ser.write(b'2:2\n')
                    elif (x < IMAGE_WIDTH/2 and y < IMAGE_HEIGHT/2):
                      ser.write(b'2:-2\n')
                    else:
                      ser.write(b'0:0\n')
            else:
                if not lost:
                  lost = True
                  ser.write(b'0:0\n')
                else:
                  pass
            
            # show the output image
            cv2.imshow("frame", np.hstack([frame, output]))

            # if key pressed is 'Esc' then exit the loop
            if cv2.waitKey(1) == 27:
                break
            
            print("check", time.time())
    except Exception as e:
        print(e)
    finally:
        # Clean up and exit the program
        cv2.destroyAllWindows()
        cap.release()      
        ser.close()
