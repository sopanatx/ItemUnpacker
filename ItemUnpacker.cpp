#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <windows.h>
#include <io.h>
using namespace std;

int ITM_HEADER1 = 0xE0F0A0B0; // weak
int ITM_HEADER2 = 0xA0B0C0D0;
int fileread() {
    FILE *v4;
    FILE *v5;
    __int64 v7;
    int v14[2];
    int Buffer[2];
    int payload1[4] = { 0xB0,0xA0,0xF0,0xE0 };
    ofstream myfile;
    myfile.open("LogData.txt");
    myfile << "Log Intialized!";


    v4 = fopen("./Table/item.itm", "rb");

    v5 = v4;
    if (!v5) {
        myfile << "[LOG]: Error Reading item file";
        cout << "[LOG]: Error Reading item file\n";
        MessageBoxA(0, "item table read failed", "Message", 0);

    }
    else {
        cout << "[LOG]: Reading...\n";
        fread(Buffer, 8, 1, v5);
        if (Buffer[0] == ITM_HEADER1 && Buffer[1] == ITM_HEADER2) {

            cout << "[LOG]: Processing file...\n";
            cout << "[LOG]: Reading 8 Byte...\n";
            fread(v14, 8, 1, v5);
        }
        else {
            MessageBoxA(0, "File Header is invalid.", "Error", 0);

        }
  
    }



    myfile.close();
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
    cout << "=========== GhostOnline Item Unpacker  V.1.1.0 Beta  [Powered By WasuthornX]  =========== \n";
    cout << "[LOG]: Loading item file ....\n";

    writelog();
    std::cin.get();
    return 0;
}