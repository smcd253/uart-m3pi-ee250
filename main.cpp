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

#define BUF_SIZE 4

extern "C" void mbed_reset();

//uart
Serial rpi(p13, p14, 9600);  // tx, rx

/** Initialize the m3pi for robot movements*/
m3pi m3pi(p23, p9, p10);

// switch case enum
enum COMMANDS{
    FORWARD,
    REVERSE,
    RIGHT_STILL,
    LEFT_STILL,
    STOP
} command;

// receive buffer
int i = 0;
int rcv[BUF_SIZE];
// rcv[0] is direction for switch case
// rcv[1] is speed hundreds
// rcv[2] is speed tens
// rcv[3] is speed ones

int speed;
int select;

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

void serial_in() {
    // Note: you need to actually read from the serial to clear the RX interrupt
    if (i < BUF_SIZE){
        rcv[i] = (int)rpi.getc() - 48;
        printf("rcv[%i] = %i\n", i, rcv[i]);
        i++;
    } 
    else{
        select = rcv[0];
        speed = rcv[1] * 100 + rcv[2] * 10 + rcv[3];
        i = 0;
    }
    // printf("rcv[] = %s", rcv);
}

void _switch(){

    switch(command){
        // ----------- m3pi mod ------------
        case FORWARD:
            printf("FORWARD\n");
            printf("Speed = %i\n", speed);
            // grab speed data
            // if(msg->content[2] != NULL){
            //     speed = int(msg->content[2]);
            //     m3pi.forward(speed);
            //     printf("FORWARD with speed %i\n", speed);
            //     if(msg->content[3] != NULL){
            //         delta_t = int(msg->content[4]);
            //         printf("Wait %ims\n", delta_t);
            //         Thread::wait(delta_t);
            //     }
            //     else{
            //         // wait 100ms
            //         Thread::wait(100);
            //     }
            // }
            break;
        case REVERSE:
            printf("REVERSE\n");
            printf("Speed = %i\n", speed);
            // movement('s', 25, 100);
            break;
        case RIGHT_STILL:
            printf("RIGHT_STILL\n");
            printf("Speed = %i\n", speed);
            // grab speed data
            // if(msg->content[2] != NULL){
            //     speed = int(msg->content[2]);
            //     m3pi.right(speed);
            //     printf("Turn RIGHT with speed %i\n", speed);
            //     if(msg->content[3] != NULL){
            //         delta_t = int(msg->content[4]);
            //         printf("Wait %ims\n", delta_t);
            //         Thread::wait(delta_t);
            //     }
            //     else{
            //         // wait 100ms
            //         Thread::wait(100);
            //     }   
            // }
            break;
        case LEFT_STILL:
            printf("LEFT_STILL\n");
            printf("Speed = %i\n", speed);
            // grab speed data
            // if(msg->content[2] != NULL){
            //     speed = int(msg->content[2]);
            //     m3pi.left(speed);
            //     printf("Turn LEFT with speed %i\n", speed);
            //     if(msg->content[3] != NULL){
            //         delta_t = int(msg->content[4]);
            //         printf("Wait %ims\n", delta_t);
            //         Thread::wait(delta_t);
            //     }
            //     else{
            //         // wait 100ms
            //         Thread::wait(100);
            //     }
            // }
            // movement('a', 25, 100);
            break;
        case STOP:
            printf("STOP\n");
            printf("Speed = %i\n", speed);
            // m3pi.stop();
            break;
        default:
            printf("default\n");
            break;
        // m3pi.stop(); 
    }     
}
int main()
{
    wait(1); // delay startup
    printf("UART Initializing...\n");
    
    // attach uart interrupt
    rpi.attach(&serial_in);
    printf("UART Initialized!\n");

    while(1){
        // int arraySize = sizeof(rcv)/sizeof(rcv[0]);
        // printf("array size: %i\n", arraySize);

        if (i == BUF_SIZE){
            _switch();
        }

        // waiting for serial from rpi
        // char A = 'A';
        // rpi.printf("%c\n", A);
        
        wait(1);
    }

    return 0;
}