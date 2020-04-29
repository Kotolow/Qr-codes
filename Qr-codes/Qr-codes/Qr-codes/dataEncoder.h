#pragma once
using namespace std;

class DataEncoder
{
  public:
    vector<bool> sequenceOfBit; //последовательность бит
    vector<int> sequenceOfByte; //последовательность байт для блоков коррекции
    vector<int> sequenceFinal; // конечная последовательность байт
    DataEncoder(const string &tx);
    ~DataEncoder() = default;

  private:
    int amount; //кол-во символов в тексте
    int version; //номер версии
    int amountOfBits; //кол-во бит в тексте
    int maxAmountOfBits; //максимальное кол-во бит, ограниченное версией
    int amountOfData; //представление размера информации(8 или 16 бит, зависит от версии)
    int difference; //различие между текущим кол-вом бит и максимальным
    int amountOfBlocks; //кол-во блоков байт
    int byteInBlock; //кол-во байт в нормальном блоке
    int additionalBlocks; //кол-во дополненных блоков(бит на 1 больше чем в обычном)
    int amountOfCorrectionBytes; //кол-во байтов коррекции
    vector<vector<int>> blocks; //массив блоков
    vector<int> generatingPolynom; //генерирующий многочлен
    vector<vector<int>> correctionByteBlocks; //блоки байтов коррекции
                                              //прототипы функций:
    void binaryConverter(string tx, vector<bool> &sequenceOfBit, int amount);
    int versionNumber(int amountOfBits, int &maxAmountOfBits, int &amountOfBlocks,
                      int &amountOfCorrectionBytes);
    void intToBinary(int amount, int amountOfData, vector<bool> &serviceInf);
    void addition1();
    void addition2();
    void bitToIntConverter(vector<bool> sequenceOfBit, vector<int> &sequenceOfByte,
                           int maxAmountOfBits);
    void blockFiller(vector<int> sequenceOfByte, vector<vector<int>> &blocks, int amountOfBlocks,
                     int byteInBlock, int additionalBlocks);
    void polynomGenerator(vector<int> &generatingPolynom);
    int galua(int num);
    int galuaReverse(int num);
    void correctionByteGenerator(vector<vector<int>> &sCorrectionByteBlocks,
                                 vector<vector<int>> blocks, vector<int> generatingPolynom);
    void blocksMerging(vector<int> &sSequenceFinal, vector<vector<int>> correctionByteBlocks,
                       vector<vector<int>> blocks);
};
