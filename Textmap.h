#ifndef TEXTMAP_H
#define TEXTMAP_H
#include <iostream>
#include <vector>

class Textmap
{
    public:
        int getWidth();
        int getHeight();
        std::vector<int> getData();
        int loadTextmap(std::string filename);
    protected:
    private:
        int width, height;
        std::vector<int> data;
};

#endif // TEXTMAP_H
