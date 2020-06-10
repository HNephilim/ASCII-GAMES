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
vector<wstring> persona;
vector<int> lines;

//KeyStuff              
string virtualkeys = "\x26\x28ZXQ"; // 0 = UP / 1 = DW / 2 = Z / 3 = X / 4 = Q
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
        persona.push_back(final);
        lines.push_back(rows);
    }
    
    //Declaring Objects
    Sprite NavePlayer((persona[0].size() / lines[0]), lines[0], persona[0]);
    //Sprite Monstro1  ((persona[1].size() / lines[1]), lines[1], persona[1]);
    //Sprite Monstro2  ((persona[2].size() / lines[2]), lines[2], persona[2]);
    //Sprite Boss      ((persona[3].size() / lines[3]), lines[3], persona[3]);
    Sprite Shot      ((persona[4].size() / lines[4]), lines[4], persona[4]);
    Sprite Powershot ((persona[5].size() / lines[5]), lines[5], persona[5]);
    Sprite Lives     ((persona[6].size() / lines[6]), lines[6], persona[6]);

    
    Buffer SpInv(120, 30);
    
    //Player Variables
    int nPlayerY = SpInv.height() / 2;
    int nPlayerX = 3;
    int nPlayerLives = 3;
    int nPlayerPowerUps = 3;

    //Monster Declaration



    //Gamelogic Stuff
    bool bGameOver;
    DrawSprite(Lives, 3, 0, SpInv);
    DrawSprite(Lives, 4, 0, SpInv);
    DrawSprite(Lives, 5, 0, SpInv);
    DrawSprite(Powershot, 9, 0, SpInv);
    DrawSprite(Powershot, 10, 0, SpInv);
    DrawSprite(Powershot, 11, 0, SpInv);
    

    while (!bGameOver)
    {  
        //GAME TIMING ==================================================================================================================
        Sleep(50);

        //GAME LOGIC ===================================================================================================================
        if (key.isKeyPressed(0) && nPlayerY < 0)
        {
            nPlayerY -= 1;
        }
        if (key.isKeyPressed(1) && nPlayerY > SpInv.height())
        {
            nPlayerY += 1;
        }
        if (key.isKeyPressed(4))
        {
            bGameOver = true;
        }

        //RENDER OUTPUT

        DrawSprite(NavePlayer, nPlayerX, nPlayerY, SpInv);
        DrawFrame(SpInv);
    }
    SpInv.Close();
    system("pause");
    return 0;
}
   
  
  
  
  
   
   
      
   
    
  
 

      
      
    
  
