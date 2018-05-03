#import the GPIO and time package
import RPi.GPIO as GPIO
import time

TX = 8
RX = 10

GPIO.setmode(GPIO.BOARD)
GPIO.setup(TX, GPIO.OUT)
GPIO.setup(RX, GPIO.IN)
# loop through 50 times, on/off for 1 second
while(1):
    GPIO.output(TX,'1')
    time.sleep(1)
    GPIO.output(TX,'2')
    time.sleep(1)
    GPIO.output(TX,'H')
    time.sleep(1)
    GPIO.output(TX,'I')
    time.sleep(1)
GPIO.cleanup()