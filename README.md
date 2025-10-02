# Command & Conquer: Generals – Zero Hour (MultiMonitor Lock)

## Overview

When playing *Command & Conquer: Generals - Zero Hour* on a dual or triple monitor setup, a common issue arises where the mouse cursor is not confined to the game window. This results in the cursor wandering off the game window and onto other monitors, which can be incredibly distracting and disrupt the gaming experience.

To address this issue, I created a simple C++ console application in 2013 that locks the mouse to the game window, ensuring that the cursor stays within the boundaries of *Generals: Zero Hour*. This eliminates the frustration of the mouse moving off-screen and provides a more immersive gameplay experience.

## Disclaimer

I am not a professional C++ developer. This application is a workaround for the lack of multi-monitor support in *Command & Conquer: Generals – Zero Hour*. The game itself does not officially support multi-monitor setups, and this tool is not officially supported by the game or its developers. 

While I personally use this application without encountering major issues, please use it at your own risk. I cannot guarantee its functionality in all setups, and it may not work for everyone.

## Usage

1. **Download and Extract**: Download Latest [CnCGenMultiMonFix.exe](https://github.com/kcobb/CnCGenMultiMonFix/releases/latest/download/CnCGenMultiMonFix.exe) and place it in the *Command & Conquer Generals – Zero Hour* installation directory.

2. **Launch the Application**: Double-click `CnCGenMultiMonFix.exe` to run it.

3. **Game Auto Launch**: The application will attempt to detect the game installation path from the Windows Registry. If found, it will automatically launch *Command & Conquer: Generals – Zero Hour*.

4. **Manual Launch (If Needed)**: If the game does not automatically launch, you will need to manually start *Command & Conquer: Generals – Zero Hour* by running `generals.exe` (the main game executable).

5. **Mouse Lock**: Once the game is running, the application will display the text `searching for C&C game window...`. Once the game window is detected, the text will change to `game window locked`, indicating that the mouse has been successfully confined to the game window.

6. **Exiting the Game**: When you quit *Command & Conquer: Generals – Zero Hour*, the application will detect that the game has closed, release the mouse lock, and automatically shut itself down.

## Important Notes

- **Multi-Monitor Support**: *Command & Conquer: Generals – Zero Hour* does not natively support multi-monitor setups. This application provides an unofficial solution to help with the mouse locking issue.

- **Use at Your Own Risk**: While I have personally used this tool with no major issues, it is not officially supported by the game and may not work for everyone. Please use it at your own discretion.

---

Feel free to open an issue or submit a pull request if you encounter any problems or have suggestions for improvements!
