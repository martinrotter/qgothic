#ifndef GSETTINGS_H
#define GSETTINGS_H

#include <QSettings>
#include <QString>
#include <QVariant>

/*!
 * \brief Application-wide settings provider.
 *
 * \warning This is entirely static class.
 */
class GSettings {

    private:
	GSettings();

    public:
	/*!
	 * \brief Public pointers for settings object.
	 *
	 * \warning DO NOT use this pointer for accessing settings, use getSettings instead.
	 */
	static QSettings *s_settings;

	static QByteArray *s_array;

	/*!
	 * \brief Sets up applications-wide settings.
	 * \return Method returns status of settings.
	 */
	static QSettings::Status setupSettings();

	/*!
	 * \brief Keeps settings in sync and returns its current status.
	 * \return Method returns status of settings.
	 */
	static QSettings::Status checkSettings();

	/*!
	 * \brief Returns application-wide settings.
	 * \return
	 */
	static const QSettings *getSettings();

	/*!
	 * \brief Gets value from settings.
	 * \return Method returns value obtained from settings.
	 */
	static QVariant value(const QString &section,
			      const QString &key,
			      const QVariant &defaultValue = QVariant());

	/*!
	 * \brief Stores value into settings.
	 */
	static void setValue(const QString &section,
			     const QString &key,
			     const QVariant &value);

};

#endif // GSETTINGS_H
