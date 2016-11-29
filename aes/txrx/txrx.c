#include "txrx_ide.h"		// Additional Header



/* FILE NAME: Welcome_SubGHz.c
 * The MIT License (MIT)
 * 
 * Copyright (c) 2015  Lapis Semiconductor Co.,Ltd.
 * All rights reserved.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
*/


#define LED 26						// pin number of Blue LED
#define SUBGHZ_CH		42			// chael number (frequency)
#define SUBGHZ_PANID	0xffff		// panid
#define HOST_ADDRESS	0xffff		// distination address

unsigned char send_data[] = {"Welcome to Lazurite Sub-GHz LAPIS Semiconductor Co,.Ltd.Lazurite"};
unsigned char key[]       = {0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c};

SUBGHZ_STATUS rx;
SUBGHZ_MSG msg;
unsigned char rx_data[256];

static void tx_aes(void);
static void rx_aes(void);

void setup(void)
{
	SubGHz.init();					// initializing Sub-GHz
    SubGHz.setKey(key);
	Serial.begin(115200);
	SubGHz.begin(SUBGHZ_CH, SUBGHZ_PANID,  SUBGHZ_100KBPS, SUBGHZ_PWR_20MW);		// start Sub-GHz
    SubGHz.rxEnable(NULL);
	pinMode(LED,OUTPUT);			// setting of LED
	digitalWrite(LED,HIGH);			// setting of LED
}

void loop(void)
{
    tx_aes();
    rx_aes();
}

static void tx_aes(void)
{
	// Initializing
    uint32_t get_time;
    uint32_t delta_time;

	// preparing data
	digitalWrite(LED,LOW);														// LED ON
    get_time = millis();
    SubGHz.send(SUBGHZ_PANID, HOST_ADDRESS, &send_data, sizeof(send_data),NULL);// send data
    delta_time = millis() - get_time;
	digitalWrite(LED,HIGH);														// LED off
    Serial.println_long(delta_time,DEC);
	SubGHz.msgOut(msg);
	
	// close
	SubGHz.close();																// Sub-GHz module sets into power down mode.
	
	return;
}

static void rx_aes(void)
{
    short rx_len;
    short index=0;
    uint16_t data16;
	long myAddress;

	myAddress = SubGHz.getMyAddress();
	Serial.print("myAddress1 = ");
	Serial.println_long(myAddress,HEX);	
	msg = SubGHz.begin(SUBGHZ_CH, SUBGHZ_PANID,  SUBGHZ_100KBPS, SUBGHZ_PWR_20MW);
	if(msg != SUBGHZ_OK)
	{
		SubGHz.msgOut(msg);
		while(1){ }
	}

	msg = SubGHz.rxEnable(NULL);
	if(msg != SUBGHZ_OK)
	{
		SubGHz.msgOut(msg);
		while(1){ }
	}
    
    while(1){
        rx_len = SubGHz.readData(rx_data,sizeof(rx_data));

        if(rx_len>0)
        {
            digitalWrite(LED, LOW);
            SubGHz.getStatus(NULL,&rx);
            // print time
            Serial.print_long((long)millis(),DEC);
            Serial.print("\t");


            // print header
            data16 = rx_data[index], index++;
            data16 = data16 + ((uint16_t)rx_data[index] << 8), index++;
            Serial.print_long((long)data16,HEX);
            Serial.print("\t");

            // print sequence number
            Serial.print_long((long)rx_data[index],HEX), index++;
            Serial.print("\t");

            // print PANID
            data16 = rx_data[index], index++;
            data16 = data16 + ((uint16_t)rx_data[index] << 8), index++;
            Serial.print_long((long)data16,HEX);
            Serial.print("\t");

            // print RX_ADDR
            data16 = rx_data[index], index++;
            data16 = data16 + ((uint16_t)rx_data[index] << 8), index++;
            Serial.print_long((long)data16,HEX);
            Serial.print("\t");

            // print TX_ADDR
            data16 = rx_data[index], index++;
            data16 = data16 + ((uint16_t)rx_data[index] << 8), index++;
            Serial.print_long((long)data16,HEX);
            Serial.print("\t");

            // print RSSI
            Serial.print_long((long)rx.rssi,DEC);
            Serial.print("\t");

            // print payload
            for(;index<rx_len;index++)
            {
                Serial.print_long(rx_data[index],HEX);
                Serial.print(" ");
            }

            // print ln
            Serial.println("");
            digitalWrite(LED, HIGH);
            break;
        }
    }

    sleep(500);
	
    return;
}
