# CnCGenMultiMonFix
Command &amp; Conquer: Generals – Zero Hour (MultiMonitor Lock)
=========================

When running C&C with a dual monitor setup. The mouse will not lock to the game window.  

This is a quick console app i wrote in C++ 2013 to solve.

Disclaimer: I am not a C++ developer. C&C does not support multi monitors and this solved the issue at hand. Use at your own risk!

# Usage

Copy [CnCGenMultiMonFix.exe](https://github.com/kcobb/CnCGenMultiMonFix/blob/main/CnCGenMultiMonFix/Release/CnCGenMultiMonFix.exe) to the `Command & Conquer Generals Zero Hour` installation directory.

Double-click `CnCGenMultiMonFix.exe` to run

A command window will open stating `searching for C&C game window...`

Leave this window running in the background

Now run `generals.exe`. This is the main executable for `Command & Conquer Generals Zero Hour` 

The `C&C Generals - MultiMonitor Lock` text output will change from `searching for C&C game window...` to `game window locked`

The mouse will now be locked to `Command & Conquer Generals Zero Hour` window. 

Once you quit `Command & Conquer Generals Zero Hour`, the `CnCGenMultiMonFix.exe` application will detect the closure, release the mouse lock and automatically close itself.
