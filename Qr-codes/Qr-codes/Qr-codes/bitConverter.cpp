#include <fstream>
#include <iostream>
#include <bitset>
#include <string>
using namespace std;

void binaryConverter(char tx[], bool *M, int amount)
/*функция предназначенная, для преобразования строки текста(текстового массива) в
строку бинарного кода (массив, содержащий значения бит)[пункт 1];
char[]tx - текст, который нужно переводить
bool *M - указатель на массив битов (через этот указатель изменения забираются из функции)
int amount - размер исодного текста*/
{
    int r = 0;
    int i = 0;
    for (r; r < amount; ++r)
    {
        for (int shift = 7; shift >= 0; --shift, ++i)
        {
            M[i] = tx[r] & (1 << shift);
        }
    }
}

int versionNumber(int amount)
{
    // эта функция определяет номер версии
    if (amount * 8 < 128)
        return 1;
    if (amount * 8 < 224)
        return 2;
    if (amount * 8 < 352)
        return 3;
    if (amount * 8 < 512)
        return 4;
    if (amount * 8 < 688)
        return 5;
    if (amount * 8 < 864)
        return 6;
    if (amount * 8 < 992)
        return 7;
    if (amount * 8 < 1232)
        return 8;
    if (amount * 8 < 1456)
        return 9;
    if (amount * 8 < 1728)
        return 10;
    if (amount * 8 < 2032)
        return 11;
    if (amount * 8 < 2320)
        return 12;
    if (amount * 8 < 2672)
        return 13;
    if (amount * 8 < 2920)
        return 14;
    if (amount * 8 < 3320)
        return 15;
    if (amount * 8 < 3624)
        return 16;
    if (amount * 8 < 4056)
        return 17;
    if (amount * 8 < 4504)
        return 18;
    if (amount * 8 < 5016)
        return 19;
    if (amount * 8 < 5352)
        return 20;
    if (amount * 8 < 5712)
        return 21;
    if (amount * 8 < 6256)
        return 22;
    if (amount * 8 < 6880)
        return 23;
    if (amount * 8 < 7312)
        return 24;
    if (amount * 8 < 8000)
        return 25;
    if (amount * 8 < 8496)
        return 26;
    if (amount * 8 < 9024)
        return 27;
    if (amount * 8 < 9544)
        return 28;
    if (amount * 8 < 10136)
        return 29;
    if (amount * 8 < 10984)
        return 30;
    if (amount * 8 < 11640)
        return 31;
    if (amount * 8 < 12328)
        return 32;
    if (amount * 8 < 13048)
        return 33;
    if (amount * 8 < 13800)
        return 34;
    if (amount * 8 < 14496)
        return 35;
    if (amount * 8 < 15312)
        return 36;
    if (amount * 8 < 15936)
        return 37;
    if (amount * 8 < 16816)
        return 38;
    if (amount * 8 < 17728)
        return 39;
    if (amount * 8 < 18672)
        return 40;
}
int bitInVersionF(int version)
{
    //функция для определения кол-ва бит в конечной цепочке
    int bitInVersion;
    switch (version)
    {
    case 1:
        bitInVersion = 128;
        break;
    case 2:
        bitInVersion = 224;
        break;
    case 3:
        bitInVersion = 352;
        break;
    case 4:
        bitInVersion = 512;
        break;
    case 5:
        bitInVersion = 688;
        break;
    case 6:
        bitInVersion = 864;
        break;
    case 7:
        bitInVersion = 992;
        break;
    case 8:
        bitInVersion = 1232;
        break;
    case 9:
        bitInVersion = 1456;
        break;
    case 10:
        bitInVersion = 1728;
        break;
    case 11:
        bitInVersion = 2032;
        break;
    case 12:
        bitInVersion = 2320;
        break;
    case 13:
        bitInVersion = 2672;
        break;
    case 14:
        bitInVersion = 2920;
        break;
    case 15:
        bitInVersion = 3320;
        break;
    case 16:
        bitInVersion = 3624;
        break;
    case 17:
        bitInVersion = 4056;
        break;
    case 18:
        bitInVersion = 4504;
        break;
    case 19:
        bitInVersion = 5016;
        break;
    case 20:
        bitInVersion = 5352;
        break;
    case 21:
        bitInVersion = 5712;
        break;
    case 22:
        bitInVersion = 6256;
        break;
    case 23:
        bitInVersion = 6880;
        break;
    case 24:
        bitInVersion = 7312;
        break;
    case 25:
        bitInVersion = 8000;
        break;
    case 26:
        bitInVersion = 8496;
        break;
    case 27:
        bitInVersion = 9024;
        break;
    case 28:
        bitInVersion = 9544;
        break;
    case 29:
        bitInVersion = 10136;
        break;
    case 30:
        bitInVersion = 10984;
        break;
    case 31:
        bitInVersion = 11640;
        break;
    case 32:
        bitInVersion = 12328;
        break;
    case 33:
        bitInVersion = 13048;
        break;
    case 34:
        bitInVersion = 13800;
        break;
    case 35:
        bitInVersion = 14496;
        break;
    case 36:
        bitInVersion = 15312;
        break;
    case 37:
        bitInVersion = 15936;
        break;
    case 38:
        bitInVersion = 16816;
        break;
    case 39:
        bitInVersion = 17728;
        break;
    case 40:
        bitInVersion = 18672;
        break;
    }
    return bitInVersion;
}
void intToBinary(int amount, int amountOfData, bool *amountOfDataMass)
{
    //Функция, которая переводит число из 10-ой С.С в 2-ую С.С
    // int amount - число, которое переводится
    // int amountOfData - предстваление числа, которое зависит от версии(8 или 16 битное)
    // bool amountOfDataMass - указатель на массив, через который забираются изменения из функции
    int i = amountOfData - 1;
    while (amount != 1)
    {
        amountOfDataMass[i] = amount % 2;
        amount = amount / 2;
        --i;
    }
    amountOfDataMass[i] = 1;
}
int addition1(bool *addition, int index)
{ //функции addition1 и addition2 предназначены для дополнения последовательности бит до полного
  //заполнения
    int i = index;
    addition[i] = 1;
    ++i;
    addition[i] = 1;
    ++i;
    addition[i] = 1;
    ++i;
    addition[i] = 0;
    ++i;
    addition[i] = 1;
    ++i;
    addition[i] = 1;
    ++i;
    addition[i] = 0;
    ++i;
    addition[i] = 0;
    ++i;
    return i;
}
int addition2(bool *addition, int index)
{
    int i = index;
    addition[i] = 0;
    ++i;
    addition[i] = 0;
    ++i;
    addition[i] = 0;
    ++i;
    addition[i] = 1;
    ++i;
    addition[i] = 0;
    ++i;
    addition[i] = 0;
    ++i;
    addition[i] = 0;
    ++i;
    addition[i] = 1;
    ++i;
    return i;
}

int main(int argc, char *argv[])
{
    char tx[] = "https://bitbucket.org/trbogdanov/qr-codes-9375/src/master/";
    int amount = sizeof(tx) - 1;
    bool *M = new bool[amount * 8];
    binaryConverter(tx, M, amount);
    int version = versionNumber(amount);
    int bitInVersion = bitInVersionF(version);
    int amountOfData;
    if (version <= 9)
    {
        amountOfData = 8;
    }
    else
    {
        amountOfData = 16;
    }
    if (amount * 8 + amountOfData + 4 > bitInVersion)
        ++version;
    if (version <= 9)
    {
        amountOfData = 8;
    }
    else
    {
        amountOfData = 16;
    }
    bool *amountOfDataMass = new bool[amountOfData];
    for (int i = 0; i < amountOfData; ++i)
    {
        amountOfDataMass[i] = 0;
    }
    intToBinary(amount, amountOfData, amountOfDataMass);
    bool *serviceInf = new bool[amount * 8 + 4 + amountOfData];
    serviceInf[0] = 0;
    serviceInf[1] = 1;
    serviceInf[2] = 0;
    serviceInf[3] = 0;
    for (int i = 4, j = 0; i < amountOfData + 4; ++i, ++j)
    {
        serviceInf[i] = amountOfDataMass[j];
    }
    delete[] amountOfDataMass;
    for (int i = amountOfData + 4, j = 0; i < amount * 8 + amountOfData + 4; ++i, ++j)
    {
        serviceInf[i] = M[j];
    }
    delete[] M;
    int amountOfBits = amount * 8 + amountOfData + 4;
    if (amountOfBits % 8 != 0)
    {
        amountOfBits += amountOfBits % 8;
    }
    bool *numOfBit = new bool[amountOfBits];
    for (int i = 0; i < amountOfBits; ++i)
    {
        numOfBit[i] = 0;
    }
    for (int i = 0; i < amount * 8 + amountOfData + 4; ++i)
    {
        numOfBit[i] = serviceInf[i];
    }
    delete[] serviceInf;
    int difference = (bitInVersion - amountOfBits) / 8;
    int indexOfDifference = 0;
    bool *addition = new bool[difference * 8];
    while (difference > 0)
    {
        if (difference % 2 != 0)
        {
            indexOfDifference = addition1(addition, indexOfDifference);
            --difference;
        }
        else
        {
            indexOfDifference = addition2(addition, indexOfDifference);
            --difference;
        }
    }
    bool *finalSequence = new bool[bitInVersion];
    for (int i = 0; i < amountOfBits; ++i)
    {
        finalSequence[i] = numOfBit[i];
    }
    delete[] numOfBit;
    for (int i = amountOfBits, j = 0; i < bitInVersion; ++i, ++j)
    {
        finalSequence[i] = addition[j];
    }
    for (int i = 0; i < bitInVersion; ++i)
    {
        cout << finalSequence[i];
    }
    delete[] addition;
    delete[] finalSequence;
    system("pause");
    return 0;
}
