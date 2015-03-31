#ifndef GLOBALDATA
#define GLOBALDATA
#include <QDir>
#include <QApplication>
#include <QSettings>

static const QDir *exePath = new QDir(QApplication::applicationDirPath());
static const QDir *settingsPath = new QDir(exePath->absolutePath().append("/settings.ini"));
static QSettings *settings = new QSettings(settingsPath->absolutePath(), QSettings::IniFormat);

#ifdef QT_DEBUG
    static const QDir *iconPath = new QDir(exePath->absolutePath().append("/../../../GS/resources/icons"));
#else
    static const QDir *iconPath = new QDir(exePath->absolutePath().append("resources/icons"));
#endif

#endif // GLOBALDATA
