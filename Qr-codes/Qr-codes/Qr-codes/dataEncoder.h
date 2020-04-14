#pragma once
#include <fstream>
#include <iostream>
#include <bitset>
#include <string>
#include <math.h>
#include <vector>
using namespace std;

class dataEncoder
{
  public:
    vector<bool> sequenceOfBit; //последовательность бит
    vector<int> sequenceOfByte; //последовательность байт для блоков коррекции
    dataEncoder(string tx)
    {
        amount = size(tx);
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
                addition1(sequenceOfBit);
            }
            else
            {
                addition2(sequenceOfBit);
            }
        }
        byteInBlock = (sequenceOfBit.size() / 8) / amountOfBlocks;
        additionalBlocks = (sequenceOfBit.size() / 8) % amountOfBlocks;
        bitToIntConverter(sequenceOfBit, sequenceOfByte, maxAmountOfBits);
        blockFiller(sequenceOfByte, blocks, amountOfBlocks, byteInBlock, additionalBlocks);
    }
    ~dataEncoder()
    {
    }

  private:
    int amount; //кол-во символов в тексте
    int version; //номер версии
    int amountOfBits; //кол-во бит в тексте
    int maxAmountOfBits; //максимальное кол-во бит, ограниченное версией
    int amountOfData; //представление размера информации(8 или 16 бит, зависит от версии)
    int difference; //различие между текущим кол-вом бит и максимальным
    int amountOfBlocks; //кол-во блоков битов
    int byteInBlock; //кол-во бит в нормальном блоке
    int additionalBlocks; //кол-во дополненных блоков(бит на 1 больше чем в обычном)
    vector<vector<int> > blocks; //массив блоков
                                 //прототипы функций:
    void binaryConverter(string tx, vector<bool> &sequenceOfBit, int amount);
    int versionNumber(int amountOfBits, int &maxAmountOfBits, int &amountOfBlocks);
    void intToBinary(int amount, int amountOfData, vector<bool> &serviceInf);
    void addition1(vector<bool> &sequence);
    void addition2(vector<bool> &sequence);
    void bitToIntConverter(vector<bool> sequenceOfBit, vector<int> &sequenceOfByte,
                           int maxAmountOfBits);
    void blockFiller(vector<int> sequenceOfByte, vector<vector<int> > blocks, int amountOfBlocks,
                     int byteInBlock, int additionalBlocks);
};
