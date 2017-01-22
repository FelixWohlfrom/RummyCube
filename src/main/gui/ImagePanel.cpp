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
