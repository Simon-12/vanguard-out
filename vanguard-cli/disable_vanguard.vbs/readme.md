# Commands to temporarily stop Riot Vanguard

As oddly as it sounds, people actually do other things on their computer than play Valorant.
I've put together a few commands to turn off Riot Vanguard for when you aren't playing Valorant.

## Proof that I'm not hacking your computer
On Riot's support website for Valorant, they list two commands in the third step in the process of manually uninstalling Riot Vanguard.
[Link to the page.](https://support-valorant.riotgames.com/hc/en-us/articles/360044648213-Uninstalling-and-Disabling-Riot-Vanguard#manual-uninstall-guide-0-6)
These commands are `sc delete vgc` and `sc delete vgk`. These commands are telling Windows Services (known to the command line as `sc`) to delete two services (`vgc` and `vgk`).
My commands simply disable these services from starting next time you start your computer by changing their configuration to `disabled` (ie. `sc config vgc start= disabled`)

## Commands for people who don't know how commands work
If you don't know how commands work or you just want the easiest way to disable and enable Vanguard, I have created a few files for the best experience:

* [disable_vanguard.vbs](#file-disable_vanguard-vbs) - Disables Vanguard (you still have to close Vanguard manually from the tray)
* [disable_vanguard_stop.vbs](#file-disable_vanguard_stop-vbs) - Disables Vanguard and stops it
* [enable_vanguard.vbs](#file-enable_vanguard-vbs) - Enables Vanguard (you still need to restart your computer)
* [enable_vanguard_restart.vbs](#file-enable_vanguard_restart-vbs) - Enables Vanguard and restarts your computer

#### How to get these files:
[Click here](https://gist.github.com/AndrewMast/742ac7e07c37096017e907b0fd8ec7bb/archive/master.zip) to download all of the files (same as hitting "Download ZIP" at the top right-ish of the page)

__OR__

1. Click on one of the links above to scroll down to the file
2. Right click on the "Raw" button for the file
3. Select "Save Link As..." or "Download File As..."
4. Before saving, make sure to set the "Save as type" field to `All Files (*.*)` to ensure that the file is saved with the extension `.vbs` (__very important__)

#### How these files work:
These files run the same commands provided below, but asks you to run the command as administrator first. Let's dissect the code, eh?
Take [disable_vanguard.vbs](#file-disable_vanguard-vbs) for example:

`Call CreateObject("Shell.Application").ShellExecute("cmd.exe", "/c ""sc config vgc start= disabled & sc config vgk start= disabled""", "", "runas")`

If you noticed, in the middle of this code you can see `sc config vgc start= disabled & sc config vgk start= disabled`
which is the *exact* command that I provide for the more "experienced" people (see below). The `Call CreateObject("Shell.Application").ShellExecute` part
of the code gets ready to launch command prompt. Then `cmd.exe` is given to this so it can be opened, and `/c ""{...}""` simply tells command prompt what
command to run (I left out the command on purpose). Finally, `runas` tells the code that it needs to ask you for permission to run the command as an administrator.

##### The restart command
If you choose to use the command to restart your computer, `shutdown /r /f /t 00` is included in the command. `shutdown` tells command prompt that you want
to shutdown your computer, but `/r` tells it to restart. `/f` tells it to close all open programs and `/t 00` tells it to shutdown right away.

##### The close command
If you choose to use the command to stop Vanguard, `net stop vgc & net stop vgk & taskkill /IM vgtray.exe` is included in the command. `net stop vgc` and `net stop vgk`
tells command prompt to stop the Vanguard services (see [above](#proof-that-im-not-hacking-your-computer) for proof that these are actually Vanguard's services)
and `taskkill /IM vgtray.exe` tells command prompt to close the Vanguard tray icon (otherwise it would stay open even after you stop the services)

## Commands for experienced cmders
If you know how to open an elevated command prompt (super easy, barely an inconvenience: search for cmd in the start menu, right click it, run as admin),
then you can just run these commands to disable or enable Riot Vanguard (still requires a reboot once you enable it)

```bash
# Disable Vanguard (in disable_vanguard.vbs)
sc config vgc start= disabled & sc config vgk start= disabled

# Disable and stop Vanguard (in disable_vanguard_stop.vbs)
sc config vgc start= disabled & sc config vgk start= disabled & net stop vgc & net stop vgk & taskkill /IM vgtray.exe

# Enable Vanguard (in enable_vanguard.vbs)
sc config vgc start= demand & sc config vgk start= system

# Enable Vanguard and restart your computer (in enable_vanguard_restart.vbs)
sc config vgc start= demand & sc config vgk start= system & shutdown /r /f /t 00
```