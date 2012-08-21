#include "gsettingsdialog.h"
#include "gsettings.h"
#include "gmainwindow.h"
#include "game.h"
#include "definitions.h"

#include <QMessageBox>
#include <QProcess>


GSettingsDialog::GSettingsDialog(Game *game, GMainWindow *parent) :
    QDialog(parent), m_ui(new Ui::GSettingsDialog), m_game(game), m_gameWindow(parent) {
    m_ui->setupUi(this);

    // Prepare interface.
    setFixedSize(sizeHint().width(), sizeHint().height());
    m_ui->m_stackedSections->setCurrentIndex(0);
    m_ui->m_spinMoves->setLineEditReadOnly(true);

    // Create connections for members.
    createConnections();

    // Load game.
    loadGame();
    enablePlayerBoxes();
    changePlayerIcons();

    // Load appearance.
    loadAppearance();
    loadLanguages();
}

GSettingsDialog::~GSettingsDialog() {
    delete m_ui;
}

void GSettingsDialog::loadLanguages() {
    m_ui->m_listLanguages->addItems(QDir(LANG_BASE).entryList(QStringList() << "*.qm"));
    QList<QListWidgetItem*> list = m_ui->m_listLanguages->findItems(GSettings::value(SET_APPEAR, "language",
										    "English (en).qm").toString(),
								   Qt::MatchExactly);
    if (list.size() > 0) {
	m_ui->m_listLanguages->setCurrentItem(list.at(0));
	m_currentLanguage = list.at(0)->text();
    }
}

void GSettingsDialog::setLanguages() {
    GSettings::setValue(SET_APPEAR, "language", m_ui->m_listLanguages->currentItem()->text());

    if (m_ui->m_listLanguages->currentItem()->text() != m_currentLanguage) {
	QMessageBox msg_box(this);
	msg_box.setWindowTitle(tr("Language Changed"));
	msg_box.setIcon(QMessageBox::Information);
	msg_box.setText(tr("Language of QGothic has been changed. Changes will take effect when application gets restarted."));
	msg_box.setInformativeText(tr("Do you want to restart now?"));
	msg_box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	msg_box.setDefaultButton(QMessageBox::Save);
	int restart_p = msg_box.exec();

	if (restart_p == QMessageBox::Yes) {
	    QProcess::startDetached(QApplication::applicationFilePath());
	    qApp->exit();
	}
    }
}

void GSettingsDialog::loadGame() {
    int white_dif = GSettings::value(SET_GAME, "white_player_dif", 0).toInt();
    int black_dif = GSettings::value(SET_GAME, "black_player_dif", 2).toInt();

    Player::State white_state = white_dif <= Player::HARD && white_dif >= Player::HUMAN ?
				    static_cast<Player::State>(white_dif) :
				    Player::HUMAN;

    Player::State black_state = black_dif <= Player::HARD && black_dif >= Player::HUMAN ?
				    static_cast<Player::State>(black_dif) :
				    Player::MEDIUM;

    m_ui->m_comboWhite->setCurrentIndex(white_state == Player::HUMAN ? 0 : 1);
    m_ui->m_comboBlack->setCurrentIndex(black_state == Player::HUMAN ? 0 : 1);

    m_ui->m_spinMoves->setValue(GSettings::value(SET_GAME, "max_moves_without_jump", 60).toInt());
    m_ui->m_comboStrategy->setCurrentIndex(GSettings::value(SET_GAME, "strategy-best-move", 0).toInt());


    QRadioButton *buttons_white[] = {
	m_ui->m_whiteEasy, m_ui->m_whiteMedium, m_ui->m_whiteHard
    };
    QRadioButton *buttons_black[] = {
	m_ui->m_blackEasy, m_ui->m_blackMedium, m_ui->m_blackHard
    };

    if (white_state != Player::HUMAN) {
	buttons_white[white_state-1]->setChecked(true);
    }
    if (black_state != Player::HUMAN) {
	buttons_black[black_state-1]->setChecked(true);
    }

    int current_player = GSettings::value(SET_GAME, "starting_player", 0).toInt();
    m_ui->m_radioBlackStarts->setChecked(current_player == Figure::BLACK);
}

void GSettingsDialog::changePlayerIcons() {
    // Setup white player.
    if (m_ui->m_comboWhite->currentIndex() == 0) {
	m_ui->m_labelWhite->setText(QString(GAM_PLAY_STYLE).arg("32",
								GAM_HUMAN_IMG_B,
								""));
    }
    else if (m_ui->m_whiteEasy->isChecked()) {
	m_ui->m_labelWhite->setText(QString(GAM_PLAY_STYLE).arg("32",
								GAM_EASY_IMG_B,
								""));
    }
    else if (m_ui->m_whiteMedium->isChecked()) {
	m_ui->m_labelWhite->setText(QString(GAM_PLAY_STYLE).arg("32",
								GAM_MEDIUM_IMG_B,
								""));
    }
    else {
	m_ui->m_labelWhite->setText(QString(GAM_PLAY_STYLE).arg("32",
								GAM_HARD_IMG_B,
								""));
    }

    // Setup black player.
    if (m_ui->m_comboBlack->currentIndex() == 0) {
	m_ui->m_labelBlack->setText(QString(GAM_PLAY_STYLE).arg("32",
								GAM_HUMAN_IMG_B,
								""));
    }
    else if (m_ui->m_blackEasy->isChecked()) {
	m_ui->m_labelBlack->setText(QString(GAM_PLAY_STYLE).arg("32",
								GAM_EASY_IMG_B,
								""));
    }
    else if (m_ui->m_blackMedium->isChecked()) {
	m_ui->m_labelBlack->setText(QString(GAM_PLAY_STYLE).arg("32",
								GAM_MEDIUM_IMG_B,
								""));
    }
    else {
	m_ui->m_labelBlack->setText(QString(GAM_PLAY_STYLE).arg("32",
								GAM_HARD_IMG_B,
								""));
    }
}

void GSettingsDialog::createConnections() {
    // Apply settings when OK button triggered.
    connect(m_ui->m_buttonBox, SIGNAL(accepted()), this, SLOT(applySettings()));

    // Player setup connections.
    connect(m_ui->m_comboWhite, SIGNAL(currentIndexChanged(int)), this, SLOT(enablePlayerBoxes()));
    connect(m_ui->m_comboBlack, SIGNAL(currentIndexChanged(int)), this, SLOT(enablePlayerBoxes()));
    connect(m_ui->m_comboWhite, SIGNAL(currentIndexChanged(int)), this, SLOT(changePlayerIcons()));
    connect(m_ui->m_comboBlack, SIGNAL(currentIndexChanged(int)), this, SLOT(changePlayerIcons()));
    connect(m_ui->m_whiteEasy, SIGNAL(clicked()), this, SLOT(changePlayerIcons()));
    connect(m_ui->m_whiteMedium, SIGNAL(clicked()), this, SLOT(changePlayerIcons()));
    connect(m_ui->m_whiteHard, SIGNAL(clicked()), this, SLOT(changePlayerIcons()));
    connect(m_ui->m_blackEasy, SIGNAL(clicked()), this, SLOT(changePlayerIcons()));
    connect(m_ui->m_blackMedium, SIGNAL(clicked()), this, SLOT(changePlayerIcons()));
    connect(m_ui->m_blackHard, SIGNAL(clicked()), this, SLOT(changePlayerIcons()));
    connect(m_ui->m_radioBlackStarts, SIGNAL(toggled(bool)), this, SLOT(changePlayerIcons()));
    connect(m_ui->m_radioWhiteStarts, SIGNAL(toggled(bool)), this, SLOT(changePlayerIcons()));
}

void GSettingsDialog::setGame() {
    Player white;
    white.setColor(Figure::WHITE);
    Player black;
    black.setColor(Figure::BLACK);

    // Setup white player.
    if (m_ui->m_comboWhite->currentIndex() == 0) {
	white.setState(Player::HUMAN);
    }
    else if (m_ui->m_whiteEasy->isChecked()) {
	white.setState(Player::EASY);
    }
    else if (m_ui->m_whiteMedium->isChecked()) {
	white.setState(Player::MEDIUM);
    }
    else {
	white.setState(Player::HARD);
    }

    // Setup black player.
    if (m_ui->m_comboBlack->currentIndex() == 0) {
	black.setState(Player::HUMAN);
    }
    else if (m_ui->m_blackEasy->isChecked()) {
	black.setState(Player::EASY);
    }
    else if (m_ui->m_blackMedium->isChecked()) {
	black.setState(Player::MEDIUM);
    }
    else {
	black.setState(Player::HARD);
    }

    m_game->setPlayer(white);
    m_game->setPlayer(black);
    GSettings::setValue(SET_GAME, "white_player_dif", white.getState());
    GSettings::setValue(SET_GAME, "black_player_dif", black.getState());
    GSettings::setValue(SET_GAME, "starting_player", static_cast<int>(m_ui->m_radioBlackStarts->isChecked()));
    GSettings::setValue(SET_GAME, "max_moves_without_jump", m_ui->m_spinMoves->value());
    GSettings::setValue(SET_GAME, "strategy-best-move", m_ui->m_comboStrategy->currentIndex());
}

void GSettingsDialog::loadAppearance() {
    m_ui->m_radioControlRight->setChecked(static_cast<Qt::Alignment>(GSettings::value(SET_APPEAR ,"controlbox_alignment",
										      Qt::AlignLeft).toInt()) == Qt::AlignRight);
}

void GSettingsDialog::setAppearance() {
    GSettings::setValue(SET_APPEAR, "controlbox_alignment",
			m_ui->m_radioControlLeft->isChecked() == true ?
			    Qt::AlignLeft :
			    Qt::AlignRight);
}

void GSettingsDialog::applySettings() {
    // Apply players setup.
    setGame();

    // Apply appearance setup. Set languages at last.
    setAppearance();
    setLanguages();

    accept();
}

void GSettingsDialog::enablePlayerBoxes() {
    m_ui->m_groupWhite->setEnabled(m_ui->m_comboWhite->currentIndex() != 0);
    m_ui->m_groupBlack->setEnabled(m_ui->m_comboBlack->currentIndex() != 0);
}
