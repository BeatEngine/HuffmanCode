#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "HuffmanCompression.h"

using namespace Huffmann;
int main(int args, char** arg)
{
    int pos = 0;

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


    free(decomp);

    return 0;
}

