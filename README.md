# Command &amp; Conquer: Generals – Zero Hour (MultiMonitor Lock)
=========================

When running `Command & Conquer: Generals - Zero Hour` with a dual monitor setup. The mouse will not lock to the game window.  

This is a quick console app i wrote in C++ 2013 to solve.

Disclaimer: I am not a C++ developer. C&C does not support multi monitors and this solved the issue at hand. Use at your own risk!

# Usage

Copy [CnCGenMultiMonFix.exe](https://github.com/kcobb/CnCGenMultiMonFix/releases/download/v1.0.0.2/CnCGenMultiMonFix.exe) to the `Command & Conquer Generals Zero Hour` installation directory.

Double-click `CnCGenMultiMonFix.exe` to run

The application will attempt to locate the game installation path from the Windows Registry. 

If found, the game will auto launch.

A command window will open stating `searching for C&C game window...`

If application does not auto launch `Command & Conquer: Generals - Zero Hour`.

Run `generals.exe` manually. This is the main executable for `Command & Conquer: Generals - Zero Hour` 

The `C&C Generals - MultiMonitor Lock` text output will change from `searching for C&C game window...` to `game window locked`

The mouse will now be locked to `Command & Conquer Generals Zero Hour` window. 

Once you quit `Command & Conquer Generals Zero Hour`, the `CnCGenMultiMonFix.exe` application will detect the closure, release the mouse lock and automatically close itself.
