#include "SubGHz_TXON_ide.h"		// Additional Header

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
#define SUBGHZ_CH		42			// channel number (frequency)
#define SUBGHZ_PANID	0xabcd		// panid
#define HOST_ADDRESS	0x5f6e		// distination address

#include "phy.h"

unsigned char send_data[] = {"Welcome to Lazurite Sub-GHz\r\n"};

uint8_t data;
uint8_t channel;

void setup(void)
{
	SubGHz.init();					// initializing Sub-GHz
	Serial.begin(115200);
	pinMode(LED,OUTPUT);			// setting of LED
	digitalWrite(LED,HIGH);			// setting of LED

    SubGHz.begin(channel, SUBGHZ_PANID,  SUBGHZ_100KBPS, SUBGHZ_PWR_20MW);		// start Sub-GHz
    SubGHz.rxEnable(NULL);

    data = 0x09;
    phy_regwrite(8,0x6c,&data,1);

    while(1) {
        for (channel=24; channel < 61; channel++) {
            if (channel == 32) continue;
            SubGHz.begin(channel, SUBGHZ_PANID,  SUBGHZ_100KBPS, SUBGHZ_PWR_20MW);		// start Sub-GHz
            SubGHz.rxEnable(NULL);

            data = 0x09;
            phy_regwrite(8,0x6c,&data,1);
            sleep(1000);
	        SubGHz.close();
        }
    }
}

void loop(void)
{
#if 1
    for (channel=24; channel < 61; channel++) {
        if (channel == 32) continue;
        SubGHz.begin(channel, SUBGHZ_PANID,  SUBGHZ_100KBPS, SUBGHZ_PWR_20MW);		// start Sub-GHz
        SubGHz.rxEnable(NULL);

        data = 0x09;
        phy_regwrite(8,0x6c,&data,1);
        sleep(1000);
        SubGHz.close();
    }
#endif
	return;
}

