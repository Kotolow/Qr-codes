#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include "dataEncoder.h"
using namespace std;

DataEncoder::DataEncoder(const std::string &tx)
{
    amount = tx.size();
    binaryConverter(tx, sequenceOfBit, amount);
    amountOfBits = amount * 8;
    version = versionNumber(amountOfBits, maxAmountOfBits, amountOfBlocks);
    if (version <= 9)
    {
        amountOfData = 8;
    }
    else
    {
        amountOfData = 16;
    }
    if (amountOfBits + amountOfData + 4 > maxAmountOfBits)
        ++version;
    if (version <= 9)
    {
        amountOfData = 8;
    }
    else
    {
        amountOfData = 16;
    }
    intToBinary(amount, amountOfData, sequenceOfBit); //добавление битов кол-ва информации
                                                      //добваление битов способа кодирования
    sequenceOfBit.insert(sequenceOfBit.begin(), 0);
    sequenceOfBit.insert(sequenceOfBit.begin(), 0);
    sequenceOfBit.insert(sequenceOfBit.begin(), 1);
    sequenceOfBit.insert(sequenceOfBit.begin(), 0);
    while (sequenceOfBit.size() % 8 != 0)
    {
        sequenceOfBit.push_back(0);
    }
    difference = (maxAmountOfBits - sequenceOfBit.size()) / 8;
    for (int i = 1; i <= difference; ++i)
    {
        if (i % 2 != 0)
        {
            addition1();
        }
        else
        {
            addition2();
        }
    }
    byteInBlock = (sequenceOfBit.size() / 8) / amountOfBlocks;
    additionalBlocks = (sequenceOfBit.size() / 8) % amountOfBlocks;
    bitToIntConverter(sequenceOfBit, sequenceOfByte, maxAmountOfBits);
    blockFiller(sequenceOfByte, blocks, amountOfBlocks, byteInBlock, additionalBlocks);
}
void DataEncoder::binaryConverter(const std::string &tx, vector<bool> &sequenceOfBit, int amount)
/*функция предназначенная, для преобразования строки текста(текстового массива) в
строку бинарного кода (массив, содержащий значения бит)[пункт 1];
string tx - текст, который нужно переводить
vector<bool> &sequenceOfBit - ссылка на массив битов (через ссылку изменения забираются из
функции) int amount - размер исодного текста*/
{
    int r = 0;
    int i = 0;
    for (r; r < amount; ++r)
    {
        for (int shift = 7; shift >= 0; --shift, ++i)
        {
            sequenceOfBit.push_back(tx[r] & (1 << shift));
        }
    }
}
int DataEncoder::versionNumber(int amountOfBits, int &maxAmountOfBits, int &amountOfBlocks)
{
    // эта функция определяет номер версии
    if (amountOfBits < 128)
    {
        maxAmountOfBits = 128;
        amountOfBlocks = 1;
        return 1;
    }

    else if (amountOfBits < 224)
    {
        maxAmountOfBits = 224;
        amountOfBlocks = 1;
        return 2;
    }
    else if (amountOfBits < 352)
    {
        maxAmountOfBits = 352;
        amountOfBlocks = 1;
        return 3;
    }
    else if (amountOfBits < 512)
    {
        maxAmountOfBits = 512;
        amountOfBlocks = 2;
        return 4;
    }
    else if (amountOfBits < 688)
    {
        maxAmountOfBits = 688;
        amountOfBlocks = 2;
        return 5;
    }
    else if (amountOfBits < 864)
    {
        maxAmountOfBits = 864;
        amountOfBlocks = 4;
        return 6;
    }
    else if (amountOfBits < 992)
    {
        maxAmountOfBits = 992;
        amountOfBlocks = 4;
        return 7;
    }
    else if (amountOfBits < 1232)
    {
        maxAmountOfBits = 1232;
        amountOfBlocks = 4;
        return 8;
    }
    else if (amountOfBits < 1456)
    {
        maxAmountOfBits = 1456;
        amountOfBlocks = 5;
        return 9;
    }
    else if (amountOfBits < 1728)
    {
        maxAmountOfBits = 1728;
        amountOfBlocks = 5;
        return 10;
    }
    else if (amountOfBits < 2032)
    {
        maxAmountOfBits = 2032;
        amountOfBlocks = 5;
        return 11;
    }
    else if (amountOfBits < 2320)
    {
        maxAmountOfBits = 2320;
        amountOfBlocks = 8;
        return 12;
    }
    else if (amountOfBits < 2672)
    {
        maxAmountOfBits = 2672;
        amountOfBlocks = 9;
        return 13;
    }
    else if (amountOfBits < 2920)
    {
        maxAmountOfBits = 2920;
        amountOfBlocks = 9;
        return 14;
    }
    else if (amountOfBits < 3320)
    {
        maxAmountOfBits = 3320;
        amountOfBlocks = 10;
        return 15;
    }
    else if (amountOfBits < 3624)
    {
        maxAmountOfBits = 3624;
        amountOfBlocks = 10;
        return 16;
    }
    else if (amountOfBits < 4056)
    {
        maxAmountOfBits = 4056;
        amountOfBlocks = 11;
        return 17;
    }
    else if (amountOfBits < 4504)
    {
        maxAmountOfBits = 4504;
        amountOfBlocks = 13;
        return 18;
    }
    else if (amountOfBits < 5016)
    {
        maxAmountOfBits = 5016;
        amountOfBlocks = 14;
        return 19;
    }
    else if (amountOfBits < 5352)
    {
        maxAmountOfBits = 5352;
        amountOfBlocks = 16;
        return 20;
    }
    else if (amountOfBits < 5712)
    {
        maxAmountOfBits = 5712;
        amountOfBlocks = 17;
        return 21;
    }
    else if (amountOfBits < 6256)
    {
        maxAmountOfBits = 6256;
        amountOfBlocks = 17;
        return 22;
    }
    else if (amountOfBits < 6880)
    {
        maxAmountOfBits = 6880;
        amountOfBlocks = 18;
        return 23;
    }
    else if (amountOfBits < 7312)
    {
        maxAmountOfBits = 7312;
        amountOfBlocks = 20;
        return 24;
    }
    else if (amountOfBits < 8000)
    {
        maxAmountOfBits = 8000;
        amountOfBlocks = 21;
        return 25;
    }
    else if (amountOfBits < 8496)
    {
        maxAmountOfBits = 8496;
        amountOfBlocks = 23;
        return 26;
    }
    else if (amountOfBits < 9024)
    {
        maxAmountOfBits = 9024;
        amountOfBlocks = 25;
        return 27;
    }
    else if (amountOfBits < 9544)
    {
        maxAmountOfBits = 9544;
        amountOfBlocks = 26;
        return 28;
    }
    else if (amountOfBits < 10136)
    {
        maxAmountOfBits = 10136;
        amountOfBlocks = 28;
        return 29;
    }
    else if (amountOfBits < 10984)
    {
        maxAmountOfBits = 10984;
        amountOfBlocks = 29;
        return 30;
    }
    else if (amountOfBits < 11640)
    {
        maxAmountOfBits = 11640;
        amountOfBlocks = 31;
        return 31;
    }
    else if (amountOfBits < 12328)
    {
        maxAmountOfBits = 12328;
        amountOfBlocks = 33;
        return 32;
    }
    else if (amountOfBits < 13048)
    {
        maxAmountOfBits = 13048;
        amountOfBlocks = 35;
        return 33;
    }
    else if (amountOfBits < 13800)
    {
        maxAmountOfBits = 13800;
        amountOfBlocks = 37;
        return 34;
    }
    else if (amountOfBits < 14496)
    {
        maxAmountOfBits = 14496;
        amountOfBlocks = 38;
        return 35;
    }
    else if (amountOfBits < 15312)
    {
        maxAmountOfBits = 15312;
        amountOfBlocks = 40;
        return 36;
    }
    else if (amountOfBits < 15936)
    {
        maxAmountOfBits = 15936;
        amountOfBlocks = 43;
        return 37;
    }
    else if (amountOfBits < 16816)
    {
        maxAmountOfBits = 16816;
        amountOfBlocks = 45;
        return 38;
    }
    else if (amountOfBits < 17728)
    {
        maxAmountOfBits = 17728;
        amountOfBlocks = 47;
        return 39;
    }
    else if (amountOfBits < 18672)
    {
        maxAmountOfBits = 18672;
        amountOfBlocks = 49;
        return 40;
    }
}
void DataEncoder::intToBinary(int amount, int amountOfData, vector<bool> &serviceInf)
{
    //Функция, которая переводит число из 10-ой С.С в 2-ую С.С
    // int amount - число, которое переводится
    // int amountOfData - предстваление числа, которое зависит от версии(8 или 16 битное)
    // bool amountOfDataMass - указатель на массив, через который забираются изменения из
    // функции
    int currentNum = 0;
    while (amount != 1)
    {
        serviceInf.insert(serviceInf.begin(), amount % 2);
        amount = amount / 2;
        ++currentNum;
    }
    serviceInf.insert(serviceInf.begin(), 1);
    ++currentNum;
    for (int i = currentNum; i < amountOfData; ++i)
    {
        serviceInf.insert(serviceInf.begin(), 0);
    }
}
void DataEncoder::addition1()
{
    sequenceOfBit.push_back(1);
    sequenceOfBit.push_back(1);
    sequenceOfBit.push_back(1);
    sequenceOfBit.push_back(0);
    sequenceOfBit.push_back(1);
    sequenceOfBit.push_back(1);
    sequenceOfBit.push_back(0);
    sequenceOfBit.push_back(0);
}
void DataEncoder::addition2()
{
    sequenceOfBit.push_back(0);
    sequenceOfBit.push_back(0);
    sequenceOfBit.push_back(0);
    sequenceOfBit.push_back(1);
    sequenceOfBit.push_back(0);
    sequenceOfBit.push_back(0);
    sequenceOfBit.push_back(0);
    sequenceOfBit.push_back(1);
}
void DataEncoder::bitToIntConverter(const vector<bool> &sequenceOfBit, vector<int> &sequenceOfByte,
                                    int maxAmountOfBits)
{ //функция для перевода последовательности бит в последовательность байт
    int bitIndex = 0;
    int sum;
    for (int j = 0; j < maxAmountOfBits / 8; ++j)
    {
        sum = 0;
        for (int i = 0; i < 8; ++i, ++bitIndex)
        {
            sum += (sequenceOfBit[bitIndex]) * (pow(2, 7 - i));
        }
        sequenceOfByte.push_back(sum);
    }
}
void DataEncoder::blockFiller(const vector<int> &sequenceOfByte, vector<vector<int> > &blocks,
                              int amountOfBlocks, int byteInBlock, int additionalBlocks)
{ //Функция для заполнения блоков последовательностью байт
    int byteIndex = 0; //индекс заполнения байтов
    int blockIndex; //индекс заполнения блоков
    blocks.resize(amountOfBlocks);
    for (blockIndex = 0; blockIndex < amountOfBlocks - additionalBlocks; ++blockIndex)
    {
        blocks[blockIndex].resize(byteInBlock);
        for (int i = 0; i < byteInBlock; ++i, ++byteIndex)
        {
            blocks[blockIndex][i] = sequenceOfByte[byteIndex];
        }
    }
    for (; blockIndex < amountOfBlocks; ++blockIndex)
    {
        blocks[blockIndex].resize(byteInBlock + 1);
        for (int i = 0; i < byteInBlock + 1; ++i, ++byteIndex)
        {
            blocks[blockIndex][i] = sequenceOfBit[byteIndex];
        }
    }
}
