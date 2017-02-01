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
 * ImagePanel.cpp
 *
 *  Created on: 21.08.2011
 *      Author: Felix Wohlfrom
 */

#include "ImagePanel.h"

#include <QPixmap>

ImagePanel::ImagePanel(QString imagePath, QString defaultImagePath,
		QWidget* parent, Qt::WindowFlags flags):
    QLabel(parent, flags), imagePath(imagePath),
	defaultImagePath(defaultImagePath)
{
    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    this->setSizePolicy(sizePolicy);
    this->setScaledContents(true);
    this->setImage(imagePath);
}

void ImagePanel::setImage(QString imagePath)
{
    this->imagePath = imagePath;
    QPixmap pixmap;
    if (imagePath == "" || !pixmap.load(imagePath))
    {
        pixmap.load(defaultImagePath);
    }

    this->setPixmap(pixmap);
}

QString ImagePanel::getImagePath()
{
    return imagePath;
}

void ImagePanel::mouseReleaseEvent(QMouseEvent *)
{
    emit clicked();
}
