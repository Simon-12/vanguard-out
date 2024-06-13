#include "controller.h"

Controller::Controller(const QString &path, QObject *parent) : QObject{parent} {
    m_state = true;  // Default
    m_restart = false;
    m_disabled = false;
    m_process = new QProcess(this);
    m_path = path;
    startController(path);
}

void Controller::changeState(const bool state) {
    // Run cli
    QString argument;
    if (state)
        argument = "-s1";
    else
        argument = "-s0";
    m_process->setArguments(QStringList() << argument);
    runProcess();

    // Change state
    m_state = state;
    m_restart = state;
    emit stateChanged();
    emit restartChanged();
}

void Controller::restartSystem() {
    // Run cli
    m_process->setArguments(QStringList() << "-s1"
                                          << "-r");
    runProcess();

    m_restart = false;
    emit restartChanged();
}

void Controller::addAutostart() {
    // Create shortcut and open folder
    QProcess process;
    QString cmd = QString(
                      "$WshShell = New-Object -ComObject WScript.Shell;"
                      "$autostartPath = \"%1\";"
                      "$ShortcutPath = \"$autostartPath\\vanguard-cli.lnk\";"
                      "$Shortcut = $WshShell.CreateShortcut($ShortcutPath);"
                      "$Shortcut.TargetPath = \"%2\";"
                      "$Shortcut.WorkingDirectory = \"%3\";"
                      "$Shortcut.Arguments = \"-c\";"
                      "$Shortcut.Save();"
                      "explorer.exe $autostartPath;")
                      .arg(m_autoPath, m_cliPath, m_path);
    process.start("powershell", QStringList() << cmd);
    process.waitForFinished();

    // Check output
    QString out = process.readAllStandardOutput();
    QString err = process.readAllStandardError();
    if (!out.isEmpty()) qInfo() << out.toUtf8().constData();
    if (!err.isEmpty()) qInfo() << err.toUtf8().constData();
}

void Controller::startController(const QString &path) {
    // Paths
    QDir dir(path);
    m_cliPath = dir.filePath("vanguard-cli.exe");
    m_statePath = dir.filePath("state");

    // Autostart path
    QProcess process;
    QString cmd = "echo \"$env:APPDATA\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\";";
    process.start("powershell", QStringList() << cmd);
    process.waitForFinished();
    m_autoPath = process.readAllStandardOutput().simplified();

    // Check for cli
    if (!dir.exists(m_cliPath)) {
        m_disabled = true;
        return;  // Exit
    }
    m_process->setProgram(m_cliPath);

    // Check autostart
    if (!QFile::exists(m_autoPath + "\\vanguard-cli.lnk")) {
        return;  // Exit
    }

    // Check state file
    if (dir.exists(m_statePath)) {
        // Read state file
        QFile file(m_statePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qInfo() << "Error: could not open state file";
            m_disabled = true;
            return;  // Exit
        }
        QTextStream in(&file);
        QString firstLine = in.readLine();
        file.close();
        m_state = firstLine.startsWith("activate");
    }
}

void Controller::runProcess() {
    m_process->start();
    m_process->waitForFinished();

    // Check output
    QString out = m_process->readAllStandardOutput();
    qInfo() << out.toUtf8().constData();
}
