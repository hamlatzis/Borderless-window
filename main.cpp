#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "BorderlessWindow.hpp"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<BorderlessWindow>("BorderlessWindow", 1, 0, "BorderlessWindow");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
