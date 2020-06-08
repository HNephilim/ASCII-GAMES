#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include <chrono>
#include <string>
#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#include <Windows.h>
using namespace std;

// Buffer Global Variable
int nScreenWidth = 80;              //Console Screen Size X
int nScreenHeight = 30;             //Console Screen Size Y

//Tetris Global Variables
wstring tetromino[7];
wstring boxnextpiece[7];
wstring asset = L" ABCDEFG=#";
int nFieldWidth = 12;
int nFieldHeight = 18;
unsigned char *pField = nullptr;

// Random Number Generator
unsigned int seed = chrono::system_clock::now().time_since_epoch().count();
minstd_rand0 rand_eng (seed);

//Functions
int Rotate(int px, int py, int r) //Return the right index for the rotate pieces. To look into Tetromino array
{
    switch (r % 4)
    {
        case 0: 
            return py*4 +px; // 0 degrees
        case 1: 
            return 12+py-(px*4); // 90 degrees
        case 2: 
            return 15-(py*4)-px; // 180 degress
        case 3:
            return 3 - py + (4 * px); // 270 degrees
    }
    return 0;
}

bool DoesPieceFit(int nTetromino, int nRotation, int nPosX, int nPosY)
{
    //Lets iterate through all the 4x4 (actually 16) long tetromino string array
    for (int px = 0; px < 4; px++)
    {
        for (int py = 0; py < 4; py++)
        {
            int pi = Rotate(px, py, nRotation); //Which index of the tetromino array i`m checking
            int fi = (nPosY + py) * nFieldWidth + (nPosX + px); //The index in the field array i`m looking into

            if ((nPosX + px) >= 0 && (nPosX + px) < nFieldWidth) //Checking x bounds
            {
                if ((nPosY + py) >= 0 && (nPosY + py) < nScreenHeight) // checking y bounds
                {
                    if (tetromino[nTetromino][pi] == L'X' && pField[fi] != 0) //If the value in the right tetromino index is X and the field has anything but 0. Its a colison
                    {
                        return false; 
                    }
                }
            }
        }

    }

        return true;
}

//Main
extern "C" 
int wmain()
{

    //Creando assets
    tetromino[0].append(L"..X.");
    tetromino[0].append(L"..X.");
    tetromino[0].append(L"..X.");
    tetromino[0].append(L"..X.");

    tetromino[1].append(L"....");
    tetromino[1].append(L"..X.");
    tetromino[1].append(L"..X.");
    tetromino[1].append(L".XX.");

    tetromino[2].append(L".XX.");
    tetromino[2].append(L"..X.");
    tetromino[2].append(L"..X.");
    tetromino[2].append(L"....");

    tetromino[3].append(L"....");
    tetromino[3].append(L".XX.");
    tetromino[3].append(L".XX.");
    tetromino[3].append(L"....");

    tetromino[4].append(L"....");
    tetromino[4].append(L"..XX");
    tetromino[4].append(L".XX.");
    tetromino[4].append(L"....");

    tetromino[5].append(L"..X.");
    tetromino[5].append(L".XX.");
    tetromino[5].append(L"..X.");
    tetromino[5].append(L"....");

    tetromino[6].append(L"....");
    tetromino[6].append(L".XX.");
    tetromino[6].append(L"..XX");
    tetromino[6].append(L"....");

    boxnextpiece[0].append(L"NEXT PIECE");
    boxnextpiece[1].append(L"x----x\0");
    boxnextpiece[2].append(L"|    |\0");
    boxnextpiece[3].append(L"|    |\0");
    boxnextpiece[4].append(L"|    |\0");
    boxnextpiece[5].append(L"|    |\0");
    boxnextpiece[6].append(L"x----x\0");

    pField = new unsigned char[nFieldHeight * nFieldWidth]; // Cria o array com o campo (de 0 a 9 sendo index para a wstring <asset>)
    for (int x = 0; x < nFieldWidth; x++) //Escaneia todas as colunas dentro do limite
    {
        for (int y = 0; y < nFieldHeight; y++) //Escaneia todas a linhas dentro dos limites
        {
            pField[y * nFieldWidth + x] = (x == 0 || x == nFieldWidth - 1 || y == nFieldHeight - 1) ? 9 : 0;
        }
    }
    
    //Create Screen Array. Modifing this array will draw to the screen
    wchar_t *screen = new wchar_t[nScreenWidth * nScreenHeight];
    for (int i = 0; i < nScreenHeight * nScreenWidth; i++) //Fill the Screen with blank
    {
        screen[i] = L' ';
        
    }

    //Windows shananigans to create the buffer
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(hConsole);
    DWORD dwBytesWritten = 0;

    
    
    // Game Stuff
    bool bGameOver = false;
    int nCurrentPiece = 1;
    int nNextPiece = rand_eng() % 7;
    int nCurrentRotation = 0;
    int nNextRotation = rand_eng() % 4;
    int nCurrentX = nFieldWidth / 2;
    int nCurrentY = 0;
    vector<int> vLines;
    int nScore = 0;

    bool bKey[5];
    bool bRotateHold = false;

    int nSpeed = 20;
    int nSpeedCouter = 0;
    bool bForceDown = false;
    int nPieceCount = 0;

    //Game Loop
    while (!bGameOver)
    {
    //GAME TIMING =========================================================================================================================================================
        Sleep(50); //game tick
        nSpeedCouter++; //increments counter each ticks
        bForceDown = (nSpeedCouter == nSpeed); //whenever ticks acumulates enough to match speed, it forces down. Decreasing nSpeed increases speed of the overall gaming exp.

        //INPUT ===========================================================================================================================================================
        for (int k = 0; k<5; k++)
        {                                                      //  0R  1L  2D 3Z       
            bKey[k] = (0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x28ZQ"[k]))) != 0;
        }

        //GAME LOGIC ======================================================================================================================================================
        if (bKey[1] && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX - 1, nCurrentY)) //If left key is pressed, and the piece fit 1 to the left, move to the left
        {
            nCurrentX -= 1;
        }

        if (bKey[0] && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX + 1, nCurrentY)) //If right key is pressed, and the piece fit 1 to the right, move to the right
        {
            nCurrentX += 1;
        }

        if (bKey[2] && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1)) //If down key is pressed, and the piece fit 1 down, move down
        {
            nCurrentY += 1;
        }

        if (bKey[3]) //Check for rotations when Z is pressed, but not hold
        {
            if (DoesPieceFit(nCurrentPiece, nCurrentRotation + 1, nCurrentX, nCurrentY) && !bRotateHold)
            {
                nCurrentRotation += 1;                
            }
            bRotateHold = true;
        }
        else
        {
            bRotateHold = false;
        }

        if (bForceDown)
        {
            nSpeedCouter = 0;
            if (DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1))
            {
                nCurrentY += 1;
            }
            else
            {
                //Lock the current piece in place by saving in pField the position of the X in the tetromino array
                for (int px = 0; px < 4; px++)
                {
                    for (int py = 0; py < 4; py++)
                    {
                        if(tetromino[nCurrentPiece][Rotate(px, py, nCurrentRotation)] == L'X')
                        {
                            pField[(nCurrentY + py) * nFieldWidth + (nCurrentX + px)] = nCurrentPiece + 1; //takes the id, sums 1 to further render the right element from asset array
                        }
                    }
                }

                //Increasse difficult as game progresses
                nPieceCount++;
                if (nPieceCount % 10 == 0)
                {
                    if  (nSpeed >= 10)
                    {
                        nSpeed--;
                    }
                }

                //Check if we got lanes
                for (int py = 0; py < 4; py++)
                {
                    if (nCurrentY + py < nFieldHeight - 1)
                    {
                        bool bLine = true;
                        for (int px = 1; px < nFieldWidth - 1; px++)
                        {
                            bLine &= pField[(nCurrentY + py) * nFieldWidth + px] != 0; //Logical and between bLine and if pField is diff than 0. A single false can`t return
                        }
                        if (bLine)
                        {
                            for (int px = 1; px < nFieldWidth - 1; px++)
                            {
                               pField[(nCurrentY + py) * nFieldWidth + px] = 8; //Changes de lines for 8th asset array element (=)
                            }
                            vLines.push_back(nCurrentY + py);
                        }
                    }
                }

                nScore += 25;
                if (!vLines.empty()) //if we have lines
                {
                    /*each line made this turn will exponentially increasse the earned score by 
                    left bitshifting 1(0b10, 0b100, 0b1000 and 0b1000) and multiplying it by 0d100*/
                    nScore += (1 << vLines.size()) * 100; 
                    
                }


                //Choose next piece
                nCurrentX = nFieldWidth / 2;
                nCurrentY = 0;
                nCurrentRotation = nNextRotation;
                nCurrentPiece = nNextPiece;
                nNextRotation = rand_eng() % 4;
                nNextPiece = rand_eng() % 7;

                //If next piece does not piece
                bGameOver = !DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY);
            }
            
        }
        //RENDER OUTPUT ===================================================================================================================================================

        //Draw Field (Alterando a wchar_t <screen> eu desenho na tela. Faco isso mexendo nos valores de pField)
        for (int x = 0; x < nFieldWidth; x++)
        {
            for (int y = 0; y < nFieldHeight; y++)
            {
                screen[(y + 2) * nScreenWidth + (x + 2)] = asset[pField[y * nFieldWidth + x]];
            }
        }

        //Draw Current Piece
        for (int px = 0; px < 4; px++)
        {
            for (int py = 0; py < 4; py++)
            {
                if(tetromino[nCurrentPiece][Rotate(px, py, nCurrentRotation)] == L'X')
                {
                    screen[(nCurrentY + py + 2) * nScreenWidth + (nCurrentX + px + 2)] = asset[nCurrentPiece + 1];
                }
            }
        }

        
        //Draw Extra stuff
        //Score
        wstring sScore = L"SCORE: ";
        sScore.append(to_wstring(nScore));

        for (int i = 0; i < sScore.size(); ++i)
        {
            screen[3 * nScreenWidth + nFieldHeight + 2 + i] = sScore[i];
        }
            

        //Next piece
        for (int i = 0; i < boxnextpiece[0].size(); ++i)
        {
            screen[5 * nScreenWidth + nFieldHeight + 2 + i] = boxnextpiece[0][i];
        }

        for (int y = 1; y < 7; ++y)
        {
            for (int x = 0; x < boxnextpiece[y].size(); ++x)
            {
                screen[(6+y) * nScreenWidth + nFieldHeight + 4 + x] = boxnextpiece[y][x];
            }
        }

        //Draw Next Piece
        for (int px = 0; px < 4; px++)
        {
            for (int py = 0; py < 4; py++)
            {
                if (tetromino[nNextPiece][Rotate(px, py, nNextRotation)] == L'X')
                {
                    screen[(8 + py) * nScreenWidth + (11 + px) + nFieldWidth] = asset[nNextPiece + 1];
                }
            }
            }

        if (!vLines.empty()) //Do we have lines made?
        {
            WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, {0, 0}, &dwBytesWritten); //draw screen with lines
            Sleep(400); //delay

            for (auto &v : vLines) //v will be equal of each element of vLine
            {
                for (int px = 1; px < nFieldWidth - 1; px++) //iterate through the line
                {
                    for (int py = v; py > 0; py--) // iterate though a column, starting in line v (a line stored in vLines in LOGIC)
                    {
                        pField[py * nFieldWidth + px] = pField[(py - 1) * nFieldWidth + px]; // pull the element above this cell one down
                    }
                    pField[px] = 0; // and cleans the first line
                }
            }
            vLines.clear(); //clear the vector
        }

            //Display Frame
            WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, {0, 0}, &dwBytesWritten);
    }
    CloseHandle(hConsole);
    cout << "Game Over!! Score: " << nScore << endl;
    system("pause");

    return 0;
}