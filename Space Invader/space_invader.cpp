#include <string>
#include <iostream>
#include <cwchar>
#include <fstream>
#include <vector>
#include <chrono>
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
                          "Assets_SP\\powershot_icon.txt",
                          "Assets_SP\\lives_icon.txt",
                          "Assets_SP\\powershot.txt",
                          "Assets_SP\\monster_shot.txt"};
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
    Sprite NavePlayer      ( objeto[0] , lines[0] );
    Sprite Monstro1        ( objeto[1] , lines[1] );
    Sprite Monstro2        ( objeto[2] , lines[2] );
    //Sprite Boss          ( objeto[3] , lines[3] );
    Sprite NormalShot_spr  ( objeto[4] , lines[4] );
    Sprite PowerShot_Icon  ( objeto[5] , lines[5] );
    Sprite Lives_Icon      ( objeto[6] , lines[6] );
    Sprite PowerShot_Shot  ( objeto[7] , lines[7] );
    Sprite Monster_Shot    ( objeto[8] , lines[8] );
    Buffer SpInv(120, 30);

    //Player Variables
    int nPlayerY = SpInv.height() / 2;
    int nPlayerX = 3;
    int nPlayerLives = 3;
    int nPlayerPowerShots = 3;
    int nFireRate = 25; //gameticks to next fire (Less = faster)
    int nTickFromLastFire = 0;
    int nSpeed = 5; //Tick to move(Less = faster)
    int nTickFromLastMove = 0;
    int nNormalShotDMG = 1;
    int nPowerShotDMG = 3;

    //Monster Declaration
    Monster VectorHolder(Monster_Shot, 1, 0, 0); //Just for keeping something in the monstervector
    Monster CraboCrab   (Monstro1, 3, SpInv.width() - 20, 4);
    Monster LittlePenis (Monstro2, 3, SpInv.width() - 20, 10);
    vector<Monster> LiveMonster;
    LiveMonster.push_back(VectorHolder);
    LiveMonster.push_back(CraboCrab);
    LiveMonster.push_back(LittlePenis);
    
    //Shots Declaration
    vector<Shot> NormalShot;
    vector<Shot> PowerShot;
    vector<Shot> MonsterShot;

    //Gamelogic Stuff
    bool bGameOver = false;
    bool bPowerShotHeld = false;
    auto FPS_now = chrono::system_clock::now();
    auto FPS_Before = chrono::system_clock::now();
    int nShotSpeed = 2;
    int nTickFromShotMove = 0;
    bool bShotMoved = false;

    while (!bGameOver)
    {  
        //GAME TIMING ==================================================================================================================

        if (!key.isKeyPressed(4))
        {
            Sleep(10);
        }
        FPS_Before = FPS_now;
        FPS_now = chrono::system_clock::now();
        chrono::duration<float> elapsedTime = FPS_now - FPS_Before;
        float FPS = 1.0 / elapsedTime.count();

        if (nTickFromLastFire < nFireRate)
        {
            ++nTickFromLastFire;
        }
        if (nTickFromLastMove < nSpeed)
        {
            ++nTickFromLastMove;
        }

        ++nTickFromShotMove;
        //INPUT =========================================================================================================================
        if (key.isKeyPressed(0) && nPlayerY > 2 && nTickFromLastMove == nSpeed)
        {
            nPlayerY -= 1;
            nTickFromLastMove = 0;
        }
        if (key.isKeyPressed(1) && nPlayerY < (SpInv.height() - NavePlayer.height()) && nTickFromLastMove == nSpeed)
        {
            nPlayerY += 1;
            nTickFromLastMove = 0;
        }

        
        if (key.isKeyPressed(5) && nPlayerLives > 0)
        {
            nPlayerLives -= 1;
            nPlayerPowerShots -= 1;
        }
        if (key.isKeyPressed(6) && nPlayerLives < 8)
        {
            nPlayerLives += 1;
            nPlayerPowerShots += 1;
        }

        if (key.isKeyPressed(2) && nTickFromLastFire == nFireRate)
        {
            Shot playershot(NormalShot_spr, nPlayerX + NavePlayer.width(), nPlayerY + (NavePlayer.height() / 2));
            NormalShot.push_back(playershot);
            nTickFromLastFire = 0;
        }

        if (key.isKeyPressed(3) && nTickFromLastFire == nFireRate && !bPowerShotHeld && nPlayerPowerShots > 0)
        {
            Shot playershot(PowerShot_Shot, nPlayerX + NavePlayer.width(), nPlayerY + (NavePlayer.height() / 2));
            PowerShot.push_back(playershot);
            nPlayerPowerShots -= 1;
            nTickFromLastFire = 0;
        }
        if (key.isKeyPressed(3))
        {
            bPowerShotHeld = true;
        }
        else
        {
            bPowerShotHeld = false;
        }
        
        
        

        //GAME LOGIC =====================================================================================================================
           //Moving Shots and erasing if out of screen
            if (nTickFromShotMove == nShotSpeed)
            {
                for (int i = 0; i < NormalShot.size(); ++i)
                {
                    NormalShot[i].X += 1;
                    if (NormalShot[i].X >= SpInv.width() - 1)
                    {
                        NormalShot.erase(NormalShot.begin() + i);
                        --i;
                    }
                }
                for (int i = 0; i < PowerShot.size(); ++i)
                {
                    PowerShot[i].X += 1;
                    if (PowerShot[i].X >= SpInv.width() - 1)
                    {
                        PowerShot.erase(PowerShot.begin() + i);
                        --i;
                    }
                }
                for (int i = 0; i < MonsterShot.size(); ++i)
                {
                    MonsterShot[i].X -= 1;
                    if (MonsterShot[i].X <= 0)
                    {
                        MonsterShot.erase(MonsterShot.begin() + i);
                        --i;
                    }
                }
                nTickFromShotMove = 0;
                bShotMoved = true;
            }

            //Check Shot Colision
            if (bShotMoved)
            {
                for (int i = 0; i < NormalShot.size(); ++i) //Player normal shot
                {
                    for (int j = 0; j < LiveMonster.size(); ++j)
                    {
                        for (int k = 0; k < LiveMonster[j].getSprite().height(); ++k)
                        {
                            for (int l = 0; l < LiveMonster[j].getSprite().width(); ++l)
                            {
                                if (NormalShot[i].X == LiveMonster[j].X+l && NormalShot[i].Y == LiveMonster[j].Y + k)
                                {
                                    LiveMonster[j].HP -= nNormalShotDMG; 
                                    if (LiveMonster[j].HP <= 0) //Monster Dead
                                    {
                                        LiveMonster.erase(LiveMonster.begin() + j);
                                        --j;
                                    }
                                    NormalShot.erase(NormalShot.begin() + i);
                                }
                            }
                        }
                    }   
                }

                for (int i = 0; i < PowerShot.size(); ++i) //Player Power shot
                {
                    for (int j = 0; j < LiveMonster.size(); ++j)
                    {
                        for (int k = 0; k < LiveMonster[j].getSprite().height(); ++k)
                        {
                            for (int l = 0; l < LiveMonster[j].getSprite().width(); ++l)
                            {
                                for (int m = 0; m < PowerShot[i].GetSprite().height(); ++m)
                                {
                                    if (PowerShot[i].X == LiveMonster[j].X + l && PowerShot[i].Y + m == LiveMonster[j].Y + k)
                                    {
                                        LiveMonster[j].HP -= nPowerShotDMG;
                                        if (LiveMonster[j].HP <= 0) //Monster Dead
                                        {
                                            LiveMonster.erase(LiveMonster.begin() + j);
                                            --j;
                                        }
                                        PowerShot.erase(PowerShot.begin() + i);
                                    }
                                }
                            }
                        }
                    }
                }

                for (int i = 0; i < MonsterShot.size(); ++i) //Monster shot
                {
                    for (int k = 0; k < NavePlayer.height(); ++k)
                    {
                        for (int l = 0; l < NavePlayer.width(); ++l)
                        {
                            if (MonsterShot[i].X == (nPlayerX + l) && MonsterShot[i].Y == nPlayerY + k)
                            {
                                nPlayerLives -= 1;
                                MonsterShot.erase(MonsterShot.begin() + i);
                            }
                        }
                    }
                }

                bShotMoved = false;
            }

        //RENDER OUTPUT===================================================================================================================
        CleanFrame(SpInv); //Clean Frame

            //Draw Shots
            for (int i = 0; i < NormalShot.size(); ++i)
            {
                DrawSprite(NormalShot[i].GetSprite(), NormalShot[i].X, NormalShot[i].Y, SpInv);
            }
            for (int i = 0; i < PowerShot.size(); ++i)
            {
                DrawSprite(PowerShot[i].GetSprite(), PowerShot[i].X, PowerShot[i].Y, SpInv);
            }
            for (int i = 0; i < MonsterShot.size(); ++i)
            {
                DrawSprite(MonsterShot[i].GetSprite(), MonsterShot[i].X, MonsterShot[i].Y, SpInv);
            }

            //Draw HUD
            for (int i = 0; i < nPlayerLives; (i += Lives_Icon.width()))
                DrawSprite(Lives_Icon, 3 + i, 0, SpInv);

            for (int i = 0; i < nPlayerPowerShots; (i += PowerShot_Icon.width()))
                DrawSprite(PowerShot_Icon, nPlayerLives*Lives_Icon.width()+4 + i, 0, SpInv);

            //Draw Player
            DrawSprite(NavePlayer, nPlayerX, nPlayerY, SpInv);

            //Draw Monsters
            for (int i = 0; i < LiveMonster.size(); ++i)
            {
                DrawSprite(LiveMonster[i].getSprite(), LiveMonster[i].X, LiveMonster[i].Y, SpInv);
            }

            //Draw Other Stuff
            wstring info = L"nPlayerY = " + to_wstring(nPlayerY) + L"  Lives_Icon e POW = " + to_wstring(nPlayerLives) + L" FPS: " + to_wstring(FPS);
            DrawString(info, 4, 29, SpInv);

          
        DrawFrame(SpInv); //Display Frame
    }
    
    SpInv.Close();
    system("pause");
    return 0;
}
   
  
  
  
  
   
   
      
   
    
  
 

      
      
    
  
