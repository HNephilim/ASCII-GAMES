#include <string>
#include <iostream>
#include "engine.hpp"
using namespace std;


extern "C"
int wmain()
{
    wstring player;
    player.append(L"   +---------+   ");
    player.append(L"   |         |   ");
    player.append(L"   |         |   ");
    player.append(L"   |         |   ");
    player.append(L"   |         |   ");
    player.append(L"   |         |   ");
    player.append(L"   +---+-+---+   ");
    player.append(L"       | |       ");
    player.append(L"      XX XX      ");
    player.append(L"     XX| |XX     ");
    player.append(L"   XXX | | XXX   ");
    player.append(L" XXX   | |   XXX ");
    player.append(L"XX     | |     XX");
    player.append(L"       | |       ");
    player.append(L"       | |       ");
    player.append(L"      XX-XX      ");
    player.append(L"    XX     XX    ");
    player.append(L" XXXX       XXXX ");
    Sprite NavePlayer(17, 18, player);

    Buffer SpInv(120, 60);
    
    DrawSprite(NavePlayer, 0, 0, SpInv);

    while (1)
    {  
        DrawFrame(SpInv);
    }
    

    return 0;
}
   
  
  
  
  
   
   
      
   
    
  
 

      
      
    
  
