# TS2 Object Limit Fix
## About
A patch that fixes crashes in Build/Buy mode when playing The Sims 2 with a large amount of custom content, a result of surpassing the game's internal 16-bit limit on object IDs.

Made for use with The Sims 2: Ultimate Collection, using either [Sims2RPC](https://modthesims.info/d/648220/sims2rpc-modded-sims-2-launcher-for-mansion-and-garden.html)
or [Ultimate ASI Loader](https://github.com/ThirteenAG/Ultimate-ASI-Loader).

## Explanation
Each OBJD for every object the game loads (including Sims) is stored in a data structure and assigned an object ID from 0&ndash;32,767. This upper limit is enforced by
a manual cast to a signed `short` integer.

There exists three functions within the game's binary at addresses `0x81101D`, `0x810EF0`, and `0x810E00`. These functions are only called when the Build/Buy mode
menu is open and each use a loop to iterate over items stored in a data structure (presumably containing the OBJDs of all the objects the player can buy). Cross-references with the
deobfuscated binary from the macOS port of the game indicate these functions perform an insertion sort.

The fatal flaw with these functions is that they contain no checks to validate whether the pointers they are trying to dereference and operate on during the sort are `NULL`.
This issue is compounded by the fact the game nullifies the pointers to any OBJDs whose assigned ID exceeds 32,767 before immediately dereferencing them anyway, leading 
to an access violation and the game crashing.

This patch merely adds the necessary validity checks to the three functions, which are performed before any dereferencing occurs.

## Things To Note
Any objects with an assigned ID of 32,768 and above will not be able to be purchased from the Build/Buy catalogue, as all of the game's functionality revolving around objects
checks only for IDs up to 32,767. This is understandably frustrating, but the alternative is to have the catalogue be completely unusable and crash every time you try to interact
with it.

## Installation
**For Sims2RPC**

1. Download the plugin found under the [Releases](https://github.com/spockthewok/TS2ObjectLimitFix/releases) section of this repository.
2. Move the downloaded plugin to the `\TSBin\mods` directory, found under wherever you have the Sims 2 installed to. For example, on my machine, the plugin would be moved to:

   `E:\Games\The Sims 2\Fun with Pets\SP9\TSBin\mods`

**For Ultimate ASI Loader**

1. Download Ultimate ASI Loader from [here](https://github.com/ThirteenAG/Ultimate-ASI-Loader/releases/download/Win32-latest/dsound-Win32.zip).
2. Extract `dsound.dll` from the zip file and place it in the game's `\TSBin` directory. On my machine, it would go here:

   `E:\Games\The Sims 2\Fun with Pets\SP9\TSBin`
3. Download this plugin and move it to the same `\TSBin` directory Ultimate ASI Loader was extracted to.

## Reporting Issues
If you still experience crashing in Build/Buy mode with this plugin, please open an issue here on GitHub and provide me with the following information from your crash log:

- `Exception address`
- `Registers`
- `Call stack`

These can be found by simply searching for the above text in your text editor. Examples of what the information for each might look like can be seen below:

`Exception address: 0x00810f45`

```
Registers
   EAX: 00000000
   EBX: 2d925480
   ECX: 2ab04fdc
   EDX: 00000128
   ESI: 00000000
   EDI: 2ab04fdc
   CS:EIP: 0023:00810f45
   SS:ESP: 002b:0351f8f8  EBP:00008000
   DS:002b  ES:002b  FS:0053  GS:002b
   Flags: 00010246
```

```
Call stack:
0x0001:0x0040ff45 E:\Games\The Sims 2\Fun with Pets\SP9\TSBin\Sims2EP9RPC.exe public: virtual bool __thiscall cTSSimCOMDirector::OnStart(class cIGZCOM *)() + 330901
0x0001:0x003d40b8 E:\Games\The Sims 2\Fun with Pets\SP9\TSBin\Sims2EP9RPC.exe public: virtual bool __thiscall cTSSimCOMDirector::OnStart(class cIGZCOM *)() + 85512
0x0001:0x004514c3 E:\Games\The Sims 2\Fun with Pets\SP9\TSBin\Sims2EP9RPC.exe public: virtual bool __thiscall cTSSimCOMDirector::OnStart(class cIGZCOM *)() + 598547
```

## Thanks
[LazyDuchess](https://github.com/LazyDuchess), whose hooking library and various TS2 plugins I used as the basis/template for my code.

[TeaAddictYT](https://github.com/TeaAddictYT) and [LordCrumps](https://github.com/LordCrumps), for being my guinea pigs and testing the plugin.
