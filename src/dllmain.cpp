#include "utils.h"

// Sims2RPC module base address = 0x400000
// Offset = instruction absolute address - 0x400000
#define FUN1_JMPFROM_OFFSET 0x41101D // 0x81101D
#define FUN1_JMPTO_OFFSET 0xDD2EB0   // 0x11D2EB0
#define FUN2_JMPFROM_OFFSET 0x410F45 // 0x810F45
#define FUN2_JMPTO_OFFSET 0xDD2EC2   // 0x11D2EC2
#define FUN3_JMPFROM_OFFSET 0x410E81 // 0x810E81
#define FUN3_JMPTO_OFFSET 0xDD2ED4   // 0x11D2ED4

DWORD WINAPI MainThread(LPVOID param)
{
    HMODULE module = GetModuleHandleA(NULL);
    char *modBase = (char *)module;

    ///* FUN_0081101D() *///
    // JMP 0x11D2EB0
    char fun1Bytes[] = {0xE9, 0x8E, 0x1E, 0x9C, 0x00};
    WriteToMemory((DWORD)modBase + FUN1_JMPFROM_OFFSET, fun1Bytes, sizeof(fun1Bytes) / sizeof(*fun1Bytes));

    ///* Code cave at 0x11D2EB0 *///
    // TEST ESI,ESI (check ESI == null)
    // JZ 0x81105C (break out of loop if null)
    // MOV EDX,[ESI] (do swallowed instructions if != null)
    // PUSH EBX
    // MOV ECX,ESI
    // JMP 0x811022 (return to loop)
    char cave1Bytes[] = {0x85, 0xF6,
                         0x0F, 0x84, 0xA4, 0xE1, 0x63, 0xFF,
                         0x8B, 0x16,
                         0x53,
                         0x89, 0xF1,
                         0xE9, 0x60, 0xE1, 0x63, 0xFF};
    WriteToMemory((DWORD)modBase + FUN1_JMPTO_OFFSET, cave1Bytes, sizeof(cave1Bytes) / sizeof(*cave1Bytes));

    ///* FUN_00810EF0 *///
    // JMP 0x11D2EC2
    char fun2Bytes[] = {0xE9, 0x78, 0x1F, 0x9C, 0x00};
    WriteToMemory((DWORD)modBase + FUN2_JMPFROM_OFFSET, fun2Bytes, sizeof(fun2Bytes) / sizeof(*fun2Bytes));

    ///* Code cave at 0x11D2EC2 *///
    // TEST ESI,ESI (check ESI == null)
    // JZ 0x810F79 (break out of loop if null)
    // MOV EDX,[ESI] (do swallowed instructions if != null)
    // PUSH EBX
    // MOV ECX,ESI
    // JMP 0x810F4A (return to loop)
    char cave2Bytes[] = {0x85, 0xF6,
                         0x0F, 0x84, 0xAF, 0xE0, 0x63, 0xFF,
                         0x8B, 0x16,
                         0x53,
                         0x89, 0xF1,
                         0xE9, 0x76, 0xE0, 0x63, 0xFF};
    WriteToMemory((DWORD)modBase + FUN2_JMPTO_OFFSET, cave2Bytes, sizeof(cave2Bytes) / sizeof(*cave2Bytes));

    ///* FUN_00810e00 *///
    // JMP 0x11D2ED4
    char fun3Bytes[] = {0xE9, 0x4E, 0x20, 0x9C, 0x00};
    WriteToMemory((DWORD)modBase + FUN3_JMPFROM_OFFSET, fun3Bytes, sizeof(fun3Bytes) / sizeof(*fun3Bytes));

    ///* Code cave at 0x11D2ED4 *///
    // TEST EDI,EDI (check EDI == null)
    // JZ 0x810EC9 (break out of loop if null)
    // MOV EDX,[EDI] (do swallowed instructions if != null)
    // PUSH EAX
    // MOV ECX,EDI
    // JMP 0x810E86 (return to loop)
    char cave3Bytes[] = {0x85, 0xFF,
                         0x0F, 0x84, 0xED, 0xDF, 0x63, 0xFF,
                         0x8B, 0x17,
                         0x50,
                         0x89, 0xF9,
                         0xE9, 0xA0, 0xDF, 0x63, 0xFF};
    WriteToMemory((DWORD)modBase + FUN3_JMPTO_OFFSET, cave3Bytes, sizeof(cave3Bytes) / sizeof(*cave3Bytes));

    FreeLibraryAndExitThread((HMODULE)param, 0);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(0, 0, MainThread, hModule, 0, 0);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}