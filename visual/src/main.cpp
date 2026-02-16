#include "GridModel.h"
#include "PathFinderController.h"
#include "LanguageManager.h"

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTranslator>
#include <QLocale>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("PathFinding");
    app.setApplicationVersion("0.0.1");
    app.setOrganizationName("PathFinding");

    GridModel gridModel;
    PathFinderController controller(&gridModel);

    QQmlApplicationEngine engine;
    LanguageManager languageManager(&engine);

    QObject::connect(
        &languageManager, &LanguageManager::languageChanged, &controller, &PathFinderController::retranslate);

    QString systemLang = QLocale::system().name().startsWith("zh") ? "zh" : "en";
    languageManager.switchLanguage(systemLang);

    engine.rootContext()->setContextProperty("gridModel", &gridModel);
    engine.rootContext()->setContextProperty("pathFinderController", &controller);
    engine.rootContext()->setContextProperty("languageManager", &languageManager);

    const QUrl url(u"qrc:/qml/Main.qml"_qs);
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl& objUrl)
        {
            if (!obj && url == objUrl)
            {
                QCoreApplication::exit(-1);
            }
        },
        Qt::QueuedConnection);

    engine.load(url);

    return app.exec();
}
