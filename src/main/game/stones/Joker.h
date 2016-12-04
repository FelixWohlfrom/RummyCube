/*
 * Joker.h
 *
 *  Created on: 04.01.2012
 *      Author: Felix Wohlfrom
 */

#ifndef JOKER_H_
#define JOKER_H_

#include "Gamestone.h"

#include <iostream>

#include <QImage>
#include <QBitmap>

/**
 * This class represents a joker on the board.
 */
class Joker : public Gamestone
{
    public:
        /**
         * Default constructor, with StoneColor black, number 0.
         * Stone is set on HEAP and with player type NONE. Initial invisible.
         */
        Joker();

        /**
         * Constructor for new joker stone. Initial invisible.
         * The color is black, number 0.
         * The stone will be set on HEAP and with player type NONE.
         *
         * @param parentWindow The (initial) parent window
         * @param stoneManager The stone manager that manages the current stone
         * @param isFirst True, if is first stone with stoneNumber, false if is second
         */
        Joker(QWidget* parentWindow, StoneManager* stoneManager, bool isFirst);

        /**
         * Returns the stone color.
         *
         * @return The stone color
         */
        StoneColor getColor();

        /**
         * Returns the stone number.
         *
         * @return The stone number
         */
        int getNumber();

        /**
         * Returns if the stone is a joker.
         *
         * @return true, the stone is a joker
         */
        bool isJoker() const;

        /**
         * Stores the stone variables.
         * In addition to Gamestone::storeStone it also stores color and number of joker.
         */
        void storeStone();

        /**
         * Restores the stone variable.
         * In addition to Gamestone::restoreStone it also restores color and number of joker.
         */
        void restoreStone();

#if 0
        /**
         * Loads the joker values from given stream.
         *
         * @param input The stream from which should be loaded
         * @param joker The joker that should be loaded
         */
        friend QXmlStreamReader &operator>>(QXmlStreamReader &input, Joker* stone);

         /**
         * Stores the joker values in given xml stream.
         *
         * @param output The stream in which should be stored
         * @param joker The joker that should be stored
         */
        friend QXmlStreamWriter &operator<<(QXmlStreamWriter &output, Joker* stone);
#endif

        #ifdef _DEBUG
            /**
             * Prints out the stone parameters.
             */
            friend std::ostream &operator<<(std::ostream &stream, Joker* stone);
        #endif

    private:
        /**
         * The joker image.
         */
        QImage jokerIcon;

        /**
         * The mask to draw joker image parts transparent.
         */
        QBitmap transparentMask;

        /**
         * Old joker color.
         */
        Gamestone::StoneColor oldColor;

        /**
         * Old joker number.
         */
        int oldNumber;

        /**
         * Updates joker number and color.
         */
        void update();

        /**
         * Checks if the given color is already in the row.
         *
         * @param color The color which should be checked
         * @return If the given color is already in the row of the current stone
         */
        bool colorIsAlreadyInRow(int color) const;

        /**
         * Checks if the current stone can be dropped on the given stone.
         *
         * @param otherStone The stone on which the current stone should be dropped
         * @return If the given stone can be dropped on the current stone.
         */
        bool acceptDropping(Gamestone& otherStone);

        /**
         * Returns if the current stone is successor of the given stone.
         *
         * @param otherStone The stone of which should be tested if the current stone is successor
         * @return if the current stone is successor of otherStone
         */
        bool isSucc(Gamestone* otherStone);

        /**
         * Returns if the current stone is predecessor of the given stone.
         *
         * @param otherStone The stone of which should be tested if the current stone is predecessor
         * @return if the current stone is predecessor of otherStone
         */
        bool isPred(Gamestone* otherStone);

        /**
         * Returns if the current stone and otherStone have the same stone number.
         *
         * @param otherStone The stone of which the stone number should be compared
         * @return if the current stone has same number as otherStone
         */
        bool isSameNumber(Gamestone* otherStone);

        /**
         * Returns if the current stone and otherStone have the same stone color.
         *
         * @param otherStone The stone of which the stone color should be compared
         * @return if the current stone has same color as otherStone
         */
        bool isSameColor(Gamestone* otherStone);

        /**
         * Returns if the stone is in a row with same number.
         *
         * @return If the row of the stone is a row with same number
         */
        bool isRowWithSameNumber();

        /**
         * Returns if the stone is in a row with same color.
         *
         * @return If the row of the stone is a row with same color
         */
        bool isRowWithSameColor();

        /**
         * Draws the stone.
         */
        virtual void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;

        #ifdef _DEBUG
            /**
             * Listener for mouse entering the current joker
             */
            virtual void enterEvent(QEvent *) Q_DECL_OVERRIDE;
        #endif
};

#endif /* JOKER_H_ */
