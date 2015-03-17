#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include "Textmap.h"
//#include <random>

using namespace sf;

void pxlOn(int x, int y, int screen[][64]);
void grphFill(int x, int y, int screen[][64]);
void grphWave(int x, int y, int screen[][64], float mdf);
void grphRand(int x, int y, int screen[][64]);
void drawOnScreen(int posx, int posy, int screen[][64], Textmap textmap);
void clearScreen(int screen[][64]);
void landGen(int* x, int* y, int screen[][64]);
int pxlTest(int x, int y, int screen[][64]);
int randint(int low, int high);
bool rectIntersect(int topleft1, int topright1, int bottomleft1, int bottemright1, int topleft2, int topright2, int bottomleft2, int bottemright2);

typedef enum { CAR, ROCK } Obstacle;

int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(1024, 512), "Casio CG9860Gii");
    window.setVerticalSyncEnabled(true);
    // Load a sprite to display
    sf::Texture texture;
    if (!texture.loadFromFile("pixels.png"))
        return EXIT_FAILURE;
    Sprite pixelOn(texture);
    pixelOn.setTextureRect(IntRect(0, 0, 8, 8));
    Sprite pixelOff(texture);
    pixelOff.setTextureRect(IntRect(8, 0, 8, 8));

    srand(clock());

    int screen[128][64];
    for (int y = 0; y < sizeof(screen[0])/sizeof(unsigned int); y++)
    {
        for (int x = 0; x < sizeof(screen)/sizeof(screen[0]); x++)
        {
            screen[x][y] = 0;
        }
    }

    int calcX = 0;
    int calcY = 0;
    int mdfCount = 1;

    int landX = 64;
    int landY = 32;

    Textmap car1;
    car1.loadTextmap("car1.txt");

    Textmap car2;
    car2.loadTextmap("car2.txt");

    Textmap road;
    road.loadTextmap("road.txt");

    Textmap othercar1;
    othercar1.loadTextmap("obstacles\\car1.txt");

    Textmap othercar2;
    othercar2.loadTextmap("obstacles\\car2.txt");

    Textmap rock;
    rock.loadTextmap("obstacles\\rock.txt");

    Textmap gameover;
    gameover.loadTextmap("gameover.txt");

    int spriteX = 2;
    int spriteY = 0;
    IntRect carRect(spriteX, spriteY, 35, 12);

    int frame = 0;

    int obstacleX = 128;
    int obstacleRow = 0; //value between 0 and 3

    Obstacle obstacle = CAR;
    IntRect othercarRect(obstacleX, obstacleRow * 15 + 3, 17, 12);
    IntRect rockRect(obstacleX, obstacleRow * 15 + 3, 14, 14);
    IntRect currentRect = othercarRect;

    bool gameOver = false;

	// Start the game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window : exit
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == Event::KeyPressed)
            {
                if (event.key.code == Keyboard::Escape)
                {
                    window.close();
                }
                /*if (event.key.code == Keyboard::Right)
                {
                    std::cout << mdfCount << "\n";
                    for (int y = 0; y < 64; y++)
                    {
                        for (int x = 0; x < 128; x++)
                        {
                            screen[x][y] = 0;
                            grphWave(x, y, screen, mdfCount);

                        }
                    }
                    mdfCount++;

                }
                if (event.key.code == Keyboard::Left)
                {
                    mdfCount--;
                    std::cout << mdfCount << "\n";
                    for (int y = 0; y < 64; y++)
                    {
                        for (int x = 0; x < 128; x++)
                        {
                            screen[x][y] = 0;
                            grphWave(x, y, screen, mdfCount);
                        }
                    }
                } //Uncomment this to cycle through patterns quickly*/
            }
        }

        //grphFill(calcX, calcY, screen);
        //grphWave(calcX, calcY, screen, 51);
        //grphRand(calcX, calcY, screen);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && spriteY - 1 > 2)
        {
            spriteY--;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && spriteY + 1 < 49)
        {
            spriteY++;
        }
        carRect.top = spriteY;

        if (obstacle == CAR)
        {
            if (frame % 2 == 1)
                obstacleX -= 1;
            othercarRect.top  = obstacleRow * 15 + 3;
            othercarRect.left = obstacleX;
            currentRect = othercarRect;
        }
        else if (obstacle == ROCK)
        {
            obstacleX--;
            rockRect.top = obstacleRow * 15 + 2;
            rockRect.left = obstacleX;
            currentRect = rockRect;
        }

        if (obstacleX < -17)
        {
            obstacleX = 128 + randint(0, 10);
            obstacleRow = randint(0, 3);
            switch(randint(1, 2))
            {
            case(1):
                obstacle = CAR;
                break;
            case(2):
                obstacle = ROCK;
                break;
            default:
                obstacle = CAR;
                break;
            }
        }

        if (carRect.intersects(currentRect))
        {
            gameOver = true;
        }

        clearScreen(screen);

        for (int i = 0; i < 12; i++)
        {
            drawOnScreen(i * 12 - frame % 6, 0, screen, road);
        }

        if ((frame / 10) % 2 == 1)
        {
            drawOnScreen(spriteX, spriteY, screen, car1);
        }
        else
        {
            drawOnScreen(spriteX, spriteY, screen, car2);
        }

        if (obstacle == CAR)
        {
            if ((frame / 20) % 2 == 1)
            {
                drawOnScreen(obstacleX, obstacleRow * 15 + 3, screen, othercar1);
            }
            else
            {
                drawOnScreen(obstacleX, obstacleRow * 15 + 3, screen, othercar2);
            }
        }
        else if (obstacle == ROCK)
        {
            drawOnScreen(obstacleX, obstacleRow * 15 + 2, screen, rock);
        }

        if (gameOver)
        {
            drawOnScreen(0, 0, screen, gameover);
        }

        // Clear screen
        window.clear();

        // Draw the sprite
        for (int y = 0; y < sizeof(screen[0])/sizeof(unsigned int); y++)
        {
            for (int x = 0; x < sizeof(screen)/sizeof(screen[0]); x++)
            {
                if (screen[x][y] == 0)
                {
                    pixelOff.setPosition(x * 8, y * 8);
                    window.draw(pixelOff);
                }
                else
                {
                    pixelOn.setPosition(x * 8, y * 8);
                    window.draw(pixelOn);
                }
            }
        }

        // Update the window
        window.display();

        calcX++;

        if (calcX > 127)
        {
            calcX = 0;
            calcY++;
        }

        calcY = calcY > 63 ? 63 : calcY;

        frame++;
    }

    return EXIT_SUCCESS;
}

int randint(int low, int high)
{
    int value = rand() % (high + 1 - low) + low;
    srand(value * clock());
    return value;
    /*std::default_random_engine generator;
    generator.seed(clock());
    std::uniform_int_distribution<int> distribution(low, high);*/
}

void pxlOn(int x, int y, int screen[][64])
{
    screen[x][y] = 1;
}

void pxlOff(int x, int y, int screen[][64])
{
    screen[x][y] = 0;
}

int pxlTest(int x, int y, int screen[][64])
{
    return screen[x][y];
}

void clearScreen(int screen[][64])
{
    for (int y = 0; y < 64; y++)
    {
        for (int x = 0; x < 128; x++)
        {
            screen[x][y] = 0;
        }
    }
}

void grphFill(int x, int y, int screen[][64])
{
    pxlOn(x, y, screen);
}

void grphWave(int x, int y, int screen[][64], float mdf)
{
    if (sin(x * mdf) * cos(y * mdf) > -0.3)
    {
        pxlOn(x, y, screen);
    }
}

void grphRand(int x, int y, int screen[][64])
{
    int value = randint(0, 100);
    if (value > (y / 63.0) * 100)
    {
        pxlOn(x, y, screen);
    }
}

void landGen(int* x, int* y, int screen[][64])
{
    pxlOn(*x, *y, screen);
    *x += randint(-1, 1);
    *y += randint(-1, 1);
    //std::cout << randint(-1, 1) << "\n";
}

void drawOnScreen(int posx, int posy, int screen[][64], Textmap textmap)
{
    int rowLength = textmap.getWidth();
    std::vector<int> data = textmap.getData();
    for (int y = 0; y < data.size() / rowLength; y++)
    {
        for (int x = 0; x < rowLength; x++)
        {
            //std::cout << y * rowLength + x <<"\n";
            if (data.at(y * rowLength + x) == 1)
            {
                if (posx + x >= 0 && posx + x < 128 && posy + y >= 0 && posy + y < 64)
                {
                    pxlOn(posx + x, posy + y, screen);
                }
            }
            else if (data.at(y * rowLength + x) == 0)
            {
                if (posx + x >= 0 && posx + x < 128 && posy + y >= 0 && posy + y < 64)
                {
                    pxlOff(posx + x, posy + y, screen);
                }
            }
            else if (data.at(y * rowLength + x) == 2)
            {
                //Do nothing! for transparency
            }
        }
    }
}
