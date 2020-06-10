
#include <string>
#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#include <windows.h>

class Sprite
{
private:
    int m_nWidth;
    int m_nHeight;
    std::wstring m_sBody;

public:
    Sprite(std::wstring Body, int Height)
    {
        m_nWidth = Body.size()/Height;
        m_nHeight = Height;
        m_sBody = Body;
    }
    Sprite() = default;

    int height()
    {
        return m_nHeight;
    }

    int width()
    {
        return m_nWidth;
    }

    std::wstring body()
    {
        return m_sBody;
    }
};

class Monster
{
private:
    Sprite m_Body;

public:
    int HP;
    int PosX;
    int PosY;

    Monster(Sprite Body, int HitPoints, int X, int Y)
    {
        m_Body = Body;
        HP = HitPoints;
        PosX = X;
        PosY = Y;
    }

    Sprite getSprite()
    {
        return m_Body;
    }
};

class Buffer
{
private:
    int m_nScreenWidth;
    int m_nScreenHeight;
    wchar_t *screen = nullptr;

public:
    HANDLE hConsole;
    DWORD dwBytesWritten = 0;
    

    Buffer(int Screen_Width, int Screen_Height)
    {
        screen = new wchar_t[Screen_Width * Screen_Height];
        m_nScreenHeight = Screen_Height;
        m_nScreenWidth = Screen_Width;
        for (int i = 0; i < Screen_Width * Screen_Height; ++i)
        {
            screen[i] = L' ';
        }
        hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
        SetConsoleActiveScreenBuffer(hConsole);
    }

    int height()
    {
        return m_nScreenHeight;
    }

    int width()
    {
        return m_nScreenWidth;
    }

    wchar_t *getBuffer()
    {
        return screen;
    }
    void Close()
    {
        CloseHandle(hConsole);
    }

};

class GetKey
{
private:
    char *m_keys = nullptr;

public:
    GetKey(std::string VirtualKeyAraay)
    {
        m_keys = new char[VirtualKeyAraay.size()];
        char buffer[20];
        VirtualKeyAraay.copy(m_keys, VirtualKeyAraay.size(), 0);
    }

    bool isKeyPressed(int k)
    {
        return (0x8000 & GetAsyncKeyState((unsigned char)m_keys[k])) != 0;
    }
};

void DrawSprite(Sprite sprite, int CoordX, int CoordY, Buffer Screen)
{
    for (int py = 0; py < sprite.height(); ++py)
    {
        for (int px = 0; px < sprite.width(); ++px)
        {
            if (sprite.body()[py * sprite.width() + px] != L' ')
            {
                Screen.getBuffer()[(CoordY + py) * Screen.width() + (CoordX + px)] = sprite.body()[py * sprite.width() + px];
            }
        }
    }
}

void DrawMonster(Monster monster, Buffer Screen)
{
    for (int py = 0; py < monster.getSprite().height(); ++py)
    {
        for (int px = 0; px < monster.getSprite().height(); ++px)
        {
            if (monster.getSprite().body()[py * monster.getSprite().width() + px] != L' ')
            {
                Screen.getBuffer()[(monster.PosY + py) * Screen.width() + (monster.PosX + px)] = monster.getSprite().body()[py * monster.getSprite().width() + px];
            }
        }
    }
}

void DrawString(std::wstring phrase, int CoordX, int CoordY, Buffer Screen)
{
    for (int i = 0; i < phrase.size(); ++i)
    {
        Screen.getBuffer()[CoordY * Screen.width() + CoordX + i] = phrase[i];
    }
}

void DrawFrame(Buffer Screen)
{
    WriteConsoleOutputCharacter(Screen.hConsole, Screen.getBuffer(),Screen.height()*Screen.width(), {0, 0}, &Screen.dwBytesWritten);
}

void CleanFrame(Buffer Screen)
{
    for (int i = 0; i < Screen.width()*Screen.height(); ++i)
    {
        Screen.getBuffer()[i] = L' ';
    }
}