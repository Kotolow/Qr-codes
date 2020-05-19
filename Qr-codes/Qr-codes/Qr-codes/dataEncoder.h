#pragma once
#include <vector>
class string;

class DataEncoder
{
  public:
    std::vector<bool> sequenceOfBit; //последовательность бит
    std::vector<int> sequenceOfByte; //последовательность байт для блоков коррекции

    DataEncoder(const std::string &tx);
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

    std::vector<std::vector<int>> blocks; //массив блоков

  private:
    void binaryConverter(const std::string &tx, std::vector<bool> &sequenceOfBit, int amount);
    int versionNumber(int amountOfBits, int &maxAmountOfBits, int &amountOfBlocks);

    void intToBinary(int amount, int amountOfData, std::vector<bool> &serviceInf);
    void addition1();
    void addition2();
    void bitToIntConverter(const std::vector<bool> &sequenceOfBit, std::vector<int> &sequenceOfByte,
                           int maxAmountOfBits);
    void blockFiller(const std::vector<int> &sequenceOfByte, std::vector<std::vector<int>> &blocks,
                     int amountOfBlocks, int byteInBlock, int additionalBlocks);
};
