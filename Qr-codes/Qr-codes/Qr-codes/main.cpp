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
    system("pause");
    return 0;
}
