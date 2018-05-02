# import the necessary packages
from picamera.array import PiRGBArray
import numpy as np
from picamera import PiCamera
import time
import cv2
import time
import picamera
import picamera.array
import cv2
# initialize the camera and grab a reference to the raw camera capture
def get_pixel_offset():
	cx = 0
	cy = 0
	with picamera.PiCamera() as camera:
		camera.start_preview()
		with picamera.array.PiRGBArray(camera) as stream:
			camera.capture(stream, format='bgr')
			# At this point the image is available as stream.array
			img = stream.array
			# capture frames from the camera
			# grab the raw NumPy array representing the image, then initialize the timestamp
			# and occupied/unoccupied text
			height, width, channels = img.shape 
			cropped_img = img[height - int(height/8): height, 0:width]	#convert the image to gray scale
			gray = cv2.cvtColor(cropped_img, cv2.COLOR_BGR2GRAY)
			#blur the image. Aka average the image
			blur = cv2.GaussianBlur(gray,(5,5),50)
			#create a binary inverse threshold of the image
			ret,thresh = cv2.threshold(blur,40,255,cv2.THRESH_BINARY_INV)
			mask = cv2.erode(thresh, None, iterations=2)
			mask = cv2.dilate(mask, None, iterations=2)
			#find the contours of the image
			image, contours, _ = cv2.findContours(mask.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
			if len(contours) > 0:
				c = max(contours, key=cv2.contourArea)
				M = cv2.moments(c)
				cx = int(M['m10']/M['m00'])
				cy = int(M['m01']/M['m00'])
				# If standard devitation of x coordinates is high then we have encountered a T intersection. Return Encountered Intersection
				if np.std(c[:,0][:, 0]) > 150:
					return "Encountered T Intersection"
				# Else if standard deviation is not high but the average is far to one side we have encountered a sharp turn. Return which way the sharp turn is
				elif (width/2 - cx) > 120:
					return "Sharp Turn Left"
				elif (width/2 - cx) < -120:
					return "Sharp Turn Right"
				# Else return pixel offset
				else:
					return (width/2 - cx)
		# show the frame
	# if the `q` key was pressed, break from the loop