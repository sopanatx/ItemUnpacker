#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <fstream>
#include <windows.h>
#include <io.h>
#include "obfuscator.hpp"
#include "fileapi.h"
#define LOWORD(l)
using namespace std;

int ITM_HEADER1 = 0xE0F0A0B0; // weak
int ITM_HEADER2 = 0xA0B0C0D0;
int PRIVATE_KEY = 0x0EBD66B29;
int PUBLIC_KEY = 0x2103A92C;
const uint32_t crc32_tab[256] = {
	0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f,
	0xe963a535, 0x9e6495a3,	0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
	0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91, 0x1db71064, 0x6ab020f2,
	0xf3b97148, 0x84be41de,	0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
	0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec,	0x14015c4f, 0x63066cd9,
	0xfa0f3d63, 0x8d080df5,	0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
	0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b,	0x35b5a8fa, 0x42b2986c,
	0xdbbbc9d6, 0xacbcf940,	0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
	0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423,
	0xcfba9599, 0xb8bda50f, 0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
	0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d,	0x76dc4190, 0x01db7106,
	0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
	0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d,
	0x91646c97, 0xe6635c01, 0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
	0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457, 0x65b0d9c6, 0x12b7e950,
	0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
	0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7,
	0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
	0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9, 0x5005713c, 0x270241aa,
	0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
	0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81,
	0xb7bd5c3b, 0xc0ba6cad, 0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
	0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683, 0xe3630b12, 0x94643b84,
	0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
	0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb,
	0x196c3671, 0x6e6b06e7, 0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
	0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5, 0xd6d6a3e8, 0xa1d1937e,
	0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
	0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55,
	0x316e8eef, 0x4669be79, 0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
	0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 0xc5ba3bbe, 0xb2bd0b28,
	0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
	0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f,
	0x72076785, 0x05005713, 0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
	0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21, 0x86d3d2d4, 0xf1d4e242,
	0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
	0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69,
	0x616bffd3, 0x166ccf45, 0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
	0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db, 0xaed16a4a, 0xd9d65adc,
	0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
	0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693,
	0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
	0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
};


int  sub_AD7D00(DWORD* a4)
{
	int result; // eax
	int v2; // edx

	result = *a4 ^ a4[2];
	v2 = a4[1] ^ a4[3];
	a4[4] = result;
	a4[5] = v2;
	return result;
}


int  sub_AD81D0(DWORD* a1, int a2, int a3)
{
	a1[2] = a2;
	a1[3] = a3;
	return sub_AD7D00(a1);
}






unsigned int Checksum(int a1,int a2, unsigned int a3) {
	unsigned int result;
	int i;
	result = a3;
	for (i = 0; i < a2; ++i)
		result = crc32_tab[(unsigned __int8)result ^ *(unsigned __int8*)(i + a1)] ^ (result >> 8);
	return result;
}

int DecryptFile1(void* a0, int a1, int a2, int a3) {
	int v4;
	int result;

	if (Checksum(a3, a1 - 4, 0xFFFFFFFF) == v4)
		result = a1 - 4;
	else
		result = -1;
	return result;
}

int  sub_AD7F10(int a2, int a3, int a4)
{
	int result; // eax
	int v5; // edx
	int v6; // esi
	BYTE* v7; // edi
	int v8; // eax
	char v9; // cl
	int v11; // [esp+18h] [ebp+Ch]
	FILE* v12; // edi
	(result) = -99 * (char)a2;
	v5 = 0;
	v6 = 2157;
	if (a2 > 0)
	{
		v11 = a3 - a4;
		do
		{
			v7 = (BYTE*)(v5 + a4);
			v8 = v6;
			v6 *= 2171;
			result = v8 >> 8;
			v9 = (-99 * a2) ^ result ^ *(BYTE*)(v11 + v5 + a4) ^ (v5 & 7) + 16;
			++v5;
			*v7 = v9;
		}     while (v5 < a2);
	}
	v12 = fopen("FCTemp", "wb");
	fwrite((void*)result, a2 - 20, 1u, v12);
	fclose(v12);
	return result;
}


int  sub_AD80D0(int a1, int a2, int a3)
{
	int v3; // esi
	int result; // eax
	int v5; // [esp+14h] [ebp+8h]

	v3 = a1 - 4;
	sub_AD7F10(a1, a2, a3);
	v5 = (DWORD)(a1 - 4 + a3);
	if (Checksum(a3, v3, -1) == v5)
		result = v3;
	else
		result = -1;
	return result;
}



void LogWriter(int LogType) {
	ofstream myfile;
	myfile.open(OBFUSCATE("LogData.txt"));
	myfile << OBFUSCATE("Log Intialized!");

	switch (LogType) {
	case 1:
		myfile << OBFUSCATE("[LOG]: Error Reading item file");
		cout << OBFUSCATE("[LOG]: Error Reading item file");

	case 2:
		myfile << OBFUSCATE("[LOG]: Processing file...\n");
		cout << OBFUSCATE("[LOG]: Processing file...\n");
	case 3:
		myfile << OBFUSCATE("[LOG]: Reading 8 Byte...\n");
		cout << OBFUSCATE("[LOG]: Reading 8 Byte...\n");


	}
	myfile.close();
}

int DecryptFile(void *a0,int a1,int a2,int a3 ){
	int v4;
	int result;

	return 0;
}



int fileread() {
	FILE* v4;
	FILE* v5;
	int v7;
	__int64 v8; // rax
	int v9;
	int v10;
	void* v11; // ebx
	FILE* v12; // edi
	size_t v13; // [esp-28h] [ebp-4Ch]
	int v14[2];
	__int64 v16; // [esp+18h] [ebp-Ch]
	int Buffer[2];
	int payload1[4] = { 0xB0,0xA0,0xF0,0xE0 };
	void* v17; // [esp+20h] [ebp-4h]
	int FileNamea; // [esp+2Ch] [ebp+8h]
	BYTE* a1;
	v4 = fopen(OBFUSCATE("./Table/item.itm"), "rb");
	v7 = _fileno(v4);
	v8 = _filelengthi64(v7);
	if (!v7 || v8) {
		MessageBoxA(0, OBFUSCATE("Processing Failed!"), "Message", 0);
		exit(3);
	}
	v9 = v8;
	v16 = v8;
	v5 = v4;
	if (!v5) {

		LogWriter(1);
		MessageBoxA(0, OBFUSCATE("item table read failed"), "Message", 0);
		exit(3);
	}
	else {
		cout << OBFUSCATE("[LOG]: Reading...\n");
		fread(Buffer, 8, 1, v5);
		LogWriter(3);

		if (Buffer[0] == ITM_HEADER1 && Buffer[1] == ITM_HEADER2) {
			v10 = v9 - 16;
			v17 = operator new(v10);
			FileNamea = v10;
			LogWriter(2);
			fread(v14, 8, 1, v5); //READ 8BYTE
			v13 = v10;
			v11 = v17;
			fread(v17, v13, 1u, v5);
			//sub_AD81D0(PRIVATE_KEY[2 * 2], PUBLIC_KEY[2 * 2]);
		//	sub_AD81D0(v14[0], v14[1]);
			sub_AD80D0(v16 - 16, (int)v11, FileNamea);
			LogWriter(3);
			fclose(v5);
			
		}
		else {

			MessageBoxA(0, OBFUSCATE("File Header is invalid."), "Error", 0);
			exit(3);
		}

	}




	return 0;


}

void writelog()

{
	int dword_89B280[] = { 0x29,0x6B,0xD6,0xEB }; // weak
	int dword_89B284[] =
	{
	0x2C, 0xA9, 0x03, 0x21, 0x91, 0x3E, 0x2A, 0x85, 0xCB, 0x4C, 0x3D, 0x1B, 0xF2, 0x32, 0xD1, 0x72, 0xA8, 0xB0, 0xE3, 0x45, 0xD4, 0x12, 0xA2, 0x82, 0x67, 0x4B, 0x13, 0x66, 0xF6, 0x25, 0x87, 0x32, 0xA4, 0x1A, 0xB2, 0x7A, 0xB1, 0x62, 0xE3, 0x1B, 0x4A, 0x6C, 0x9A, 0x35, 0x72, 0x61, 0x74, 0x73, 0x79, 0x73, 0x61, 0x65
	}; // idb
	SYSTEMTIME st, lt;

	GetSystemTime(&st);
	GetLocalTime(&lt);



	fileread();
	//int a1 = dword_89B280[2 % 8];
   // cout << "a1: " + a1;
	//   MessageBoxA(0, a1, "Message", 0);

}





int main()

{
	cout << OBFUSCATE("=========== GhostOnline Item Unpacker  V.1.1.0 Beta  [Powered By WasuthornX]  =========== \n");
	cout << OBFUSCATE("[LOG]: Loading item file ....\n");

	writelog();
	std::cin.get();
	return 0;
}