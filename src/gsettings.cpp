#include "gsettings.h"
#include "definitions.h"

#include <QApplication>
#include <QDebug>


GSettings::GSettings() {
}

QSettings::Status GSettings::checkSettings() {
    qDebug() << "Syncing settings.";
    s_settings->sync();
    return s_settings->status();
}

const QSettings *GSettings::getSettings() {
    if (s_settings == NULL) {
	setupSettings();
    }
    return s_settings;
}

QVariant GSettings::value(const QString &section,
			  const QString &key,
			  const QVariant &defaultValue) {
    return s_settings->value(QString("%1/%2").arg(section, key), defaultValue);
}

void GSettings::setValue(const QString &section,
			 const QString &key,
			 const QVariant &value) {
    s_settings->setValue(QString("%1/%2").arg(section, key), value);
}

QSettings::Status GSettings::setupSettings() {
    // If settings file exists in executable file working directory, then use it (portable settings).
    // Otherwise use settings file stored in homePath();
    QString defaultPath	= CONFIG_BASE + QDir::separator() + CONFIG_NAME;
    QString appPath	= qApp->applicationDirPath() + QDir::separator() + CONFIG_NAME;

    if (QFile(appPath).exists() == true) {
	s_settings = new QSettings(appPath, QSettings::IniFormat);
    qDebug("Initializing settings in %s.", qPrintable(appPath));
    }
    else {
      qDebug("Initializing settings in %s.", qPrintable(defaultPath));
	s_settings = new QSettings(defaultPath, QSettings::IniFormat);
    }
    return checkSettings();
}
