

#include <iostream>
#include <string>

#include "dataEncoder.h"
using namespace std;

int main(int argc, char *argv[])
{
    std::string tx;
    cout << "Qr-code converter" << endl << endl;
    cout << "Enter the text you want to converte: " << endl;
    getline(cin, tx);
    DataEncoder sequence(tx);
    system("pause");
    return 0;
}
