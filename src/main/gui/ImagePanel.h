/*
 * ImagePanel.h
 *
 *  Created on: 21.08.2011
 *      Author: Felix Wohlfrom
 */

#ifndef IMAGEPANEL_H_
#define IMAGEPANEL_H_

#include <QLabel>
#include <QString>

/**
 * A panel containing an image as background. Image resizes with panel.
 */
class ImagePanel : public QLabel
{
    Q_OBJECT

    public:
        /**
         * Constructor.
         *
         * @param imagePath The image file to load
         * @param defaultImagePath The default image shown if the image file
         * 						   can not be loaded
         * @param parent The parent window of the panel
         * @param flags The flags that describe the panel
         */
        ImagePanel(QString imagePath, QString defaultImagePath,
                QWidget* parent = 0, Qt::WindowFlags flags = 0);

        /**
         * Sets the image to the specified image.
         *
         * @param imagePath The path to the image file
         */
        void setImage(QString imagePath);

        /**
         * Returns the current image path.
         * If default image is used, an empty string will be returned.
         *
         * @return The current image path. "" if default image
         */
        QString getImagePath();

    protected:
        /**
         * This event is called on mouse releases.
         */
        void mouseReleaseEvent(QMouseEvent*);

    private:
        /**
         * Path to the image on disk.
         */
        QString imagePath;

        /**
         * Default image if no file could be loaded.
         */
        QString defaultImagePath;

    signals:
        /**
         * Emited when the label is clicked.
         */
        void clicked();
};

#endif /* IMAGEPANEL_H_ */
