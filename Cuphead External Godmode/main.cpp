/*

NOTE TO EVERYONE

Please never read any of the code, you risk severe trauma from the terrible code ahead.

PROCEED WITH CAUTION

*/


#include <iostream>
#include <Windows.h>
#include <cstdint>
#include <vector>
#include <TlHelp32.h>
#include <tchar.h>
#include <stdlib.h>
#include <string>
#include <thread>
#include "offsets.h"

namespace game
{
    HWND window;
    DWORD procId;

    HANDLE process;

    const int five = 5;
    const int four = 4;

    uintptr_t client;
}

uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName)
{
    uintptr_t modBaseAddr = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
    if (hSnap != INVALID_HANDLE_VALUE)
    {
        MODULEENTRY32 modEntry;
        modEntry.dwSize = sizeof(modEntry);
        if (Module32First(hSnap, &modEntry))
        {
            do
            {
                if (!_wcsicmp(modEntry.szModule, modName))
                {
                    modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
                    break;
                }
            } while (Module32Next(hSnap, &modEntry));
        }
    }
    CloseHandle(hSnap);
    return modBaseAddr;
}

std::uintptr_t findFromOffset(HANDLE handle, std::uintptr_t baseAddr, std::vector<std::ptrdiff_t> offsets)
{
    std::uintptr_t addr = baseAddr;
    bool success = true;
    for (int i = 0; i < offsets.size() - 1; i++)
    {
        if (!addr)
        {
            addr = NULL;
            success = false;
            break;
        }
        ReadProcessMemory(game::process, (LPVOID)(addr + offsets.at(i)), &addr, sizeof(addr), nullptr);
    }
    if (success)
        addr += offsets.at(offsets.size() - 1);

    return addr;
}

void getWindowsShit()
{
    game::window = FindWindowA(NULL, "Cuphead");

    if (!game::window)
    {
        std::cout << "Window not found, closing in 3 seconds\n\n";
        std::this_thread::sleep_for(std::chrono::seconds(3));
        return;
    }

    GetWindowThreadProcessId(game::window, &game::procId);

    game::process = OpenProcess(PROCESS_ALL_ACCESS, NULL, game::procId);

    game::client = GetModuleBaseAddress(game::procId, L"mono.dll");

    if (!game::client) // checking whether client.dll is a in the game
    {
        std::cout << "mono.dll not found, Closing in 3 seconds\n\n";
        std::this_thread::sleep_for(std::chrono::seconds(3));
        return;
    }
}


void godmode()
{
    getWindowsShit();

    bool godmodeEnabled = true;
    bool wasHoldingInsert = false;
    int goal = 4;

    std::cout << "   __       __                        __     \n  / \     /  |                      /  |\n  $$  \ / $$ | __    __   _______  _$$ | _\n  $$$  \ / $$$ |/  |  /  | /       |/ $$ |\n  $$$$ / $$$$ | $$ | $$ |/$$$$$$$ / $$$$$$ /\n  $$ $$ $$ / $$ | $$ | $$ | $$      \   $$ | __\n  $$ | $$$ / $$ | $$ \__$$ | $$$$$$ | $$ |/  |\n  $$ | $ / $$ | $$    $$ |/     $$ / $$  $$ /\n  $$ / $$ / $$$$$$$ | $$$$$$$ / $$$$/\n               /  \__$$ |\n               $$    $$ /\n                $$$$$$ / \n\n";

    std::cout << " Dm https://discordapp.com/users/717369125965398027 if you find any bugs or it doesnt work\n\n";

    std::cout << " Press insert to enable / disable godmode\n\n";
    std::cout << " Godmode Enabled [Y/N]: Y";

    // Loop
    while (true)
    {
        bool isHoldingInsert = GetAsyncKeyState(VK_INSERT);
        if (GetAsyncKeyState(VK_INSERT) && !wasHoldingInsert)
        {
            godmodeEnabled = !godmodeEnabled;

            system("CLS");

            std::cout << "   __       __                        __     \n  / \     /  |                      /  |\n  $$  \ / $$ | __    __   _______  _$$ | _\n  $$$  \ / $$$ |/  |  /  | /       |/ $$ |\n  $$$$ / $$$$ | $$ | $$ |/$$$$$$$ / $$$$$$ /\n  $$ $$ $$ / $$ | $$ | $$ | $$      \   $$ | __\n  $$ | $$$ / $$ | $$ \__$$ | $$$$$$ | $$ |/  |\n  $$ | $ / $$ | $$    $$ |/     $$ / $$  $$ /\n  $$ / $$ / $$$$$$$ | $$$$$$$ / $$$$/\n               /  \__$$ |\n               $$    $$ /\n                $$$$$$ / \n\n";

            std::cout << " Dm https://discordapp.com/users/717369125965398027 if you find any bugs or it doesnt work\n\n";

            std::cout << " Press insert to enable / disable godmode\n\n";

            if (godmodeEnabled)
                std::cout << " Godmode Enabled [Y/N]: Y";
            else
                std::cout << " Godmode Enabled [Y/N]: N";
        }

        wasHoldingInsert = isHoldingInsert;

        // Getting Player
        std::uintptr_t addr = findFromOffset(game::process, game::client, offset::plr);

        // Setting Value
        if (godmodeEnabled && addr != NULL)
        {
            WriteProcessMemory(game::process, (LPVOID)(addr + 0xB4), &goal, sizeof(goal), nullptr);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(3)); // sleeping thread
    }
}

int main()
{
    SetConsoleTitle(L"Cuphead Godmode - https://discord.gg/PfYMfbzMj7");
    godmode();
    return 0;
}