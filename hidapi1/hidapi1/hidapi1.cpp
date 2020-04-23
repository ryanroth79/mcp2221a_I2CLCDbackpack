// hidapi1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <stdlib.h>
#include "hidapi.h"

// Headers needed for sleeping.
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#define MAX_STR 255

void writec(char, char, hid_device*);
void writed(char, char, hid_device*);
void writeLetter(char, hid_device*);
void readResp(hid_device*);

int main(int argc, char* argv[])
{
	int res;
	unsigned char buf[65];
	int q=0;
	wchar_t wstr[MAX_STR];
	hid_device* handle;
	int i;
	char c;

	// Initialize the hidapi library
	res = hid_init();

	// Open the device using the VID, PID,
	// and optionally the Serial number.
	handle = hid_open(0x04d8, 0xdd, NULL);

	// Read the Manufacturer String
	res = hid_get_manufacturer_string(handle, wstr, MAX_STR);
	wprintf(L"Manufacturer String: %s\n", wstr);

	// Read the Product String
	res = hid_get_product_string(handle, wstr, MAX_STR);
	wprintf(L"Product String: %s\n", wstr);

	// Read the Serial Number String
	res = hid_get_serial_number_string(handle, wstr, MAX_STR);
	wprintf(L"Serial Number String: (%d) %s\n", wstr[0], wstr);

	// Read Indexed String 1
	res = hid_get_indexed_string(handle, 1, wstr, MAX_STR);
	wprintf(L"Indexed String 1: %s\n", wstr);

	writec(0x4E, 0x03, handle);
	Sleep(10);
	writec(0x4E, 0x03, handle);
	Sleep(1);
	writec(0x4E, 0x03, handle);

	writec(0x4E, 0x02, handle); 
	writec(0x4E, 0x02, handle);

	writec(0x4E, 0x08, handle);

	writec(0x4E, 0x00, handle);
	writec(0x4E, 0x08, handle);

	writec(0x4E, 0x00, handle);
	writec(0x4E, 0x01, handle);

	writec(0x4E, 0x00, handle);
	writec(0x4E, 0x0E, handle);

	writeLetter('h', handle);
	writeLetter('e', handle);
	writeLetter('l', handle);
	writeLetter('l', handle);
	writeLetter('o', handle);
	writeLetter(' ', handle);
	writeLetter('W', handle);
	writeLetter('o', handle);
	writeLetter('r', handle);
	writeLetter('l', handle);
	writeLetter('D', handle);
	writeLetter(0xE4, handle);
	writeLetter(0xF4, handle);

	hid_close(handle);
	hid_exit();

	return 0;
}

void writec(char addr, char val, hid_device* handle) {
	unsigned char buf[65];
	int q, i;
	char v = 0;

	buf[0] = 0;
	buf[1] = 0x90;
	buf[2] = 0x1;
	buf[3] = 0x0;
	buf[4] = 0x4E;
	v = (val << 4 | 0x08) & 0xFF;
	printf("0x%x\t", v);
	buf[5] = v;
	hid_write(handle, buf, 65);
	Sleep(10);
	v = (val << 4 | 0x0C) & 0xFF;
	printf("0x%x\n", v);
	buf[5] = v;
	hid_write(handle, buf, 65);
	Sleep(10);
	v = (val << 4 | 0x08) & 0xFF;
	printf("0x%x\t", v);
	buf[5] = v;
	hid_write(handle, buf, 65);
	Sleep(10);
}

void writed(char addr, char val, hid_device* handle) {
	unsigned char buf[65];

	buf[0] = 0;
	buf[1] = 0x90;
	buf[2] = 0x1;
	buf[3] = 0x0;
	buf[4] = 0x4E;
	buf[5] = (val<<4) | 0x09;
	hid_write(handle, buf, 65);
	Sleep(1);
	buf[5] = (val<<4) | 0x0D;
	hid_write(handle, buf, 65);
	Sleep(1);
	buf[5] = (val << 4) | 0x09;
	hid_write(handle, buf, 65);
	Sleep(1);
}

void writeLetter(char c, hid_device* handle) {
	writed(0x4E, c >> 4 & 0xF, handle);
	writed(0x4E, c & 0xF, handle);
}

void readResp(hid_device* handle) {
	int q, i = 0;
	unsigned char buf[65];

	for (q = 0; q < 65; q++) {
		buf[q] = 0;
	}
	hid_read(handle, buf, 65);

	// Print out the returned buffer.
	for (i = 0; i < 4; i++)
		printf("buf[%d]: 0x%x\n", i, buf[i]);
	for (q = 0; q < 65; q++) {
		buf[q] = 0;
	}
}