#ifndef LANGUAGE_MANAGER_H
#define LANGUAGE_MANAGER_H

#include <QObject>
#include <QTranslator>
#include <QQmlEngine>

class LanguageManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString currentLanguage READ currentLanguage NOTIFY languageChanged)

public:
    explicit LanguageManager(QQmlEngine *engine, QObject *parent = nullptr);

    QString currentLanguage() const;

    Q_INVOKABLE void switchLanguage(const QString& language);

Q_SIGNALS:
    void languageChanged();

private:
    QQmlEngine *m_engine;
    QTranslator m_translator;
    QString m_currentLanguage;
};

#endif // !LANGUAGE_MANAGER_H
