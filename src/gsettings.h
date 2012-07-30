#ifndef GSETTINGS_H
#define GSETTINGS_H

#include <QSettings>
#include <QString>
#include <QVariant>


class GSettings {

    private:
	GSettings();

    public:
	// Public pointers for settings object. DO NOT use this pointer
	// for accessing settings, use getSettings instead.
	static QSettings *s_settings;

	// Sets up applications-wide settings.
	static QSettings::Status setupSettings();

	// Keeps settings in sync and returns its current status.
	static QSettings::Status checkSettings();

	static QSettings *getSettings();

	static QVariant value(const QString &section,
			      const QString &key,
			      const QVariant &defaultValue = QVariant());

	static void setValue(const QString &section,
			     const QString &key,
			     const QVariant &value);

};

#endif // GSETTINGS_H
