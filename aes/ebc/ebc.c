#include "ebc_ide.h"		// Additional Header

// Additional Header


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

unsigned const char plain[] = {"Welcome to Lazurite Sub-GHz    "};
unsigned char key[]         = {0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c};
// initioal vecter table
unsigned char iv[]          = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
unsigned char tx_buf[50];
unsigned char cipher[50];
unsigned char dencrypt[50];
unsigned char plain_len;

//void AES128_ECB_encrypt(uint8_t* input, const uint8_t* key, uint8_t *output);
//void AES128_ECB_decrypt(uint8_t* input, const uint8_t* key, uint8_t *output);


uint8_t flg;

void setup(void)
{
	SubGHz.init();					// initializing Sub-GHz
	Serial.begin(115200);
	pinMode(LED,OUTPUT);			// setting of LED
	digitalWrite(LED,HIGH);			// setting of LED
    Serial.print("=============== Plain test length: 0x");
    Serial.print_long((long)sizeof(plain),HEX);
	Serial.print("\r\n");
    Serial.print("=============== Key length: 0x");
    Serial.print_long((long)sizeof(key),HEX);
	Serial.print("\r\n");
    plain_len = sizeof(plain);
    flg=0;
}

void loop(void)
{
    uint8_t i;

    Serial.print("--------------Plain text    ----------------\r\n");
    Serial.print(plain);
    for(i=0;i<plain_len;i++)
    {
        Serial.print_long((long)plain[i],HEX);
    }
	Serial.print("\r\n");


    for(i=0;i<plain_len;i++)
    {
        Serial.print_long((long)tx_buf[i],HEX);
    }
	Serial.print("\r\n");
    
    memcpy(tx_buf, plain, plain_len);

    AES128_ECB_encrypt(tx_buf, key, cipher);
	Serial.print("\r\n");
    AES128_ECB_encrypt(tx_buf+16, key, cipher+16);
    Serial.print("--------------Encryption text---------------\r\n");
    for(i=0;i<50;i++)
    {
	    Serial.print_long((long)cipher[i],HEX);
    }
	Serial.print("\r\n");


    AES128_ECB_decrypt(cipher, key, dencrypt);
    AES128_ECB_decrypt(cipher+16, key, dencrypt+16);
    Serial.print("--------------Dencryption text--------------\r\n");
    Serial.print(dencrypt);
	Serial.print("\r\n");
    for(i=0;i<plain_len;i++)
    {
	    Serial.print_long((long)dencrypt[i],HEX);
    }
	Serial.print("\r\n");


    sleep(1000);
	return;
}

