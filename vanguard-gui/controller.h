#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QDir>
#include <QFile>
#include <QFileSystemWatcher>
#include <QObject>
#include <QProcess>
#include <QTextStream>

class Controller : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool state READ state NOTIFY stateChanged)
    Q_PROPERTY(bool restart READ restart NOTIFY restartChanged)
    Q_PROPERTY(bool disabled READ disabled NOTIFY stateChanged)

public:
    explicit Controller(const QString &path, QObject *parent = nullptr);

    bool state() { return m_state; }
    bool restart() { return m_restart; }
    bool disabled() { return m_disabled; }

signals:
    void stateChanged();
    void restartChanged();

public slots:
    void changeState(const bool state);
    void restartSystem();
    void addAutostart();

private:
    void startController(const QString &path);
    void runProcess();

    bool m_state;
    bool m_restart;
    bool m_disabled;
    QString m_path;
    QString m_cliPath;
    QString m_statePath;
    QString m_autoPath;
    QProcess *m_process;
};

#endif  // CONTROLLER_H
