#include "LazAes_ide.h"		// Additional Header
#include "aes.h"
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

#define BUF_SIZE        80
unsigned const char plainStr[] = {"Welcome to Lazurite Sub-GHz     LAPIS Semiconductor Co,.Ltd.    "};
unsigned char key[]            = {0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c};
//unsigned char iv[]           = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
unsigned char iv[]             = {0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c};
unsigned char plain[BUF_SIZE];
unsigned char cipher[BUF_SIZE];
unsigned char dencrypt[BUF_SIZE];
unsigned char plainLen;

// void AES128_CBC_encrypt_buffer(uint8_t* output, uint8_t* input, uint32_t length, const uint8_t* key, const uint8_t* iv);
// void AES128_CBC_decrypt_buffer(uint8_t* output, uint8_t* input, uint32_t length, const uint8_t* key, const uint8_t* iv);

uint8_t counter;

void setup(void)
{
	SubGHz.init();					// initializing Sub-GHz
	Serial.begin(115200);
	pinMode(LED,OUTPUT);			// setting of LED
	digitalWrite(LED,HIGH);			// setting of LED
    Serial.print("=============== Plain test length: 0x");
    Serial.print_long((long)sizeof(plainStr),HEX);
	Serial.print("\r\n");
    Serial.print("=============== Key length: 0x");
    Serial.print_long((long)sizeof(key),HEX);
	Serial.print("\r\n");
    plainLen = sizeof(plainStr)-1;
    counter=0;
}

void loop(void)
{
    uint8_t i;
    uint8_t iv_val;


    Serial.print("--------------Encryption text---------------\r\n");
    Serial.println_long((long)plainLen,DEC);
    Serial.print(plainStr);
	Serial.print("\r\n");

    memset(plain, 0, BUF_SIZE);
    memset(cipher, 0, BUF_SIZE);
    memset(dencrypt, 0, BUF_SIZE);

    memcpy(plain, plainStr, plainLen);
    for(i=0;i<=plainLen;i++)
    {
        Serial.print_long((long)plain[i],HEX);
    }
	Serial.print("\r\n");

    AES128_CBC_encrypt_buffer(cipher, plain, plainLen, key, &iv_val);
    for(i=0;i<plainLen;i++)
    {
	    Serial.print_long((long)cipher[i],HEX);
    }
	Serial.print("\r\n");


    Serial.print("--------------Dencryption text--------------\r\n");
    AES128_CBC_decrypt_buffer(dencrypt, cipher, plainLen, key, &iv_val);
    Serial.print(dencrypt);
	Serial.print("\r\n");

    for(i=0;i<=plainLen;i++)
    {
        Serial.print_long((long)plain[i],HEX);
    }
	Serial.print("\r\n");
    
    for(i=0;i<plainLen;i++)
    {
	    Serial.print_long((long)dencrypt[i],HEX);
    }
	Serial.print("\r\n");

    counter++;

    iv_val = iv[counter&0x0f] ^ counter;

    Serial.println_long((long)counter,HEX);
    Serial.println_long((long)iv[counter],HEX);
    Serial.println_long((long)iv_val,HEX);

    sleep(1000);
	return;
}
