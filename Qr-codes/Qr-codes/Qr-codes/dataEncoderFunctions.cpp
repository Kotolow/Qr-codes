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
