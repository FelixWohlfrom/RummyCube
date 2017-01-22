/*
 * SettingsWindow.cpp
 *
 *  Created on: 22.08.2011
 *      Author: Felix Wohlfrom
 *
 */

#include "SettingsWindow.h"
#include "ui_SettingsWindow.h"

#include "../game/Settings.h"

#include <QFileDialog>
#include <QMessageBox>

SettingsWindow::SettingsWindow(QWidget* parent) :
    QDialog(parent, Qt::WindowSystemMenuHint | Qt::WindowTitleHint
             | Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint),
    ui(new Ui::SettingsWindow)
{
    // Initialize settings
    Settings* settings = Settings::getInstance();

    // Initialize window
    ui->setupUi(this);

    ui->stonesAtBeginning->setValue(
            settings->value("General/StonesAtBeginning", 14).toInt());
    ui->sumAtBeginning->setValue(
            settings->value("General/SumAtBeginning", 27).toInt());
    ui->stonesInOneRow->setChecked(
            settings->value("General/StonesInOneRow", true).toBool());
    ui->aiPlayerCount->setCurrentIndex(
            settings->value("General/AIPlayers", 1).toInt() - 1);
    ui->timeLimitEnabled->setChecked(
            settings->value("General/TimeLimit", false).toBool());
    ui->timeLimitValue->setValue(
            settings->value("General/TimeLimitTime", 60).toInt());

    ui->timeLimitLabel->setEnabled(ui->timeLimitEnabled->isChecked());
    ui->timeLimitValue->setEnabled(ui->timeLimitEnabled->isChecked());
    updateTimeLimitLabel();

    // The background images
    holderPanel = new ImagePanel(
                settings->value("General/HolderPath").toString(),
                QString(":images/holder"),
                this);
    holderPanel->setMinimumSize(QSize(140, 40));
    holderPanel->setMaximumSize(holderPanel->minimumSize());
    holderPanel->setToolTip(tr("Click here to choose another background"
            " picture for the holder"));
    ui->imageLayout->addWidget(holderPanel, 1, 0, 1, 1);

    gameboardPanel = new ImagePanel(
            settings->value("General/GameboardPath").toString(),
            QString(":images/gameboard"),
            this);
    gameboardPanel->setMinimumSize(QSize(140, 40));
    gameboardPanel->setMaximumSize(gameboardPanel->minimumSize());
    gameboardPanel->setToolTip(tr("Click here to choose another background"
            " picture for the gameboard"));
    ui->imageLayout->addWidget(gameboardPanel, 1, 2, 1, 1);

    connect(holderPanel, SIGNAL(clicked()),
                   this, SLOT(imageClickEvent()));
    connect(gameboardPanel, SIGNAL(clicked()),
                      this, SLOT(imageClickEvent()));
}

SettingsWindow::~SettingsWindow() {
    delete ui;
}

void SettingsWindow::updateTimeLimitLabel()
{
    int min = ui->timeLimitValue->value() / 60;
    int sec = ui->timeLimitValue->value() % 60;
    ui->timeLimitLabel->setText(
            tr("%1:%2 minutes").arg(min).arg(sec, 2, 10, QLatin1Char('0')));
}

void SettingsWindow::timeLimitEnabledClickEvent()
{
    ui->timeLimitLabel->setEnabled(ui->timeLimitEnabled->isChecked());
    ui->timeLimitValue->setEnabled(ui->timeLimitEnabled->isChecked());
}

void SettingsWindow::defaultClickEvent()
{
    QMessageBox closeGame(this);
    closeGame.setWindowTitle("RummyCube");
    closeGame.setText(tr("Do you really want to use the default pictures?"));
    closeGame.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    closeGame.setIcon(QMessageBox::Question);

    if (closeGame.exec() == QMessageBox::Yes)
    {
        holderPanel->setImage("");
        gameboardPanel->setImage("");
    }
}

void SettingsWindow::imageClickEvent()
{
    QFileDialog dialog(this, tr("Please select image"), "",
            tr("All Image Files (*.bmp; *.gif; *.png; *.jpg; *.jpeg; *.pcx;"
                    " *.pnm; *.tiff; *.tga; *.iff; *.xpm)"));
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);

    if (dialog.exec() == QDialog::Accepted)
    {
        ((ImagePanel*)(sender()))->setImage(dialog.selectedFiles().at(0));
    }
}

void SettingsWindow::okClickEvent()
{
    Settings* settings = Settings::getInstance();

    settings->setValue("General/StonesAtBeginning",
            ui->stonesAtBeginning->value());
    settings->setValue("General/SumAtBeginning",
            ui->sumAtBeginning->value());
    settings->setValue("General/StonesInOneRow",
            ui->stonesInOneRow->isChecked());
    settings->setValue("General/AIPlayers",
            ui->aiPlayerCount->currentIndex() + 1);
    settings->setValue("General/TimeLimit",
            ui->timeLimitEnabled->isChecked());
    settings->setValue("General/TimeLimitTime",
            ui->timeLimitValue->value());
    settings->setValue("General/HolderPath",
            holderPanel->getImagePath());
    settings->setValue("General/GameboardPath",
            gameboardPanel->getImagePath());

    this->accept();
}

void SettingsWindow::resetClickEvent()
{
    QMessageBox resetSettings(this);
    resetSettings.setWindowTitle("RummyCube");
    resetSettings.setText(tr("Do you really want to reset the settings?"));
    resetSettings.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    resetSettings.setIcon(QMessageBox::Question);
    if (resetSettings.exec() == QMessageBox::Yes)
    {
        ui->stonesAtBeginning->setValue(14);
        ui->sumAtBeginning->setValue(27);
        ui->stonesInOneRow->setChecked(true);
        ui->aiPlayerCount->setCurrentIndex(0);
        ui->timeLimitEnabled->setChecked(false);
        ui->timeLimitValue->setValue(60);
        holderPanel->setImage("");
        gameboardPanel->setImage("");
    }
}

void SettingsWindow::timeLimitSlideEvent(int)
{
    updateTimeLimitLabel();
}
