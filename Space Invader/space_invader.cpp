#include <string>
#include <iostream>
#include <cwchar>
#include <fstream>
#include <vector>
#include "engine.hpp"
using namespace std;

//Functions
wstring StrToWstr(string s)
{
    wstring wsTmp(s.begin(), s.end());

    return wsTmp;
    
}

//Assets Stuff
FILE *input;
vector<string> pathway = {"Assets_SP\\player.txt",
                          "Assets_SP\\monster1.txt",
                          "Assets_SP\\monster2.txt",
                          "Assets_SP\\boss.txt",
                          "Assets_SP\\shot.txt",
                          "Assets_SP\\powershot.txt",
                          "Assets_SP\\lives.txt"};
vector<wstring> objeto;
vector<int> lines;

//KeyStuff              
string virtualkeys = "\x26\x28ZXQOL"; // 0 = UP / 1 = DW / 2 = Z / 3 = X / 4 = Q / 5 = O / 6 = L
GetKey key(virtualkeys);

extern "C"
int wmain()
{
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         
    //Loading assets                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
    for (int i = 0; i < pathway.size(); ++i)
    {
        input = fopen(pathway[i].c_str(), "r");
        int rows = 0;
        wchar_t temp[30];
        wstring final;

        while (fgetws(temp, 30, input))
        {
            wstring temp_temp(temp);
            temp_temp.pop_back();
            final.append(temp_temp);
            ++rows;
        }
        objeto.push_back(final);
        lines.push_back(rows);
    }
    
    //Declaring Objects
    Sprite NavePlayer  ( objeto[0] , lines[0] );
    //Sprite Monstro1  ( objeto[1] , lines[1] );
    //Sprite Monstro2  ( objeto[2] , lines[2] );
    //Sprite Boss      ( objeto[3] , lines[3] );
    Sprite Shot        ( objeto[4] , lines[4] );
    Sprite Powershot   ( objeto[5] , lines[5] );
    Sprite Lives       ( objeto[6] , lines[6] );

    Buffer SpInv(120, 30);
    
    //Player Variables
    int nPlayerY = SpInv.height() / 2;
    int nPlayerX = 3;
    int nPlayerLives = 3;
    int nPlayerPowerUps = 3;

    //Monster Declaration



    //Gamelogic Stuff
    bool bGameOver;
    

    while (!bGameOver)
    {  
        //GAME TIMING ==================================================================================================================
        Sleep(50);

        //GAME LOGIC ===================================================================================================================
        if (key.isKeyPressed(0) && nPlayerY > 2)
        {
            nPlayerY -= 1;
        }
        if (key.isKeyPressed(1) && nPlayerY < (SpInv.height()-NavePlayer.height()))
        {
            nPlayerY += 1;
        }
        if (key.isKeyPressed(4))
        {
            bGameOver = true;
        }
        if (key.isKeyPressed(5) && nPlayerLives > 0)
        {
            nPlayerLives -= 1;
            nPlayerPowerUps -= 1;
        }
        if (key.isKeyPressed(6) && nPlayerLives < 8)
        {
            nPlayerLives += 1;
            nPlayerPowerUps += 1;
        }

        //RENDER OUTPUT
        CleanFrame(SpInv);
        for (int i = 0; i < nPlayerLives; (i += Lives.width()))
            DrawSprite(Lives, 3 + i, 0, SpInv);

        for (int i = 0; i < nPlayerLives; (i += Powershot.width()))
            DrawSprite(Powershot, nPlayerLives*Lives.width()+4 + i, 0, SpInv);

        DrawSprite(NavePlayer, nPlayerX, nPlayerY, SpInv);
        wstring info = L"nPlayerY = " + to_wstring(nPlayerY) + L"  Lives e POW = " + to_wstring(nPlayerLives);
        DrawString(info, 4, 29, SpInv);
        DrawFrame(SpInv);
    }
    
    SpInv.Close();
    system("pause");
    return 0;
}
   
  
  
  
  
   
   
      
   
    
  
 

      
      
    
  
