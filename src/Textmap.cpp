#include "Textmap.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int Textmap::getWidth() { return width; }
int Textmap::getHeight() { return height; }
vector<int> Textmap::getData() { return data; };

int Textmap::loadTextmap(std::string filename)
{
    width = 0;
    height = 0;

    ifstream reader(filename.c_str());
    if (!reader)
    {
        std::cout << "Error opening file\n";
        return -1;
    }

    for (int i = 0; ! reader.eof(); i++)
    {
        char letter = reader.get();
        if (letter == '0')
        {
            data.push_back(0);
        }
        else if (letter == '1')
        {
            data.push_back(1);
        }
        else if (letter == '2')
        {
            data.push_back(2);
        }
        if (width == 0 && (int)letter == 10)
        {
            width = i;
        }
        if ((int)letter == 10)
        {
            height++;
        }
    }
    height++;
}

