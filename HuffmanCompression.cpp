#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <vector>
#include <algorithm>


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
    short bit;
    int count;
};

class CTnode
{
    public:
    char symbol;
    CTnode* one;
    CTnode* zero;

    CTnode()
    {
        one = 0;
        zero = 0;
    }

    CTnode(char sym)
    {
        one = 0;
        zero = 0;
        symbol = sym;
    }

    ~CTnode()
    {
        one->~CTnode();
        zero->~CTnode();
    }

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
                            free(tmp);
                            memcpy(result, tmp, resultSize-1);
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
    char* text = "Memory allocated by copy constructors for pointer duplication: Check the pointer in the destructor and delete if necessary. Memory allocated when passing the class by value, invokes the copy constructor. Also beware, the default copy constructor may not give you the results you want especially when dealing with pointers as the default copy constructor has no knowledge of how to copy the contents of what the pointer points to. To prohibit the use of the default copy constructor one can define a null assignment operator. One can also write a proper copy constructor.";
    pos = strlen(text);


    
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

    free(decomp);
    //free(text);

    return 0;
}

