#include "LanguageManager.h"

#include <QApplication>

LanguageManager::LanguageManager(QQmlEngine *engine, QObject *parent)
    : QObject(parent)
    , m_engine(engine)
    , m_currentLanguage("en")
{
}

QString LanguageManager::currentLanguage() const
{
    return m_currentLanguage;
}

void LanguageManager::switchLanguage(const QString& language)
{
    if (m_currentLanguage == language)
    {
        return;
    }

    qApp->removeTranslator(&m_translator);

    QString qmFile = QString(":/i18n/pathfinding_%1.qm").arg(language);

    if (m_translator.load(qmFile))
    {
        qApp->installTranslator(&m_translator);
        m_currentLanguage = language;
        m_engine->retranslate();
        Q_EMIT languageChanged();
    }
}
