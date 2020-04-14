﻿#include <fstream>
#include <iostream>
#include <bitset>
#include <string>
#include <vector>
#include "dataEncoder.h"
using namespace std;

int main(int argc, char *argv[])
{
    string tx = "https://bitbucket.org/trbogdanov/qr-codes-9375/src/master/";
    dataEncoder *sequence = new dataEncoder(tx);
    for (int n : sequence->sequenceOfBit)
    {
        cout << n;
    }
    cout << endl;
    for (int i = 0; i < sequence->sequenceOfByte.size(); ++i)
    {
        cout << sequence->sequenceOfByte[i] << ' ';
    }
    system("pause");
    return 0;
}
