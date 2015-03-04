#ifndef GLOBALDATA
#define GLOBALDATA
#include <QDir>
#include <QApplication>
#include <QSettings>

static const QDir exePath = QApplication::applicationDirPath();
static const QDir settingsPath = exePath.absolutePath().append("/settings.ini");
static QSettings settings(settingsPath.absolutePath(), QSettings::IniFormat);

#endif // GLOBALDATA

