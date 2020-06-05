#include <iostream>
#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#include <Windows.h>
#include <thread>
using namespace std;

// Buffer Global Variable
int nScreenWidth = 80;              //Console Screen Size X
int nScreenHeight = 30;             //Console Screen Size Y

//Tetris Global Variables
wstring tetromino[7];
wstring asset = L" ABCDEFG=#";
int nFieldWidth = 12;
int nFieldHeight = 18;
unsigned char *pField = nullptr;

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

bool bDoesPieceFit(int nTetromino, int nRotation, int nPosX, int nPosY)
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

    pField = new unsigned char(nFieldHeight * nFieldWidth); // Cria o array com o campo (de 0 a 9 sendo index para a wstring <asset>)
    for (int x = 0; x < nFieldWidth; x++) //Escaneia todas as colunas dentro do limite
    {
        for (int y = 0; y < nFieldHeight; y++) //Escaneia todas a linhas dentro dos limites
        {
            pField[y * nFieldWidth + x] = (x == 0 || x == nFieldWidth - 1 || y == nFieldHeight - 1) ? 9 : 0;
        }
    }

    //Create Screen Array. Modifing this array will draw to the screen(???)
    wchar_t *screen = new wchar_t[nScreenWidth * nScreenHeight];
    for (int i = 0; i < nScreenHeight *nScreenWidth; i++) //Fill the Screen with blank
    {
        screen[i] = L' ';
    }

    //Windows shananigans to create the buffer
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(hConsole);
    DWORD dwBytesWritten = 0;

    
    
    // Game Stuff
    bool bGameOver = false;
    int nCurrentPiece = 2;
    int nCurrentRotation = 0;
    int nCurrentX = nFieldWidth / 2;
    int nCurrentY = 0;
    bool bKey[4];

    //Game Loop
    while (!bGameOver)
    {
    //GAME TIMING =======================================
    this_thread::sleep_for(50ms);

    //INPUT =============================================
        for (int k = 0; k<4; k++)
        {
            bKey[k] = (0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x28Z"[k]))) != 0;
        }

    //GAME LOGIC ========================================


    //RENDER OUTPUT =====================================

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

            //Display Frame
            WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, {0, 0}, &dwBytesWritten);
    }

    return 0;
}