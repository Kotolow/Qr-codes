

#include <iostream>
#include <string>

#include "dataEncoder.h"
using namespace std;

int main(int argc, char *argv[])
{
    std::string tx = "https://bitbucket.org/trbogdanov/qr-codes-9375/src/master/";

    DataEncoder sequence(tx);
    cout << "Final sequence of bytes: " << endl;
    for (int i = 0; i < sequence.sequenceFinal.size(); ++i)
    {
        cout << sequence.sequenceFinal[i] << ' ';
    }
    cout << endl << sequence.sizeOfCanvas;
    cout << endl << endl << "Qr-code:" << endl << endl;
    for (int i = 0; i < sequence.sizeOfCanvas; ++i)
    {
        for (int j = 0; j < sequence.sizeOfCanvas; ++j)
        {
            cout << sequence.qrCode[i][j] << ' ';
        }
        cout << endl;
    }
    system("pause");
    return 0;
}