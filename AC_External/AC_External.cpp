// AC_External.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "stdafx.h"
#include "proc.h"
#include "mem.h"
#include "color.hpp"
#include<fcntl.h> 
#include <io.h>

//Player Base : [<ac_client.exe> + 0x109B74] / 0x509B74
//Player Primary : 0x128
//Player Primary Clip : 0x150
//Player Secondary : 0x114
//Player SecondaryClip : 0x13C
//Player Grenade Ammo : 0x158
//Player Armor : 0xFC
//Player Position XY : 0x4
//Player Position XY : 0x8
//Player Position Z : 0xC
//Player View Angle Verticle : 0x44
//Player View Angle Horizontal : 0x40
//Time Between Knifes : 0x160
//Pistol Timer : 0x164
//Primary Timer : 0x178
//Grenade Timer(? ) : 0x180
//Mouse Button Down : 0x224
//Entity Base : [<ac_client.exe> + 0x110D90] / 0x510D90
//TeamOffset1 : 0x204
//TeamOffset2 : 0x32C

//Kills : 1FC
//Deaths : 1F8
//Player Health : 0xF8

//Player Pos X : 0x38
//Player Pos Y : 0x34
//Player Pos Z : 0x3C


//- Renngeschwindigkeit ändern
//- Sprunghöhe ändern

//- Ein Schuss Kill

using speedFunc_t = void(*)();


#define pGame 0x509B74 //GameBase
#define ofFire 0x224
#define ofKnifeTimer 0x160
#define ofTargetName 0x501C38
#define ofSuperJump 0x18

void ShowConsoleCursor(bool showFlag)
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO     cursorInfo;

    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
}

void gotoXY(int x, int y)
{
	COORD coord = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	return;
}


int main()
{
    HWND console{ GetConsoleWindow() };
    RECT r;
    GetWindowRect(console, &r); //stores the console's current dimensions
    MoveWindow(console, r.left, r.top, 570, 600, TRUE); // 570 width, 600 height
    ShowConsoleCursor(false);


    HANDLE hProcess{};
    uintptr_t moduleBase{}, localPlayerPtr{}, healthAddr{}, xPos{}, zPos{}, yPos{}, superJump{};
 /*   speedFunc_t speed{};*/
    bool bHealth{}, bAmmo{}, bRecoil{};
    const int newValue{ 999 };
     float newJump{  };

     float x{};
     float y{};
     float z{};

    //Get ProcId of the target process
    DWORD procId{ GetProcId((L"ac_client.exe")) };

    if (procId)
    {
        //Get Handle to Process
        hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);
        //Getmodulebaseaddress
        moduleBase = GetModuleBaseAddress(procId, L"ac_client.exe");
        //Resolve address
        localPlayerPtr = moduleBase + 0x10f4f4;
        //Resolve base address of the pointer chain
        healthAddr = FindDMAAddy(hProcess, localPlayerPtr, { 0xf8 });
        xPos = FindDMAAddy(hProcess, localPlayerPtr, { 0x38 });
        yPos = FindDMAAddy(hProcess, localPlayerPtr, { 0x34 });
        zPos = FindDMAAddy(hProcess, localPlayerPtr, { 0x3c });
        superJump = FindDMAAddy(hProcess, localPlayerPtr, { 0x18 });
         /*speed = (speedFunc_t)(moduleBase + 0x25770);*/
        
    }

    else
    {
        std::cout << "Process not found";
        getchar();
        return 0;
    }

    auto red{ dye::on_light_red("SUB") };
    auto NUM1{ dye::on_light_red("NUM1") };
    auto NUM2{ dye::on_light_red("NUM2") };
    auto NUM3{ dye::on_light_red("NUM3") };
    auto NUM0{ dye::on_light_red("NUM0") };

    std::cout << dye::light_blue("=================================================================\n");
    /*std::cout << "\t\t MOD MENU BY VALENTYN\n";
    std::cout << "\t\t    Assault Cube\n";
    std::cout << "\t\t         GG\n";*/
    ::_setmode(::_fileno(stdout), _O_U16TEXT);
    ::std::wcout <<
        L"\u001b[35m      ██       ▄▄█▀▀▀█▄█   ▀████▄     ▄███▀ ▄▄█▀▀██▄ ▀███▀▀▀██▄  \u001b[0m\n"
        L"\u001b[35m     ▄██▄    ▄██▀     ▀█     ████    ████ ▄██▀    ▀██▄ ██    ▀██▄\u001b[0m\n"
        L"\u001b[31m    ▄█▀██▄   ██▀       ▀     █ ██   ▄█ ██ ██▀      ▀██ ██     ▀██\u001b[0m\n"
        L"\u001b[31m   ▄█  ▀██   ██              █  ██  █▀ ██ ██        ██ ██      ██\u001b[0m\n"
        L"\u001b[31m   ████████  ██▄             █  ██▄█▀  ██ ██▄      ▄██ ██     ▄██\u001b[0m\n"
        L"\u001b[35m  █▀      ██ ▀██▄     ▄▀     █  ▀██▀   ██ ▀██▄    ▄██▀ ██    ▄██▀\u001b[0m\n"
        L"\u001b[35m▄███▄   ▄████▄ ▀▀█████▀    ▄███▄ ▀▀  ▄████▄ ▀▀████▀▀ ▄████████▀  \u001b[0m\n";
    ::_setmode(::_fileno(stdout), _O_TEXT);

    std::cout << dye::light_yellow("=================================================================\n");
    std::cout << "[" << NUM1 << "] > > > Health\t\t >> " << red << " <<\n\n"; // /SUB/NOP
    std::cout << "[" << NUM2 << "] > > > Ammo  \t\t >> " << red << " <<\n\n"; // ADD/SUB/NOP/999/1
    std::cout << "[" << NUM3 << "] > > > Recoil\t\t >> " << red << " <<\n\n"; //NOP/RES
    std::cout << "[" << NUM0 << "] > > > Exit\t\t\n";




    DWORD dwExit{};
    auto switchHealth{ 1 };
    while (GetExitCodeProcess(hProcess, &dwExit) && dwExit == STILL_ACTIVE)
    {
        //Health bug affects all players
        if (GetAsyncKeyState(VK_NUMPAD1) & 1)
        {
            switch (switchHealth)
            {
                case 1: 
                    gotoXY(36, 5);
                    std::cout << dye::on_light_green("ADD") << "\n";
                    gotoXY(1, 5);
                    std::cout << dye::on_light_green("NUM1") << "\n";

                    mem::PatchEx((BYTE*)(moduleBase + 0x29D1F), (BYTE*)"\x01\x7B\x04", 3, hProcess); //add
                    switchHealth++;
                    break;
                case 2:
                    gotoXY(36, 5);
                    std::cout << dye::on_purple("NOP") << "\n";
                    gotoXY(1, 5);
                    std::cout << dye::on_purple("NUM1") << "\n";

                    mem::NopEx((BYTE*)(moduleBase + 0x29D1F), 3, hProcess); //nop
                    switchHealth++;
                    break;
                case 3:
                    gotoXY(36, 5);
                    std::cout << dye::on_light_red("SUB") << "\n";
                    gotoXY(1, 5);
                    std::cout << dye::on_light_red("NUM1") << "\n";

                    mem::PatchEx((BYTE*)(moduleBase + 0x29D1F), (BYTE*)"\x29\x7B\x04", 3, hProcess); //sub
                    switchHealth++;
                    break;
                case 4:
                    gotoXY(36, 5);
                    std::cout << dye::on_light_aqua("999") << "\n";
                    gotoXY(1, 5);
                    std::cout << dye::on_light_aqua("NUM1") << "\n";
                    WriteProcessMemory(hProcess, (BYTE*)healthAddr, &newValue, sizeof(newValue), nullptr);
                    switchHealth = 1;
                    break;
                default: 
                    return 0;
            }
        }
        //unlimited ammo patch
        if (GetAsyncKeyState(VK_NUMPAD2) & 1)
        {
            bAmmo = !bAmmo;

            if (bAmmo)
            {
                //ff 06 = inc[esi]
                mem::PatchEx((BYTE*)(moduleBase + 0x637e9), (BYTE*)"\xFF\x06", 2, hProcess);
            }
            else
            {
                //ff 0E = dec[esi]
                mem::PatchEx((BYTE*)(moduleBase + 0x637e9), (BYTE*)"\xFF\x0E", 2, hProcess);
            }
        }
        //no recoil NOP
        if (GetAsyncKeyState(VK_NUMPAD3) & 1) // Recoil
        {
            bRecoil = !bRecoil;

            if (bRecoil)
            {
                mem::NopEx((BYTE*)(moduleBase + 0x63786), 10, hProcess);
            } 
            else
            {
                //50 8D 4C 24 1C 51 8B CE FF D2; the original stack setup and call
                mem::PatchEx((BYTE*)(moduleBase + 0x63786), (BYTE*)"\x50\x8d\x4c\x24\x1c\x51\x8b\xce\xff\xd2", 10, hProcess);
            }

        }
        if (GetAsyncKeyState(VK_NUMPAD0) & 1)
        {
            return 0;
        }
        if (!GetAsyncKeyState(VK_NUMPAD5) & 1)
        {
            /*gotoXY(17, 1);
            std::cout << dye::on_light_purple("MOD MENU BY VALENTYN");
            gotoXY(20, 2);
            std::cout << dye::on_light_green("Assault Cube");
            gotoXY(25,3);
            std::cout << dye::on_light_red("GG");
            Sleep(100);

            gotoXY(17, 1);
            std::cout << dye::on_light_green("MOD MENU BY VALENTYN");
            gotoXY(20, 2);
            std::cout << dye::on_light_red("Assault Cube");
            gotoXY(25, 3);
            std::cout << dye::on_light_purple("GG");
            Sleep(100);

            gotoXY(17, 1);
            std::cout << dye::on_light_red("MOD MENU BY VALENTYN");
            gotoXY(20, 2);
            std::cout << dye::on_light_purple("Assault Cube");
            gotoXY(25, 3);
            std::cout << dye::on_light_green("GG");
            Sleep(100);*/
        }
        //Sleep(100); 

        if (GetAsyncKeyState(VK_SPACE) & 1 )
        {
            newJump = 4.0f;
            WriteProcessMemory(hProcess, (FLOAT*)superJump, &newJump, sizeof(newJump), nullptr);

        }

        if (GetAsyncKeyState(VK_DIVIDE) & 1)
        {
            ReadProcessMemory(hProcess, (BYTE*)xPos, &x, sizeof(x), nullptr);
            ReadProcessMemory(hProcess, (BYTE*)yPos, &y, sizeof(y), nullptr);
            ReadProcessMemory(hProcess, (BYTE*)zPos, &z, sizeof(z), nullptr);
        }
        if (GetAsyncKeyState(VK_MULTIPLY) & 1 && x!=0 && y!=0) 
        {
            WriteProcessMemory(hProcess, (BYTE*)xPos, &x, sizeof(x), nullptr);
            WriteProcessMemory(hProcess, (BYTE*)yPos, &y, sizeof(y), nullptr);
            WriteProcessMemory(hProcess, (BYTE*)zPos, &z, sizeof(z), nullptr);
        }
        if (GetAsyncKeyState(VK_NUMPAD8) & 1)
        {
            mem::PatchEx((BYTE*)(moduleBase + 0x5bea0 + 0x1), (BYTE*)"\x01", 1, hProcess); //rechts 0045BF60  B8 FF FF FF FF //backwards 5BE40  B8 FFFFFFFF //left 5BF00 - B8 01000000                
        }

    }

    std::cout << "Process not found";
    getchar();
    return 0;
}


/*
int main()
{
    //Get ProcID of the target process
    DWORD procId{ GetProcId(L"ac_client.exe") };
    //Getmodulebaseaddress
    uintptr_t moduleBase{ GetModuleBaseAddress(procId, L"ac_client.exe") };
    //Get handle to Process
    HANDLE hProcess{};
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);
    //Resolve base Address of the pointer chain
    uintptr_t dynamicPtrBaseAddr{ moduleBase + 0x10F4F4 };
    std::cout << "DynamicPtrBaseAddress = " << "0x" << std::hex << dynamicPtrBaseAddr << "\n";
    //Resolve amo pointer chain
    std::vector<unsigned int> ammoOffsets = { 0x374, 0x14, 0x0 };
    uintptr_t ammoAddr{ FindDMAAddy(hProcess, dynamicPtrBaseAddr, ammoOffsets) };
    std::cout << "Ammo Address = " << "0x" << std::hex << ammoAddr<< "\n";
    //Read amo value
    int ammoValue{};
    ReadProcessMemory(hProcess, (BYTE*)ammoAddr, &ammoValue, sizeof(ammoValue), nullptr);
    std::cout << "Current Ammo = " <<  std::dec << ammoValue << "\n";
    //Write to amo value
    std::cout << "Add ammo :";
    int newAmmo{ 2222 };
    std::cin >> newAmmo;
    WriteProcessMemory(hProcess, (BYTE*)ammoAddr, &newAmmo, sizeof(newAmmo), nullptr);
    //Read out again
    ReadProcessMemory(hProcess, (BYTE*)ammoAddr, &ammoValue, sizeof(ammoValue), nullptr);
    std::cout << "New Ammo = " << std::dec << ammoValue << "\n";
    return 0;
}
*/


/*::_setmode(::_fileno(stdout), _O_U16TEXT);
::std::wcout <<
    L" __       __  ______  _______       __       __ ________ __    __ __    __\n"
    L"|  \\     /  \\/      \\|       \\     |  \\     /  \\        \\  \\  |  \\  \\  |  \\\n"
    L"| ▓▓\\   /  ▓▓  ▓▓▓▓▓▓\\ ▓▓▓▓▓▓▓\\    | ▓▓\\   /  ▓▓ ▓▓▓▓▓▓▓▓ ▓▓\\ | ▓▓ ▓▓  | ▓▓\n"
    L"| ▓▓▓\\ /  ▓▓▓ ▓▓  | ▓▓ ▓▓  | ▓▓    | ▓▓▓\\ /  ▓▓▓ ▓▓__   | ▓▓▓\\| ▓▓ ▓▓  | ▓▓\n"
    L"| ▓▓▓▓\\  ▓▓▓▓ ▓▓  | ▓▓ ▓▓  | ▓▓    | ▓▓▓▓\\  ▓▓▓▓ ▓▓  \\  | ▓▓▓▓\\ ▓▓ ▓▓  | ▓▓\n"
    L"| ▓▓\\▓▓ ▓▓ ▓▓ ▓▓  | ▓▓ ▓▓  | ▓▓    | ▓▓\\▓▓ ▓▓ ▓▓ ▓▓▓▓▓  | ▓▓\\▓▓ ▓▓ ▓▓  | ▓▓\n"
    L"| ▓▓ \\▓▓▓| ▓▓ ▓▓__/ ▓▓ ▓▓__/ ▓▓    | ▓▓ \\▓▓▓| ▓▓ ▓▓_____| ▓▓ \\▓▓▓▓ ▓▓__/ ▓▓\n"
    L"| ▓▓  \\▓ | ▓▓\\▓▓    ▓▓ ▓▓    ▓▓    | ▓▓  \\▓ | ▓▓ ▓▓     \\ ▓▓  \\▓▓▓\\▓▓    ▓▓\n"
    L" \\▓▓      \\▓▓ \\▓▓▓▓▓▓ \\▓▓▓▓▓▓▓      \\▓▓      \\▓▓\\▓▓▓▓▓▓▓▓\\▓▓   \\▓▓ \\▓▓▓▓▓▓\n";
::_setmode(::_fileno(stdout), _O_TEXT);*/


//L"\u001b[35m▀████▄     ▄███▀ ▄▄█▀▀██▄ ▀███▀▀▀██▄     ▀████▄     ▄███▀███▀▀▀███▀███▄   ▀███▀███▀   ▀███▀\u001b[0m\n"
//L"\u001b[31m  ████    ████ ▄██▀    ▀██▄ ██    ▀██▄     ████    ████   ██    ▀█  ███▄    █  ██       █\u001b[0m\n"
//L"\u001b[31m  █ ██   ▄█ ██ ██▀      ▀██ ██     ▀██     █ ██   ▄█ ██   ██   █    █ ███   █  ██       █\u001b[0m\n"
//L"\u001b[31m  █  ██  █▀ ██ ██        ██ ██      ██     █  ██  █▀ ██   ██████    █  ▀██▄ █  ██       █\u001b[0m\n"
//L"\u001b[31m  █  ██▄█▀  ██ ██▄      ▄██ ██     ▄██     █  ██▄█▀  ██   ██   █  ▄ █   ▀██▄█  ██       █\u001b[0m\n"
//L"\u001b[31m  █  ▀██▀   ██ ▀██▄    ▄██▀ ██    ▄██▀     █  ▀██▀   ██   ██     ▄█ █     ███  ██▄     ▄█\u001b[0m\n"
//L"\u001b[31m▄███▄ ▀▀  ▄████▄ ▀▀████▀▀ ▄████████▀     ▄███▄ ▀▀  ▄████▄█████████████▄    ██   ▀██████▀▀\u001b[0m\n";


//struct DoubleVector : private std::vector<double>
//{
//    using vector<double>::vector;
//    friend std::ostream& operator<<(std::ostream&, const DoubleVector&);
//};
//
//std::ostream& operator<<(std::ostream& os, const DoubleVector& v)
//{
//    for (const auto& e : v)
//        os << e << "  ";
//    return os;
//}



//void ClearConsole()
//{
//	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
//	COORD coord = { 0, 0 };
//	DWORD count;
//	CONSOLE_SCREEN_BUFFER_INFO csbi;
//	if (GetConsoleScreenBufferInfo(hStdOut, &csbi))
//	{
//		FillConsoleOutputCharacter(hStdOut, (TCHAR)32, csbi.dwSize.X * csbi.dwSize.Y, coord, &count);
//		FillConsoleOutputAttribute(hStdOut, csbi.wAttributes, csbi.dwSize.X * csbi.dwSize.Y, coord, &count);
//		SetConsoleCursorPosition(hStdOut, coord);
//	}
//	return;
//}

https://guidedhacking.com/threads/updated-assaultcube-addies-offsets-functions.6072/
https://www.mpgh.net/forum/showthread.php?t=664311
https://www.youtube.com/watch?v=3yVB-6nnzXI&ab_channel=RyneRun
https://guidedhacking.com/threads/how-to-do-assaultcube-always-headshot.6149/
