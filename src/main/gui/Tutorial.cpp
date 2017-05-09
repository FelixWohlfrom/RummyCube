/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  RummyCube. A small board game.                                       *
 *  Copyright (C) 2017 Felix Wohlfrom                                    *
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
 * Tutorial.cpp
 *
 *  Created on: 18.03.2017
 *      Author: Felix Wohlfrom
 */

#include "Tutorial.h"
#include "ui_Tutorial.h"
#include "ui_MainWindow.h"

#include "../game/stones/Joker.h"
#include "../game/stones/StoneManager.h"

#include "animations/DisplayJoker.h"
#include "animations/ElementBlinker.h"
#include "animations/SameColorIncrementingNumber.h"
#include "animations/SameColorIncrementingNumberJoker.h"
#include "animations/SameNumberDifferentColor.h"
#include "animations/SameNumberDifferentColorJoker.h"

#include <QMutexLocker>

#ifdef _DEBUG
    #include <iostream>
#endif

Tutorial::Tutorial(QWidget* parent, QWidget* heap, QWidget* gameboard, QWidget* holder) :
    QWizard(parent, Qt::WindowSystemMenuHint | Qt::WindowTitleHint
        | Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint),
    ui(new Ui::Tutorial), heap(heap), gameboard(gameboard), holder(holder),
    currentAnimation(NULL)
{
    // Initialize widget
    ui->setupUi(this);

    connect(ui->gameRules, SIGNAL(linkHovered(QString)),
                     this, SLOT(playAnimation(QString)));

    connect(ui->playingOutStones, SIGNAL(linkHovered(QString)),
                     this, SLOT(playAnimation(QString)));

    connect(ui->joker, SIGNAL(linkHovered(QString)),
                     this, SLOT(playAnimation(QString)));

    this->layout();
}

Tutorial::~Tutorial()
{
    delete ui;
}

int Tutorial::nextId() const
{
    if (currentPage() == ui->tutorialSelectorPage)
    {
        // Stone ids are hardcoded, since there is no way to receive them programatically -.-
        if (ui->gameRulesTutorial->isChecked())
        {
            return 10;
        }
        if (ui->playingOutStonesTutorial->isChecked())
        {
            return 20;
        }
        if (ui->playingOutInFirstRoundTutorial->isChecked())
        {
            return 30;
        }
        if (ui->jokerTutorial->isChecked())
        {
            return 40;
        }
    }

    return currentPage()->nextId();
}

void Tutorial::playAnimation(QString animation)
{
    #ifdef _DEBUG
        std::cout << "Url: " << animation.toUtf8().constData() << std::endl;
    #endif

    // Synchronize access to currentAnimation object. Will free the mutex once the object
    // is destroyed aka on leaving the method.
    QMutexLocker syncher(&currentAnimationSyncher);

    if (currentAnimation != NULL) {
        delete currentAnimation;
        currentAnimation = NULL;
    }

    if (animation.isEmpty())
    {
        return;
    }

    if (animation == "blinkHeap")
    {
        currentAnimation = new ElementBlinker(heap);
        return;
    }

    if (animation == "blinkHolder")
    {
        currentAnimation = new ElementBlinker(holder);
        return;
    }

    if (animation == "blinkGameboard")
    {
        currentAnimation = new ElementBlinker(gameboard);
        return;
    }

    if (animation == "sameColorIncrementingNumber")
    {
        currentAnimation = new SameColorIncrementingNumber(holder);
        return;
    }

    if (animation == "sameNumberDifferentColor")
    {
        currentAnimation = new SameNumberDifferentColor(holder);
        return;
    }

    if (animation == "showJoker")
    {
        currentAnimation = new DisplayJoker(holder);
        return;
    }

    if (animation == "sameColorIncrementingNumberJoker")
    {
        currentAnimation = new SameColorIncrementingNumberJoker(holder);
        return;
    }

    if (animation == "sameNumberDifferentColorJoker")
    {
        currentAnimation = new SameNumberDifferentColorJoker(holder);
        return;
    }
}
