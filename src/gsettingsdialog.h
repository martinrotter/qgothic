#ifndef GSETTINGSDIALOG_H
#define GSETTINGSDIALOG_H

#include "ui_gsettingsdialog.h"

#include <QDialog>


namespace Ui {
    class GSettingsDialog;
}

class Game;
class GMainWindow;

/*!
 * \brief Configuration dialog.
 * \ingroup Gui
 */
class GSettingsDialog : public QDialog {
	Q_OBJECT
	
    public:
	explicit GSettingsDialog(Game *game, GMainWindow *parent = 0);
	~GSettingsDialog();

    protected:
	void createConnections();

	// Players setup.
	void loadGame();
	void setGame();

	// Language setup.
	void loadLanguages();
	void setLanguages();

	// Appearance setup.
	void loadAppearance();
	void setAppearance();

    protected slots:
	// Players setup.
	void enablePlayerBoxes();
	void changePlayerIcons();

	// Applying settings to the application.
	void applySettings();
	
    private:
	Ui::GSettingsDialog *m_ui;
	Game *m_game;
	GMainWindow *m_gameWindow;

	QString m_currentLanguage;

};

#endif // GSETTINGSDIALOG_H
