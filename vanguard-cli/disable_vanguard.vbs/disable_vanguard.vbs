' Disables Vanguard from starting when you boot your computer
Call CreateObject("Shell.Application").ShellExecute("cmd.exe", "/c ""sc config vgc start= disabled & sc config vgk start= disabled""", "", "runas")
