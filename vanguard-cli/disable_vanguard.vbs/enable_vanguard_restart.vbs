' Enables Vanguard and then reboots your computer to Vanguard
Call CreateObject("Shell.Application").ShellExecute("cmd.exe", "/c ""sc config vgc start= demand & sc config vgk start= system & shutdown /r /f /t 00""", "", "runas")
