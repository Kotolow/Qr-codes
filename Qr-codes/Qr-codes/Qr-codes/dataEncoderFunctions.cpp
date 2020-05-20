#include "dataEncoder.h"
using namespace std;
DataEncoder::DataEncoder(const std::string &tx)
{
    amount = tx.size();
    binaryConverter(tx, sequenceOfBit, amount);
    amountOfBits = amount * 8;

    version = versionNumber(amountOfBits, maxAmountOfBits, amountOfBlocks, amountOfCorrectionBytes);

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

    polynomGenerator(generatingPolynom);
    correctionByteGenerator(correctionByteBlocks, blocks, generatingPolynom);
    blocksMerging(sequenceFinal, correctionByteBlocks, blocks);
    correctionPatternFiller(positionOfCorrectionPattern);
    sizeOfCanvasFinder(sizeOfCanvas, positionOfCorrectionPattern, amountOfCorrectingPattern);
    byteToBit(sequenceFinal, finalBits);
    qrCodeFiller(qrCode, sizeOfCanvas, positionOfCorrectionPattern);
}
void DataEncoder::binaryConverter(const std::string &tx, vector<bool> &sequenceOfBit, int amount)
/*функция предназначенная, для преобразования строки текста(текстового массива) в
строку бинарного кода (массив, содержащий значения бит)[пункт 1];
string tx - текст, который нужно переводить
vector<bool> &sequenceOfBit - ссылка на массив битов (через ссылку изменения забираются из
функции) int amount - размер исодного текста*/
{
    for (int r = 0; r < amount; ++r)
    {
        for (int shift = 7, i = 0; shift >= 0; --shift, ++i)

        {
            sequenceOfBit.push_back(tx[r] & (1 << shift));
        }
    }
}

int DataEncoder::versionNumber(int amountOfBits, int &maxAmountOfBits, int &amountOfBlocks,
                               int &amountOfCorrectionBytes)
{
    // эта функция определяет номер версии, максимальное кол-во бит, кол-во блоков и кол-во байтов
    // коррекции

    if (amountOfBits < 128)
    {
        maxAmountOfBits = 128;
        amountOfBlocks = 1;

        amountOfCorrectionBytes = 10;

        return 1;
    }

    else if (amountOfBits < 224)
    {
        maxAmountOfBits = 224;
        amountOfBlocks = 1;

        amountOfCorrectionBytes = 16;

        return 2;
    }
    else if (amountOfBits < 352)
    {
        maxAmountOfBits = 352;
        amountOfBlocks = 1;

        amountOfCorrectionBytes = 26;

        return 3;
    }
    else if (amountOfBits < 512)
    {
        maxAmountOfBits = 512;
        amountOfBlocks = 2;

        amountOfCorrectionBytes = 18;

        return 4;
    }
    else if (amountOfBits < 688)
    {
        maxAmountOfBits = 688;
        amountOfBlocks = 2;

        amountOfCorrectionBytes = 24;

        return 5;
    }
    else if (amountOfBits < 864)
    {
        maxAmountOfBits = 864;
        amountOfBlocks = 4;

        amountOfCorrectionBytes = 16;

        return 6;
    }
    else if (amountOfBits < 992)
    {
        maxAmountOfBits = 992;
        amountOfBlocks = 4;

        amountOfCorrectionBytes = 18;

        return 7;
    }
    else if (amountOfBits < 1232)
    {
        maxAmountOfBits = 1232;
        amountOfBlocks = 4;

        amountOfCorrectionBytes = 22;

        return 8;
    }
    else if (amountOfBits < 1456)
    {
        maxAmountOfBits = 1456;
        amountOfBlocks = 5;

        amountOfCorrectionBytes = 22;

        return 9;
    }
    else if (amountOfBits < 1728)
    {
        maxAmountOfBits = 1728;
        amountOfBlocks = 5;

        amountOfCorrectionBytes = 26;

        return 10;
    }
    else if (amountOfBits < 2032)
    {
        maxAmountOfBits = 2032;
        amountOfBlocks = 5;

        amountOfCorrectionBytes = 30;

        return 11;
    }
    else if (amountOfBits < 2320)
    {
        maxAmountOfBits = 2320;
        amountOfBlocks = 8;

        amountOfCorrectionBytes = 22;

        return 12;
    }
    else if (amountOfBits < 2672)
    {
        maxAmountOfBits = 2672;
        amountOfBlocks = 9;

        amountOfCorrectionBytes = 22;

        return 13;
    }
    else if (amountOfBits < 2920)
    {
        maxAmountOfBits = 2920;
        amountOfBlocks = 9;

        amountOfCorrectionBytes = 24;

        return 14;
    }
    else if (amountOfBits < 3320)
    {
        maxAmountOfBits = 3320;
        amountOfBlocks = 10;

        amountOfCorrectionBytes = 24;

        return 15;
    }
    else if (amountOfBits < 3624)
    {
        maxAmountOfBits = 3624;
        amountOfBlocks = 10;

        amountOfCorrectionBytes = 28;

        return 16;
    }
    else if (amountOfBits < 4056)
    {
        maxAmountOfBits = 4056;
        amountOfBlocks = 11;

        amountOfCorrectionBytes = 28;

        return 17;
    }
    else if (amountOfBits < 4504)
    {
        maxAmountOfBits = 4504;
        amountOfBlocks = 13;

        amountOfCorrectionBytes = 26;

        return 18;
    }
    else if (amountOfBits < 5016)
    {
        maxAmountOfBits = 5016;
        amountOfBlocks = 14;

        amountOfCorrectionBytes = 26;

        return 19;
    }
    else if (amountOfBits < 5352)
    {
        maxAmountOfBits = 5352;
        amountOfBlocks = 16;

        amountOfCorrectionBytes = 26;

        return 20;
    }
    else if (amountOfBits < 5712)
    {
        maxAmountOfBits = 5712;
        amountOfBlocks = 17;

        amountOfCorrectionBytes = 26;

        return 21;
    }
    else if (amountOfBits < 6256)
    {
        maxAmountOfBits = 6256;
        amountOfBlocks = 17;

        amountOfCorrectionBytes = 28;

        return 22;
    }
    else if (amountOfBits < 6880)
    {
        maxAmountOfBits = 6880;
        amountOfBlocks = 18;

        amountOfCorrectionBytes = 28;

        return 23;
    }
    else if (amountOfBits < 7312)
    {
        maxAmountOfBits = 7312;
        amountOfBlocks = 20;

        amountOfCorrectionBytes = 28;

        return 24;
    }
    else if (amountOfBits < 8000)
    {
        maxAmountOfBits = 8000;
        amountOfBlocks = 21;

        amountOfCorrectionBytes = 28;

        return 25;
    }
    else if (amountOfBits < 8496)
    {
        maxAmountOfBits = 8496;
        amountOfBlocks = 23;

        amountOfCorrectionBytes = 28;

        return 26;
    }
    else if (amountOfBits < 9024)
    {
        maxAmountOfBits = 9024;
        amountOfBlocks = 25;

        amountOfCorrectionBytes = 28;

        return 27;
    }
    else if (amountOfBits < 9544)
    {
        maxAmountOfBits = 9544;
        amountOfBlocks = 26;

        amountOfCorrectionBytes = 28;

        return 28;
    }
    else if (amountOfBits < 10136)
    {
        maxAmountOfBits = 10136;
        amountOfBlocks = 28;

        amountOfCorrectionBytes = 28;

        return 29;
    }
    else if (amountOfBits < 10984)
    {
        maxAmountOfBits = 10984;
        amountOfBlocks = 29;

        amountOfCorrectionBytes = 28;

        return 30;
    }
    else if (amountOfBits < 11640)
    {
        maxAmountOfBits = 11640;
        amountOfBlocks = 31;

        amountOfCorrectionBytes = 28;

        return 31;
    }
    else if (amountOfBits < 12328)
    {
        maxAmountOfBits = 12328;
        amountOfBlocks = 33;

        amountOfCorrectionBytes = 28;

        return 32;
    }
    else if (amountOfBits < 13048)
    {
        maxAmountOfBits = 13048;
        amountOfBlocks = 35;

        amountOfCorrectionBytes = 28;

        return 33;
    }
    else if (amountOfBits < 13800)
    {
        maxAmountOfBits = 13800;
        amountOfBlocks = 37;

        amountOfCorrectionBytes = 28;

        return 34;
    }
    else if (amountOfBits < 14496)
    {
        maxAmountOfBits = 14496;
        amountOfBlocks = 38;

        amountOfCorrectionBytes = 28;

        return 35;
    }
    else if (amountOfBits < 15312)
    {
        maxAmountOfBits = 15312;
        amountOfBlocks = 40;

        amountOfCorrectionBytes = 28;

        return 36;
    }
    else if (amountOfBits < 15936)
    {
        maxAmountOfBits = 15936;
        amountOfBlocks = 43;

        amountOfCorrectionBytes = 28;

        return 37;
    }
    else if (amountOfBits < 16816)
    {
        maxAmountOfBits = 16816;
        amountOfBlocks = 45;

        amountOfCorrectionBytes = 28;

        return 38;
    }
    else if (amountOfBits < 17728)
    {
        maxAmountOfBits = 17728;
        amountOfBlocks = 47;

        amountOfCorrectionBytes = 28;

        return 39;
    }
    else if (amountOfBits < 18672)
    {
        maxAmountOfBits = 18672;
        amountOfBlocks = 49;

        amountOfCorrectionBytes = 28;
        return 40;
    }
    else if (amountOfBits >= 18672)
    {
        return -1;
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

void DataEncoder::blockFiller(const vector<int> &sequenceOfByte, vector<vector<int>> &blocks,

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

void DataEncoder::polynomGenerator(vector<int> &generatingPolynom)
{
    if (amountOfCorrectionBytes == 10)
    {
        generatingPolynom = {251, 67, 46, 61, 118, 70, 64, 94, 32, 45};
    }
    else if (amountOfCorrectionBytes == 13)
    {
        generatingPolynom = {74, 152, 176, 100, 86, 100, 106, 104, 130, 218, 206, 140, 78};
    }
    else if (amountOfCorrectionBytes == 15)
    {
        generatingPolynom = {8, 183, 61, 91, 202, 37, 51, 58, 58, 237, 140, 124, 5, 99, 105};
    }
    else if (amountOfCorrectionBytes == 16)
    {
        generatingPolynom = {120, 104, 107, 109, 102, 161, 76,  3,
                             91,  191, 147, 169, 182, 194, 225, 120};
    }
    else if (amountOfCorrectionBytes == 17)
    {
        generatingPolynom = {43,  139, 206, 78,  43, 239, 123, 206, 214,
                             147, 24,  99,  150, 39, 243, 163, 136};
    }
    else if (amountOfCorrectionBytes == 18)
    {
        generatingPolynom = {215, 234, 158, 94,  184, 97, 118, 170, 79,
                             187, 152, 148, 252, 179, 5,  98,  96,  153};
    }
    else if (amountOfCorrectionBytes == 20)
    {
        generatingPolynom = {17,  60,  79, 50,  61,  163, 26,  187, 202, 180,
                             221, 225, 83, 239, 156, 164, 212, 212, 188, 190};
    }
    else if (amountOfCorrectionBytes == 22)
    {
        generatingPolynom = {210, 171, 247, 242, 93, 230, 14,  109, 221, 53,  200,
                             74,  8,   172, 98,  80, 219, 134, 160, 105, 165, 231};
    }
    else if (amountOfCorrectionBytes == 24)
    {
        generatingPolynom = {229, 121, 135, 48,  211, 117, 251, 126, 159, 180, 169, 152,
                             192, 226, 228, 218, 111, 0,   117, 232, 87,  96,  227, 21};
    }
    else if (amountOfCorrectionBytes == 26)
    {
        generatingPolynom = {173, 125, 158, 2,   103, 182, 118, 17, 145, 201, 111, 28,  165,
                             53,  161, 21,  245, 142, 13,  102, 48, 227, 153, 145, 218, 70};
    }
    else if (amountOfCorrectionBytes == 28)
    {
        generatingPolynom = {168, 223, 200, 104, 224, 234, 108, 180, 110, 190, 195, 147, 205, 27,
                             232, 201, 21,  43,  245, 87,  42,  195, 212, 119, 242, 37,  9,   123};
    }
    else if (amountOfCorrectionBytes == 30)
    {
        generatingPolynom = {41,  173, 145, 152, 216, 31,  179, 182, 50,  48,
                             110, 86,  239, 96,  222, 125, 42,  173, 226, 193,
                             224, 130, 156, 37,  251, 216, 238, 40,  192, 180};
    }
}
int DataEncoder::galua(int num)
{
    switch (num)
    {
    case 0:
        return 1;
    case 1:
        return 2;
    case 2:
        return 4;
    case 3:
        return 8;
    case 4:
        return 16;
    case 5:
        return 32;
    case 6:
        return 64;
    case 7:
        return 128;
    case 8:
        return 29;
    case 9:
        return 58;
    case 10:
        return 116;
    case 11:
        return 232;
    case 12:
        return 205;
    case 13:
        return 135;
    case 14:
        return 19;
    case 15:
        return 38;
    case 16:
        return 76;
    case 17:
        return 152;
    case 18:
        return 45;
    case 19:
        return 90;
    case 20:
        return 180;
    case 21:
        return 117;
    case 22:
        return 234;
    case 23:
        return 201;
    case 24:
        return 143;
    case 25:
        return 3;
    case 26:
        return 6;
    case 27:
        return 12;
    case 28:
        return 24;
    case 29:
        return 48;
    case 30:
        return 96;
    case 31:
        return 192;
    case 32:
        return 157;
    case 33:
        return 39;
    case 34:
        return 78;
    case 35:
        return 156;
    case 36:
        return 37;
    case 37:
        return 74;
    case 38:
        return 148;
    case 39:
        return 53;
    case 40:
        return 106;
    case 41:
        return 212;
    case 42:
        return 181;
    case 43:
        return 119;
    case 44:
        return 238;
    case 45:
        return 193;
    case 46:
        return 159;
    case 47:
        return 35;
    case 48:
        return 70;
    case 49:
        return 140;
    case 50:
        return 5;
    case 51:
        return 10;
    case 52:
        return 20;
    case 53:
        return 40;
    case 54:
        return 80;
    case 55:
        return 160;
    case 56:
        return 93;
    case 57:
        return 186;
    case 58:
        return 105;
    case 59:
        return 210;
    case 60:
        return 185;
    case 61:
        return 111;
    case 62:
        return 222;
    case 63:
        return 161;
    case 64:
        return 95;
    case 65:
        return 190;
    case 66:
        return 97;
    case 67:
        return 194;
    case 68:
        return 153;
    case 69:
        return 47;
    case 70:
        return 94;
    case 71:
        return 188;
    case 72:
        return 101;
    case 73:
        return 202;
    case 74:
        return 137;
    case 75:
        return 12;
    case 76:
        return 30;
    case 77:
        return 60;
    case 78:
        return 120;
    case 79:
        return 240;
    case 80:
        return 253;
    case 81:
        return 231;
    case 82:
        return 211;
    case 83:
        return 187;
    case 84:
        return 107;
    case 85:
        return 214;
    case 86:
        return 177;
    case 87:
        return 127;
    case 88:
        return 254;
    case 89:
        return 225;
    case 90:
        return 223;
    case 91:
        return 163;
    case 92:
        return 91;
    case 93:
        return 182;
    case 94:
        return 113;
    case 95:
        return 226;
    case 96:
        return 217;
    case 97:
        return 175;
    case 98:
        return 67;
    case 99:
        return 134;
    case 100:
        return 17;
    case 101:
        return 34;
    case 102:
        return 68;
    case 103:
        return 136;
    case 104:
        return 13;
    case 105:
        return 26;
    case 106:
        return 52;
    case 107:
        return 104;
    case 108:
        return 208;
    case 109:
        return 189;
    case 110:
        return 103;
    case 111:
        return 206;
    case 112:
        return 129;
    case 113:
        return 31;
    case 114:
        return 62;
    case 115:
        return 124;
    case 116:
        return 248;
    case 117:
        return 237;
    case 118:
        return 199;
    case 119:
        return 147;
    case 120:
        return 59;
    case 121:
        return 118;
    case 122:
        return 236;
    case 123:
        return 197;
    case 124:
        return 151;
    case 125:
        return 51;
    case 126:
        return 102;
    case 127:
        return 204;
    case 128:
        return 133;
    case 129:
        return 23;
    case 130:
        return 46;
    case 131:
        return 92;
    case 132:
        return 184;
    case 133:
        return 109;
    case 134:
        return 218;
    case 135:
        return 169;
    case 136:
        return 79;
    case 137:
        return 158;
    case 138:
        return 33;
    case 139:
        return 66;
    case 140:
        return 132;
    case 141:
        return 21;
    case 142:
        return 42;
    case 143:
        return 84;
    case 144:
        return 168;
    case 145:
        return 77;
    case 146:
        return 154;
    case 147:
        return 41;
    case 148:
        return 82;
    case 149:
        return 164;
    case 150:
        return 85;
    case 151:
        return 170;
    case 152:
        return 73;
    case 153:
        return 146;
    case 154:
        return 57;
    case 155:
        return 114;
    case 156:
        return 228;
    case 157:
        return 213;
    case 158:
        return 183;
    case 159:
        return 115;
    case 160:
        return 230;
    case 161:
        return 209;
    case 162:
        return 191;
    case 163:
        return 99;
    case 164:
        return 198;
    case 165:
        return 145;
    case 166:
        return 63;
    case 167:
        return 126;
    case 168:
        return 252;
    case 169:
        return 229;
    case 170:
        return 215;
    case 171:
        return 179;
    case 172:
        return 123;
    case 173:
        return 246;
    case 174:
        return 241;
    case 175:
        return 255;
    case 176:
        return 227;
    case 177:
        return 219;
    case 178:
        return 171;
    case 179:
        return 75;
    case 180:
        return 150;
    case 181:
        return 49;
    case 182:
        return 98;
    case 183:
        return 196;
    case 184:
        return 149;
    case 185:
        return 55;
    case 186:
        return 110;
    case 187:
        return 220;
    case 188:
        return 165;
    case 189:
        return 87;
    case 190:
        return 174;
    case 191:
        return 65;
    case 192:
        return 130;
    case 193:
        return 25;
    case 194:
        return 50;
    case 195:
        return 100;
    case 196:
        return 200;
    case 197:
        return 141;
    case 198:
        return 7;
    case 199:
        return 14;
    case 200:
        return 28;
    case 201:
        return 56;
    case 202:
        return 112;
    case 203:
        return 224;
    case 204:
        return 221;
    case 205:
        return 167;
    case 206:
        return 83;
    case 207:
        return 166;
    case 208:
        return 81;
    case 209:
        return 162;
    case 210:
        return 89;
    case 211:
        return 178;
    case 212:
        return 121;
    case 213:
        return 242;
    case 214:
        return 249;
    case 215:
        return 239;
    case 216:
        return 195;
    case 217:
        return 155;
    case 218:
        return 43;
    case 219:
        return 86;
    case 220:
        return 172;
    case 221:
        return 69;
    case 222:
        return 138;
    case 223:
        return 9;
    case 224:
        return 18;
    case 225:
        return 36;
    case 226:
        return 72;
    case 227:
        return 144;
    case 228:
        return 61;
    case 229:
        return 122;
    case 230:
        return 244;
    case 231:
        return 245;
    case 232:
        return 247;
    case 233:
        return 243;
    case 234:
        return 251;
    case 235:
        return 235;
    case 236:
        return 203;
    case 237:
        return 139;
    case 238:
        return 11;
    case 239:
        return 22;
    case 240:
        return 44;
    case 241:
        return 88;
    case 242:
        return 176;
    case 243:
        return 125;
    case 244:
        return 250;
    case 245:
        return 233;
    case 246:
        return 207;
    case 247:
        return 131;
    case 248:
        return 27;
    case 249:
        return 54;
    case 250:
        return 108;
    case 251:
        return 216;
    case 252:
        return 173;
    case 253:
        return 71;
    case 254:
        return 142;
    case 255:
        return 1;
    default:
        return -1;
    }
}
int DataEncoder::galuaReverse(int num)
{
    switch (num)
    {
    case 0:
        return 1;
    case 2:
        return 1;
    case 3:
        return 25;
    case 4:
        return 2;
    case 5:
        return 50;
    case 6:
        return 26;
    case 7:
        return 198;
    case 8:
        return 3;
    case 9:
        return 223;
    case 10:
        return 51;
    case 11:
        return 238;
    case 12:
        return 27;
    case 13:
        return 104;
    case 14:
        return 199;
    case 15:
        return 75;
    case 16:
        return 4;
    case 17:
        return 100;
    case 18:
        return 224;
    case 19:
        return 14;
    case 20:
        return 52;
    case 21:
        return 141;
    case 22:
        return 239;
    case 23:
        return 129;
    case 24:
        return 28;
    case 25:
        return 193;
    case 26:
        return 105;
    case 27:
        return 248;
    case 28:
        return 200;
    case 29:
        return 8;
    case 30:
        return 76;
    case 31:
        return 113;
    case 32:
        return 5;
    case 33:
        return 138;
    case 34:
        return 101;
    case 35:
        return 47;
    case 36:
        return 225;
    case 37:
        return 36;
    case 38:
        return 15;
    case 39:
        return 33;
    case 40:
        return 53;
    case 41:
        return 147;
    case 42:
        return 142;
    case 43:
        return 218;
    case 44:
        return 240;
    case 45:
        return 18;
    case 46:
        return 130;
    case 47:
        return 69;
    case 48:
        return 29;
    case 49:
        return 181;
    case 50:
        return 194;
    case 51:
        return 125;
    case 52:
        return 106;
    case 53:
        return 39;
    case 54:
        return 249;
    case 55:
        return 185;
    case 56:
        return 201;
    case 57:
        return 154;
    case 58:
        return 9;
    case 59:
        return 120;
    case 60:
        return 77;
    case 61:
        return 228;
    case 62:
        return 114;
    case 63:
        return 166;
    case 64:
        return 6;
    case 65:
        return 191;
    case 66:
        return 139;
    case 67:
        return 98;
    case 68:
        return 102;
    case 69:
        return 221;
    case 70:
        return 48;
    case 71:
        return 253;
    case 72:
        return 226;
    case 73:
        return 152;
    case 74:
        return 37;
    case 75:
        return 179;
    case 76:
        return 16;
    case 77:
        return 145;
    case 78:
        return 34;
    case 79:
        return 136;
    case 80:
        return 54;
    case 81:
        return 208;
    case 82:
        return 148;
    case 83:
        return 206;
    case 84:
        return 143;
    case 85:
        return 150;
    case 86:
        return 219;
    case 87:
        return 189;
    case 88:
        return 241;
    case 89:
        return 210;
    case 90:
        return 19;
    case 91:
        return 92;
    case 92:
        return 131;
    case 93:
        return 56;
    case 94:
        return 70;
    case 95:
        return 64;
    case 96:
        return 30;
    case 97:
        return 66;
    case 98:
        return 182;
    case 99:
        return 163;
    case 100:
        return 195;
    case 101:
        return 72;
    case 102:
        return 126;
    case 103:
        return 110;
    case 104:
        return 107;
    case 105:
        return 58;
    case 106:
        return 40;
    case 107:
        return 84;
    case 108:
        return 250;
    case 109:
        return 133;
    case 110:
        return 186;
    case 111:
        return 61;
    case 112:
        return 202;
    case 113:
        return 94;
    case 114:
        return 155;
    case 115:
        return 159;
    case 116:
        return 10;
    case 117:
        return 21;
    case 118:
        return 121;
    case 119:
        return 43;
    case 120:
        return 78;
    case 121:
        return 212;
    case 122:
        return 229;
    case 123:
        return 172;
    case 124:
        return 115;
    case 125:
        return 243;
    case 126:
        return 167;
    case 127:
        return 87;
    case 128:
        return 7;
    case 129:
        return 112;
    case 130:
        return 192;
    case 131:
        return 247;
    case 132:
        return 140;
    case 133:
        return 128;
    case 134:
        return 99;
    case 135:
        return 13;
    case 136:
        return 103;
    case 137:
        return 74;
    case 138:
        return 222;
    case 139:
        return 237;
    case 140:
        return 49;
    case 141:
        return 197;
    case 142:
        return 254;
    case 143:
        return 24;
    case 144:
        return 227;
    case 145:
        return 165;
    case 146:
        return 153;
    case 147:
        return 119;
    case 148:
        return 38;
    case 149:
        return 184;
    case 150:
        return 180;
    case 151:
        return 124;
    case 152:
        return 17;
    case 153:
        return 68;
    case 154:
        return 146;
    case 155:
        return 217;
    case 156:
        return 35;
    case 157:
        return 32;
    case 158:
        return 137;
    case 159:
        return 46;
    case 160:
        return 55;
    case 161:
        return 63;
    case 162:
        return 209;
    case 163:
        return 91;
    case 164:
        return 149;
    case 165:
        return 188;
    case 166:
        return 207;
    case 167:
        return 205;
    case 168:
        return 144;
    case 169:
        return 135;
    case 170:
        return 151;
    case 171:
        return 178;
    case 172:
        return 220;
    case 173:
        return 252;
    case 174:
        return 190;
    case 175:
        return 97;
    case 176:
        return 242;
    case 177:
        return 86;
    case 178:
        return 211;
    case 179:
        return 171;
    case 180:
        return 20;
    case 181:
        return 42;
    case 182:
        return 93;
    case 183:
        return 158;
    case 184:
        return 132;
    case 185:
        return 60;
    case 186:
        return 57;
    case 187:
        return 83;
    case 188:
        return 71;
    case 189:
        return 109;
    case 190:
        return 65;
    case 191:
        return 162;
    case 192:
        return 31;
    case 193:
        return 45;
    case 194:
        return 67;
    case 195:
        return 216;
    case 196:
        return 183;
    case 197:
        return 123;
    case 198:
        return 164;
    case 199:
        return 118;
    case 200:
        return 196;
    case 201:
        return 23;
    case 202:
        return 73;
    case 203:
        return 236;
    case 204:
        return 127;
    case 205:
        return 12;
    case 206:
        return 111;
    case 207:
        return 246;
    case 208:
        return 108;
    case 209:
        return 161;
    case 210:
        return 59;
    case 211:
        return 82;
    case 212:
        return 41;
    case 213:
        return 157;
    case 214:
        return 85;
    case 215:
        return 170;
    case 216:
        return 251;
    case 217:
        return 96;
    case 218:
        return 134;
    case 219:
        return 177;
    case 220:
        return 187;
    case 221:
        return 204;
    case 222:
        return 62;
    case 223:
        return 90;
    case 224:
        return 203;
    case 225:
        return 89;
    case 226:
        return 95;
    case 227:
        return 176;
    case 228:
        return 156;
    case 229:
        return 169;
    case 230:
        return 160;
    case 231:
        return 81;
    case 232:
        return 11;
    case 233:
        return 245;
    case 234:
        return 22;
    case 235:
        return 235;
    case 236:
        return 122;
    case 237:
        return 117;
    case 238:
        return 44;
    case 239:
        return 215;
    case 240:
        return 79;
    case 241:
        return 174;
    case 242:
        return 213;
    case 243:
        return 233;
    case 244:
        return 230;
    case 245:
        return 231;
    case 246:
        return 173;
    case 247:
        return 232;
    case 248:
        return 116;
    case 249:
        return 214;
    case 250:
        return 244;
    case 251:
        return 234;
    case 252:
        return 168;
    case 253:
        return 80;
    case 254:
        return 88;
    case 255:
        return 175;
    default:
        return -1;
    }
}
void DataEncoder::correctionByteGenerator(vector<vector<int>> &sCorrectionByteBlocks,
                                          const vector<vector<int>> &blocks,
                                          const vector<int> &generatingPolynom)
{ //функция отвечающая за создание блоков байтов коррекции
  // vector<vector<int>> &sCorrectionByteBlocks - блоки байтов коррекции
  // vector<vector<int>> blocks - блоки данных
  // vector<int> generatingPolynom - генерирующий многочлен многочлен
    vector<int> currentPolynom;
    int a, b;
    sCorrectionByteBlocks.resize(amountOfBlocks);
    for (int k = 0; k < amountOfBlocks; ++k)
    {
        sCorrectionByteBlocks[k].resize(blocks[k].size());
        sCorrectionByteBlocks[k] = blocks[k];
        if (generatingPolynom.size() > blocks[k].size())
        {
            for (int i = 0; i < generatingPolynom.size() - blocks[k].size(); ++i)
            {
                sCorrectionByteBlocks[k].push_back(0);
            }
        }
        currentPolynom = generatingPolynom;
        for (int i = 0; i < blocks[k].size(); ++i)
        {
            a = sCorrectionByteBlocks[k][0];
            if (a == 0)
            {
                continue;
            }
            sCorrectionByteBlocks[k].erase(sCorrectionByteBlocks[k].begin());
            sCorrectionByteBlocks[k].push_back(0);
            b = galuaReverse(a);
            for (int j = 0; j < amountOfCorrectionBytes; ++j)
            {
                currentPolynom[j] += b;
                if (currentPolynom[j] > 254)
                {
                    currentPolynom[j] = currentPolynom[j] % 255;
                }
                currentPolynom[j] = galua(currentPolynom[j]);
                sCorrectionByteBlocks[k][j] ^= currentPolynom[j];
            }
        }
    }
}
void DataEncoder::blocksMerging(vector<int> &sSequenceFinal,
                                const vector<vector<int>> &correctionByteBlocks,
                                const vector<vector<int>> &blocks)
{ //функция отвечающая за компановку конечной последовательности байт, на основе которой
  //выстраивается Qr-code vector<int> &sSequenceFinal - конечная поседовательность байт
  // vector<vector<int>> correctionByteBlocks - блок байтов коррекции
  // vector<vector<int>> blocks - блок байтов данных
    for (int i = 0; i < byteInBlock; ++i)
    {
        for (int j = 0; j < amountOfBlocks; ++j)
        {
            sSequenceFinal.push_back(blocks[j][i]);
        }
    }
    for (int i = amountOfBlocks - additionalBlocks; i < amountOfBlocks; ++i)
    {
        sSequenceFinal.push_back(blocks[i][byteInBlock]);
    }
    for (int i = 0; i < amountOfCorrectionBytes; ++i)
    {
        for (int j = 0; j < amountOfBlocks; ++j)
        {
            sSequenceFinal.push_back(correctionByteBlocks[j][i]);
        }
    }
}
void DataEncoder::correctionPatternFiller(vector<int> &sPositionOfCorrectionPattern)
{
    sPositionOfCorrectionPattern.resize(7);
    switch (version)
    {
    case 2:
        sPositionOfCorrectionPattern[0] = 18;
        break;
    case 3:
        sPositionOfCorrectionPattern[0] = 22;
        break;
    case 4:
        sPositionOfCorrectionPattern[0] = 26;
        break;
    case 5:
        sPositionOfCorrectionPattern[0] = 30;
        break;
    case 6:
        sPositionOfCorrectionPattern[0] = 34;
        break;
    case 7:
        sPositionOfCorrectionPattern[0] = 6;
        sPositionOfCorrectionPattern[1] = 22;
        sPositionOfCorrectionPattern[2] = 38;
        break;
    case 8:
        sPositionOfCorrectionPattern[0] = 6;
        sPositionOfCorrectionPattern[1] = 24;
        sPositionOfCorrectionPattern[2] = 42;
        break;
    case 9:
        sPositionOfCorrectionPattern[0] = 6;
        sPositionOfCorrectionPattern[1] = 26;
        sPositionOfCorrectionPattern[2] = 46;
        break;
    case 10:
        sPositionOfCorrectionPattern[0] = 6;
        sPositionOfCorrectionPattern[1] = 28;
        sPositionOfCorrectionPattern[2] = 50;
        break;
    case 11:
        sPositionOfCorrectionPattern[0] = 6;
        sPositionOfCorrectionPattern[1] = 30;
        sPositionOfCorrectionPattern[2] = 54;
        break;
    case 12:
        sPositionOfCorrectionPattern[0] = 6;
        sPositionOfCorrectionPattern[1] = 32;
        sPositionOfCorrectionPattern[2] = 58;
        break;
    case 13:
        sPositionOfCorrectionPattern[0] = 6;
        sPositionOfCorrectionPattern[1] = 34;
        sPositionOfCorrectionPattern[2] = 62;
        break;
    case 14:
        sPositionOfCorrectionPattern[0] = 6;
        sPositionOfCorrectionPattern[1] = 26;
        sPositionOfCorrectionPattern[2] = 46;
        sPositionOfCorrectionPattern[3] = 66;
        break;
    case 15:
        sPositionOfCorrectionPattern[0] = 6;
        sPositionOfCorrectionPattern[1] = 34;
        sPositionOfCorrectionPattern[2] = 62;
        sPositionOfCorrectionPattern[3] = 90;
        break;
    case 16:
        sPositionOfCorrectionPattern[0] = 6;
        sPositionOfCorrectionPattern[1] = 26;
        sPositionOfCorrectionPattern[2] = 50;
        sPositionOfCorrectionPattern[3] = 74;
        break;
    case 17:
        sPositionOfCorrectionPattern[0] = 6;
        sPositionOfCorrectionPattern[1] = 30;
        sPositionOfCorrectionPattern[2] = 54;
        sPositionOfCorrectionPattern[3] = 78;
        break;
    case 18:
        sPositionOfCorrectionPattern[0] = 6;
        sPositionOfCorrectionPattern[1] = 30;
        sPositionOfCorrectionPattern[2] = 56;
        sPositionOfCorrectionPattern[3] = 82;
        break;
    case 19:
        sPositionOfCorrectionPattern[0] = 6;
        sPositionOfCorrectionPattern[1] = 30;
        sPositionOfCorrectionPattern[2] = 58;
        sPositionOfCorrectionPattern[3] = 86;
        break;
    case 20:
        sPositionOfCorrectionPattern[0] = 6;
        sPositionOfCorrectionPattern[1] = 34;
        sPositionOfCorrectionPattern[2] = 62;
        sPositionOfCorrectionPattern[3] = 90;
        break;
    case 21:
        sPositionOfCorrectionPattern[0] = 6;
        sPositionOfCorrectionPattern[1] = 28;
        sPositionOfCorrectionPattern[2] = 50;
        sPositionOfCorrectionPattern[3] = 72;
        sPositionOfCorrectionPattern[4] = 94;
        break;
    case 22:
        sPositionOfCorrectionPattern[0] = 6;
        sPositionOfCorrectionPattern[1] = 26;
        sPositionOfCorrectionPattern[2] = 50;
        sPositionOfCorrectionPattern[3] = 74;
        sPositionOfCorrectionPattern[4] = 98;
        break;
    case 23:
        sPositionOfCorrectionPattern[0] = 6;
        sPositionOfCorrectionPattern[1] = 30;
        sPositionOfCorrectionPattern[2] = 54;
        sPositionOfCorrectionPattern[3] = 78;
        sPositionOfCorrectionPattern[4] = 102;
        break;
    case 24:
        sPositionOfCorrectionPattern[0] = 6;
        sPositionOfCorrectionPattern[1] = 28;
        sPositionOfCorrectionPattern[2] = 54;
        sPositionOfCorrectionPattern[3] = 80;
        sPositionOfCorrectionPattern[4] = 106;
        break;
    case 25:
        sPositionOfCorrectionPattern[0] = 6;
        sPositionOfCorrectionPattern[1] = 32;
        sPositionOfCorrectionPattern[2] = 58;
        sPositionOfCorrectionPattern[3] = 84;
        sPositionOfCorrectionPattern[4] = 110;
        break;
    case 26:
        sPositionOfCorrectionPattern[0] = 6;
        sPositionOfCorrectionPattern[1] = 30;
        sPositionOfCorrectionPattern[2] = 58;
        sPositionOfCorrectionPattern[3] = 86;
        sPositionOfCorrectionPattern[4] = 114;
        break;
    case 27:
        sPositionOfCorrectionPattern[0] = 6;
        sPositionOfCorrectionPattern[1] = 34;
        sPositionOfCorrectionPattern[2] = 62;
        sPositionOfCorrectionPattern[3] = 90;
        sPositionOfCorrectionPattern[4] = 118;
        break;
    case 28:
        sPositionOfCorrectionPattern[0] = 6;
        sPositionOfCorrectionPattern[1] = 26;
        sPositionOfCorrectionPattern[2] = 50;
        sPositionOfCorrectionPattern[3] = 74;
        sPositionOfCorrectionPattern[4] = 98;
        sPositionOfCorrectionPattern[5] = 122;
        break;
    case 29:
        sPositionOfCorrectionPattern[0] = 6;
        sPositionOfCorrectionPattern[1] = 30;
        sPositionOfCorrectionPattern[2] = 54;
        sPositionOfCorrectionPattern[3] = 78;
        sPositionOfCorrectionPattern[4] = 102;
        sPositionOfCorrectionPattern[5] = 126;
        break;
    case 30:
        sPositionOfCorrectionPattern[0] = 6;
        sPositionOfCorrectionPattern[1] = 26;
        sPositionOfCorrectionPattern[2] = 52;
        sPositionOfCorrectionPattern[3] = 78;
        sPositionOfCorrectionPattern[4] = 104;
        sPositionOfCorrectionPattern[5] = 130;
        break;
    case 31:
        sPositionOfCorrectionPattern[0] = 6;
        sPositionOfCorrectionPattern[1] = 30;
        sPositionOfCorrectionPattern[2] = 56;
        sPositionOfCorrectionPattern[3] = 82;
        sPositionOfCorrectionPattern[4] = 108;
        sPositionOfCorrectionPattern[5] = 134;
        break;
    case 32:
        sPositionOfCorrectionPattern[0] = 6;
        sPositionOfCorrectionPattern[1] = 34;
        sPositionOfCorrectionPattern[2] = 60;
        sPositionOfCorrectionPattern[3] = 86;
        sPositionOfCorrectionPattern[4] = 112;
        sPositionOfCorrectionPattern[5] = 138;
        break;
    case 33:
        sPositionOfCorrectionPattern[0] = 6;
        sPositionOfCorrectionPattern[1] = 30;
        sPositionOfCorrectionPattern[2] = 58;
        sPositionOfCorrectionPattern[3] = 86;
        sPositionOfCorrectionPattern[4] = 114;
        sPositionOfCorrectionPattern[5] = 142;
        break;
    case 34:
        sPositionOfCorrectionPattern[0] = 6;
        sPositionOfCorrectionPattern[1] = 34;
        sPositionOfCorrectionPattern[2] = 62;
        sPositionOfCorrectionPattern[3] = 90;
        sPositionOfCorrectionPattern[4] = 118;
        sPositionOfCorrectionPattern[5] = 146;
        break;
    case 35:
        sPositionOfCorrectionPattern[0] = 6;
        sPositionOfCorrectionPattern[1] = 30;
        sPositionOfCorrectionPattern[2] = 54;
        sPositionOfCorrectionPattern[3] = 78;
        sPositionOfCorrectionPattern[4] = 102;
        sPositionOfCorrectionPattern[5] = 126;
        sPositionOfCorrectionPattern[6] = 150;
        break;
    case 36:
        sPositionOfCorrectionPattern[0] = 6;
        sPositionOfCorrectionPattern[1] = 24;
        sPositionOfCorrectionPattern[2] = 50;
        sPositionOfCorrectionPattern[3] = 76;
        sPositionOfCorrectionPattern[4] = 102;
        sPositionOfCorrectionPattern[5] = 128;
        sPositionOfCorrectionPattern[6] = 154;
        break;
    case 37:
        sPositionOfCorrectionPattern[0] = 6;
        sPositionOfCorrectionPattern[1] = 28;
        sPositionOfCorrectionPattern[2] = 54;
        sPositionOfCorrectionPattern[3] = 80;
        sPositionOfCorrectionPattern[4] = 106;
        sPositionOfCorrectionPattern[5] = 132;
        sPositionOfCorrectionPattern[6] = 158;
        break;
    case 38:
        sPositionOfCorrectionPattern[0] = 6;
        sPositionOfCorrectionPattern[1] = 32;
        sPositionOfCorrectionPattern[2] = 58;
        sPositionOfCorrectionPattern[3] = 84;
        sPositionOfCorrectionPattern[4] = 110;
        sPositionOfCorrectionPattern[5] = 136;
        sPositionOfCorrectionPattern[6] = 162;
        break;
    case 39:
        sPositionOfCorrectionPattern[0] = 6;
        sPositionOfCorrectionPattern[1] = 26;
        sPositionOfCorrectionPattern[2] = 54;
        sPositionOfCorrectionPattern[3] = 82;
        sPositionOfCorrectionPattern[4] = 110;
        sPositionOfCorrectionPattern[5] = 138;
        sPositionOfCorrectionPattern[6] = 166;
        break;
    case 40:
        sPositionOfCorrectionPattern[0] = 6;
        sPositionOfCorrectionPattern[1] = 30;
        sPositionOfCorrectionPattern[2] = 58;
        sPositionOfCorrectionPattern[3] = 86;
        sPositionOfCorrectionPattern[4] = 114;
        sPositionOfCorrectionPattern[5] = 142;
        sPositionOfCorrectionPattern[6] = 170;
        break;
    default:
        break;
    }
}
void DataEncoder::sizeOfCanvasFinder(int &sSizeOfCanvas,
                                     const vector<int> &sPositionOfCorrectionPattern,
                                     int &sAmountOfCorrectingPattern)
{
    if (version == 1)
    {
        sSizeOfCanvas = 21 + 7 + 4;
        sAmountOfCorrectingPattern = 0;
    }
    else
    {
        int max = 0;
        sAmountOfCorrectingPattern = 0;
        for (int i = 0; i < 7; ++i)
        {
            if (sPositionOfCorrectionPattern[i] > max)
            {
                max = sPositionOfCorrectionPattern[i];
                ++sAmountOfCorrectingPattern;
            }
        }
        sSizeOfCanvas = max + 7 + 4;
    }
}
void DataEncoder::byteToBit(const vector<int> &sSequenceFinal, vector<bool> &sFinalBits)
{ //функция для получения битовых значений, заносимых в qr-код
    for (int r = 0; r < sSequenceFinal.size(); ++r)
    {
        for (int shift = 7, i = 0; shift >= 0; --shift, ++i)

        {
            sFinalBits.push_back(sSequenceFinal[r] & (1 << shift));
        }
    }
}
void DataEncoder::qrCodeFiller(vector<vector<int>> &sQrCode, const int &sSizeOfCanvas,
                               const vector<int> &sPositionOfCorrectionPattern)
{ //функция отвечающая за заполнение qr кода значениями, где 0 соответствует белому цвету, 1 -
  //чёрному
    sQrCode.resize(sSizeOfCanvas);
    for (int i = 0; i < sSizeOfCanvas; ++i)
    {
        sQrCode[i].resize(sSizeOfCanvas);
    }
    for (int i = 0; i < sSizeOfCanvas; ++i)
    {
        for (int j = 0; j < sSizeOfCanvas; ++j)
        {
            sQrCode[i][j] = 2; //проверка на наличие присваивамого значения, т.к в qr-code могут
                               //быть только 2 значения: 0 и 1
        }
    }
    for (int i = 0; i < sSizeOfCanvas; ++i)
    { //заполнение белым цветом полос рамки
        sQrCode[0][i] = 0;
        sQrCode[i][0] = 0;
        sQrCode[1][i] = 0;
        sQrCode[i][1] = 0;
        sQrCode[sSizeOfCanvas - 1][i] = 0;
        sQrCode[i][sSizeOfCanvas - 1] = 0;
        sQrCode[sSizeOfCanvas - 2][i] = 0;
        sQrCode[i][sSizeOfCanvas - 2] = 0;
    }
    //заполнение верхнего левого поискового узора
    for (int i = 4; i <= 6; ++i)
    {
        for (int j = 4; j <= 6; ++j)
        {
            sQrCode[i][j] = 1;
        }
    }
    for (int i = 3; i <= 7; ++i)
    {
        sQrCode[3][i] = 0;
        sQrCode[i][3] = 0;
        sQrCode[7][i] = 0;
        sQrCode[i][7] = 0;
    }
    for (int i = 2; i <= 8; ++i)
    {
        sQrCode[2][i] = 1;
        sQrCode[i][2] = 1;
        sQrCode[8][i] = 1;
        sQrCode[i][8] = 1;
    }
    for (int i = 2; i <= 9; ++i)
    {
        sQrCode[9][i] = 0;
        sQrCode[i][9] = 0;
    }
    //верхний правый поисковый узор
    for (int i = 4; i <= 6; ++i)
    {
        for (int j = sizeOfCanvas - 7; j <= sizeOfCanvas - 5; ++j)
        {
            sQrCode[i][j] = 1;
        }
    }
    for (int i = sizeOfCanvas - 9; i <= sizeOfCanvas - 3; ++i)
    {
        sQrCode[2][i] = 1;
        sQrCode[8][i] = 1;
    }
    for (int i = 2; i <= 8; ++i)
    {
        sQrCode[i][sizeOfCanvas - 3] = 1;
        sQrCode[i][sizeOfCanvas - 9] = 1;
    }
    for (int i = sizeOfCanvas - 8; i <= sizeOfCanvas - 4; ++i)
    {
        sQrCode[3][i] = 0;
        sQrCode[7][i] = 0;
    }
    for (int i = 3; i <= 7; ++i)
    {
        sQrCode[i][sizeOfCanvas - 4] = 0;
        sQrCode[i][sizeOfCanvas - 8] = 0;
    }
    for (int i = 2; i <= 9; ++i)
    {
        sQrCode[i][sizeOfCanvas - 10] = 0;
        sQrCode[9][sizeOfCanvas - i - 1] = 0;
    }
    //левый нижний поисковый узор
    for (int i = sizeOfCanvas - 7; i <= sizeOfCanvas - 5; ++i)
    {
        for (int j = 4; j <= 6; ++j)
        {
            sQrCode[i][j] = 1;
        }
    }
    for (int i = sizeOfCanvas - 8; i <= sizeOfCanvas - 4; ++i)
    {
        sQrCode[i][3] = 0;
        sQrCode[i][7] = 0;
    }
    for (int i = 3; i <= 7; ++i)
    {
        sQrCode[sizeOfCanvas - 8][i] = 0;
        sQrCode[sizeOfCanvas - 4][i] = 0;
    }
    for (int i = sizeOfCanvas - 9; i <= sizeOfCanvas - 3; ++i)
    {
        sQrCode[i][2] = 1;
        sQrCode[i][8] = 1;
    }
    for (int i = 2; i <= 8; ++i)
    {
        sQrCode[sizeOfCanvas - 9][i] = 1;
        sQrCode[sizeOfCanvas - 3][i] = 1;
    }
    for (int i = 2; i <= 9; ++i)
    {
        sQrCode[sizeOfCanvas - 10][i] = 0;
        sQrCode[sizeOfCanvas - i - 1][9] = 0;
    }
    //выравнивающие узоры
    if ((version >= 2) && (version < 6))
    {
        for (int i = 0; i < amountOfCorrectingPattern; ++i)
        {
            for (int j = 0; j < amountOfCorrectingPattern; ++j)
            {
                sQrCode[sPositionOfCorrectionPattern[i] + 2][sPositionOfCorrectionPattern[j] + 2] =
                    1;
                for (int k = sPositionOfCorrectionPattern[i] + 1;
                     k <= sPositionOfCorrectionPattern[i] + 3; ++k)
                {
                    sQrCode[sPositionOfCorrectionPattern[j] + 1][k] = 0;
                    sQrCode[sPositionOfCorrectionPattern[j] + 3][k] = 0;
                }
                for (int k = sPositionOfCorrectionPattern[j] + 1;
                     k <= sPositionOfCorrectionPattern[j] + 3; ++k)
                {
                    sQrCode[k][sPositionOfCorrectionPattern[i] + 1] = 0;
                    sQrCode[k][sPositionOfCorrectionPattern[i] + 3] = 0;
                }
                for (int k = sPositionOfCorrectionPattern[i];
                     k <= sPositionOfCorrectionPattern[i] + 4; ++k)
                {
                    sQrCode[sPositionOfCorrectionPattern[j]][k] = 1;
                    sQrCode[sPositionOfCorrectionPattern[j] + 4][k] = 1;
                }
                for (int k = sPositionOfCorrectionPattern[j];
                     k <= sPositionOfCorrectionPattern[j] + 4; ++k)
                {
                    sQrCode[k][sPositionOfCorrectionPattern[i]] = 1;
                    sQrCode[k][sPositionOfCorrectionPattern[i] + 4] = 1;
                }
            }
        }
    }
    else if (version >= 6)
    {
        for (int i = 0; i < amountOfCorrectingPattern; ++i)
        {
            for (int j = 1; j < amountOfCorrectingPattern - 1; ++j)
            {
                sQrCode[sPositionOfCorrectionPattern[i] + 2][sPositionOfCorrectionPattern[j] + 2] =
                    1;
                for (int k = sPositionOfCorrectionPattern[i] + 1;
                     k <= sPositionOfCorrectionPattern[i] + 3; ++k)
                {
                    sQrCode[sPositionOfCorrectionPattern[j] + 1][k] = 0;
                    sQrCode[sPositionOfCorrectionPattern[j] + 3][k] = 0;
                }
                for (int k = sPositionOfCorrectionPattern[j] + 1;
                     k <= sPositionOfCorrectionPattern[j] + 3; ++k)
                {
                    sQrCode[k][sPositionOfCorrectionPattern[i] + 1] = 0;
                    sQrCode[k][sPositionOfCorrectionPattern[i] + 3] = 0;
                }
                for (int k = sPositionOfCorrectionPattern[i];
                     k <= sPositionOfCorrectionPattern[i] + 4; ++k)
                {
                    sQrCode[sPositionOfCorrectionPattern[j]][k] = 1;
                    sQrCode[sPositionOfCorrectionPattern[j] + 4][k] = 1;
                }
                for (int k = sPositionOfCorrectionPattern[j];
                     k <= sPositionOfCorrectionPattern[j] + 4; ++k)
                {
                    sQrCode[k][sPositionOfCorrectionPattern[i]] = 1;
                    sQrCode[k][sPositionOfCorrectionPattern[i] + 4] = 1;
                }
            }
        }
        for (int i = amountOfCorrectingPattern - 1; i < amountOfCorrectingPattern; ++i)
        {
            for (int j = 1; j < amountOfCorrectingPattern - 1; ++j)
            {
                sQrCode[sPositionOfCorrectionPattern[i] + 2][sPositionOfCorrectionPattern[j] + 2] =
                    1;
                for (int k = sPositionOfCorrectionPattern[i] + 1;
                     k <= sPositionOfCorrectionPattern[i] + 3; ++k)
                {
                    sQrCode[sPositionOfCorrectionPattern[j] + 1][k] = 0;
                    sQrCode[sPositionOfCorrectionPattern[j] + 3][k] = 0;
                }
                for (int k = sPositionOfCorrectionPattern[j] + 1;
                     k <= sPositionOfCorrectionPattern[j] + 3; ++k)
                {
                    sQrCode[k][sPositionOfCorrectionPattern[i] + 1] = 0;
                    sQrCode[k][sPositionOfCorrectionPattern[i] + 3] = 0;
                }
                for (int k = sPositionOfCorrectionPattern[i];
                     k <= sPositionOfCorrectionPattern[i] + 4; ++k)
                {
                    sQrCode[sPositionOfCorrectionPattern[j]][k] = 1;
                    sQrCode[sPositionOfCorrectionPattern[j] + 4][k] = 1;
                }
                for (int k = sPositionOfCorrectionPattern[j];
                     k <= sPositionOfCorrectionPattern[j] + 4; ++k)
                {
                    sQrCode[k][sPositionOfCorrectionPattern[i]] = 1;
                    sQrCode[k][sPositionOfCorrectionPattern[i] + 4] = 1;
                }
            }
        }
    }
    //полосы синхронизации
    for (int i = 10; i < sSizeOfCanvas - 10; ++i)
    {
        if ((sQrCode[i][8] == 2) && (i % 2 == 0))
            sQrCode[i][8] = 1;
        else if ((sQrCode[i][8] == 2) && (i % 2 != 0))
            sQrCode[i][8] = 0;
        if ((sQrCode[8][i] == 2) && (i % 2 == 0))
            sQrCode[8][i] = 1;
        else if ((sQrCode[8][i] == 2) && (i % 2 != 0))
            sQrCode[8][i] = 0;
    }
    //коды версии
    if (version == 7)
    {
        sQrCode[2][sSizeOfCanvas - 13] = 0;
        sQrCode[3][sSizeOfCanvas - 13] = 0;
        sQrCode[4][sSizeOfCanvas - 13] = 0;
        sQrCode[5][sSizeOfCanvas - 13] = 0;
        sQrCode[6][sSizeOfCanvas - 13] = 1;
        sQrCode[7][sSizeOfCanvas - 13] = 0;
        sQrCode[sSizeOfCanvas - 13][2] = 0;
        sQrCode[sSizeOfCanvas - 13][3] = 0;
        sQrCode[sSizeOfCanvas - 13][4] = 0;
        sQrCode[sSizeOfCanvas - 13][5] = 0;
        sQrCode[sSizeOfCanvas - 13][6] = 1;
        sQrCode[sSizeOfCanvas - 13][7] = 0;

        sQrCode[2][sSizeOfCanvas - 12] = 0;
        sQrCode[3][sSizeOfCanvas - 12] = 1;
        sQrCode[4][sSizeOfCanvas - 12] = 1;
        sQrCode[5][sSizeOfCanvas - 12] = 1;
        sQrCode[6][sSizeOfCanvas - 12] = 1;
        sQrCode[7][sSizeOfCanvas - 12] = 0;
        sQrCode[sSizeOfCanvas - 12][2] = 0;
        sQrCode[sSizeOfCanvas - 12][3] = 1;
        sQrCode[sSizeOfCanvas - 12][4] = 1;
        sQrCode[sSizeOfCanvas - 12][5] = 1;
        sQrCode[sSizeOfCanvas - 12][6] = 1;
        sQrCode[sSizeOfCanvas - 12][7] = 0;

        sQrCode[2][sSizeOfCanvas - 11] = 1;
        sQrCode[3][sSizeOfCanvas - 11] = 0;
        sQrCode[4][sSizeOfCanvas - 11] = 0;
        sQrCode[5][sSizeOfCanvas - 11] = 1;
        sQrCode[6][sSizeOfCanvas - 11] = 1;
        sQrCode[7][sSizeOfCanvas - 11] = 0;
        sQrCode[sSizeOfCanvas - 11][2] = 1;
        sQrCode[sSizeOfCanvas - 11][3] = 0;
        sQrCode[sSizeOfCanvas - 11][4] = 0;
        sQrCode[sSizeOfCanvas - 11][5] = 1;
        sQrCode[sSizeOfCanvas - 11][6] = 1;
        sQrCode[sSizeOfCanvas - 11][7] = 0;
    }
    if (version == 8)
    {
        sQrCode[2][sSizeOfCanvas - 13] = 0;
        sQrCode[3][sSizeOfCanvas - 13] = 1;
        sQrCode[4][sSizeOfCanvas - 13] = 0;
        sQrCode[5][sSizeOfCanvas - 13] = 0;
        sQrCode[6][sSizeOfCanvas - 13] = 0;
        sQrCode[7][sSizeOfCanvas - 13] = 1;
        sQrCode[sSizeOfCanvas - 13][2] = 0;
        sQrCode[sSizeOfCanvas - 13][3] = 1;
        sQrCode[sSizeOfCanvas - 13][4] = 0;
        sQrCode[sSizeOfCanvas - 13][5] = 0;
        sQrCode[sSizeOfCanvas - 13][6] = 0;
        sQrCode[sSizeOfCanvas - 13][7] = 1;

        sQrCode[2][sSizeOfCanvas - 12] = 0;
        sQrCode[3][sSizeOfCanvas - 12] = 1;
        sQrCode[4][sSizeOfCanvas - 12] = 1;
        sQrCode[5][sSizeOfCanvas - 12] = 1;
        sQrCode[6][sSizeOfCanvas - 12] = 0;
        sQrCode[7][sSizeOfCanvas - 12] = 0;
        sQrCode[sSizeOfCanvas - 12][2] = 0;
        sQrCode[sSizeOfCanvas - 12][3] = 1;
        sQrCode[sSizeOfCanvas - 12][4] = 1;
        sQrCode[sSizeOfCanvas - 12][5] = 1;
        sQrCode[sSizeOfCanvas - 12][6] = 0;
        sQrCode[sSizeOfCanvas - 12][7] = 0;

        sQrCode[2][sSizeOfCanvas - 11] = 1;
        sQrCode[3][sSizeOfCanvas - 11] = 1;
        sQrCode[4][sSizeOfCanvas - 11] = 1;
        sQrCode[5][sSizeOfCanvas - 11] = 0;
        sQrCode[6][sSizeOfCanvas - 11] = 0;
        sQrCode[7][sSizeOfCanvas - 11] = 0;
        sQrCode[sSizeOfCanvas - 11][2] = 1;
        sQrCode[sSizeOfCanvas - 11][3] = 1;
        sQrCode[sSizeOfCanvas - 11][4] = 1;
        sQrCode[sSizeOfCanvas - 11][5] = 0;
        sQrCode[sSizeOfCanvas - 11][6] = 0;
        sQrCode[sSizeOfCanvas - 11][7] = 0;
    }
    if (version == 9)
    {
        sQrCode[2][sSizeOfCanvas - 13] = 1;
        sQrCode[3][sSizeOfCanvas - 13] = 1;
        sQrCode[4][sSizeOfCanvas - 13] = 0;
        sQrCode[5][sSizeOfCanvas - 13] = 1;
        sQrCode[6][sSizeOfCanvas - 13] = 1;
        sQrCode[7][sSizeOfCanvas - 13] = 1;
        sQrCode[sSizeOfCanvas - 13][2] = 1;
        sQrCode[sSizeOfCanvas - 13][3] = 1;
        sQrCode[sSizeOfCanvas - 13][4] = 0;
        sQrCode[sSizeOfCanvas - 13][5] = 1;
        sQrCode[sSizeOfCanvas - 13][6] = 1;
        sQrCode[sSizeOfCanvas - 13][7] = 1;

        sQrCode[2][sSizeOfCanvas - 12] = 0;
        sQrCode[3][sSizeOfCanvas - 12] = 1;
        sQrCode[4][sSizeOfCanvas - 12] = 1;
        sQrCode[5][sSizeOfCanvas - 12] = 0;
        sQrCode[6][sSizeOfCanvas - 12] = 0;
        sQrCode[7][sSizeOfCanvas - 12] = 0;
        sQrCode[sSizeOfCanvas - 12][2] = 0;
        sQrCode[sSizeOfCanvas - 12][3] = 1;
        sQrCode[sSizeOfCanvas - 12][4] = 1;
        sQrCode[sSizeOfCanvas - 12][5] = 0;
        sQrCode[sSizeOfCanvas - 12][6] = 0;
        sQrCode[sSizeOfCanvas - 12][7] = 0;

        sQrCode[2][sSizeOfCanvas - 11] = 0;
        sQrCode[3][sSizeOfCanvas - 11] = 0;
        sQrCode[4][sSizeOfCanvas - 11] = 0;
        sQrCode[5][sSizeOfCanvas - 11] = 1;
        sQrCode[6][sSizeOfCanvas - 11] = 0;
        sQrCode[7][sSizeOfCanvas - 11] = 0;
        sQrCode[sSizeOfCanvas - 11][2] = 0;
        sQrCode[sSizeOfCanvas - 11][3] = 0;
        sQrCode[sSizeOfCanvas - 11][4] = 0;
        sQrCode[sSizeOfCanvas - 11][5] = 1;
        sQrCode[sSizeOfCanvas - 11][6] = 0;
        sQrCode[sSizeOfCanvas - 11][7] = 0;
    }
    if (version == 10)
    {
        sQrCode[2][sSizeOfCanvas - 13] = 1;
        sQrCode[3][sSizeOfCanvas - 13] = 0;
        sQrCode[4][sSizeOfCanvas - 13] = 1;
        sQrCode[5][sSizeOfCanvas - 13] = 0;
        sQrCode[6][sSizeOfCanvas - 13] = 0;
        sQrCode[7][sSizeOfCanvas - 13] = 1;
        sQrCode[sSizeOfCanvas - 13][2] = 1;
        sQrCode[sSizeOfCanvas - 13][3] = 0;
        sQrCode[sSizeOfCanvas - 13][4] = 1;
        sQrCode[sSizeOfCanvas - 13][5] = 0;
        sQrCode[sSizeOfCanvas - 13][6] = 0;
        sQrCode[sSizeOfCanvas - 13][7] = 1;

        sQrCode[2][sSizeOfCanvas - 12] = 1;
        sQrCode[3][sSizeOfCanvas - 12] = 1;
        sQrCode[4][sSizeOfCanvas - 12] = 1;
        sQrCode[5][sSizeOfCanvas - 12] = 1;
        sQrCode[6][sSizeOfCanvas - 12] = 1;
        sQrCode[7][sSizeOfCanvas - 12] = 0;
        sQrCode[sSizeOfCanvas - 12][2] = 1;
        sQrCode[sSizeOfCanvas - 12][3] = 1;
        sQrCode[sSizeOfCanvas - 12][4] = 1;
        sQrCode[sSizeOfCanvas - 12][5] = 1;
        sQrCode[sSizeOfCanvas - 12][6] = 1;
        sQrCode[sSizeOfCanvas - 12][7] = 0;

        sQrCode[2][sSizeOfCanvas - 11] = 0;
        sQrCode[3][sSizeOfCanvas - 11] = 0;
        sQrCode[4][sSizeOfCanvas - 11] = 0;
        sQrCode[5][sSizeOfCanvas - 11] = 0;
        sQrCode[6][sSizeOfCanvas - 11] = 0;
        sQrCode[7][sSizeOfCanvas - 11] = 0;
        sQrCode[sSizeOfCanvas - 11][2] = 0;
        sQrCode[sSizeOfCanvas - 11][3] = 0;
        sQrCode[sSizeOfCanvas - 11][4] = 0;
        sQrCode[sSizeOfCanvas - 11][5] = 0;
        sQrCode[sSizeOfCanvas - 11][6] = 0;
        sQrCode[sSizeOfCanvas - 11][7] = 0;
    }
    if (version == 11)
    {
        sQrCode[2][sSizeOfCanvas - 13] = 0;
        sQrCode[3][sSizeOfCanvas - 13] = 0;
        sQrCode[4][sSizeOfCanvas - 13] = 1;
        sQrCode[5][sSizeOfCanvas - 13] = 1;
        sQrCode[6][sSizeOfCanvas - 13] = 1;
        sQrCode[7][sSizeOfCanvas - 13] = 1;
        sQrCode[sSizeOfCanvas - 13][2] = 0;
        sQrCode[sSizeOfCanvas - 13][3] = 0;
        sQrCode[sSizeOfCanvas - 13][4] = 1;
        sQrCode[sSizeOfCanvas - 13][5] = 1;
        sQrCode[sSizeOfCanvas - 13][6] = 1;
        sQrCode[sSizeOfCanvas - 13][7] = 1;

        sQrCode[2][sSizeOfCanvas - 12] = 1;
        sQrCode[3][sSizeOfCanvas - 12] = 1;
        sQrCode[4][sSizeOfCanvas - 12] = 1;
        sQrCode[5][sSizeOfCanvas - 12] = 0;
        sQrCode[6][sSizeOfCanvas - 12] = 1;
        sQrCode[7][sSizeOfCanvas - 12] = 0;
        sQrCode[sSizeOfCanvas - 12][2] = 1;
        sQrCode[sSizeOfCanvas - 12][3] = 1;
        sQrCode[sSizeOfCanvas - 12][4] = 1;
        sQrCode[sSizeOfCanvas - 12][5] = 0;
        sQrCode[sSizeOfCanvas - 12][6] = 1;
        sQrCode[sSizeOfCanvas - 12][7] = 0;

        sQrCode[2][sSizeOfCanvas - 11] = 1;
        sQrCode[3][sSizeOfCanvas - 11] = 1;
        sQrCode[4][sSizeOfCanvas - 11] = 1;
        sQrCode[5][sSizeOfCanvas - 11] = 1;
        sQrCode[6][sSizeOfCanvas - 11] = 0;
        sQrCode[7][sSizeOfCanvas - 11] = 0;
        sQrCode[sSizeOfCanvas - 11][2] = 1;
        sQrCode[sSizeOfCanvas - 11][3] = 1;
        sQrCode[sSizeOfCanvas - 11][4] = 1;
        sQrCode[sSizeOfCanvas - 11][5] = 1;
        sQrCode[sSizeOfCanvas - 11][6] = 0;
        sQrCode[sSizeOfCanvas - 11][7] = 0;
    }
    if (version == 12)
    {
        sQrCode[2][sSizeOfCanvas - 13] = 0;
        sQrCode[3][sSizeOfCanvas - 13] = 0;
        sQrCode[4][sSizeOfCanvas - 13] = 1;
        sQrCode[5][sSizeOfCanvas - 13] = 1;
        sQrCode[6][sSizeOfCanvas - 13] = 0;
        sQrCode[7][sSizeOfCanvas - 13] = 1;
        sQrCode[sSizeOfCanvas - 13][2] = 0;
        sQrCode[sSizeOfCanvas - 13][3] = 0;
        sQrCode[sSizeOfCanvas - 13][4] = 1;
        sQrCode[sSizeOfCanvas - 13][5] = 1;
        sQrCode[sSizeOfCanvas - 13][6] = 0;
        sQrCode[sSizeOfCanvas - 13][7] = 1;

        sQrCode[2][sSizeOfCanvas - 12] = 1;
        sQrCode[3][sSizeOfCanvas - 12] = 0;
        sQrCode[4][sSizeOfCanvas - 12] = 0;
        sQrCode[5][sSizeOfCanvas - 12] = 1;
        sQrCode[6][sSizeOfCanvas - 12] = 0;
        sQrCode[7][sSizeOfCanvas - 12] = 0;
        sQrCode[sSizeOfCanvas - 12][2] = 1;
        sQrCode[sSizeOfCanvas - 12][3] = 0;
        sQrCode[sSizeOfCanvas - 12][4] = 0;
        sQrCode[sSizeOfCanvas - 12][5] = 1;
        sQrCode[sSizeOfCanvas - 12][6] = 0;
        sQrCode[sSizeOfCanvas - 12][7] = 0;

        sQrCode[2][sSizeOfCanvas - 11] = 0;
        sQrCode[3][sSizeOfCanvas - 11] = 1;
        sQrCode[4][sSizeOfCanvas - 11] = 1;
        sQrCode[5][sSizeOfCanvas - 11] = 0;
        sQrCode[6][sSizeOfCanvas - 11] = 1;
        sQrCode[7][sSizeOfCanvas - 11] = 0;
        sQrCode[sSizeOfCanvas - 11][2] = 0;
        sQrCode[sSizeOfCanvas - 11][3] = 1;
        sQrCode[sSizeOfCanvas - 11][4] = 1;
        sQrCode[sSizeOfCanvas - 11][5] = 0;
        sQrCode[sSizeOfCanvas - 11][6] = 1;
        sQrCode[sSizeOfCanvas - 11][7] = 0;
    }
    if (version == 13)
    {
        sQrCode[2][sSizeOfCanvas - 13] = 1;
        sQrCode[3][sSizeOfCanvas - 13] = 0;
        sQrCode[4][sSizeOfCanvas - 13] = 1;
        sQrCode[5][sSizeOfCanvas - 13] = 0;
        sQrCode[6][sSizeOfCanvas - 13] = 1;
        sQrCode[7][sSizeOfCanvas - 13] = 1;
        sQrCode[sSizeOfCanvas - 13][2] = 1;
        sQrCode[sSizeOfCanvas - 13][3] = 0;
        sQrCode[sSizeOfCanvas - 13][4] = 1;
        sQrCode[sSizeOfCanvas - 13][5] = 0;
        sQrCode[sSizeOfCanvas - 13][6] = 1;
        sQrCode[sSizeOfCanvas - 13][7] = 1;

        sQrCode[2][sSizeOfCanvas - 12] = 1;
        sQrCode[3][sSizeOfCanvas - 12] = 0;
        sQrCode[4][sSizeOfCanvas - 12] = 0;
        sQrCode[5][sSizeOfCanvas - 12] = 0;
        sQrCode[6][sSizeOfCanvas - 12] = 0;
        sQrCode[7][sSizeOfCanvas - 12] = 0;
        sQrCode[sSizeOfCanvas - 12][2] = 1;
        sQrCode[sSizeOfCanvas - 12][3] = 0;
        sQrCode[sSizeOfCanvas - 12][4] = 0;
        sQrCode[sSizeOfCanvas - 12][5] = 0;
        sQrCode[sSizeOfCanvas - 12][6] = 0;
        sQrCode[sSizeOfCanvas - 12][7] = 0;

        sQrCode[2][sSizeOfCanvas - 11] = 1;
        sQrCode[3][sSizeOfCanvas - 11] = 0;
        sQrCode[4][sSizeOfCanvas - 11] = 0;
        sQrCode[5][sSizeOfCanvas - 11] = 1;
        sQrCode[6][sSizeOfCanvas - 11] = 1;
        sQrCode[7][sSizeOfCanvas - 11] = 0;
        sQrCode[sSizeOfCanvas - 11][2] = 1;
        sQrCode[sSizeOfCanvas - 11][3] = 0;
        sQrCode[sSizeOfCanvas - 11][4] = 0;
        sQrCode[sSizeOfCanvas - 11][5] = 1;
        sQrCode[sSizeOfCanvas - 11][6] = 1;
        sQrCode[sSizeOfCanvas - 11][7] = 0;
    }
    if (version == 14)
    {
        sQrCode[2][sSizeOfCanvas - 13] = 1;
        sQrCode[3][sSizeOfCanvas - 13] = 1;
        sQrCode[4][sSizeOfCanvas - 13] = 0;
        sQrCode[5][sSizeOfCanvas - 13] = 1;
        sQrCode[6][sSizeOfCanvas - 13] = 0;
        sQrCode[7][sSizeOfCanvas - 13] = 1;
        sQrCode[sSizeOfCanvas - 13][2] = 1;
        sQrCode[sSizeOfCanvas - 13][3] = 1;
        sQrCode[sSizeOfCanvas - 13][4] = 0;
        sQrCode[sSizeOfCanvas - 13][5] = 1;
        sQrCode[sSizeOfCanvas - 13][6] = 0;
        sQrCode[sSizeOfCanvas - 13][7] = 1;

        sQrCode[2][sSizeOfCanvas - 12] = 0;
        sQrCode[3][sSizeOfCanvas - 12] = 0;
        sQrCode[4][sSizeOfCanvas - 12] = 0;
        sQrCode[5][sSizeOfCanvas - 12] = 1;
        sQrCode[6][sSizeOfCanvas - 12] = 1;
        sQrCode[7][sSizeOfCanvas - 12] = 0;
        sQrCode[sSizeOfCanvas - 12][2] = 0;
        sQrCode[sSizeOfCanvas - 12][3] = 0;
        sQrCode[sSizeOfCanvas - 12][4] = 0;
        sQrCode[sSizeOfCanvas - 12][5] = 1;
        sQrCode[sSizeOfCanvas - 12][6] = 1;
        sQrCode[sSizeOfCanvas - 12][7] = 0;

        sQrCode[2][sSizeOfCanvas - 11] = 1;
        sQrCode[3][sSizeOfCanvas - 11] = 0;
        sQrCode[4][sSizeOfCanvas - 11] = 0;
        sQrCode[5][sSizeOfCanvas - 11] = 0;
        sQrCode[6][sSizeOfCanvas - 11] = 1;
        sQrCode[7][sSizeOfCanvas - 11] = 0;
        sQrCode[sSizeOfCanvas - 11][2] = 1;
        sQrCode[sSizeOfCanvas - 11][3] = 0;
        sQrCode[sSizeOfCanvas - 11][4] = 0;
        sQrCode[sSizeOfCanvas - 11][5] = 0;
        sQrCode[sSizeOfCanvas - 11][6] = 1;
        sQrCode[sSizeOfCanvas - 11][7] = 0;
    }
    if (version == 15)
    {
        sQrCode[2][sSizeOfCanvas - 13] = 0;
        sQrCode[3][sSizeOfCanvas - 13] = 1;
        sQrCode[4][sSizeOfCanvas - 13] = 0;
        sQrCode[5][sSizeOfCanvas - 13] = 0;
        sQrCode[6][sSizeOfCanvas - 13] = 1;
        sQrCode[7][sSizeOfCanvas - 13] = 1;
        sQrCode[sSizeOfCanvas - 13][2] = 0;
        sQrCode[sSizeOfCanvas - 13][3] = 1;
        sQrCode[sSizeOfCanvas - 13][4] = 0;
        sQrCode[sSizeOfCanvas - 13][5] = 0;
        sQrCode[sSizeOfCanvas - 13][6] = 1;
        sQrCode[sSizeOfCanvas - 13][7] = 1;

        sQrCode[2][sSizeOfCanvas - 12] = 0;
        sQrCode[3][sSizeOfCanvas - 12] = 0;
        sQrCode[4][sSizeOfCanvas - 12] = 0;
        sQrCode[5][sSizeOfCanvas - 12] = 0;
        sQrCode[6][sSizeOfCanvas - 12] = 1;
        sQrCode[7][sSizeOfCanvas - 12] = 0;
        sQrCode[sSizeOfCanvas - 12][2] = 0;
        sQrCode[sSizeOfCanvas - 12][3] = 0;
        sQrCode[sSizeOfCanvas - 12][4] = 0;
        sQrCode[sSizeOfCanvas - 12][5] = 0;
        sQrCode[sSizeOfCanvas - 12][6] = 1;
        sQrCode[sSizeOfCanvas - 12][7] = 0;

        sQrCode[2][sSizeOfCanvas - 11] = 0;
        sQrCode[3][sSizeOfCanvas - 11] = 1;
        sQrCode[4][sSizeOfCanvas - 11] = 1;
        sQrCode[5][sSizeOfCanvas - 11] = 1;
        sQrCode[6][sSizeOfCanvas - 11] = 1;
        sQrCode[7][sSizeOfCanvas - 11] = 0;
        sQrCode[sSizeOfCanvas - 11][2] = 0;
        sQrCode[sSizeOfCanvas - 11][3] = 1;
        sQrCode[sSizeOfCanvas - 11][4] = 1;
        sQrCode[sSizeOfCanvas - 11][5] = 1;
        sQrCode[sSizeOfCanvas - 11][6] = 1;
        sQrCode[sSizeOfCanvas - 11][7] = 0;
    }
    if (version == 16)
    {
        sQrCode[2][sSizeOfCanvas - 13] = 0;
        sQrCode[3][sSizeOfCanvas - 13] = 1;
        sQrCode[4][sSizeOfCanvas - 13] = 1;
        sQrCode[5][sSizeOfCanvas - 13] = 1;
        sQrCode[6][sSizeOfCanvas - 13] = 0;
        sQrCode[7][sSizeOfCanvas - 13] = 0;
        sQrCode[sSizeOfCanvas - 13][2] = 0;
        sQrCode[sSizeOfCanvas - 13][3] = 1;
        sQrCode[sSizeOfCanvas - 13][4] = 1;
        sQrCode[sSizeOfCanvas - 13][5] = 1;
        sQrCode[sSizeOfCanvas - 13][6] = 0;
        sQrCode[sSizeOfCanvas - 13][7] = 0;

        sQrCode[2][sSizeOfCanvas - 12] = 0;
        sQrCode[3][sSizeOfCanvas - 12] = 1;
        sQrCode[4][sSizeOfCanvas - 12] = 0;
        sQrCode[5][sSizeOfCanvas - 12] = 0;
        sQrCode[6][sSizeOfCanvas - 12] = 0;
        sQrCode[7][sSizeOfCanvas - 12] = 1;
        sQrCode[sSizeOfCanvas - 12][2] = 0;
        sQrCode[sSizeOfCanvas - 12][3] = 1;
        sQrCode[sSizeOfCanvas - 12][4] = 0;
        sQrCode[sSizeOfCanvas - 12][5] = 0;
        sQrCode[sSizeOfCanvas - 12][6] = 0;
        sQrCode[sSizeOfCanvas - 12][7] = 1;

        sQrCode[2][sSizeOfCanvas - 11] = 0;
        sQrCode[3][sSizeOfCanvas - 11] = 1;
        sQrCode[4][sSizeOfCanvas - 11] = 1;
        sQrCode[5][sSizeOfCanvas - 11] = 1;
        sQrCode[6][sSizeOfCanvas - 11] = 0;
        sQrCode[7][sSizeOfCanvas - 11] = 0;
        sQrCode[sSizeOfCanvas - 11][2] = 0;
        sQrCode[sSizeOfCanvas - 11][3] = 1;
        sQrCode[sSizeOfCanvas - 11][4] = 1;
        sQrCode[sSizeOfCanvas - 11][5] = 1;
        sQrCode[sSizeOfCanvas - 11][6] = 0;
        sQrCode[sSizeOfCanvas - 11][7] = 0;
    }
    if (version == 17)
    {
        sQrCode[2][sSizeOfCanvas - 13] = 1;
        sQrCode[3][sSizeOfCanvas - 13] = 1;
        sQrCode[4][sSizeOfCanvas - 13] = 1;
        sQrCode[5][sSizeOfCanvas - 13] = 0;
        sQrCode[6][sSizeOfCanvas - 13] = 1;
        sQrCode[7][sSizeOfCanvas - 13] = 0;
        sQrCode[sSizeOfCanvas - 13][2] = 1;
        sQrCode[sSizeOfCanvas - 13][3] = 1;
        sQrCode[sSizeOfCanvas - 13][4] = 1;
        sQrCode[sSizeOfCanvas - 13][5] = 0;
        sQrCode[sSizeOfCanvas - 13][6] = 1;
        sQrCode[sSizeOfCanvas - 13][7] = 0;

        sQrCode[2][sSizeOfCanvas - 12] = 0;
        sQrCode[3][sSizeOfCanvas - 12] = 1;
        sQrCode[4][sSizeOfCanvas - 12] = 0;
        sQrCode[5][sSizeOfCanvas - 12] = 1;
        sQrCode[6][sSizeOfCanvas - 12] = 0;
        sQrCode[7][sSizeOfCanvas - 12] = 1;
        sQrCode[sSizeOfCanvas - 12][2] = 0;
        sQrCode[sSizeOfCanvas - 12][3] = 1;
        sQrCode[sSizeOfCanvas - 12][4] = 0;
        sQrCode[sSizeOfCanvas - 12][5] = 1;
        sQrCode[sSizeOfCanvas - 12][6] = 0;
        sQrCode[sSizeOfCanvas - 12][7] = 1;

        sQrCode[2][sSizeOfCanvas - 11] = 1;
        sQrCode[3][sSizeOfCanvas - 11] = 0;
        sQrCode[4][sSizeOfCanvas - 11] = 0;
        sQrCode[5][sSizeOfCanvas - 11] = 0;
        sQrCode[6][sSizeOfCanvas - 11] = 0;
        sQrCode[7][sSizeOfCanvas - 11] = 0;
        sQrCode[sSizeOfCanvas - 11][2] = 1;
        sQrCode[sSizeOfCanvas - 11][3] = 0;
        sQrCode[sSizeOfCanvas - 11][4] = 0;
        sQrCode[sSizeOfCanvas - 11][5] = 0;
        sQrCode[sSizeOfCanvas - 11][6] = 0;
        sQrCode[sSizeOfCanvas - 11][7] = 0;
    }
    if (version == 18)
    {
        sQrCode[2][sSizeOfCanvas - 13] = 1;
        sQrCode[3][sSizeOfCanvas - 13] = 0;
        sQrCode[4][sSizeOfCanvas - 13] = 0;
        sQrCode[5][sSizeOfCanvas - 13] = 1;
        sQrCode[6][sSizeOfCanvas - 13] = 0;
        sQrCode[7][sSizeOfCanvas - 13] = 0;
        sQrCode[sSizeOfCanvas - 13][2] = 1;
        sQrCode[sSizeOfCanvas - 13][3] = 0;
        sQrCode[sSizeOfCanvas - 13][4] = 0;
        sQrCode[sSizeOfCanvas - 13][5] = 1;
        sQrCode[sSizeOfCanvas - 13][6] = 0;
        sQrCode[sSizeOfCanvas - 13][7] = 0;

        sQrCode[2][sSizeOfCanvas - 12] = 1;
        sQrCode[3][sSizeOfCanvas - 12] = 1;
        sQrCode[4][sSizeOfCanvas - 12] = 0;
        sQrCode[5][sSizeOfCanvas - 12] = 0;
        sQrCode[6][sSizeOfCanvas - 12] = 1;
        sQrCode[7][sSizeOfCanvas - 12] = 1;
        sQrCode[sSizeOfCanvas - 12][2] = 1;
        sQrCode[sSizeOfCanvas - 12][3] = 1;
        sQrCode[sSizeOfCanvas - 12][4] = 0;
        sQrCode[sSizeOfCanvas - 12][5] = 0;
        sQrCode[sSizeOfCanvas - 12][6] = 1;
        sQrCode[sSizeOfCanvas - 12][7] = 1;

        sQrCode[2][sSizeOfCanvas - 11] = 1;
        sQrCode[3][sSizeOfCanvas - 11] = 0;
        sQrCode[4][sSizeOfCanvas - 11] = 0;
        sQrCode[5][sSizeOfCanvas - 11] = 1;
        sQrCode[6][sSizeOfCanvas - 11] = 0;
        sQrCode[7][sSizeOfCanvas - 11] = 0;
        sQrCode[sSizeOfCanvas - 11][2] = 1;
        sQrCode[sSizeOfCanvas - 11][3] = 0;
        sQrCode[sSizeOfCanvas - 11][4] = 0;
        sQrCode[sSizeOfCanvas - 11][5] = 1;
        sQrCode[sSizeOfCanvas - 11][6] = 0;
        sQrCode[sSizeOfCanvas - 11][7] = 0;
    }
    if (version == 19)
    {
        sQrCode[2][sSizeOfCanvas - 13] = 0;
        sQrCode[3][sSizeOfCanvas - 13] = 0;
        sQrCode[4][sSizeOfCanvas - 13] = 0;
        sQrCode[5][sSizeOfCanvas - 13] = 0;
        sQrCode[6][sSizeOfCanvas - 13] = 1;
        sQrCode[7][sSizeOfCanvas - 13] = 0;
        sQrCode[sSizeOfCanvas - 13][2] = 0;
        sQrCode[sSizeOfCanvas - 13][3] = 0;
        sQrCode[sSizeOfCanvas - 13][4] = 0;
        sQrCode[sSizeOfCanvas - 13][5] = 0;
        sQrCode[sSizeOfCanvas - 13][6] = 1;
        sQrCode[sSizeOfCanvas - 13][7] = 0;

        sQrCode[2][sSizeOfCanvas - 12] = 1;
        sQrCode[3][sSizeOfCanvas - 12] = 1;
        sQrCode[4][sSizeOfCanvas - 12] = 0;
        sQrCode[5][sSizeOfCanvas - 12] = 1;
        sQrCode[6][sSizeOfCanvas - 12] = 1;
        sQrCode[7][sSizeOfCanvas - 12] = 1;
        sQrCode[sSizeOfCanvas - 12][2] = 1;
        sQrCode[sSizeOfCanvas - 12][3] = 1;
        sQrCode[sSizeOfCanvas - 12][4] = 0;
        sQrCode[sSizeOfCanvas - 12][5] = 1;
        sQrCode[sSizeOfCanvas - 12][6] = 1;
        sQrCode[sSizeOfCanvas - 12][7] = 1;

        sQrCode[2][sSizeOfCanvas - 11] = 0;
        sQrCode[3][sSizeOfCanvas - 11] = 1;
        sQrCode[4][sSizeOfCanvas - 11] = 1;
        sQrCode[5][sSizeOfCanvas - 11] = 0;
        sQrCode[6][sSizeOfCanvas - 11] = 0;
        sQrCode[7][sSizeOfCanvas - 11] = 0;
        sQrCode[sSizeOfCanvas - 11][2] = 0;
        sQrCode[sSizeOfCanvas - 11][3] = 1;
        sQrCode[sSizeOfCanvas - 11][4] = 1;
        sQrCode[sSizeOfCanvas - 11][5] = 0;
        sQrCode[sSizeOfCanvas - 11][6] = 0;
        sQrCode[sSizeOfCanvas - 11][7] = 0;
    }
    if (version == 20)
    {
        sQrCode[2][sSizeOfCanvas - 13] = 0;
        sQrCode[3][sSizeOfCanvas - 13] = 0;
        sQrCode[4][sSizeOfCanvas - 13] = 0;
        sQrCode[5][sSizeOfCanvas - 13] = 0;
        sQrCode[6][sSizeOfCanvas - 13] = 0;
        sQrCode[7][sSizeOfCanvas - 13] = 0;
        sQrCode[sSizeOfCanvas - 13][2] = 0;
        sQrCode[sSizeOfCanvas - 13][3] = 0;
        sQrCode[sSizeOfCanvas - 13][4] = 0;
        sQrCode[sSizeOfCanvas - 13][5] = 0;
        sQrCode[sSizeOfCanvas - 13][6] = 0;
        sQrCode[sSizeOfCanvas - 13][7] = 0;

        sQrCode[2][sSizeOfCanvas - 12] = 1;
        sQrCode[3][sSizeOfCanvas - 12] = 0;
        sQrCode[4][sSizeOfCanvas - 12] = 1;
        sQrCode[5][sSizeOfCanvas - 12] = 0;
        sQrCode[6][sSizeOfCanvas - 12] = 0;
        sQrCode[7][sSizeOfCanvas - 12] = 1;
        sQrCode[sSizeOfCanvas - 12][2] = 1;
        sQrCode[sSizeOfCanvas - 12][3] = 0;
        sQrCode[sSizeOfCanvas - 12][4] = 1;
        sQrCode[sSizeOfCanvas - 12][5] = 0;
        sQrCode[sSizeOfCanvas - 12][6] = 0;
        sQrCode[sSizeOfCanvas - 12][7] = 1;

        sQrCode[2][sSizeOfCanvas - 11] = 1;
        sQrCode[3][sSizeOfCanvas - 11] = 1;
        sQrCode[4][sSizeOfCanvas - 11] = 1;
        sQrCode[5][sSizeOfCanvas - 11] = 1;
        sQrCode[6][sSizeOfCanvas - 11] = 1;
        sQrCode[7][sSizeOfCanvas - 11] = 0;
        sQrCode[sSizeOfCanvas - 11][2] = 1;
        sQrCode[sSizeOfCanvas - 11][3] = 1;
        sQrCode[sSizeOfCanvas - 11][4] = 1;
        sQrCode[sSizeOfCanvas - 11][5] = 1;
        sQrCode[sSizeOfCanvas - 11][6] = 1;
        sQrCode[sSizeOfCanvas - 11][7] = 0;
    }
    if (version == 21)
    {
        sQrCode[2][sSizeOfCanvas - 13] = 1;
        sQrCode[3][sSizeOfCanvas - 13] = 0;
        sQrCode[4][sSizeOfCanvas - 13] = 0;
        sQrCode[5][sSizeOfCanvas - 13] = 1;
        sQrCode[6][sSizeOfCanvas - 13] = 1;
        sQrCode[7][sSizeOfCanvas - 13] = 0;
        sQrCode[sSizeOfCanvas - 13][2] = 1;
        sQrCode[sSizeOfCanvas - 13][3] = 0;
        sQrCode[sSizeOfCanvas - 13][4] = 0;
        sQrCode[sSizeOfCanvas - 13][5] = 1;
        sQrCode[sSizeOfCanvas - 13][6] = 1;
        sQrCode[sSizeOfCanvas - 13][7] = 0;

        sQrCode[2][sSizeOfCanvas - 12] = 1;
        sQrCode[3][sSizeOfCanvas - 12] = 0;
        sQrCode[4][sSizeOfCanvas - 12] = 1;
        sQrCode[5][sSizeOfCanvas - 12] = 1;
        sQrCode[6][sSizeOfCanvas - 12] = 0;
        sQrCode[7][sSizeOfCanvas - 12] = 1;
        sQrCode[sSizeOfCanvas - 12][2] = 1;
        sQrCode[sSizeOfCanvas - 12][3] = 0;
        sQrCode[sSizeOfCanvas - 12][4] = 1;
        sQrCode[sSizeOfCanvas - 12][5] = 1;
        sQrCode[sSizeOfCanvas - 12][6] = 0;
        sQrCode[sSizeOfCanvas - 12][7] = 1;

        sQrCode[2][sSizeOfCanvas - 11] = 0;
        sQrCode[3][sSizeOfCanvas - 11] = 0;
        sQrCode[4][sSizeOfCanvas - 11] = 0;
        sQrCode[5][sSizeOfCanvas - 11] = 0;
        sQrCode[6][sSizeOfCanvas - 11] = 1;
        sQrCode[7][sSizeOfCanvas - 11] = 0;
        sQrCode[sSizeOfCanvas - 11][2] = 0;
        sQrCode[sSizeOfCanvas - 11][3] = 0;
        sQrCode[sSizeOfCanvas - 11][4] = 0;
        sQrCode[sSizeOfCanvas - 11][5] = 0;
        sQrCode[sSizeOfCanvas - 11][6] = 1;
        sQrCode[sSizeOfCanvas - 11][7] = 0;
    }
    if (version == 22)
    {
        sQrCode[2][sSizeOfCanvas - 13] = 1;
        sQrCode[3][sSizeOfCanvas - 13] = 1;
        sQrCode[4][sSizeOfCanvas - 13] = 1;
        sQrCode[5][sSizeOfCanvas - 13] = 0;
        sQrCode[6][sSizeOfCanvas - 13] = 0;
        sQrCode[7][sSizeOfCanvas - 13] = 0;
        sQrCode[sSizeOfCanvas - 13][2] = 1;
        sQrCode[sSizeOfCanvas - 13][3] = 1;
        sQrCode[sSizeOfCanvas - 13][4] = 1;
        sQrCode[sSizeOfCanvas - 13][5] = 0;
        sQrCode[sSizeOfCanvas - 13][6] = 0;
        sQrCode[sSizeOfCanvas - 13][7] = 0;

        sQrCode[2][sSizeOfCanvas - 12] = 0;
        sQrCode[3][sSizeOfCanvas - 12] = 0;
        sQrCode[4][sSizeOfCanvas - 12] = 1;
        sQrCode[5][sSizeOfCanvas - 12] = 0;
        sQrCode[6][sSizeOfCanvas - 12] = 1;
        sQrCode[7][sSizeOfCanvas - 12] = 1;
        sQrCode[sSizeOfCanvas - 12][2] = 0;
        sQrCode[sSizeOfCanvas - 12][3] = 0;
        sQrCode[sSizeOfCanvas - 12][4] = 1;
        sQrCode[sSizeOfCanvas - 12][5] = 0;
        sQrCode[sSizeOfCanvas - 12][6] = 1;
        sQrCode[sSizeOfCanvas - 12][7] = 1;

        sQrCode[2][sSizeOfCanvas - 11] = 0;
        sQrCode[3][sSizeOfCanvas - 11] = 0;
        sQrCode[4][sSizeOfCanvas - 11] = 0;
        sQrCode[5][sSizeOfCanvas - 11] = 1;
        sQrCode[6][sSizeOfCanvas - 11] = 1;
        sQrCode[7][sSizeOfCanvas - 11] = 0;
        sQrCode[sSizeOfCanvas - 11][2] = 0;
        sQrCode[sSizeOfCanvas - 11][3] = 0;
        sQrCode[sSizeOfCanvas - 11][4] = 0;
        sQrCode[sSizeOfCanvas - 11][5] = 1;
        sQrCode[sSizeOfCanvas - 11][6] = 1;
        sQrCode[sSizeOfCanvas - 11][7] = 0;
    }
    if (version == 23)
    {
        sQrCode[2][sSizeOfCanvas - 13] = 0;
        sQrCode[3][sSizeOfCanvas - 13] = 1;
        sQrCode[4][sSizeOfCanvas - 13] = 1;
        sQrCode[5][sSizeOfCanvas - 13] = 1;
        sQrCode[6][sSizeOfCanvas - 13] = 1;
        sQrCode[7][sSizeOfCanvas - 13] = 0;
        sQrCode[sSizeOfCanvas - 13][2] = 0;
        sQrCode[sSizeOfCanvas - 13][3] = 1;
        sQrCode[sSizeOfCanvas - 13][4] = 1;
        sQrCode[sSizeOfCanvas - 13][5] = 1;
        sQrCode[sSizeOfCanvas - 13][6] = 1;
        sQrCode[sSizeOfCanvas - 13][7] = 0;

        sQrCode[2][sSizeOfCanvas - 12] = 0;
        sQrCode[3][sSizeOfCanvas - 12] = 0;
        sQrCode[4][sSizeOfCanvas - 12] = 1;
        sQrCode[5][sSizeOfCanvas - 12] = 1;
        sQrCode[6][sSizeOfCanvas - 12] = 1;
        sQrCode[7][sSizeOfCanvas - 12] = 1;
        sQrCode[sSizeOfCanvas - 12][2] = 0;
        sQrCode[sSizeOfCanvas - 12][3] = 0;
        sQrCode[sSizeOfCanvas - 12][4] = 1;
        sQrCode[sSizeOfCanvas - 12][5] = 1;
        sQrCode[sSizeOfCanvas - 12][6] = 1;
        sQrCode[sSizeOfCanvas - 12][7] = 1;

        sQrCode[2][sSizeOfCanvas - 11] = 1;
        sQrCode[3][sSizeOfCanvas - 11] = 1;
        sQrCode[4][sSizeOfCanvas - 11] = 1;
        sQrCode[5][sSizeOfCanvas - 11] = 0;
        sQrCode[6][sSizeOfCanvas - 11] = 1;
        sQrCode[7][sSizeOfCanvas - 11] = 0;
        sQrCode[sSizeOfCanvas - 11][2] = 1;
        sQrCode[sSizeOfCanvas - 11][3] = 1;
        sQrCode[sSizeOfCanvas - 11][4] = 1;
        sQrCode[sSizeOfCanvas - 11][5] = 0;
        sQrCode[sSizeOfCanvas - 11][6] = 1;
        sQrCode[sSizeOfCanvas - 11][7] = 0;
    }
    if (version == 24)
    {
        sQrCode[2][sSizeOfCanvas - 13] = 0;
        sQrCode[3][sSizeOfCanvas - 13] = 0;
        sQrCode[4][sSizeOfCanvas - 13] = 1;
        sQrCode[5][sSizeOfCanvas - 13] = 1;
        sQrCode[6][sSizeOfCanvas - 13] = 0;
        sQrCode[7][sSizeOfCanvas - 13] = 1;
        sQrCode[sSizeOfCanvas - 13][2] = 0;
        sQrCode[sSizeOfCanvas - 13][3] = 0;
        sQrCode[sSizeOfCanvas - 13][4] = 1;
        sQrCode[sSizeOfCanvas - 13][5] = 1;
        sQrCode[sSizeOfCanvas - 13][6] = 0;
        sQrCode[sSizeOfCanvas - 13][7] = 1;

        sQrCode[2][sSizeOfCanvas - 12] = 0;
        sQrCode[3][sSizeOfCanvas - 12] = 0;
        sQrCode[4][sSizeOfCanvas - 12] = 1;
        sQrCode[5][sSizeOfCanvas - 12] = 1;
        sQrCode[6][sSizeOfCanvas - 12] = 0;
        sQrCode[7][sSizeOfCanvas - 12] = 1;
        sQrCode[sSizeOfCanvas - 12][2] = 0;
        sQrCode[sSizeOfCanvas - 12][3] = 0;
        sQrCode[sSizeOfCanvas - 12][4] = 1;
        sQrCode[sSizeOfCanvas - 12][5] = 1;
        sQrCode[sSizeOfCanvas - 12][6] = 0;
        sQrCode[sSizeOfCanvas - 12][7] = 1;

        sQrCode[2][sSizeOfCanvas - 11] = 1;
        sQrCode[3][sSizeOfCanvas - 11] = 0;
        sQrCode[4][sSizeOfCanvas - 11] = 0;
        sQrCode[5][sSizeOfCanvas - 11] = 1;
        sQrCode[6][sSizeOfCanvas - 11] = 0;
        sQrCode[7][sSizeOfCanvas - 11] = 0;
        sQrCode[sSizeOfCanvas - 11][2] = 1;
        sQrCode[sSizeOfCanvas - 11][3] = 0;
        sQrCode[sSizeOfCanvas - 11][4] = 0;
        sQrCode[sSizeOfCanvas - 11][5] = 1;
        sQrCode[sSizeOfCanvas - 11][6] = 0;
        sQrCode[sSizeOfCanvas - 11][7] = 0;
    }
    if (version == 25)
    {
        sQrCode[2][sSizeOfCanvas - 13] = 1;
        sQrCode[3][sSizeOfCanvas - 13] = 0;
        sQrCode[4][sSizeOfCanvas - 13] = 1;
        sQrCode[5][sSizeOfCanvas - 13] = 0;
        sQrCode[6][sSizeOfCanvas - 13] = 1;
        sQrCode[7][sSizeOfCanvas - 13] = 1;
        sQrCode[sSizeOfCanvas - 13][2] = 1;
        sQrCode[sSizeOfCanvas - 13][3] = 0;
        sQrCode[sSizeOfCanvas - 13][4] = 1;
        sQrCode[sSizeOfCanvas - 13][5] = 0;
        sQrCode[sSizeOfCanvas - 13][6] = 1;
        sQrCode[sSizeOfCanvas - 13][7] = 1;

        sQrCode[2][sSizeOfCanvas - 12] = 0;
        sQrCode[3][sSizeOfCanvas - 12] = 0;
        sQrCode[4][sSizeOfCanvas - 12] = 1;
        sQrCode[5][sSizeOfCanvas - 12] = 0;
        sQrCode[6][sSizeOfCanvas - 12] = 0;
        sQrCode[7][sSizeOfCanvas - 12] = 1;
        sQrCode[sSizeOfCanvas - 12][2] = 0;
        sQrCode[sSizeOfCanvas - 12][3] = 0;
        sQrCode[sSizeOfCanvas - 12][4] = 1;
        sQrCode[sSizeOfCanvas - 12][5] = 0;
        sQrCode[sSizeOfCanvas - 12][6] = 0;
        sQrCode[sSizeOfCanvas - 12][7] = 1;

        sQrCode[2][sSizeOfCanvas - 11] = 0;
        sQrCode[3][sSizeOfCanvas - 11] = 1;
        sQrCode[4][sSizeOfCanvas - 11] = 1;
        sQrCode[5][sSizeOfCanvas - 11] = 0;
        sQrCode[6][sSizeOfCanvas - 11] = 0;
        sQrCode[7][sSizeOfCanvas - 11] = 0;
        sQrCode[sSizeOfCanvas - 11][2] = 0;
        sQrCode[sSizeOfCanvas - 11][3] = 1;
        sQrCode[sSizeOfCanvas - 11][4] = 1;
        sQrCode[sSizeOfCanvas - 11][5] = 0;
        sQrCode[sSizeOfCanvas - 11][6] = 0;
        sQrCode[sSizeOfCanvas - 11][7] = 0;
    }
    if (version == 26)
    {
        sQrCode[2][sSizeOfCanvas - 13] = 1;
        sQrCode[3][sSizeOfCanvas - 13] = 1;
        sQrCode[4][sSizeOfCanvas - 13] = 0;
        sQrCode[5][sSizeOfCanvas - 13] = 1;
        sQrCode[6][sSizeOfCanvas - 13] = 0;
        sQrCode[7][sSizeOfCanvas - 13] = 1;
        sQrCode[sSizeOfCanvas - 13][2] = 1;
        sQrCode[sSizeOfCanvas - 13][3] = 1;
        sQrCode[sSizeOfCanvas - 13][4] = 0;
        sQrCode[sSizeOfCanvas - 13][5] = 1;
        sQrCode[sSizeOfCanvas - 13][6] = 0;
        sQrCode[sSizeOfCanvas - 13][7] = 1;

        sQrCode[2][sSizeOfCanvas - 12] = 1;
        sQrCode[3][sSizeOfCanvas - 12] = 0;
        sQrCode[4][sSizeOfCanvas - 12] = 1;
        sQrCode[5][sSizeOfCanvas - 12] = 1;
        sQrCode[6][sSizeOfCanvas - 12] = 1;
        sQrCode[7][sSizeOfCanvas - 12] = 1;
        sQrCode[sSizeOfCanvas - 12][2] = 1;
        sQrCode[sSizeOfCanvas - 12][3] = 0;
        sQrCode[sSizeOfCanvas - 12][4] = 1;
        sQrCode[sSizeOfCanvas - 12][5] = 1;
        sQrCode[sSizeOfCanvas - 12][6] = 1;
        sQrCode[sSizeOfCanvas - 12][7] = 1;

        sQrCode[2][sSizeOfCanvas - 11] = 0;
        sQrCode[3][sSizeOfCanvas - 11] = 1;
        sQrCode[4][sSizeOfCanvas - 11] = 1;
        sQrCode[5][sSizeOfCanvas - 11] = 1;
        sQrCode[6][sSizeOfCanvas - 11] = 0;
        sQrCode[7][sSizeOfCanvas - 11] = 0;
        sQrCode[sSizeOfCanvas - 11][2] = 0;
        sQrCode[sSizeOfCanvas - 11][3] = 1;
        sQrCode[sSizeOfCanvas - 11][4] = 1;
        sQrCode[sSizeOfCanvas - 11][5] = 1;
        sQrCode[sSizeOfCanvas - 11][6] = 0;
        sQrCode[sSizeOfCanvas - 11][7] = 0;
    }
    if (version == 27)
    {
        sQrCode[2][sSizeOfCanvas - 13] = 0;
        sQrCode[3][sSizeOfCanvas - 13] = 1;
        sQrCode[4][sSizeOfCanvas - 13] = 0;
        sQrCode[5][sSizeOfCanvas - 13] = 0;
        sQrCode[6][sSizeOfCanvas - 13] = 1;
        sQrCode[7][sSizeOfCanvas - 13] = 1;
        sQrCode[sSizeOfCanvas - 13][2] = 0;
        sQrCode[sSizeOfCanvas - 13][3] = 1;
        sQrCode[sSizeOfCanvas - 13][4] = 0;
        sQrCode[sSizeOfCanvas - 13][5] = 0;
        sQrCode[sSizeOfCanvas - 13][6] = 1;
        sQrCode[sSizeOfCanvas - 13][7] = 1;

        sQrCode[2][sSizeOfCanvas - 12] = 1;
        sQrCode[3][sSizeOfCanvas - 12] = 0;
        sQrCode[4][sSizeOfCanvas - 12] = 1;
        sQrCode[5][sSizeOfCanvas - 12] = 0;
        sQrCode[6][sSizeOfCanvas - 12] = 1;
        sQrCode[7][sSizeOfCanvas - 12] = 1;
        sQrCode[sSizeOfCanvas - 12][2] = 1;
        sQrCode[sSizeOfCanvas - 12][3] = 0;
        sQrCode[sSizeOfCanvas - 12][4] = 1;
        sQrCode[sSizeOfCanvas - 12][5] = 0;
        sQrCode[sSizeOfCanvas - 12][6] = 1;
        sQrCode[sSizeOfCanvas - 12][7] = 1;

        sQrCode[2][sSizeOfCanvas - 11] = 1;
        sQrCode[3][sSizeOfCanvas - 11] = 0;
        sQrCode[4][sSizeOfCanvas - 11] = 0;
        sQrCode[5][sSizeOfCanvas - 11] = 0;
        sQrCode[6][sSizeOfCanvas - 11] = 0;
        sQrCode[7][sSizeOfCanvas - 11] = 0;
        sQrCode[sSizeOfCanvas - 11][2] = 1;
        sQrCode[sSizeOfCanvas - 11][3] = 0;
        sQrCode[sSizeOfCanvas - 11][4] = 0;
        sQrCode[sSizeOfCanvas - 11][5] = 0;
        sQrCode[sSizeOfCanvas - 11][6] = 0;
        sQrCode[sSizeOfCanvas - 11][7] = 0;
    }
    if (version == 28)
    {
        sQrCode[2][sSizeOfCanvas - 13] = 0;
        sQrCode[3][sSizeOfCanvas - 13] = 1;
        sQrCode[4][sSizeOfCanvas - 13] = 0;
        sQrCode[5][sSizeOfCanvas - 13] = 0;
        sQrCode[6][sSizeOfCanvas - 13] = 0;
        sQrCode[7][sSizeOfCanvas - 13] = 1;
        sQrCode[sSizeOfCanvas - 13][2] = 0;
        sQrCode[sSizeOfCanvas - 13][3] = 1;
        sQrCode[sSizeOfCanvas - 13][4] = 0;
        sQrCode[sSizeOfCanvas - 13][5] = 0;
        sQrCode[sSizeOfCanvas - 13][6] = 0;
        sQrCode[sSizeOfCanvas - 13][7] = 1;

        sQrCode[2][sSizeOfCanvas - 12] = 1;
        sQrCode[3][sSizeOfCanvas - 12] = 1;
        sQrCode[4][sSizeOfCanvas - 12] = 0;
        sQrCode[5][sSizeOfCanvas - 12] = 1;
        sQrCode[6][sSizeOfCanvas - 12] = 0;
        sQrCode[7][sSizeOfCanvas - 12] = 1;
        sQrCode[sSizeOfCanvas - 12][2] = 1;
        sQrCode[sSizeOfCanvas - 12][3] = 1;
        sQrCode[sSizeOfCanvas - 12][4] = 0;
        sQrCode[sSizeOfCanvas - 12][5] = 1;
        sQrCode[sSizeOfCanvas - 12][6] = 0;
        sQrCode[sSizeOfCanvas - 12][7] = 1;

        sQrCode[2][sSizeOfCanvas - 11] = 0;
        sQrCode[3][sSizeOfCanvas - 11] = 0;
        sQrCode[4][sSizeOfCanvas - 11] = 0;
        sQrCode[5][sSizeOfCanvas - 11] = 1;
        sQrCode[6][sSizeOfCanvas - 11] = 1;
        sQrCode[7][sSizeOfCanvas - 11] = 0;
        sQrCode[sSizeOfCanvas - 11][2] = 0;
        sQrCode[sSizeOfCanvas - 11][3] = 0;
        sQrCode[sSizeOfCanvas - 11][4] = 0;
        sQrCode[sSizeOfCanvas - 11][5] = 1;
        sQrCode[sSizeOfCanvas - 11][6] = 1;
        sQrCode[sSizeOfCanvas - 11][7] = 0;
    }
    if (version == 29)
    {
        sQrCode[2][sSizeOfCanvas - 13] = 1;
        sQrCode[3][sSizeOfCanvas - 13] = 1;
        sQrCode[4][sSizeOfCanvas - 13] = 0;
        sQrCode[5][sSizeOfCanvas - 13] = 1;
        sQrCode[6][sSizeOfCanvas - 13] = 1;
        sQrCode[7][sSizeOfCanvas - 13] = 1;
        sQrCode[sSizeOfCanvas - 13][2] = 1;
        sQrCode[sSizeOfCanvas - 13][3] = 1;
        sQrCode[sSizeOfCanvas - 13][4] = 0;
        sQrCode[sSizeOfCanvas - 13][5] = 1;
        sQrCode[sSizeOfCanvas - 13][6] = 1;
        sQrCode[sSizeOfCanvas - 13][7] = 1;

        sQrCode[2][sSizeOfCanvas - 12] = 1;
        sQrCode[3][sSizeOfCanvas - 12] = 1;
        sQrCode[4][sSizeOfCanvas - 12] = 0;
        sQrCode[5][sSizeOfCanvas - 12] = 0;
        sQrCode[6][sSizeOfCanvas - 12] = 0;
        sQrCode[7][sSizeOfCanvas - 12] = 1;
        sQrCode[sSizeOfCanvas - 12][2] = 1;
        sQrCode[sSizeOfCanvas - 12][3] = 1;
        sQrCode[sSizeOfCanvas - 12][4] = 0;
        sQrCode[sSizeOfCanvas - 12][5] = 0;
        sQrCode[sSizeOfCanvas - 12][6] = 0;
        sQrCode[sSizeOfCanvas - 12][7] = 1;

        sQrCode[2][sSizeOfCanvas - 11] = 1;
        sQrCode[3][sSizeOfCanvas - 11] = 1;
        sQrCode[4][sSizeOfCanvas - 11] = 1;
        sQrCode[5][sSizeOfCanvas - 11] = 0;
        sQrCode[6][sSizeOfCanvas - 11] = 1;
        sQrCode[7][sSizeOfCanvas - 11] = 0;
        sQrCode[sSizeOfCanvas - 11][2] = 1;
        sQrCode[sSizeOfCanvas - 11][3] = 1;
        sQrCode[sSizeOfCanvas - 11][4] = 1;
        sQrCode[sSizeOfCanvas - 11][5] = 0;
        sQrCode[sSizeOfCanvas - 11][6] = 1;
        sQrCode[sSizeOfCanvas - 11][7] = 0;
    }
    if (version == 30)
    {
        sQrCode[2][sSizeOfCanvas - 13] = 1;
        sQrCode[3][sSizeOfCanvas - 13] = 0;
        sQrCode[4][sSizeOfCanvas - 13] = 1;
        sQrCode[5][sSizeOfCanvas - 13] = 0;
        sQrCode[6][sSizeOfCanvas - 13] = 0;
        sQrCode[7][sSizeOfCanvas - 13] = 1;
        sQrCode[sSizeOfCanvas - 13][2] = 1;
        sQrCode[sSizeOfCanvas - 13][3] = 0;
        sQrCode[sSizeOfCanvas - 13][4] = 1;
        sQrCode[sSizeOfCanvas - 13][5] = 0;
        sQrCode[sSizeOfCanvas - 13][6] = 0;
        sQrCode[sSizeOfCanvas - 13][7] = 1;

        sQrCode[2][sSizeOfCanvas - 12] = 0;
        sQrCode[3][sSizeOfCanvas - 12] = 1;
        sQrCode[4][sSizeOfCanvas - 12] = 0;
        sQrCode[5][sSizeOfCanvas - 12] = 1;
        sQrCode[6][sSizeOfCanvas - 12] = 1;
        sQrCode[7][sSizeOfCanvas - 12] = 1;
        sQrCode[sSizeOfCanvas - 12][2] = 0;
        sQrCode[sSizeOfCanvas - 12][3] = 1;
        sQrCode[sSizeOfCanvas - 12][4] = 0;
        sQrCode[sSizeOfCanvas - 12][5] = 1;
        sQrCode[sSizeOfCanvas - 12][6] = 1;
        sQrCode[sSizeOfCanvas - 12][7] = 1;

        sQrCode[2][sSizeOfCanvas - 11] = 1;
        sQrCode[3][sSizeOfCanvas - 11] = 1;
        sQrCode[4][sSizeOfCanvas - 11] = 1;
        sQrCode[5][sSizeOfCanvas - 11] = 1;
        sQrCode[6][sSizeOfCanvas - 11] = 1;
        sQrCode[7][sSizeOfCanvas - 11] = 0;
        sQrCode[sSizeOfCanvas - 11][2] = 1;
        sQrCode[sSizeOfCanvas - 11][3] = 1;
        sQrCode[sSizeOfCanvas - 11][4] = 1;
        sQrCode[sSizeOfCanvas - 11][5] = 1;
        sQrCode[sSizeOfCanvas - 11][6] = 1;
        sQrCode[sSizeOfCanvas - 11][7] = 0;
    }
    if (version == 31)
    {
        sQrCode[2][sSizeOfCanvas - 13] = 0;
        sQrCode[3][sSizeOfCanvas - 13] = 0;
        sQrCode[4][sSizeOfCanvas - 13] = 1;
        sQrCode[5][sSizeOfCanvas - 13] = 1;
        sQrCode[6][sSizeOfCanvas - 13] = 1;
        sQrCode[7][sSizeOfCanvas - 13] = 1;
        sQrCode[sSizeOfCanvas - 13][2] = 0;
        sQrCode[sSizeOfCanvas - 13][3] = 0;
        sQrCode[sSizeOfCanvas - 13][4] = 1;
        sQrCode[sSizeOfCanvas - 13][5] = 1;
        sQrCode[sSizeOfCanvas - 13][6] = 1;
        sQrCode[sSizeOfCanvas - 13][7] = 1;

        sQrCode[2][sSizeOfCanvas - 12] = 0;
        sQrCode[3][sSizeOfCanvas - 12] = 1;
        sQrCode[4][sSizeOfCanvas - 12] = 0;
        sQrCode[5][sSizeOfCanvas - 12] = 0;
        sQrCode[6][sSizeOfCanvas - 12] = 1;
        sQrCode[7][sSizeOfCanvas - 12] = 1;
        sQrCode[sSizeOfCanvas - 12][2] = 0;
        sQrCode[sSizeOfCanvas - 12][3] = 1;
        sQrCode[sSizeOfCanvas - 12][4] = 0;
        sQrCode[sSizeOfCanvas - 12][5] = 0;
        sQrCode[sSizeOfCanvas - 12][6] = 1;
        sQrCode[sSizeOfCanvas - 12][7] = 1;

        sQrCode[2][sSizeOfCanvas - 11] = 0;
        sQrCode[3][sSizeOfCanvas - 11] = 0;
        sQrCode[4][sSizeOfCanvas - 11] = 0;
        sQrCode[5][sSizeOfCanvas - 11] = 0;
        sQrCode[6][sSizeOfCanvas - 11] = 1;
        sQrCode[7][sSizeOfCanvas - 11] = 0;
        sQrCode[sSizeOfCanvas - 11][2] = 0;
        sQrCode[sSizeOfCanvas - 11][3] = 0;
        sQrCode[sSizeOfCanvas - 11][4] = 0;
        sQrCode[sSizeOfCanvas - 11][5] = 0;
        sQrCode[sSizeOfCanvas - 11][6] = 1;
        sQrCode[sSizeOfCanvas - 11][7] = 0;
    }
    if (version == 32)
    {
        sQrCode[2][sSizeOfCanvas - 13] = 1;
        sQrCode[3][sSizeOfCanvas - 13] = 0;
        sQrCode[4][sSizeOfCanvas - 13] = 1;
        sQrCode[5][sSizeOfCanvas - 13] = 0;
        sQrCode[6][sSizeOfCanvas - 13] = 0;
        sQrCode[7][sSizeOfCanvas - 13] = 0;
        sQrCode[sSizeOfCanvas - 13][2] = 1;
        sQrCode[sSizeOfCanvas - 13][3] = 0;
        sQrCode[sSizeOfCanvas - 13][4] = 1;
        sQrCode[sSizeOfCanvas - 13][5] = 0;
        sQrCode[sSizeOfCanvas - 13][6] = 0;
        sQrCode[sSizeOfCanvas - 13][7] = 0;

        sQrCode[2][sSizeOfCanvas - 12] = 0;
        sQrCode[3][sSizeOfCanvas - 12] = 1;
        sQrCode[4][sSizeOfCanvas - 12] = 1;
        sQrCode[5][sSizeOfCanvas - 12] = 0;
        sQrCode[6][sSizeOfCanvas - 12] = 0;
        sQrCode[7][sSizeOfCanvas - 12] = 0;
        sQrCode[sSizeOfCanvas - 12][2] = 0;
        sQrCode[sSizeOfCanvas - 12][3] = 1;
        sQrCode[sSizeOfCanvas - 12][4] = 1;
        sQrCode[sSizeOfCanvas - 12][5] = 0;
        sQrCode[sSizeOfCanvas - 12][6] = 0;
        sQrCode[sSizeOfCanvas - 12][7] = 0;

        sQrCode[2][sSizeOfCanvas - 11] = 1;
        sQrCode[3][sSizeOfCanvas - 11] = 0;
        sQrCode[4][sSizeOfCanvas - 11] = 1;
        sQrCode[5][sSizeOfCanvas - 11] = 1;
        sQrCode[6][sSizeOfCanvas - 11] = 0;
        sQrCode[7][sSizeOfCanvas - 11] = 1;
        sQrCode[sSizeOfCanvas - 11][2] = 1;
        sQrCode[sSizeOfCanvas - 11][3] = 0;
        sQrCode[sSizeOfCanvas - 11][4] = 1;
        sQrCode[sSizeOfCanvas - 11][5] = 1;
        sQrCode[sSizeOfCanvas - 11][6] = 0;
        sQrCode[sSizeOfCanvas - 11][7] = 1;
    }
    if (version == 33)
    {
        sQrCode[2][sSizeOfCanvas - 13] = 0;
        sQrCode[3][sSizeOfCanvas - 13] = 0;
        sQrCode[4][sSizeOfCanvas - 13] = 1;
        sQrCode[5][sSizeOfCanvas - 13] = 1;
        sQrCode[6][sSizeOfCanvas - 13] = 1;
        sQrCode[7][sSizeOfCanvas - 13] = 0;
        sQrCode[sSizeOfCanvas - 13][2] = 0;
        sQrCode[sSizeOfCanvas - 13][3] = 0;
        sQrCode[sSizeOfCanvas - 13][4] = 1;
        sQrCode[sSizeOfCanvas - 13][5] = 1;
        sQrCode[sSizeOfCanvas - 13][6] = 1;
        sQrCode[sSizeOfCanvas - 13][7] = 0;

        sQrCode[2][sSizeOfCanvas - 12] = 0;
        sQrCode[3][sSizeOfCanvas - 12] = 1;
        sQrCode[4][sSizeOfCanvas - 12] = 1;
        sQrCode[5][sSizeOfCanvas - 12] = 1;
        sQrCode[6][sSizeOfCanvas - 12] = 0;
        sQrCode[7][sSizeOfCanvas - 12] = 0;
        sQrCode[sSizeOfCanvas - 12][2] = 0;
        sQrCode[sSizeOfCanvas - 12][3] = 1;
        sQrCode[sSizeOfCanvas - 12][4] = 1;
        sQrCode[sSizeOfCanvas - 12][5] = 1;
        sQrCode[sSizeOfCanvas - 12][6] = 0;
        sQrCode[sSizeOfCanvas - 12][7] = 0;

        sQrCode[2][sSizeOfCanvas - 11] = 0;
        sQrCode[3][sSizeOfCanvas - 11] = 1;
        sQrCode[4][sSizeOfCanvas - 11] = 0;
        sQrCode[5][sSizeOfCanvas - 11] = 0;
        sQrCode[6][sSizeOfCanvas - 11] = 0;
        sQrCode[7][sSizeOfCanvas - 11] = 1;
        sQrCode[sSizeOfCanvas - 11][2] = 0;
        sQrCode[sSizeOfCanvas - 11][3] = 1;
        sQrCode[sSizeOfCanvas - 11][4] = 0;
        sQrCode[sSizeOfCanvas - 11][5] = 0;
        sQrCode[sSizeOfCanvas - 11][6] = 0;
        sQrCode[sSizeOfCanvas - 11][7] = 1;
    }
    if (version == 34)
    {
        sQrCode[2][sSizeOfCanvas - 13] = 0;
        sQrCode[3][sSizeOfCanvas - 13] = 1;
        sQrCode[4][sSizeOfCanvas - 13] = 0;
        sQrCode[5][sSizeOfCanvas - 13] = 0;
        sQrCode[6][sSizeOfCanvas - 13] = 0;
        sQrCode[7][sSizeOfCanvas - 13] = 0;
        sQrCode[sSizeOfCanvas - 13][2] = 0;
        sQrCode[sSizeOfCanvas - 13][3] = 1;
        sQrCode[sSizeOfCanvas - 13][4] = 0;
        sQrCode[sSizeOfCanvas - 13][5] = 0;
        sQrCode[sSizeOfCanvas - 13][6] = 0;
        sQrCode[sSizeOfCanvas - 13][7] = 0;

        sQrCode[2][sSizeOfCanvas - 12] = 1;
        sQrCode[3][sSizeOfCanvas - 12] = 1;
        sQrCode[4][sSizeOfCanvas - 12] = 1;
        sQrCode[5][sSizeOfCanvas - 12] = 0;
        sQrCode[6][sSizeOfCanvas - 12] = 1;
        sQrCode[7][sSizeOfCanvas - 12] = 0;
        sQrCode[sSizeOfCanvas - 12][2] = 1;
        sQrCode[sSizeOfCanvas - 12][3] = 1;
        sQrCode[sSizeOfCanvas - 12][4] = 1;
        sQrCode[sSizeOfCanvas - 12][5] = 0;
        sQrCode[sSizeOfCanvas - 12][6] = 1;
        sQrCode[sSizeOfCanvas - 12][7] = 0;

        sQrCode[2][sSizeOfCanvas - 11] = 0;
        sQrCode[3][sSizeOfCanvas - 11] = 1;
        sQrCode[4][sSizeOfCanvas - 11] = 0;
        sQrCode[5][sSizeOfCanvas - 11] = 1;
        sQrCode[6][sSizeOfCanvas - 11] = 0;
        sQrCode[7][sSizeOfCanvas - 11] = 1;
        sQrCode[sSizeOfCanvas - 11][2] = 0;
        sQrCode[sSizeOfCanvas - 11][3] = 1;
        sQrCode[sSizeOfCanvas - 11][4] = 0;
        sQrCode[sSizeOfCanvas - 11][5] = 1;
        sQrCode[sSizeOfCanvas - 11][6] = 0;
        sQrCode[sSizeOfCanvas - 11][7] = 1;
    }
    if (version == 35)
    {
        sQrCode[2][sSizeOfCanvas - 13] = 1;
        sQrCode[3][sSizeOfCanvas - 13] = 1;
        sQrCode[4][sSizeOfCanvas - 13] = 0;
        sQrCode[5][sSizeOfCanvas - 13] = 1;
        sQrCode[6][sSizeOfCanvas - 13] = 1;
        sQrCode[7][sSizeOfCanvas - 13] = 0;
        sQrCode[sSizeOfCanvas - 13][2] = 1;
        sQrCode[sSizeOfCanvas - 13][3] = 1;
        sQrCode[sSizeOfCanvas - 13][4] = 0;
        sQrCode[sSizeOfCanvas - 13][5] = 1;
        sQrCode[sSizeOfCanvas - 13][6] = 1;
        sQrCode[sSizeOfCanvas - 13][7] = 0;

        sQrCode[2][sSizeOfCanvas - 12] = 1;
        sQrCode[3][sSizeOfCanvas - 12] = 1;
        sQrCode[4][sSizeOfCanvas - 12] = 1;
        sQrCode[5][sSizeOfCanvas - 12] = 1;
        sQrCode[6][sSizeOfCanvas - 12] = 1;
        sQrCode[7][sSizeOfCanvas - 12] = 0;
        sQrCode[sSizeOfCanvas - 12][2] = 1;
        sQrCode[sSizeOfCanvas - 12][3] = 1;
        sQrCode[sSizeOfCanvas - 12][4] = 1;
        sQrCode[sSizeOfCanvas - 12][5] = 1;
        sQrCode[sSizeOfCanvas - 12][6] = 1;
        sQrCode[sSizeOfCanvas - 12][7] = 0;

        sQrCode[2][sSizeOfCanvas - 11] = 1;
        sQrCode[3][sSizeOfCanvas - 11] = 0;
        sQrCode[4][sSizeOfCanvas - 11] = 1;
        sQrCode[5][sSizeOfCanvas - 11] = 0;
        sQrCode[6][sSizeOfCanvas - 11] = 0;
        sQrCode[7][sSizeOfCanvas - 11] = 1;
        sQrCode[sSizeOfCanvas - 11][2] = 1;
        sQrCode[sSizeOfCanvas - 11][3] = 0;
        sQrCode[sSizeOfCanvas - 11][4] = 1;
        sQrCode[sSizeOfCanvas - 11][5] = 0;
        sQrCode[sSizeOfCanvas - 11][6] = 0;
        sQrCode[sSizeOfCanvas - 11][7] = 1;
    }
    if (version == 36)
    {
        sQrCode[2][sSizeOfCanvas - 13] = 1;
        sQrCode[3][sSizeOfCanvas - 13] = 1;
        sQrCode[4][sSizeOfCanvas - 13] = 0;
        sQrCode[5][sSizeOfCanvas - 13] = 1;
        sQrCode[6][sSizeOfCanvas - 13] = 0;
        sQrCode[7][sSizeOfCanvas - 13] = 0;
        sQrCode[sSizeOfCanvas - 13][2] = 1;
        sQrCode[sSizeOfCanvas - 13][3] = 1;
        sQrCode[sSizeOfCanvas - 13][4] = 0;
        sQrCode[sSizeOfCanvas - 13][5] = 1;
        sQrCode[sSizeOfCanvas - 13][6] = 0;
        sQrCode[sSizeOfCanvas - 13][7] = 0;

        sQrCode[2][sSizeOfCanvas - 12] = 1;
        sQrCode[3][sSizeOfCanvas - 12] = 0;
        sQrCode[4][sSizeOfCanvas - 12] = 0;
        sQrCode[5][sSizeOfCanvas - 12] = 0;
        sQrCode[6][sSizeOfCanvas - 12] = 0;
        sQrCode[7][sSizeOfCanvas - 12] = 0;
        sQrCode[sSizeOfCanvas - 12][2] = 1;
        sQrCode[sSizeOfCanvas - 12][3] = 0;
        sQrCode[sSizeOfCanvas - 12][4] = 0;
        sQrCode[sSizeOfCanvas - 12][5] = 0;
        sQrCode[sSizeOfCanvas - 12][6] = 0;
        sQrCode[sSizeOfCanvas - 12][7] = 0;

        sQrCode[2][sSizeOfCanvas - 11] = 0;
        sQrCode[3][sSizeOfCanvas - 11] = 0;
        sQrCode[4][sSizeOfCanvas - 11] = 1;
        sQrCode[5][sSizeOfCanvas - 11] = 1;
        sQrCode[6][sSizeOfCanvas - 11] = 1;
        sQrCode[7][sSizeOfCanvas - 11] = 1;
        sQrCode[sSizeOfCanvas - 11][2] = 0;
        sQrCode[sSizeOfCanvas - 11][3] = 0;
        sQrCode[sSizeOfCanvas - 11][4] = 1;
        sQrCode[sSizeOfCanvas - 11][5] = 1;
        sQrCode[sSizeOfCanvas - 11][6] = 1;
        sQrCode[sSizeOfCanvas - 11][7] = 1;
    }
    if (version == 37)
    {
        sQrCode[2][sSizeOfCanvas - 13] = 0;
        sQrCode[3][sSizeOfCanvas - 13] = 1;
        sQrCode[4][sSizeOfCanvas - 13] = 0;
        sQrCode[5][sSizeOfCanvas - 13] = 0;
        sQrCode[6][sSizeOfCanvas - 13] = 1;
        sQrCode[7][sSizeOfCanvas - 13] = 0;
        sQrCode[sSizeOfCanvas - 13][2] = 0;
        sQrCode[sSizeOfCanvas - 13][3] = 1;
        sQrCode[sSizeOfCanvas - 13][4] = 0;
        sQrCode[sSizeOfCanvas - 13][5] = 0;
        sQrCode[sSizeOfCanvas - 13][6] = 0;
        sQrCode[sSizeOfCanvas - 13][7] = 1;

        sQrCode[2][sSizeOfCanvas - 12] = 1;
        sQrCode[3][sSizeOfCanvas - 12] = 0;
        sQrCode[4][sSizeOfCanvas - 12] = 0;
        sQrCode[5][sSizeOfCanvas - 12] = 1;
        sQrCode[6][sSizeOfCanvas - 12] = 0;
        sQrCode[7][sSizeOfCanvas - 12] = 0;
        sQrCode[sSizeOfCanvas - 12][2] = 1;
        sQrCode[sSizeOfCanvas - 12][3] = 0;
        sQrCode[sSizeOfCanvas - 12][4] = 0;
        sQrCode[sSizeOfCanvas - 12][5] = 1;
        sQrCode[sSizeOfCanvas - 12][6] = 0;
        sQrCode[sSizeOfCanvas - 12][7] = 0;

        sQrCode[2][sSizeOfCanvas - 11] = 1;
        sQrCode[3][sSizeOfCanvas - 11] = 1;
        sQrCode[4][sSizeOfCanvas - 11] = 0;
        sQrCode[5][sSizeOfCanvas - 11] = 0;
        sQrCode[6][sSizeOfCanvas - 11] = 1;
        sQrCode[7][sSizeOfCanvas - 11] = 1;
        sQrCode[sSizeOfCanvas - 11][2] = 1;
        sQrCode[sSizeOfCanvas - 11][3] = 1;
        sQrCode[sSizeOfCanvas - 11][4] = 0;
        sQrCode[sSizeOfCanvas - 11][5] = 0;
        sQrCode[sSizeOfCanvas - 11][6] = 1;
        sQrCode[sSizeOfCanvas - 11][7] = 1;
    }
    if (version == 38)
    {
        sQrCode[2][sSizeOfCanvas - 13] = 0;
        sQrCode[3][sSizeOfCanvas - 13] = 0;
        sQrCode[4][sSizeOfCanvas - 13] = 1;
        sQrCode[5][sSizeOfCanvas - 13] = 1;
        sQrCode[6][sSizeOfCanvas - 13] = 0;
        sQrCode[7][sSizeOfCanvas - 13] = 0;
        sQrCode[sSizeOfCanvas - 13][2] = 0;
        sQrCode[sSizeOfCanvas - 13][3] = 0;
        sQrCode[sSizeOfCanvas - 13][4] = 1;
        sQrCode[sSizeOfCanvas - 13][5] = 1;
        sQrCode[sSizeOfCanvas - 13][6] = 0;
        sQrCode[sSizeOfCanvas - 13][7] = 0;

        sQrCode[2][sSizeOfCanvas - 12] = 0;
        sQrCode[3][sSizeOfCanvas - 12] = 0;
        sQrCode[4][sSizeOfCanvas - 12] = 0;
        sQrCode[5][sSizeOfCanvas - 12] = 0;
        sQrCode[6][sSizeOfCanvas - 12] = 1;
        sQrCode[7][sSizeOfCanvas - 12] = 0;
        sQrCode[sSizeOfCanvas - 12][2] = 0;
        sQrCode[sSizeOfCanvas - 12][3] = 0;
        sQrCode[sSizeOfCanvas - 12][4] = 0;
        sQrCode[sSizeOfCanvas - 12][5] = 0;
        sQrCode[sSizeOfCanvas - 12][6] = 1;
        sQrCode[sSizeOfCanvas - 12][7] = 0;

        sQrCode[2][sSizeOfCanvas - 11] = 1;
        sQrCode[3][sSizeOfCanvas - 11] = 1;
        sQrCode[4][sSizeOfCanvas - 11] = 0;
        sQrCode[5][sSizeOfCanvas - 11] = 1;
        sQrCode[6][sSizeOfCanvas - 11] = 1;
        sQrCode[7][sSizeOfCanvas - 11] = 1;
        sQrCode[sSizeOfCanvas - 11][2] = 1;
        sQrCode[sSizeOfCanvas - 11][3] = 1;
        sQrCode[sSizeOfCanvas - 11][4] = 0;
        sQrCode[sSizeOfCanvas - 11][5] = 1;
        sQrCode[sSizeOfCanvas - 11][6] = 1;
        sQrCode[sSizeOfCanvas - 11][7] = 1;
    }
    if (version == 39)
    {
        sQrCode[2][sSizeOfCanvas - 13] = 1;
        sQrCode[3][sSizeOfCanvas - 13] = 0;
        sQrCode[4][sSizeOfCanvas - 13] = 1;
        sQrCode[5][sSizeOfCanvas - 13] = 0;
        sQrCode[6][sSizeOfCanvas - 13] = 1;
        sQrCode[7][sSizeOfCanvas - 13] = 0;
        sQrCode[sSizeOfCanvas - 13][2] = 1;
        sQrCode[sSizeOfCanvas - 13][3] = 0;
        sQrCode[sSizeOfCanvas - 13][4] = 1;
        sQrCode[sSizeOfCanvas - 13][5] = 0;
        sQrCode[sSizeOfCanvas - 13][6] = 1;
        sQrCode[sSizeOfCanvas - 13][7] = 0;

        sQrCode[2][sSizeOfCanvas - 12] = 0;
        sQrCode[3][sSizeOfCanvas - 12] = 0;
        sQrCode[4][sSizeOfCanvas - 12] = 0;
        sQrCode[5][sSizeOfCanvas - 12] = 1;
        sQrCode[6][sSizeOfCanvas - 12] = 1;
        sQrCode[7][sSizeOfCanvas - 12] = 0;
        sQrCode[sSizeOfCanvas - 12][2] = 0;
        sQrCode[sSizeOfCanvas - 12][3] = 0;
        sQrCode[sSizeOfCanvas - 12][4] = 0;
        sQrCode[sSizeOfCanvas - 12][5] = 1;
        sQrCode[sSizeOfCanvas - 12][6] = 1;
        sQrCode[sSizeOfCanvas - 12][7] = 0;

        sQrCode[2][sSizeOfCanvas - 11] = 0;
        sQrCode[3][sSizeOfCanvas - 11] = 0;
        sQrCode[4][sSizeOfCanvas - 11] = 1;
        sQrCode[5][sSizeOfCanvas - 11] = 0;
        sQrCode[6][sSizeOfCanvas - 11] = 1;
        sQrCode[7][sSizeOfCanvas - 11] = 1;
        sQrCode[sSizeOfCanvas - 11][2] = 0;
        sQrCode[sSizeOfCanvas - 11][3] = 0;
        sQrCode[sSizeOfCanvas - 11][4] = 1;
        sQrCode[sSizeOfCanvas - 11][5] = 0;
        sQrCode[sSizeOfCanvas - 11][6] = 1;
        sQrCode[sSizeOfCanvas - 11][7] = 1;
    }
    if (version == 40)
    {
        sQrCode[2][sSizeOfCanvas - 13] = 1;
        sQrCode[3][sSizeOfCanvas - 13] = 1;
        sQrCode[4][sSizeOfCanvas - 13] = 1;
        sQrCode[5][sSizeOfCanvas - 13] = 0;
        sQrCode[6][sSizeOfCanvas - 13] = 0;
        sQrCode[7][sSizeOfCanvas - 13] = 1;
        sQrCode[sSizeOfCanvas - 13][2] = 1;
        sQrCode[sSizeOfCanvas - 13][3] = 1;
        sQrCode[sSizeOfCanvas - 13][4] = 1;
        sQrCode[sSizeOfCanvas - 13][5] = 0;
        sQrCode[sSizeOfCanvas - 13][6] = 0;
        sQrCode[sSizeOfCanvas - 13][7] = 1;

        sQrCode[2][sSizeOfCanvas - 12] = 0;
        sQrCode[3][sSizeOfCanvas - 12] = 0;
        sQrCode[4][sSizeOfCanvas - 12] = 0;
        sQrCode[5][sSizeOfCanvas - 12] = 1;
        sQrCode[6][sSizeOfCanvas - 12] = 0;
        sQrCode[7][sSizeOfCanvas - 12] = 0;
        sQrCode[sSizeOfCanvas - 12][2] = 0;
        sQrCode[sSizeOfCanvas - 12][3] = 0;
        sQrCode[sSizeOfCanvas - 12][4] = 0;
        sQrCode[sSizeOfCanvas - 12][5] = 1;
        sQrCode[sSizeOfCanvas - 12][6] = 0;
        sQrCode[sSizeOfCanvas - 12][7] = 0;

        sQrCode[2][sSizeOfCanvas - 11] = 0;
        sQrCode[3][sSizeOfCanvas - 11] = 1;
        sQrCode[4][sSizeOfCanvas - 11] = 0;
        sQrCode[5][sSizeOfCanvas - 11] = 1;
        sQrCode[6][sSizeOfCanvas - 11] = 0;
        sQrCode[7][sSizeOfCanvas - 11] = 1;
        sQrCode[sSizeOfCanvas - 11][2] = 0;
        sQrCode[sSizeOfCanvas - 11][3] = 1;
        sQrCode[sSizeOfCanvas - 11][4] = 0;
        sQrCode[sSizeOfCanvas - 11][5] = 1;
        sQrCode[sSizeOfCanvas - 11][6] = 0;
        sQrCode[sSizeOfCanvas - 11][7] = 1;
    }
    //заполнение кода маски и кода уровня коррекции
    sQrCode[10][2] = 1;
    sQrCode[10][3] = 0;
    sQrCode[10][4] = 1;
    sQrCode[10][5] = 0;
    sQrCode[10][6] = 1;
    sQrCode[10][7] = 0;
    sQrCode[10][9] = 0;
    sQrCode[10][10] = 0;
    sQrCode[9][10] = 0;
    sQrCode[7][10] = 0;
    sQrCode[6][10] = 1;
    sQrCode[5][10] = 0;
    sQrCode[4][10] = 0;
    sQrCode[3][10] = 1;
    sQrCode[2][10] = 0;
    sQrCode[sSizeOfCanvas - 3][10] = 1;
    sQrCode[sSizeOfCanvas - 4][10] = 0;
    sQrCode[sSizeOfCanvas - 5][10] = 1;
    sQrCode[sSizeOfCanvas - 6][10] = 0;
    sQrCode[sSizeOfCanvas - 7][10] = 1;
    sQrCode[sSizeOfCanvas - 8][10] = 0;
    sQrCode[sSizeOfCanvas - 9][10] = 0;
    sQrCode[sSizeOfCanvas - 10][10] = 1;
    sQrCode[10][sSizeOfCanvas - 10] = 0;
    sQrCode[10][sSizeOfCanvas - 9] = 0;
    sQrCode[10][sSizeOfCanvas - 8] = 0;
    sQrCode[10][sSizeOfCanvas - 7] = 1;
    sQrCode[10][sSizeOfCanvas - 6] = 0;
    sQrCode[10][sSizeOfCanvas - 5] = 0;
    sQrCode[10][sSizeOfCanvas - 4] = 1;
    sQrCode[10][sSizeOfCanvas - 3] = 0;
    //заполнение битами данных
    int amountOfCouples = (sSizeOfCanvas - 5) / 2;
    int fillerIndex = 0;
    int currentCouple = 1;
    for (currentCouple = 1; currentCouple <= 4; ++currentCouple)
    {
        if (fillerIndex >= finalBits.size() - 1)
            break;
        if (currentCouple % 2 != 0)
        {
            for (int line = sSizeOfCanvas - 3; line >= 11; --line)
            {
                if (fillerIndex >= finalBits.size() - 1)
                    break;
                if ((sQrCode[line][sSizeOfCanvas - 1 - 2 * currentCouple] == 2) &&
                    ((line + sSizeOfCanvas - 1 - 2 * currentCouple) % 2 != 0))
                    sQrCode[line][sSizeOfCanvas - 1 - 2 * currentCouple] = finalBits[fillerIndex++];
                else if ((sQrCode[line][sSizeOfCanvas - 1 - 2 * currentCouple] == 2) &&
                         ((line + sSizeOfCanvas - 1 - 2 * currentCouple) % 2 == 0))
                {
                    if (finalBits[fillerIndex] == 0)
                    {
                        sQrCode[line][sSizeOfCanvas - 1 - 2 * currentCouple] = 1;
                        ++fillerIndex;
                    }
                    else
                    {
                        sQrCode[line][sSizeOfCanvas - 1 - 2 * currentCouple] = 0;
                        ++fillerIndex;
                    }
                }
                if ((sQrCode[line][sSizeOfCanvas - 2 - 2 * currentCouple] == 2) &&
                    ((line + sSizeOfCanvas - 2 - 2 * currentCouple) % 2 != 0))
                    sQrCode[line][sSizeOfCanvas - 2 - 2 * currentCouple] = finalBits[fillerIndex++];
                else if ((sQrCode[line][sSizeOfCanvas - 2 - 2 * currentCouple] == 2) &&
                         ((line + sSizeOfCanvas - 2 - 2 * currentCouple) % 2 == 0))
                {
                    if (finalBits[fillerIndex] == 0)
                    {
                        sQrCode[line][sSizeOfCanvas - 2 - 2 * currentCouple] = 1;
                        ++fillerIndex;
                    }
                    else
                    {
                        sQrCode[line][sSizeOfCanvas - 2 - 2 * currentCouple] = 0;
                        ++fillerIndex;
                    }
                }
            }
        }
        if (currentCouple % 2 == 0)
        {
            for (int line = 11; line <= sSizeOfCanvas - 3; ++line)
            {
                if (fillerIndex >= finalBits.size() - 1)
                    break;
                if ((sQrCode[line][sSizeOfCanvas - 1 - 2 * currentCouple] == 2) &&
                    ((line + sSizeOfCanvas - 1 - 2 * currentCouple) % 2 != 0))
                    sQrCode[line][sSizeOfCanvas - 1 - 2 * currentCouple] = finalBits[fillerIndex++];
                else if ((sQrCode[line][sSizeOfCanvas - 1 - 2 * currentCouple] == 2) &&
                         ((line + sSizeOfCanvas - 1 - 2 * currentCouple) % 2 == 0))
                {
                    if (finalBits[fillerIndex] == 0)
                    {
                        sQrCode[line][sSizeOfCanvas - 1 - 2 * currentCouple] = 1;
                        ++fillerIndex;
                    }
                    else
                    {
                        sQrCode[line][sSizeOfCanvas - 1 - 2 * currentCouple] = 0;
                        ++fillerIndex;
                    }
                }
                if ((sQrCode[line][sSizeOfCanvas - 2 - 2 * currentCouple] == 2) &&
                    ((line + sSizeOfCanvas - 2 - 2 * currentCouple) % 2 != 0))
                    sQrCode[line][sSizeOfCanvas - 2 - 2 * currentCouple] = finalBits[fillerIndex++];
                else if ((sQrCode[line][sSizeOfCanvas - 2 - 2 * currentCouple] == 2) &&
                         ((line + sSizeOfCanvas - 2 - 2 * currentCouple) % 2 == 0))
                {
                    if (finalBits[fillerIndex] == 0)
                    {
                        sQrCode[line][sSizeOfCanvas - 2 - 2 * currentCouple] = 1;
                        ++fillerIndex;
                    }
                    else
                    {
                        sQrCode[line][sSizeOfCanvas - 2 - 2 * currentCouple] = 0;
                        ++fillerIndex;
                    }
                }
            }
        }
    }
    for (currentCouple = 4; currentCouple <= amountOfCouples - 3; ++currentCouple)
    {
        if (fillerIndex >= finalBits.size() - 1)
            break;
        if (currentCouple % 2 != 0)
        {
            if (fillerIndex >= finalBits.size() - 1)
                break;
            for (int line = sSizeOfCanvas - 3; line >= 2; --line)
            {
                if ((sQrCode[line][sSizeOfCanvas - 1 - 2 * currentCouple] == 2) &&
                    ((line + sSizeOfCanvas - 1 - 2 * currentCouple) % 2 != 0))
                    sQrCode[line][sSizeOfCanvas - 1 - 2 * currentCouple] = finalBits[fillerIndex++];
                else if ((sQrCode[line][sSizeOfCanvas - 1 - 2 * currentCouple] == 2) &&
                         ((line + sSizeOfCanvas - 1 - 2 * currentCouple) % 2 == 0))
                {
                    if (finalBits[fillerIndex] == 0)
                    {
                        sQrCode[line][sSizeOfCanvas - 1 - 2 * currentCouple] = 1;
                        ++fillerIndex;
                    }
                    else
                    {
                        sQrCode[line][sSizeOfCanvas - 1 - 2 * currentCouple] = 0;
                        ++fillerIndex;
                    }
                }
                if ((sQrCode[line][sSizeOfCanvas - 2 - 2 * currentCouple] == 2) &&
                    ((line + sSizeOfCanvas - 2 - 2 * currentCouple) % 2 != 0))
                    sQrCode[line][sSizeOfCanvas - 2 - 2 * currentCouple] = finalBits[fillerIndex++];
                else if ((sQrCode[line][sSizeOfCanvas - 2 - 2 * currentCouple] == 2) &&
                         ((line + sSizeOfCanvas - 2 - 2 * currentCouple) % 2 == 0))
                {
                    if (finalBits[fillerIndex] == 0)
                    {
                        sQrCode[line][sSizeOfCanvas - 2 - 2 * currentCouple] = 1;
                        ++fillerIndex;
                    }
                    else
                    {
                        sQrCode[line][sSizeOfCanvas - 2 - 2 * currentCouple] = 0;
                        ++fillerIndex;
                    }
                }
            }
        }
        if (currentCouple % 2 == 0)
        {
            for (int line = 2; line <= sSizeOfCanvas - 3; ++line)
            {
                if (fillerIndex >= finalBits.size() - 1)
                    break;
                if ((sQrCode[line][sSizeOfCanvas - 1 - 2 * currentCouple] == 2) &&
                    ((line + sSizeOfCanvas - 1 - 2 * currentCouple) % 2 != 0))
                    sQrCode[line][sSizeOfCanvas - 1 - 2 * currentCouple] = finalBits[fillerIndex++];
                else if ((sQrCode[line][sSizeOfCanvas - 1 - 2 * currentCouple] == 2) &&
                         ((line + sSizeOfCanvas - 1 - 2 * currentCouple) % 2 == 0))
                {
                    if (finalBits[fillerIndex] == 0)
                    {
                        sQrCode[line][sSizeOfCanvas - 1 - 2 * currentCouple] = 1;
                        ++fillerIndex;
                    }
                    else
                    {
                        sQrCode[line][sSizeOfCanvas - 1 - 2 * currentCouple] = 0;
                        ++fillerIndex;
                    }
                }
                if ((sQrCode[line][sSizeOfCanvas - 2 - 2 * currentCouple] == 2) &&
                    ((line + sSizeOfCanvas - 2 - 2 * currentCouple) % 2 != 0))
                    sQrCode[line][sSizeOfCanvas - 2 - 2 * currentCouple] = finalBits[fillerIndex++];
                else if ((sQrCode[line][sSizeOfCanvas - 2 - 2 * currentCouple] == 2) &&
                         ((line + sSizeOfCanvas - 2 - 2 * currentCouple) % 2 == 0))
                {
                    if (finalBits[fillerIndex] == 0)
                    {
                        sQrCode[line][sSizeOfCanvas - 2 - 2 * currentCouple] = 1;
                        ++fillerIndex;
                    }
                    else
                    {
                        sQrCode[line][sSizeOfCanvas - 2 - 2 * currentCouple] = 0;
                        ++fillerIndex;
                    }
                }
            }
        }
    }
    for (currentCouple = amountOfCouples - 3; currentCouple <= amountOfCouples; ++currentCouple)
    {
        if (fillerIndex >= finalBits.size())
            break;
        if (currentCouple % 2 != 0)
        {
            for (int line = sSizeOfCanvas - 3; line >= 2; --line)
            {
                if (fillerIndex >= finalBits.size())
                    break;
                if ((sQrCode[line][sSizeOfCanvas - 2 - 2 * currentCouple] == 2) &&
                    ((line + sSizeOfCanvas - 2 - 2 * currentCouple) % 2 != 0))
                    sQrCode[line][sSizeOfCanvas - 2 - 2 * currentCouple] = finalBits[fillerIndex++];
                else if ((sQrCode[line][sSizeOfCanvas - 2 - 2 * currentCouple] == 2) &&
                         ((line + sSizeOfCanvas - 2 - 2 * currentCouple) % 2 == 0))
                {
                    if (finalBits[fillerIndex] == 0)
                    {
                        sQrCode[line][sSizeOfCanvas - 2 - 2 * currentCouple] = 1;
                        ++fillerIndex;
                    }
                    else
                    {
                        sQrCode[line][sSizeOfCanvas - 2 - 2 * currentCouple] = 0;
                        ++fillerIndex;
                    }
                }
                if ((sQrCode[line][sSizeOfCanvas - 3 - 2 * currentCouple] == 2) &&
                    ((line + sSizeOfCanvas - 3 - 2 * currentCouple) % 2 != 0))
                    sQrCode[line][sSizeOfCanvas - 3 - 2 * currentCouple] = finalBits[fillerIndex++];
                else if ((sQrCode[line][sSizeOfCanvas - 3 - 2 * currentCouple] == 2) &&
                         ((line + sSizeOfCanvas - 3 - 2 * currentCouple) % 2 == 0))
                {
                    if (finalBits[fillerIndex] == 0)
                    {
                        sQrCode[line][sSizeOfCanvas - 3 - 2 * currentCouple] = 1;
                        ++fillerIndex;
                    }
                    else
                    {
                        sQrCode[line][sSizeOfCanvas - 3 - 2 * currentCouple] = 0;
                        ++fillerIndex;
                    }
                }
            }
        }
        if (currentCouple % 2 == 0)
        {
            for (int line = 2; line <= sSizeOfCanvas - 3; ++line)
            {
                if (fillerIndex >= finalBits.size() - 1)
                    break;
                if ((sQrCode[line][sSizeOfCanvas - 2 - 2 * currentCouple] == 2) &&
                    ((line + sSizeOfCanvas - 2 - 2 * currentCouple) % 2 != 0))
                {
                    sQrCode[line][sSizeOfCanvas - 2 - 2 * currentCouple] = finalBits[fillerIndex];
                    ++fillerIndex;
                }
                else if ((sQrCode[line][sSizeOfCanvas - 2 - 2 * currentCouple] == 2) &&
                         ((line + sSizeOfCanvas - 2 - 2 * currentCouple) % 2 == 0))
                {
                    if (finalBits[fillerIndex] == 0)
                    {
                        sQrCode[line][sSizeOfCanvas - 2 - 2 * currentCouple] = 1;
                        ++fillerIndex;
                    }
                    else
                    {
                        sQrCode[line][sSizeOfCanvas - 2 - 2 * currentCouple] = 0;
                        ++fillerIndex;
                    }
                }

                if ((sQrCode[line][sSizeOfCanvas - 3 - 2 * currentCouple] == 2) &&
                    ((line + sSizeOfCanvas - 3 - 2 * currentCouple) % 2 != 0))
                {
                    sQrCode[line][sSizeOfCanvas - 3 - 2 * currentCouple] = finalBits[fillerIndex];
                    ++fillerIndex;
                }
                else if ((sQrCode[line][sSizeOfCanvas - 3 - 2 * currentCouple] == 2) &&
                         ((line + sSizeOfCanvas - 3 - 2 * currentCouple) % 2 == 0))
                {
                    if (finalBits[fillerIndex] == 0)
                    {
                        sQrCode[line][sSizeOfCanvas - 3 - 2 * currentCouple] = 1;
                        ++fillerIndex;
                    }
                    else
                    {
                        sQrCode[line][sSizeOfCanvas - 3 - 2 * currentCouple] = 0;
                        ++fillerIndex;
                    }
                }
            }
        }
    }
    //замена пустых значений нулями  и использование на них маски
    for (int i = 0; i < sSizeOfCanvas; ++i)
    {
        for (int j = 0; j < sSizeOfCanvas; ++j)
        {
            if (qrCode[i][j] == 2)
            {
                if ((i + j) % 2 != 0)
                    qrCode[i][j] = 0;
                else
                    qrCode[i][j] = 1;
            }
        }
    }
}
