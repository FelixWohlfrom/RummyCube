/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  RummyCube. A small board game.                                       *
 *  Copyright (C) 2016 Felix Wohlfrom                                    *
 *                                                                       *
 *  This program is free software: you can redistribute it and/or modify *
 *  it under the terms of the GNU General Public License as published by *
 *  the Free Software Foundation, either version 3 of the License, or    *
 *  (at your option) any later version.                                  *
 *                                                                       *
 *  This program is distributed in the hope that it will be useful,      *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
 *  GNU General Public License for more details.                         *
 *                                                                       *
 *  You should have received a copy of the GNU General Public License    *
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*
 * PropertiesWindow.h
 *
 *  Created on: 22.08.2011
 *      Author: Felix Wohlfrom
 */

#ifndef SETTINGSWINDOW_H_
#define SETTINGSWINDOW_H_

#include "ImagePanel.h"

#include <QDialog>

namespace Ui {
    class SettingsWindow;
}

/**
 * The settings window in which the game settings can be defined by the user.
 */
class SettingsWindow : public QDialog
{
    Q_OBJECT

    public:
        /**
         * Constructor. Initializes the window elements.
         *
         * @param parent The parent window
         */
        SettingsWindow(QWidget *parent);

        /**
         * Destructor. Deletes the ui object.
         */
        virtual ~SettingsWindow();

    private:
        /**
         * The settings window ui.
         */
        Ui::SettingsWindow* ui;

        /**
         * The panel that shows the holder image.
         */
        ImagePanel* holderPanel;

        /**
         * The panel that shows the gameboard image.
         */
        ImagePanel* gameboardPanel;

        /**
         * Updates the time limit label.
         */
        void updateTimeLimitLabel();

    private slots:
        /**
         * Handles clicks on the time limit checkbox.
         */
        void timeLimitEnabledClickEvent();

        /**
         * Resets the background images to default.
         */
        void defaultClickEvent();

        /**
         * Lets the user select a new background image.
         */
        void imageClickEvent();

        /**
         * Stores the settings and closes the window with success state.
         */
        void okClickEvent();

        /**
         * Resets all settings.
         */
        void resetClickEvent();

        /**
         * Updates time limit label. Value is ignored.
         */
        void timeLimitSlideEvent(int);
};

#endif /* SETTINGSWINDOW_H_ */
