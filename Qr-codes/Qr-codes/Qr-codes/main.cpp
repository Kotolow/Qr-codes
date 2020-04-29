#include <fstream>
#include <iostream>
#include <bitset>
#include <string>
#include <vector>
#include "dataEncoder.h"
using namespace std;

int main(int argc, char *argv[])
{
    string tx = "https://bitbucket.org/trbogdanov/qr-codes-9375/src/master/";
    DataEncoder *sequence = new DataEncoder(tx);
    cout << "Sequence of data bits with service information: " << endl;
    for (int n : sequence->sequenceOfBit)
    {
        cout << n;
    }
    cout << endl << endl << "Sequence of data bytes before processing: " << endl;
    for (int i = 0; i < sequence->sequenceOfByte.size(); ++i)
    {
        cout << sequence->sequenceOfByte[i] << ' ';
    }
    cout << endl << endl << "Final sequence of bytes: " << endl;
    for (int i = 0; i < sequence->sequenceFinal.size(); ++i)
    {
        cout << sequence->sequenceFinal[i] << ' ';
    }
    system("pause");
    return 0;
}
