#ifndef GLOBALDATA
#define GLOBALDATA
#include <QDir>
#include <QApplication>
#include <QSettings>

#ifdef QT_DEBUG
    static const QDir *exePath = new QDir(QDir::currentPath().append("/../GS"));
#else
    static const QDir *exePath = new QDir::currentPath();
#endif
static const QDir *settingsPath = new QDir(exePath->absolutePath().append("/settings.ini"));
static QSettings *settings = new QSettings(settingsPath->absolutePath(), QSettings::IniFormat);
static const QDir *iconsPath = new QDir(exePath->absolutePath().append("/resources/icons"));

#endif // GLOBALDATA
