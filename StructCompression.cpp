#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <vector>
#include <algorithm>

#include <string>


class CompressionTableElement
{
    public:

    CompressionTableElement()
    {

    }
    CompressionTableElement(char value, int count_)
    {
        x = value;
        count = count_;
    }

    int operator < (CompressionTableElement& other)
    {
        return count - other.count;
    }

    int operator > (CompressionTableElement& other)
    {
        return other.count - count;
    }

    static bool isless (CompressionTableElement& i,CompressionTableElement& j) { return (i.count>j.count); }


    char x;
    int count;
};


class CodeTable
{

    char* symbols;
    int length;
    public:
    char* result;
    int resultSize;

    CodeTable(char* sortedSymbols, int size)
    {
        resultSize = 0;
        result = 0;
        length = size;
        symbols = (char*)calloc(size, sizeof(char));
        for(int i = 0; i < size; i++)
        {
            symbols[i] = sortedSymbols[i];
        }
    }

    size_t size()
    {
        return length*sizeof(char) + sizeof(int)*2 + resultSize*sizeof(char);
    }

    ~CodeTable()
    {
        free(symbols);
        if(result != 0)
        {
            //free(result);
        }
    }

    void compress(char* input, int inputSz)
    {
        result = (char*)calloc(inputSz+1, 1);
        resultSize = inputSz+1;
        int bitPointer = 0;
        int biggest = 0;
        for(int i = 0; i < inputSz; i++)
        {
            for(int s = 0; s < length; s++)
            {
                if(symbols[s] == input[i])
                {
                    bitPointer += s;
                    int byte = bitPointer / 8;
                    int bit = bitPointer - 8*byte;
                    if(byte > resultSize-1)
                    {
                        resultSize++;
                        char* tmp;
                        char* that;
                        /*try
                        {
                            char* that = (char*)realloc(result, resultSize);
                        }
                        catch(const std::exception& e)
                        {*/
                            tmp = (char*)calloc(resultSize+50, 1);
                            memcpy(tmp, result, resultSize-1);
                            free(result);
                            result = 0;
                            result = (char*)calloc(resultSize+50, 1);
                            memset(result, 0, resultSize+50);
                            memcpy(result, tmp, resultSize-1);
                            free(tmp);
                            //result = tmp;
                            //result = (char*)realloc(result, resultSize+50);
                            resultSize += 50;
                        //}
                        result[resultSize-1] = 0;
                    }
                    biggest = byte;
                    result[byte] |= 1 << (bit);
                    bitPointer++;
                    break;
                }
            }
        }
        if(biggest < resultSize-1)
        {
            result = (char*)realloc(result, biggest+1);
            resultSize = biggest+1;
        }
    }

    char* decompress(int* returnSize)
    {
        char* res = (char*)calloc(1, resultSize*3);
        int mxsz = resultSize*3;
        int resSz = 0;
        int bitPointer = 0;
        int sigCounter = 0;
        res = (char*)calloc(1, sizeof(char));
        if(resultSize > 0)
        {
            while (bitPointer < resultSize*8)
            {
                int byte = bitPointer / 8;
                int bit = bitPointer - 8*byte;
                if(((result[byte] >> bit) & 1) == 1)
                {
                    resSz++;
                    if(resSz >= mxsz)
                    {
                        res = (char*)realloc(res, resSz*sizeof(char));
                    }

                    res[resSz-1] = symbols[sigCounter];

                    sigCounter = -1;
                }
                bitPointer++;
                sigCounter++;
            }
        }
        *returnSize = resSz;
        return res;
    }


};

class CompressionTableElementWord
{
    public:

    CompressionTableElementWord()
    {

    }
    CompressionTableElementWord(std::string& value, int count_)
    {
        x = value;
        count = count_;
    }

    int operator < (CompressionTableElementWord& other)
    {
        return count - other.count;
    }

    int operator > (CompressionTableElementWord& other)
    {
        return other.count - count;
    }

    static bool isless (CompressionTableElementWord& i,CompressionTableElementWord& j) { return (i.count>j.count); }


    std::string x;
    int count;
};

class CodeTableWords
{

    std::vector<std::string> symbols;

    int symbolsSize()
    {
        int bytes = 0;
        for(int i = 0; i < symbols.size(); i++)
        {
            bytes += symbols.at(i).length();
            bytes += sizeof(int);
        }
        return bytes;
    }

    public:
    char* result;
    int resultSize;

    CodeTableWords(std::vector<std::string> sortedSymbols)
    {
        resultSize = 0;
        result = 0;
        for(int i = 0; i < sortedSymbols.size(); i++)
        {
            symbols.push_back(sortedSymbols.at(i));
        }
    }

    size_t size()
    {
        int bytes = 0;
        for(int i = 0; i < symbols.size(); i++)
        {
            bytes += symbols.at(i).length();
            bytes += sizeof(int);
        }
        bytes += sizeof(int);
        return bytes + sizeof(int) + resultSize*sizeof(char);
    }

    ~CodeTableWords()
    {
        if(result != 0)
        {
            //free(result);
        }
    }

    void compress(std::vector<std::string>& input)
    {
        resultSize = symbolsSize()+1;
        result = (char*)calloc(resultSize, 1);
        memset(result, 0, resultSize);
        int bitPointer = 0;
        int biggest = 0;
        bool found;
        for(int i = 0; i < input.size(); i++)
        {
            found = false;
            for(int s = 0; s < symbols.size(); s++)
            {
                if(symbols.at(s) == input.at(i))
                {
                    bitPointer += s;
                    int byte = bitPointer / 8;
                    int bit = bitPointer - 8*byte;
                    if(byte > resultSize-1)
                    {
                        resultSize++;
                        char* tmp;
                        char* that;
                        /*try
                        {
                            char* that = (char*)realloc(result, resultSize);
                        }
                        catch(const std::exception& e)
                        {*/
                            tmp = (char*)calloc(resultSize+50, 1);
                            memcpy(tmp, result, resultSize-1);
                            free(result);
                            result = 0;
                            result = (char*)calloc(resultSize+50, 1);
                            memset(result, 0, resultSize+50);
                            memcpy(result, tmp, resultSize-1);
                            free(tmp);
                            //result = tmp;
                            //result = (char*)realloc(result, resultSize+50);
                            resultSize += 50;
                        //}
                        result[resultSize-1] = 0;
                    }
                    biggest = byte;
                    result[byte] |= 1 << (bit);
                    bitPointer++;
                    found = true;
                    break;
                }
            }
        }
        if(biggest < resultSize-1)
        {
            result = (char*)realloc(result, biggest+1);
            resultSize = biggest+1;
        }
    }

    std::string decompress()
    {
        std::string res;
        int bitPointer = 0;
        int sigCounter = 0;
        if(resultSize > 0)
        {
            while (bitPointer < resultSize*8)
            {
                int byte = bitPointer / 8;
                int bit = bitPointer - 8*byte;
                if(((result[byte] >> bit) & 1) == 1)
                {
                    res.append(symbols.at(sigCounter));
                    sigCounter = -1;
                }
                bitPointer++;
                sigCounter++;
            }
        }
        return res;
    }

};




CodeTable compress(char* data, int size, bool printWordBook)
{

    std::vector<CompressionTableElement> wordBook;

    for(int i = 0; i < size; i++)
    {
        int f = 0;
        for(f = 0; f < wordBook.size(); f++)
        {
            if(wordBook.at(f).x == data[i])
            {
                break;
            }
        }
        if(f == wordBook.size())
        {
            int count = 0;
            for(int c = 0; c < size; c++)
            {
                if(data[i] == data[c])
                {
                    count++;
                }
            }
            wordBook.push_back(CompressionTableElement(data[i], count));
        }
    }

    std::sort(wordBook.begin(), wordBook.end(), CompressionTableElement::isless);

    char* symbols = (char*)calloc(1 + wordBook.size(), sizeof(char));

    if(printWordBook)
    {
        for(int i = 0; i < wordBook.size(); i++)
        {
            printf("%c(%d) ", wordBook.at(i).x, wordBook.at(i).count);
            symbols[i] = wordBook.at(i).x;
        }
        printf("\n");
    }
    
    CodeTable stuff(symbols, wordBook.size());
    free(symbols);

    stuff.compress(data, size);

    return stuff;
}

std::vector<std::string> textToWordFragments(std::string& text)
{
    std::vector<std::string> results;
    int a = 0;
    for(int i = 0; i < text.length(); i++)
    {
        if(text.at(i) == ' ' || text.at(i) == '.' || text.at(i) == ',' || text.at(i) == '!' || text.at(i) == '?' || text.at(i) == ':' || text.at(i) == ';')
        {
            if(a < i)
            {
                results.push_back(text.substr(a, i - a));
                
                
                if(i > 0)
                {
                    std::string tmp = "";
                    tmp.append(1, text.at(i));
                    results.push_back(tmp);
                }
                a = i + 1;
                if(a < text.length())
                {
                    if(text.at(a) == ' ')
                    {
                        std::string tmp = "";
                        tmp.append(1, text.at(a));
                        results.push_back(tmp);
                        a++;
                    }
                }
            }
            /*else
            {
                std::string tmp = "0";
                tmp.at(0) = text.at(0);
                results.push_back(tmp);
                a++;
            }*/
        }
    }
    if(a < text.length())
    {
        results.push_back(text.substr(a));
    }
    return results;
}

CodeTableWords compressText(std::string& text, bool printWordBook)
{

    std::vector<std::string> data;
    data = textToWordFragments(text);
    std::vector<CompressionTableElementWord> wordBook;
    int size = data.size();
    for(int i = 0; i < size; i++)
    {
        int f = 0;
        for(f = 0; f < wordBook.size(); f++)
        {
            if(wordBook.at(f).x == data.at(i))
            {
                break;
            }
        }
        if(f == wordBook.size())
        {
            int count = 0;
            for(int c = 0; c < size; c++)
            {
                if(data.at(i) == data.at(c))
                {
                    count++;
                }
            }
            wordBook.push_back(CompressionTableElementWord(data.at(i), count));
        }
    }

    std::sort(wordBook.begin(), wordBook.end(), CompressionTableElementWord::isless);

    std::vector<std::string> symbols;

    if(printWordBook)
    {
        for(int i = 0; i < wordBook.size(); i++)
        {
            printf("%s(%d) ", wordBook.at(i).x.c_str(), wordBook.at(i).count);
            symbols.push_back(wordBook.at(i).x);
        }
        printf("\n");
    }
    
    CodeTableWords stuff(symbols);
    stuff.compress(data);
    return stuff;
}


int main(int args, char** arg)
{
    int pos = 0;
    /*FILE* file = fopen("/home/david/Downloads/VorlageLastenheft.pdf", "rb");

    fseek(file, 0, SEEK_END);
    long fsz = ftell(file);
    fseek(file, 0, SEEK_SET);
    long iter = 0;
    char* text = (char*)malloc(fsz);
    
    while(pos < fsz)
    {
        if(fsz - pos < 4096)
        {
            pos += fread(text, sizeof(char), fsz - pos, file);
        }
        else
        {
            pos += fread(text, sizeof(char), 4096, file);
        }
    }
    fclose(file);
    printf("File size: %d\n", fsz);
*/
    char* text = "During the early part of the 20th century, the role of the Stirling engine as a domestic motor[32] was gradually taken over by electric motors and small internal combustion engines. By the late 1930s, it was largely forgotten, only produced for toys and a few small ventilating fans.[33]Around that time, Philips was seeking to expand sales of its radios into parts of the world where grid electricity and batteries were not consistently available. Philips' management decided that offering a low-power portable generator would facilitate such sales and asked a group of engineers at the company's research lab in Eindhoven to evaluate alternative ways of achieving this aim. After a systematic comparison of various prime movers, the team decided to go forward with the Stirling engine, citing its quiet operation (both audibly and in terms of radio interference) and ability to run on a variety of heat sources (common lamp oil  cheap and available everywhere  was favored).[34] They were also aware that, unlike steam and internal combustion engines, virtually no serious development work had been carried out on the Stirling engine for many years and asserted that modern materials and know-how should enable great improvements.[35]Philips MP1002CA Stirling generator of 1951.By 1951, the 180/200 W generator set designated MP1002CA (known as the Bungalow set) was ready for production and an initial batch of 250 was planned, but soon it became clear that they could not be made at a competitive price. Additionally, the advent of transistor radios and their much lower power requirements meant that the original rationale for the set was disappearing. Approximately 150 of these sets were eventually produced.[36] Some found their way into university and college engineering departments around the world[37] giving generations of students a valuable introduction to the Stirling engine.[citation needed]In parallel with the Bungalow set, Philips developed experimental Stirling engines for a wide variety of applications and continued to work in the field until the late 1970s, but only achieved commercial success with the reversed Stirling engine cryocooler. However, they filed a large number of patents and amassed a wealth of information, which they licensed to other companies and which formed the basis of much of the development work in the modern era.[38]In 1996, the Swedish navy commissioned three Gotland-class submarines. On the surface, these boats are propelled by marine diesel engines. However, when submerged, they use a Stirling-driven generator developed by Swedish shipbuilder Kockums to recharge batteries and provide electrical power for propulsion.[39] A supply of liquid oxygen is carried to support burning of diesel fuel to power the engine. Stirling engines are also fitted to the Swedish Södermanland-class submarines, the Archer-class submarines in service in Singapore and, license-built by Kawasaki Heavy Industries for the Japanese Sōryū-class submarines. In a submarine application, the Stirling engine offers the advantage of being exceptionally quiet when running.";
    pos = strlen(text);
    printf("Compression only works on redundant data:\n");
    printf("Huffman-Code Words:\n");
    std::string text2(text);
    CodeTableWords dataText = compressText(text2, true);
    printf("Compressed  : ");
    for(int i = 0; i < dataText.resultSize && i < 15; i++)
    {
        for(int p = 0; p < 8; p++)
        {
            printf("%d", (dataText.result[i] >> p) & 1);
        }
        printf(" ");
    }
    printf("...\n");
    printf("Compressed size --> %f%\n", (float)100*dataText.size()/(text2.length()));
    std::string decomp2 = dataText.decompress();
    printf("Original    : \"%.40s...\"  size in bytes: %d\n", text2.c_str(), pos);
    printf("Decompressed: \"%.40s...\"  compressed size in bytes: %d\n", decomp2.c_str(), dataText.size());

    
    
    CodeTable data = compress(text, pos, true);


    
    printf("Huffman-Code:\n");
    int decompSz;
    char* decomp = data.decompress(&decompSz);
    printf("Original    : \"%.40s...\"  size in bytes: %d\n", text, pos);
    printf("Decompressed: \"%.40s...\"  compressed size in bytes: %d\n", decomp, data.size());
    printf("Compressed  : ");
    for(int i = 0; i < data.resultSize && i < 15; i++)
    {
        for(int p = 0; p < 8; p++)
        {
            printf("%d", (data.result[i] >> p) & 1);
        }
        printf(" ");
    }
    printf("...\n");
    printf("Compressed size --> %f%\n", (float)100*data.size()/(pos));



    



    //free(decomp);
    //free(text);

    return 0;
}

