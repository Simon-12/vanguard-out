#include <QGuiApplication>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>

#include "controller.h"

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);
    app.setOrganizationName("Simon-12");
    app.setOrganizationDomain("https://github.com/Simon-12");
    app.setApplicationName("vanguard-out");
    app.setWindowIcon(QIcon(":/icon.ico"));

    // Start controller
    Controller controller(QCoreApplication::applicationDirPath(), &app);

    // Application style
    QQuickStyle::setStyle("Universal");
    qputenv("QT_QUICK_CONTROLS_UNIVERSAL_ACCENT", "#41cd52");

    // Start engine and set controller property
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("controller", &controller);

    // Start qml
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreationFailed, &app, []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);
    engine.loadFromModule("qml.components", "Main");

    return app.exec();
}
