#include "txlen_ide.h"		// Additional Header


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
#define SUBGHZ_PANID	0xabcd		// panid
//#define HOST_ADDRESS	0xac54		// distination address
#define HOST_ADDRESS	0x6670		// distination address

unsigned char key[]  = {0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c};
unsigned char data1[]= {"Welcome to Lazurite Sub-GHz LAPIS Semiconductor Co,.Ltd.Lazurite"};
unsigned char data2[]= {"Welcome to Lazurite Sub-GHz LAPIS Semiconductor Co,.Ltd.Lazurit"};
unsigned char data3[]= {"Welcome to Lazurite Sub-GHz LAPIS Semiconductor Co,.Ltd.Lazuri"};
unsigned char data4[]= {"Welcome to Lazurite Sub-GHz LAPIS Semiconductor Co,.Ltd.Lazur"};
unsigned char data5[]= {"Welcome to Lazurite Sub-GHz LAPIS Semiconductor Co,.Ltd.Lazu"};
unsigned char data6[]= {"Welcome to Lazurite Sub-GHz LAPIS Semiconductor Co,.Ltd.Laz"};
unsigned char data7[]= {"Welcome to Lazurite Sub-GHz LAPIS Semiconductor Co,.Ltd.La"};
unsigned char data8[]= {"Welcome to Lazurite Sub-GHz LAPIS Semiconductor Co,.Ltd.L"};
unsigned char data9[]= {"Welcome to Lazurite Sub-GHz LAPIS Semiconductor Co,.Ltd."};
unsigned char data10[]={"Welcome to Lazurite Sub-GHz LAPIS Semiconductor Co,.Ltd"};
unsigned char data11[]={"Welcome to Lazurite Sub-GHz LAPIS Semiconductor Co,.Lt"};
unsigned char data12[]={"Welcome to Lazurite Sub-GHz LAPIS Semiconductor Co,.L"};
unsigned char data13[]={"Welcome to Lazurite Sub-GHz LAPIS Semiconductor Co,."};
unsigned char data14[]={"Welcome to Lazurite Sub-GHz LAPIS Semiconductor Co,"};
unsigned char data15[]={"Welcome to Lazurite Sub-GHz LAPIS Semiconductor Co"};
unsigned char data16[]={"Welcome to Lazurite Sub-GHz LAPIS Semiconductor C"};
unsigned char *data[] = { data1,data2,data3,data4,data5,data6,data7,data8,data9,data10,data11,data12,data13,data14,data15,data16 };
unsigned char maxLen;
unsigned char index;



void setup(void)
{
	
	SubGHz.init();					// initializing Sub-GHz
    SubGHz.setKey(key);
	Serial.begin(115200);
    maxLen = sizeof(data1);
	pinMode(LED,OUTPUT);			// setting of LED
	digitalWrite(LED,HIGH);			// setting of LED
    index=0;
}

void loop(void)
{
	SUBGHZ_MSG msg;
    uint32_t get_time;
    uint32_t delta_time;
    uint8_t *p_data;
    uint8_t len;

    p_data = data[index];
    len = maxLen - index;

    index++;
    index=index&0x0f;

	SubGHz.begin(SUBGHZ_CH, SUBGHZ_PANID,  SUBGHZ_100KBPS, SUBGHZ_PWR_20MW);	// start Sub-GHz

	// unicast preparing data
	digitalWrite(LED,LOW);														// LED ON
    get_time = millis();
    msg=SubGHz.send(SUBGHZ_PANID, HOST_ADDRESS, p_data, len ,NULL);     // send data
    delta_time = millis() - get_time;
	digitalWrite(LED,HIGH);														// LED off
    Serial.println_long(delta_time,DEC);
	SubGHz.msgOut(msg);
	
	// close
	SubGHz.close();																// Sub-GHz module sets into power down mode.
	
	sleep(1000);																// sleep

	return;
}

