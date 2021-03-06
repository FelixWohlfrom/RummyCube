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
 * Gamestone.h
 *
 *  Created on: 21.07.2011
 *      Author: Felix Wohlfrom
 */

#ifndef GAMESTONE_H_
#define GAMESTONE_H_

#include <QWidget>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

#ifdef _DEBUG
    #include <iostream>
#endif

/**
 * Stone size at different screen resolutions.
 */
#define STONE_WIDTH_800 27
#define STONE_HEIGHT_800 30
#define STONE_WIDTH_1024 30
#define STONE_HEIGHT_1024 37
#define STONE_WIDTH_1280 32
#define STONE_HEIGHT_1280 40
#define STONE_WIDTH_1600 36
#define STONE_HEIGHT_1600 45

/**
 * The mimetype to identify gamestones during drag'n'drop.
 */
#define GAMESTONE_MIMETYPE "application/x-gamestone"

/**
 * Forward declarations of classes that contain references to Gamestones.
 */
class Player;
class StoneManager;
class ParentChangedEvent;

/**
 * This class represents a normal stone on the board.
 */
class Gamestone : public QWidget
{
    Q_OBJECT

    // Declare ai player as friend as it uses many private classes for playing
    friend class AIPlayer;
    // Also the joker has to be a friend because it stores pointers to this class.
    // Using pointers, the protected variables/methods can not be called anymore
    // For details see http://stackoverflow.com/questions/4672438/
    // /how-to-access-protected-method-in-base-class-from-derived-class
    friend class Joker;

    // To test qt internal classes, the test classes need to be friend classes
    friend class GamestoneTest;
    friend class GamestoneDragNDropTest;
    friend class JokerDragNDropTest;

    public:
        /**
         * Indicates if stones can be moved. Needed to disable movement e.g. when ai is playing.
         */
        static bool canMoveStones; // Initialized in Gamestone.cpp

        /**
         * The color of the current stone.
         */
        enum StoneColor { UNKNOWN = -1, BLACK = 0, BLUE, YELLOW, RED };

        /**
         * The parent of the current stone.
         */
        enum StoneParent { HEAP = 0, HOLDER, AIHOLDER, BOARD };

        /**
         * Default constructor, with StoneColor black, number 0.
         * Stone is set on HEAP and with player type NONE. Initial invisible.
         */
        Gamestone();

        /**
         * Constructor for new stone. Initial invisible.
         * The stone will be set on HEAP and with player type NONE.
         *
         * @param parentWindow The (initial) parent window
         * @param stoneManager The stone manager that manages the current stone
         * @param isFirst True, if is first stone with stoneNumber, false if is second
         * @param color The color of the stone
         * @param number the number of the stone
         */
        Gamestone(QWidget* parentWindow, StoneManager* stoneManager, bool isFirst,
                StoneColor color, int number);

        /**
         * Virtual destructor because we have virtual functions.
         */
        virtual ~Gamestone();

        /**
         * Returns the height of the stone.
         *
         * @return The height in pixels
         */
        int getHeight() const;

        /**
         * Returns the width of the stone.
         *
         * @return The width in pixels
         */
        int getWidth() const;

        /**
         * Sets if the button is invalid or not.
         * Used to show if all rules are observed.
         *
         * @param isInvalid Set to true, if not all rules are observed
         */
        void setInvalid(bool isInvalid);

        /**
         * Returns if the stone is invalid or not.
         *
         * @return if the stone is invalid
         */
        bool isInvalid() const;

        /**
         * Returns if is first stone with the number (true) or second.
         *
         * @return If is first stone or not
         */
        bool isFirst() const;

        /**
         * Returns the stone color.
         *
         * @return The stone color
         */
        virtual StoneColor getColor();

        /**
         * Returns the stone number.
         *
         * @return The stone number
         */
        virtual int getNumber();

        /**
         * Returns the stone parent.
         *
         * @return The current stone parent of the stone
         */
        StoneParent getParent() const;

        /**
         * Sets the parent of the widget to parent.
         *
         * @param parent The new parent widget.
         */
        void setParent(QWidget* parent);

        /**
         * Sets the new stone parent of the stone.
         *
         * @param newParent The new stone parent
         * @param stoneMoved If the stone has been moved during parent change
         */
        void setParent(StoneParent newParent, bool stoneMoved = false);

        /**
         * Returns the player of the stone.
         *
         * @return The player
         */
        Player* getPlayer() const;

        /**
         * Sets the player of the stone.
         *
         * @param newPlayer the new player of the stone
         */
        void setPlayer(Player* newPlayer);

        /**
         * Returns the previous stone in row.
         *
         * @return A pointer to the previous stone
         */
        Gamestone* getPrev() const;

        /**
         * Returns the next stone in row.
         *
         * @return A pointer to the next stone
         */
        Gamestone* getNext() const;

        /**
         * Returns if the stone is a joker.
         *
         * @return false, if the stone is no joker
         */
        virtual bool isJoker() const;

        /**
         * If the current stone has already been played out.
         *
         * @return true if the current round is the first round the stone was played out
         */
        bool isFirstTimePlayedOut() const;

        /**
         * This method needs to be called after the first time the stone has been played out.
         */
        void finishedFirstTimePlayedOut();

        /**
         * Stores the stone variables.
         */
        virtual void storeStone();

        /**
         * Stores the stone variables, using given parent.
         *
         * @param parent The parent which should be stored
         */
        void storeStone(StoneParent parent);

        /**
         * Restores the stone variables.
         */
        virtual void restoreStone();

        /**
         * Returns if the stone is first in row.
         *
         * @return True, if the stone is first in row
         */
        bool isFirstInRow() const;

        /**
         * Returns if the stone is last in row.
         *
         * @return True, if the stone is last in row
         */
        bool isLastInRow() const;

        /**
         * Counts the stones in the row of current stone.
         *
         * @return The count of stones in the row
         */
        int countStonesInRow() const;

        /**
         * Returns the color, number, isInvalid, isJoker and isFirst in one integer value
         * with following format:
         * msb                                     lsb
         *   0000 0000 0000 0000 0000 0000 0000 0000
         * lsb = isFirst
         * lsb + 1 = isJoker
         * lsb + 2 = isInvalid
         * lsb + 3 = color + 1   - Note the +1 to avoid negative numbers
         * lsb + 8 = number + 1  - same here
         *
         * @return The stone configuration as int
         */
        int asInt();

        /**
         * Moves the added stones of the current stone.
         */
        void moveAddedStones();

        /**
         * Listener for event when stone is dragged over another.
         *
         * @param event The drag enter event
         */
        void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;

        /**
         * This event is called once the gamestone is dropped on another.
         * Same as call to {@link #dropEvent(event, false)};
         *
         * @param event The drop event
         */
        void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;

        /**
         * This event is called once the gamestone is dropped on another.
         *
         * @param event The drop event
         * @param redirected If the drop event is redirected, e.g. from a parent window
         */
        void dropEvent(QDropEvent *event, bool redirected);

        /**
         * Loads the stone values from given stream.
         *
         * @param input The stream from which should be loaded
         * @param stone The stone that should be loaded
         */
        friend QXmlStreamReader &operator>>(QXmlStreamReader &input, Gamestone* stone);

         /**
         * Stores the stone values in given xml stream.
         *
         * @param output The stream in which should be stored
         * @param stone The stone that should be stored
         */
        friend QXmlStreamWriter &operator<<(QXmlStreamWriter &output, Gamestone* stone);

        #ifdef _DEBUG
            /**
             * Prints out the stone parameters.
             *
             * @param stream The stream on which should be printed
             * @param stone The stone that should be printed
             */
            friend std::ostream &operator<<(std::ostream &stream, Gamestone* stone);
        #endif


    // Don't need protected here although accessed from derived class Joker.
    // Joker is also a friend class, reason see on friend declaration.
    private:
        /**
         * The stone manager which manages this stone.
         * Pointer instead of reference for default c'tor.
         */
        StoneManager* stoneManager;

        /**
         * The stone width.
         */
        static int width;

        /**
         * The stone height.
         */
        static int height;

        /**
         * If the stone is first with this number (true) or second (false).
         */
        bool first;

        /**
         * The color of the stone.
         */
        StoneColor color;

        /**
         * The number of the stone.
         */
        int number;

        /**
         * If the stone hurts some rules.
         */
        bool invalid;

        /**
         * The player of the stone.
         */
        Player* player;

        /**
         * Stone position on board.
         */
        StoneParent stoneParent;

        /**
         * The stones that is before the current stone.
         */
        Gamestone* prev;

        /**
         * Zhe stone that is behind the current stone.
         */
        Gamestone* next;

        /**
         * The position of the stone on start of round.
         */
        QPoint oldPos;

        /**
         * The parent of the stone on start of round.
         */
        StoneParent oldParent;

        /**
         * The previous stone at start of round.
         */
        Gamestone* oldPrev;

        /**
         * The next stone at start of round.
         */
        Gamestone* oldNext;

        /**
         * The position of the cursor where the dragging started.
         */
        QPoint dragStartPosition;

        /**
         * If the current round is the first round the stone is played out on the gameboard.
         */
        bool firstTimePlayedOut;

        /**
         * Dummy to make updating of joker possible.
         */
        virtual void update();

        /**
         * Sets the previous stone in row.
         *
         * @param previous The previous stone
         */
        void setPrev(Gamestone* previous);

        /**
         * Sets the next stone in row.
         *
         * @param next The next stone
         */
        void setNext(Gamestone* next);

        /**
         * Appends the stone depending on conditions (number, color,
         * current stone is first or last in row ...).
         *
         * @param otherStone The stone that should be appended
         * @return if the stone has been appended
         */
        bool appendStone(Gamestone& otherStone);

        /**
         * Checks if the current stone can be dropped on the given stone.
         *
         * @param otherStone The stone on which the current stone should be dropped
         * @return If the given stone can be dropped on the current stone.
         */
        virtual bool acceptDropping(Gamestone& otherStone);

        /**
         * Returns if the current stone is successor of the given stone
         *
         * @param otherStone The stone of which should be tested if the current stone is successor
         * @return if the current stone is successor of otherStone
         */
        virtual bool isSucc(Gamestone* otherStone);

        /**
         * Returns if the current stone is predecessor of the given stone
         *
         * @param otherStone The stone of which should be tested if the current stone is predecessor
         * @return if the current stone is predecessor of otherStone
         */
        virtual bool isPred(Gamestone* otherStone);

        /**
         * Returns if the current stone and otherStone have the same stone number.
         *
         * @param otherStone The stone of which the stone number should be compared
         * @return if the current stone has same number as otherStone
         */
        virtual bool isSameNumber(Gamestone* otherStone);

        /**
         * Returns if the current stone and otherStone have the same stone color.
         *
         * @param otherStone The stone of which the stone color should be compared
         * @return if the current stone has same color as otherStone
         */
        virtual bool isSameColor(Gamestone* otherStone);

        /**
         * Returns if the stone is in a row with same number.
         *
         * @return If the row of the stone is a row with same number
         */
        virtual bool isRowWithSameNumber();

        /**
         * Returns if the stone is in a row with same color.
         *
         * @return If the row of the stone is a row with same color
         */
        virtual bool isRowWithSameColor();

        /**
         * Returns if a color of the row with current stone is already in the row that
         * contains otherStone.
         *
         * @param otherStone The stone in whose row should be checked if the color is already
         *                   available
         * @return if a color of the row containing current stone is already available in the row
         *            containing otherStone
         */
        bool colorIsAlreadyInRow(Gamestone* otherStone);

        /**
         * Returns if the color of firststone is already in the row that contains otherStone.
         *
         * @param firstStone The stone whose color should be checked in the row of otherStone
         * @param otherStone The stone in whose row the color should be checked
         * @return If the color of firstStone is already in the row containing otherStone
         */
        bool colorIsAlreadyInRow(Gamestone* firstStone, Gamestone* otherStone);

        /**
         * Returns if the row containing the current stone can be moved (back) to board.
         *
         * @return If the row can be moved to board
         */
        bool canMoveRowToBoard() const;

        /**
         * Shows or hides a complete row.
         *
         * @param show True if the row should be shown, false if it should be hidden
         */
        void showRow(bool show);

        /**
         * Renders the row containing the current stone into a pixmap. This is required
         * for drag'n'drop.
         *
         * @param firstInRow The first stone in the row from which the pixmap should be rendered
         * @return The pixmap of the current row
         */
        QPixmap* renderPixmap(Gamestone* firstInRow);

        /**
         * Draws the stone.
         */
        virtual void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;

        /**
         * This event is called once the gamestone is moved during drag'n'drop.
         */
        void dragMoveEvent(QDragMoveEvent *) Q_DECL_OVERRIDE;

        /**
         * Listener for mouse entering the current gamestone.
         */
        virtual void enterEvent(QEvent *) Q_DECL_OVERRIDE;

        /**
         * Listener for mouse leaving the current gamestone.
         */
        virtual void leaveEvent(QEvent *) Q_DECL_OVERRIDE;

        /**
         * Listener for mouse button click events.
         *
         * @param event The mouse event
         */
        virtual void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;

        /**
         * Listener for mouse button release events.
         */
        virtual void mouseReleaseEvent(QMouseEvent *) Q_DECL_OVERRIDE;

        /**
         * Listener for stone moving events.
         *
         * @param event The mouse event
         */
        virtual void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;

    signals:
        /**
         * This signal is emitted once the parent of the gamestone should be changed.
         *
         * @param event A {@link ParentChangedEvent}
         */
        void parentChanged(ParentChangedEvent& event);
};

#endif /* GAMESTONE_H_ */
