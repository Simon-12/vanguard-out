' Enables Vanguard so it starts next time you boot your computer
Call CreateObject("Shell.Application").ShellExecute("cmd.exe", "/c ""sc config vgc start= demand & sc config vgk start= system""", "", "runas")
