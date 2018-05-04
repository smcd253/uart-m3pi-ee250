/**
 * Copyright (c) 2017, Autonomous Networks Research Group. All rights reserved.
 * Developed by:
 * Autonomous Networks Research Group (ANRG)
 * University of Southern California
 * http://anrg.usc.edu/
 *
 * Contributors:
 * Jason A. Tran <jasontra@usc.edu>
 * Bhaskar Krishnamachari <bkrishna@usc.edu>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy 
 * of this software and associated documentation files (the "Software"), to deal
 * with the Software without restriction, including without limitation the 
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or 
 * sell copies of the Software, and to permit persons to whom the Software is 
 * furnished to do so, subject to the following conditions:
 * - Redistributions of source code must retain the above copyright notice, this
 *     list of conditions and the following disclaimers.
 * - Redistributions in binary form must reproduce the above copyright notice, 
 *     this list of conditions and the following disclaimers in the 
 *     documentation and/or other materials provided with the distribution.
 * - Neither the names of Autonomous Networks Research Group, nor University of 
 *     Southern California, nor the names of its contributors may be used to 
 *     endorse or promote products derived from this Software without specific 
 *     prior written permission.
 * - A citation to the Autonomous Networks Research Group must be included in 
 *     any publications benefiting from the use of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * CONTRIBUTORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS WITH 
 * THE SOFTWARE.
 */

/**
 * @file       main.cpp
 * @brief      Example using an ESP8266 to pub/sub using MQTT on MBED OS.
 * 
 *             This example is specifically built for the FIE workshop.
 *
 * @author     Jason Tran <jasontra@usc.edu>
 * @author     Bhaskar Krishnachari <bkrishna@usc.edu>
 * 
 */

#include "mbed.h"
#include "m3pi.h"


extern "C" void mbed_reset();


/** Initialize the m3pi for robot movements. There is an atmega328p MCU in the
 *  3pi robot base. It's UART lines are connected to the LPC1768's p9 and p10.
 *  If you send the right sequence of UART characters to the atmega328p, it will
 *  move the robot for you. We provide a movement() function below for you to use
 */
m3pi m3pi(p23, p9, p10);


/**
 * @brief      controls movement of the 3pi
 *
 *  If you want to use this function in a file outside of main.cpp, the easiest
 *  way to get access to it is to add
 *      
 *      extern movement(char command, char speed, int delta_t)
 *
 * in the .cpp file in which you want to use it.
 *
 * @param[in]  command  The movement command
 * @param[in]  speed    The speed of the movement (start by trying 25)
 * @param[in]  delta_t  The time for each movement in msec (start by trying 100)
 *
 * @return     { void }
 */
void movement(char command, char speed, int delta_t)
{
    if (command == 's')
    {
        m3pi.forward(speed);
        Thread::wait(delta_t);
        m3pi.stop();
    }    
    else if (command == 'a')
    {
        m3pi.left(speed);
        Thread::wait(delta_t);
        m3pi.stop();
    }   
    else if (command == 'w')
    {
        m3pi.backward(speed);
        Thread::wait(delta_t);
        m3pi.stop();
    }
    else if (command == 'd')
    {
        m3pi.right(speed);
        Thread::wait(delta_t);
        m3pi.stop();
    }
}


//uart
Serial rpi(p13, p14, 9600);  // tx, rx

void callback() {
    // Note: you need to actually read from the serial to clear the RX interrupt
    printf("%c\n", rpi.getc());
    printf("callback called\n");
}

int main()
{
    wait(1); // delay startup
    // attach uart interrupt
    rpi.attach(&callback);
    printf("initialized callback");

    /* Uncomment this to see how the m3pi moves. This sequence of functions
       represent w-a-s-d like controlling. Each button press moves the robot
       at a speed of 25 (speed can be between -127 to 127) for 100 ms. Use
       functions like this in your program to move your m3pi when you get 
       MQTT messages! */
    // movement('w', 25, 100);
    // movement('w', 25, 100);
    // movement('w', 25, 100);
    // movement('w', 25, 100);
    // movement('a', 25, 100);
    // movement('a', 25, 100);
    // movement('a', 25, 100);
    // movement('a', 25, 100);
    // movement('d', 25, 100);
    // movement('d', 25, 100);
    // movement('d', 25, 100);
    // movement('d', 25, 100);
    // movement('s', 25, 100);
    // movement('s', 25, 100);
    // movement('s', 25, 100);
    // movement('s', 25, 100);

    while(1){
        // waiting for serial from rpi
    }

    return 0;
}