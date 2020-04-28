#pragma once
using namespace std;

class DataEncoder
{
  public:
    vector<bool> sequenceOfBit; //последовательность бит
    vector<int> sequenceOfByte; //последовательность байт для блоков коррекции
    DataEncoder(const string &tx);
    ~DataEncoder() = default;

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
    void addition1();
    void addition2();
    void bitToIntConverter(vector<bool> sequenceOfBit, vector<int> &sequenceOfByte,
                           int maxAmountOfBits);
    void blockFiller(vector<int> sequenceOfByte, vector<vector<int> > blocks, int amountOfBlocks,
                     int byteInBlock, int additionalBlocks);
};
