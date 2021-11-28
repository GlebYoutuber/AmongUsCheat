#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <vector>
#include <conio.h>
using namespace std;

HWND hwnd;
DWORD pID;
HANDLE hProcess;

uintptr_t ModuleBase;
uintptr_t PlayerBase;

float speedhack = 0;
int isImposter = 1;
float nocooldown = 0;

uintptr_t GetModuleBaseAddress(const char* modName) {
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pID);
    if (hSnap != INVALID_HANDLE_VALUE) {
        MODULEENTRY32 modEntry;
        modEntry.dwSize = sizeof(modEntry);
        if (Module32First(hSnap, &modEntry)) {
            do {
                if (!strcmp(modEntry.szModule, modName)) {
                    CloseHandle(hSnap);
                    return (uintptr_t)modEntry.modBaseAddr;
                }
            } while (Module32Next(hSnap, &modEntry));
        }
    }
}

uintptr_t FindDMAAddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets)
{
    uintptr_t addr = ptr;
    for (unsigned int i = 0; i < offsets.size(); ++i)
    {
        ReadProcessMemory(hProc, (BYTE*)addr, &addr, sizeof(addr), 0);
        addr += offsets[i];
    }
    return addr;
}

void Speedhack() {

    if (GetAsyncKeyState(VK_NUMPAD1) & 1) {
        Sleep(100);
        speedhack++;
    }

    if (GetAsyncKeyState(VK_NUMPAD2) & 1) {
        Sleep(100);
        --speedhack;
    }

    if (speedhack <= 0) {
        speedhack = 1;
    }

    if (speedhack >= 10) {
        speedhack = 10;
    }

    uintptr_t dynamicptrbaseaddr = ModuleBase + 0x2C616F8;
    std::vector<unsigned int> speedoffset = { 0x5C, 0x24, 0x14 };

    uintptr_t myAddr = FindDMAAddy(hProcess, dynamicptrbaseaddr, speedoffset);

    WriteProcessMemory(hProcess, (LPVOID)myAddr, &speedhack, sizeof(speedhack), 0);

}

void AlwaysImposter() {
    if (GetAsyncKeyState(VK_NUMPAD3) & 1) {
        isImposter = !isImposter;
        Sleep(100);
    }

    uintptr_t imposterbaseaddr = ModuleBase + 0x2C6C310;
    std::vector<unsigned int> imposteroffset = { 0x5C, 0x0, 0x34, 0x28 };

    uintptr_t imposteraddr = FindDMAAddy(hProcess, imposterbaseaddr, imposteroffset);

    WriteProcessMemory(hProcess, (LPVOID)imposteraddr, &isImposter, sizeof(isImposter), 0);

}

void NoCoolDown() {
    uintptr_t ncdbaseAddr = ModuleBase + 0x2C6C310;
    std::vector<unsigned int> ncdoffset = { 0x5C, 0x0, 0x44 };

    uintptr_t ncdAddr = FindDMAAddy(hProcess, ncdbaseAddr, ncdoffset);

    WriteProcessMemory(hProcess, (LPVOID)ncdAddr, &nocooldown, sizeof(nocooldown), 0);
}

int main() 
{
    SetConsoleTitle("Among Us Cheat by GlebYoutuber"); //Console Title for Among Us :D

    hwnd = FindWindow(0, "Among Us"); //This is for Inject this Cheat 

    if (hwnd == NULL) {
        cout << "Open Among Us" << endl;
        Sleep(1000);
        return 0;
    }
    pID = 0;
    GetWindowThreadProcessId(hwnd, &pID);

    if (pID == 0) {
        cout << "\n\n Process ID not Found..." << endl;
        return 0;
    }
    ModuleBase = GetModuleBaseAddress("GameAssembly.dll");
    PlayerBase = GetModuleBaseAddress("UnityPlayer.dll");
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pID);

    cout << "Process Among Us is Found, Good Luck with this Cheat!" << endl;
    cout << "     Among Us Cheat by GlebYoutuber" << endl;

    cout << "\n\n [ Cheat SpeedHack ] Speedhack: " << speedhack;
    cout << "\n [ Always Imposter ] Imposter";
    cout << "\n [ NOCOOLDOWN Imposter ] No Cool Down Imposter";

    while (true) {
        Speedhack();
        AlwaysImposter();
        NoCoolDown();
    }
    system("pause");
    return 0;
}