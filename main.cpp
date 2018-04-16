#include <QCoreApplication>
#include <fstream>
#include <conio.h>
#include <iostream>
#include<zlib.h>
#include <string>
\


#include <algorithm>

#include <sstream>
#include <stdint.h>
#include <intrin.h>
#include <vector>
using namespace std;
//using System;


int main()
{
    int signLength=8;
    string str1;
    cin>>str1;


    ifstream InStream(str1, ios::binary);
    if (!InStream.is_open()) {
                cout<<"error";
          return 2;
    }


    char sign[signLength+1];
    memset(sign,0,signLength+1);

    InStream.read((char*)sign,8);
    if (strcmp((char*)sign, "\x89\x50\x4E\x47\x0D\x0A\x1A\x0A") != 0) {
        cout<<"error";
     return 3;
    }


    InStream.seekg(-12, ios_base::end);
    unsigned char iENDSign[13] = { 0 };
    InStream.read((char *)iENDSign, 12);
    if (strcmp((char*)iENDSign, "\x00\x00\x00\x00\x49\x45\x4E\x44\xAE\x42\x60\x82") != 0) {
            cout<<"error";
        return 4;
    }



    InStream.seekg(8, ios_base::beg); //seek to file beginning
    string str;
    string trueIDAT = "IDAT";
    vector <int> chunkspos;
    vector <int> chunklenght;
    vector <string> chunkstr;

    while (!InStream.eof()){
        str+= InStream.get();

        if (str == trueIDAT) {
            int tempChunkpos=0;
            int tempChunklength=0;
            string tempChunkstr;
            InStream.seekg(- signLength , ios_base::cur);
            tempChunkpos = InStream.tellg();
            char tempBuff[4] = { 0 };
            InStream.read((char *)tempBuff, 4);


          tempChunklength+= tempBuff[3]*512;
           tempChunklength+= tempBuff[2]*256;
            tempChunklength+= tempBuff[1]*128;
            tempChunklength+= tempBuff[0]*64;

            InStream.seekg(tempChunklength + 4, ios_base::cur);
            InStream.read((char *)tempBuff, 4);
            tempChunkstr =  string(tempBuff);


            chunklenght.push_back(tempChunklength);
            chunkspos.push_back(tempChunkpos);
            chunkstr.push_back(tempChunkstr);
            str.clear();
        }

        for (int i(0); i < str.length() && i < trueIDAT.length(); i++) {
            if (str[i] != trueIDAT[i]) {
                str.clear();
            }
        }
    }


    if (chunklenght.size()==0) {
     cout<<"error";
        return 5;
    }

    for(int i=0;i<chunklenght.size()-1;i++){
        cout << chunkspos[i] << ' ';
        cout << chunklenght[i] << ' ';
        cout << chunkstr [i] << endl;
    }
    return 0;
}
