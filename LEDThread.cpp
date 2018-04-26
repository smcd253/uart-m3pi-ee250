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
 * @file       LEDThread.cpp
 * @brief      Implementation of thread that handles LED requests.
 *
 * @author     Jason Tran <jasontra@usc.edu>
 * @author     Bhaskar Krishnachari <bkrishna@usc.edu>
 */

#include "LEDThread.h"
#include "MQTTmbed.h"
#include "MQTTNetwork.h"

#include "MQTTClient.h"

// ----------- m3pi mod ------------
#include "m3pi.h"

Mail<MailMsg, LEDTHREAD_MAILBOX_SIZE> LEDMailbox;

static DigitalOut led2(LED2);

static const char *topic = "m3pi-mqtt-ee250/led-thread";

// ----------- m3pi mod ------------
m3pi m3pi(p23, p9, p10);

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

void LEDThread(void *args) 
{
    MQTT::Client<MQTTNetwork, Countdown> *client = (MQTT::Client<MQTTNetwork, Countdown> *)args;
    MailMsg *msg;
    MQTT::Message message;
    osEvent evt;
    char pub_buf[16];


    while(1) {

        evt = LEDMailbox.get();

        if(evt.status == osEventMail) {
            msg = (MailMsg *)evt.value.p;
            int speed = 0; // speed
            int delta_t = 0; // time
            /* the second byte in the message denotes the action type */
            switch (msg->content[1]) {
                case LED_THR_PUBLISH_MSG:
                    printf("LEDThread: received command to publish to topic"
                           "m3pi-mqtt-example/led-thread\n");
                    pub_buf[0] = 'h';
                    pub_buf[1] = 'i';
                    message.qos = MQTT::QOS0;
                    message.retained = false;
                    message.dup = false;
                    message.payload = (void*)pub_buf;
                    message.payloadlen = 2; //MQTTclient.h takes care of adding null char?
                    /* Lock the global MQTT mutex before publishing */
                    mqttMtx.lock();
                    client->publish(topic, message);
                    mqttMtx.unlock();
                    break;
                case LED_ON_ONE_SEC:
                    printf("LEDThread: received message to turn LED2 on for"
                           "one second...\n");
                    led2 = 1;
                    wait(1);
                    led2 = 0;
                    break;
                case LED_BLINK_FAST:
                    printf("LEDThread: received message to blink LED2 fast for"
                           "one second...\n");
                    for(int i = 0; i < 10; i++)
                    {
                        led2 = !led2;
                        wait(0.1);
                    }
                    led2 = 0;
                    break;

                // ----------- m3pi mod ------------
                case FORWARD:
                    printf("LEDThread: received message to move FORWARD\n");
                    // movement('w', 25, 100);
                    break;
                case BACKWARD:
                    printf("LEDThread: received message to move BACKWARD\n");
                    // movement('s', 25, 100);
                    break;
                case RIGHT:
                    printf("LEDThread: received message to move RIGHT\n");
                    // grab speed data
                    if(msg->content[2] != NULL){
                        speed = int(msg->content[2]);
                        m3pi.right(speed);
                        printf("Turn RIGHT with speed %i\n", speed);
                        if(msg->content[3] != NULL){
                            delta_t = int(msg->content[4]);
                             printf("Wait %ims\n", delta_t);
                            Thread::wait(delta_t);
                        }
                        else{
                            // wait 100ms
                            Thread::wait(100);
                        }   
                    }
                    // movement('d', 25, 100);
                    break;
                case LEFT:
                    printf("LEDThread: received message to move LEFT\n");
                    // grab speed data
                    if(msg->content[2] != NULL){
                        speed = int(msg->content[2]);
                        m3pi.left(speed);
                        printf("Turn LEFT with speed %i\n", speed);
                        if(msg->content[3] != NULL){
                            delta_t = int(msg->content[4]);
                            printf("Wait %ims\n", delta_t);
                            Thread::wait(delta_t);
                        }
                        else{
                            // wait 100ms
                            Thread::wait(100);
                        }
                    }
                    // movement('a', 25, 100);
                    break;
                case STOP:
                    printf("LEDThread: received message to STOP\n");
                    m3pi.stop();
                    break;
                default:
                    printf("LEDThread: invalid message\n");
                    break;
                m3pi.stop(); 
            }            

            LEDMailbox.free(msg);
        }
    } /* while */

    /* this should never be reached */
}

Mail<MailMsg, LEDTHREAD_MAILBOX_SIZE> *getLEDThreadMailbox() 
{
    return &LEDMailbox;
}


