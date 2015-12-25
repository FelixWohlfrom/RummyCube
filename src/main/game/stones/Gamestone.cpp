/*
 * Gamestone.cpp
 *
 *  Created on: 21.07.2011
 *      Author: Felix Wohlfrom
 */

#include "Gamestone.h"
#include "StoneManager.h"

#if 0
#include "ParentChangedEvent.h"
#endif

#include <QPen>
#include <QDrag>
#include <QPainter>
#include <QMimeData>
#include <QApplication>
#include <QMouseEvent>
#include <QDesktopWidget>

#ifdef _WIN32
    // Makes the debug output more colorful
    #include <windows.h>
#endif

/****************************************/
/* GENERAL STUFF, PLATTFORM INDEPENDEND */
/****************************************/
// Initialize static variables //
int Gamestone::height(40);
int Gamestone::width(30);
bool Gamestone::canMoveStones(true);

// Constructors //
Gamestone::Gamestone() :
        QWidget(), stoneManager(NULL), first(false), color(BLACK), number(0), invalid(
                false), player(NULL), stoneParent(HEAP), prev(NULL), next(NULL), oldPos(
                0, 0), oldParent(HEAP), oldPrev(NULL), oldNext(NULL)
{
}

Gamestone::Gamestone(QWidget* parent, StoneManager* stoneManager, bool isFirst,
        StoneColor color, int number) :
        QWidget(parent), stoneManager(stoneManager), first(isFirst), color(
                color), number(number), invalid(false), player(NULL), stoneParent(
                HEAP), prev(NULL), next(NULL), oldPos(0, 0), oldParent(HEAP), oldPrev(
                NULL), oldNext(NULL)
{
    Q_ASSERT(stoneManager != NULL);

    // TODO Just set if not on holder
    setAcceptDrops(true);

    // TODO Rework to use signals?
    this->hide();
    //wxYield();

    QRect screen = QApplication::desktop()->screenGeometry();
    if (screen.width() <= 1023)
    {
        this->width = STONE_WIDTH_800;
        this->height = STONE_HEIGHT_800;
    }
    else if (screen.width() >= 1024 && screen.width() <= 1279)
    {
        this->width = STONE_WIDTH_1024;
        this->height = STONE_HEIGHT_1024;
    }
    else if (screen.width() >= 1280 && screen.width() <= 1599)
    {
        this->width = STONE_WIDTH_1280;
        this->height = STONE_HEIGHT_1280;
    }
    else
    {
        this->width = STONE_WIDTH_1600;
        this->height = STONE_HEIGHT_1600;
    }

    this->resize(width, height);
}

Gamestone::~Gamestone()
{
}

// Getters and setters //
int Gamestone::getHeight() const
{
    return height;
}

int Gamestone::getWidth() const
{
    return width;
}

bool Gamestone::isJoker() const
{
    return false;
}

void Gamestone::setInvalid(bool isInvalid)
{
    invalid = isInvalid;
}

bool Gamestone::isInvalid() const
{
    return invalid;
}

bool Gamestone::isFirst() const
{
    return first;
}

int Gamestone::getNumber()
{
    return number;
}

Gamestone::StoneColor Gamestone::getColor()
{
    return color;
}

Gamestone::StoneParent Gamestone::getParent() const
{
    return stoneParent;
}

void Gamestone::setParent(StoneParent newParent, bool stoneMoved)
{
    // TODO Rework
    /*ParentChangedEvent e(wxPARENT_CHANGED, newParent, this, stoneMoved);
     this->GetEventHandler()->ProcessEvent(e);

     // Just change parent if has not been vetoed
     if (!e.GetVeto())
     {*/
    this->stoneParent = newParent;
    /*}

     // Reset invalidation if moved back to holder
     if (this->stoneParent == Gamestone::HOLDER)
     {
     this->setInvalid(false);
     }

     this->Refresh();*/
}

Player* Gamestone::getPlayer() const
{
    return player;
}

void Gamestone::setPlayer(Player* newPlayer)
{
    player = newPlayer;
}

Gamestone* Gamestone::getPrev() const
{
    return prev;
}

void Gamestone::setPrev(Gamestone* prevStone)
{
    // Reset old previous stone
    if (this->prev != NULL)
    {
        this->prev->next = NULL;
    }

    // Set new previous stone
    this->prev = prevStone;

    // Set next stone of new previous stone
    if (prevStone != NULL)
    {
        prevStone->next = this;
    }

    /*#ifdef _DEBUG
     std::cout << "Combining (prev) " << this << " and " << prevStone << std::endl;
     #endif*/
}

Gamestone* Gamestone::getNext() const
{
    return next;
}

void Gamestone::setNext(Gamestone* nextStone)
{
    // Reset old next stone
    if (this->next != NULL)
    {
        this->next->prev = NULL;
    }

    // Set new next stone
    this->next = nextStone;

    // Set previous stone of new next stone
    if (nextStone != NULL)
    {
        nextStone->prev = this;
    }

    /*#ifdef _DEBUG
     std::cout << "Combining (next) " << this << " and " << nextStone << std::endl;
     #endif*/
}

bool Gamestone::appendStone(Gamestone& otherStone)
{
    // Drop on right if other stone is predecessor ...
    if ((otherStone.getNumber() != -1 && otherStone.isPred(this)) ||
    // ... if is same number and current stone is last in row ...
            (this->isFirstInRow() && !this->isLastInRow()) ||
            // ... if is same number and other is last in row
            (otherStone.isLastInRow() && !otherStone.isFirstInRow()))
    {
        otherStone.setNext(this);
        return true;
    }

    // Dropping on left if other stone is successor ...
    else if ((otherStone.getNumber() != -1 && otherStone.isSucc(this)) ||
    // ... if is same color and current stone is first in row ..
            (this->isLastInRow() && !this->isFirstInRow()) ||
            // ... if is same color and dragging over stone is first in row
            (otherStone.isFirstInRow() && !otherStone.isLastInRow()))
    {
        otherStone.setPrev(this);
        return true;
    }

    return false;
}

void Gamestone::storeStone()
{
    /*#ifdef _DEBUG
     std::cout << "Storing stone " << this << ": " << this->stoneParent << ", ";
     std::cout << this->GetPosition().x  << ", " << this->GetPosition().y << ", ";
     std::cout << this->prev << ", " << this->next << std::endl;
     #endif*/

    this->oldParent = this->stoneParent;
    this->oldPos = this->pos();
    this->oldPrev = this->prev;
    this->oldNext = this->next;
}

void Gamestone::storeStone(StoneParent parent)
{
    this->storeStone();
    this->oldParent = parent; // Overwrites oldParent
}

void Gamestone::restoreStone()
{
    // Use moved stoned flag to make moving back to holder possible
    this->setParent(this->oldParent, true);
    this->move(this->oldPos);
    this->setPrev(oldPrev);
    this->setNext(oldNext);

    /*#ifdef _DEBUG
     std::cout << "Restored stone " << this << ": " << this->stoneParent << ", ";
     std::cout << this->GetPosition().x  << ", " << this->GetPosition().y << ", ";
     std::cout << this->prev << ", " << this->next << std::endl;
     #endif*/
}

// Private methods //
void Gamestone::update()
{
    // Empty dummy
}

bool Gamestone::acceptDropping(Gamestone& otherStone)
{
    // Never accept if dragged stone or stone dragging over is in middle of row
    if ((!this->isFirstInRow() && !this->isLastInRow())
            || (!otherStone.isFirstInRow() && !otherStone.isLastInRow()))
    {
        return false;
    }

    // Also don't accept if a first stone is dragged over a first or a last is dragged over a last
    // Just fail, if a row is dragged and not single stones
    if ((this->isFirstInRow() && otherStone.isFirstInRow()
            && !(this->isLastInRow() || otherStone.isLastInRow()))
            || (this->isLastInRow() && otherStone.isLastInRow()
                    && !(this->isFirstInRow() || otherStone.isFirstInRow())))
    {
        return false;
    }

    // Accept, if is same color and a row with same color
    if (otherStone.isSameColor(this) && this->isRowWithSameColor()
            && otherStone.isRowWithSameColor()
            && ((this->isFirstInRow() && otherStone.isLastInRow()
                    && otherStone.isPred(this))
                    || (this->isLastInRow() && otherStone.isFirstInRow()
                            && otherStone.isSucc(this))))
    {
        return true;
    }

    // Also accept if is same number and various color and row with same number
    if (otherStone.isSameNumber(this) && this->isRowWithSameNumber()
            && otherStone.isRowWithSameNumber()
            // Needed, because next test doesn't fail if a joker is in the middle of a row with already 4 stones
            && this->countStonesInRow() + otherStone.countStonesInRow() <= 4
            && !otherStone.colorIsAlreadyInRow(this))
    {
        return true;
    }

    return false;
}

bool Gamestone::isFirstInRow() const
{
    return (prev == NULL);
}

bool Gamestone::isLastInRow() const
{
    return (next == NULL);
}

bool Gamestone::isSucc(Gamestone* otherStone)
{
    return (this->getNumber() == otherStone->getNumber() + 1);
}

bool Gamestone::isPred(Gamestone* otherStone)
{
    return (this->getNumber() == otherStone->getNumber() - 1);
}

bool Gamestone::isSameNumber(Gamestone* otherStone)
{
    return (otherStone == NULL || (this->getNumber() == otherStone->getNumber())
            || (otherStone->isJoker() && otherStone->isSameNumber(this)));
}

bool Gamestone::isSameColor(Gamestone* otherStone)
{
    return (otherStone == NULL || (this->getColor() == otherStone->getColor())
            || (otherStone->isJoker() && otherStone->isSameColor(this)));
}

bool Gamestone::isRowWithSameNumber()
{
    return (this->isSameNumber(this->getPrev())
            && this->isSameNumber(this->getNext()));
}

bool Gamestone::isRowWithSameColor()
{
    return (this->isSameColor(this->getPrev())
            && this->isSameColor(this->getNext()));
}

bool Gamestone::colorIsAlreadyInRow(Gamestone* otherStone)
{
    bool result(colorIsAlreadyInRow(this, otherStone));

    Gamestone* prev(this->getPrev());
    while (prev != NULL)
    {
        result = result || colorIsAlreadyInRow(prev, otherStone);
        prev = prev->getPrev();
    }

    Gamestone* next(this->getNext());
    while (next != NULL)
    {
        result = result || colorIsAlreadyInRow(next, otherStone);
        next = next->getNext();
    }

    return result;
}

bool Gamestone::colorIsAlreadyInRow(Gamestone* firstStone,
        Gamestone* otherStone)
{
    // The checks for unknown colors avoid that joker colors are threaten as already available colors
    if (otherStone == NULL || firstStone->getColor() == UNKNOWN)
    {
        return false;
    }

    if (firstStone->isSameColor(otherStone)
            && otherStone->getColor() != UNKNOWN)
    {
        return true;
    }

    Gamestone* prev(otherStone->getPrev());
    while (prev != NULL)
    {
        if (prev->getColor() != UNKNOWN && firstStone->isSameColor(prev))
        {
            return true;
        }
        prev = prev->getPrev();
    }

    Gamestone* next(otherStone->getNext());
    while (next != NULL)
    {
        if (next->getColor() != UNKNOWN && firstStone->isSameColor(next))
        {
            return true;
        }
        next = next->getNext();
    }

    return false;
}

int Gamestone::countStonesInRow() const
{
    int count(1); // We have at least one stone (the current one)

    // Count next stones
    Gamestone* next(this->getNext());
    while (next != NULL)
    {
        ++count;
        next = next->getNext();
    }

    // Count previous stones
    Gamestone* prev(this->getPrev());
    while (prev != NULL)
    {
        ++count;
        prev = prev->getPrev();
    }

    return count;
}

bool Gamestone::isInSameRowAs(const Gamestone* otherStone) const
{
    if (otherStone == NULL)
    {
        return false;
    }

    if (this == otherStone)
    {
        return true;
    }

    Gamestone* prev(this->getPrev());
    while (prev != NULL)
    {
        if (otherStone == prev)
        {
            return true;
        }
        prev = prev->getPrev();
    }

    Gamestone* next(this->getNext());
    while (next != NULL)
    {
        if (otherStone == next)
        {
            return true;
        }
        next = next->getNext();
    }

    return false;
}

void Gamestone::moveAddedStones()
{
    // Move next stones
    Gamestone* curr(this);
    Gamestone* next(this->getNext());
    while (next != NULL)
    {
        next->setParent(curr->getParent(), true);
        next->move(curr->pos() + QPoint(curr->getWidth(), 0));
        // TODO Reimplement?
        //next->Refresh();
        curr = next;
        next = next->getNext();
    }

    // Move previous stones
    curr = this;
    Gamestone* prev(this->getPrev());
    while (prev != NULL)
    {
        prev->setParent(curr->getParent(), true);
        prev->move(curr->pos() - QPoint(prev->getWidth(), 0));
        // TODO Reimplement
        //prev->Refresh();
        curr = prev;
        prev = prev->getPrev();
    }
}

bool Gamestone::canMoveRowToBoard() const
{
    if (this->getPlayer() == NULL)
    {
        return false;
    }

    // Check next stones
    Gamestone* next(this->getNext());
    while (next != NULL)
    {
        if (next->getPlayer() == NULL)
        {
            return false;
        }
        next = next->getNext();
    }

    // Check previous stones
    Gamestone* prev(this->getPrev());
    while (prev != NULL)
    {
        if (prev->getPlayer() == NULL)
        {
            return false;
        }
        prev = prev->getPrev();
    }

    return true;
}

int Gamestone::asInt()
{
    int stoneData(0);
    // The +1 is needed because on jokers if the number is unknown it's represented as -1 which breaks the shift
    stoneData = (stoneData | (this->getNumber() + 1)) << 5;
    // The same here, unknown colors are represented as -1
    stoneData = (stoneData | (this->getColor() + 1)) << 1;

    if (this->isInvalid())
    {
        stoneData = (stoneData | 1) << 1;
    }
    else
    {
        stoneData = stoneData << 1;
    }

    if (this->isJoker())
    {
        stoneData = (stoneData | 1) << 1;
    }
    else
    {
        stoneData = stoneData << 1;
    }

    if (first)
    {
        stoneData = stoneData | 1;
    }

    return stoneData;
}

/*************/
/* Operators */
/*************/
#if 0
wxDataInputStream &operator>>(wxDataInputStream &input, Gamestone* stone)
{
    // Restore position and parent
    int x, y, parent;
    input >> x >> y >> parent;
    stone->setParent((Gamestone::StoneParent)parent, true);
    stone->SetPosition(wxPoint(x, y));// First set parent, there the position will be automatically
                                      // set if stone is from heap and overwritten here

                                      // Restore next and previous stones
    int buf;
    input >> buf;
    if (buf != 0)
    {
        input >> buf;
        stone->setNext(stone->stoneManager->getStoneFromInt(buf));
    }
    else
    {
        stone->setNext(NULL);
    }

    input >> buf;
    if (buf != 0)
    {
        input >> buf;
        stone->setPrev(stone->stoneManager->getStoneFromInt(buf));
    }
    else
    {
        stone->setPrev(NULL);
    }

    // Restore saved settings of stone
    input >> x >> y >> parent;
    stone->oldPos = wxPoint(x, y);
    stone->oldParent = (Gamestone::StoneParent)parent;

    input >> buf;
    if (buf != 0)
    {
        input >> buf;
        stone->oldNext = stone->stoneManager->getStoneFromInt(buf);
    }
    input >> buf;
    if (buf != 0)
    {
        input >> buf;
        stone->oldPrev = stone->stoneManager->getStoneFromInt(buf);
    }

    return input;
}

wxDataOutputStream &operator<<(wxDataOutputStream &output, Gamestone* stone)
{
    // First store the stone parameters
    output << stone->asInt();
    output << stone->GetPosition().x << stone->GetPosition().y;
    output << stone->stoneParent;

    // Next and previous stones
    if (stone->getNext() != NULL)
    {
        output << true;
        output << stone->next->asInt();
    }
    else
    {
        output << false;
    }

    if (stone->getPrev() != NULL)
    {
        output << true;
        output << stone->prev->asInt();
    }
    else
    {
        output << false;
    }

    // Store saved settings of stone
    output << stone->oldPos.x << stone->oldPos.y;
    output << stone->oldParent;

    if (stone->oldNext != NULL)
    {
        output << true;
        output << stone->oldNext->asInt();
    }
    else
    {
        output << false;
    }
    if (stone->oldPrev != NULL)
    {
        output << true;
        output << stone->oldPrev->asInt();
    }
    else
    {
        output << false;
    }

    return output;
}
#endif

std::ostream &operator<<(std::ostream &stream, Gamestone* stone)
{
    const std::string colorNames[] = { "Unknown", "Black", "Blue", "Yellow", "Red" };
    if (stone != NULL)
    {
        // Changes console color
        #ifdef _WIN32
            CONSOLE_SCREEN_BUFFER_INFO conInfo;
            GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &conInfo);
            const int savedColor = conInfo.wAttributes;

            // Extend color definitions from WinCon.h
            #define BACKGROUND_WHITE BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE
            #define FOREGROUND_BLACK 0x0000
            #define FOREGROUND_YELLOW FOREGROUND_RED | FOREGROUND_GREEN
            const int colors[] =
            { FOREGROUND_BLACK, FOREGROUND_BLACK, FOREGROUND_BLUE,
                    FOREGROUND_YELLOW, FOREGROUND_RED };
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                    colors[stone->color + 1] | BACKGROUND_WHITE);
        #endif

            // The color name resolving is a little hack, currently works with gcc, maybe there should be used something better?
            // But for debugging, its doing its job ;).
            stream << stone->number << " (" << colorNames[stone->color + 1] << ") "
                    << stone->first;

        #ifdef _WIN32
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), savedColor);
        #endif
    }
    else
    {
        stream << "NULL";
    }

    return stream;
}
}

/******************************************/
/*    HERE COMES THE QT SPECIFIC STUFF    */
/******************************************/
void Gamestone::paintEvent(QPaintEvent *)
{
    // TODO Buffer in QPixmap

    // Just invisible, if it's on the holder of ai
    if (stoneParent == AIHOLDER)
    {
        return;
    }

    QPainter painter(this);

    // first draw the background
    if (!invalid)
    {
        painter.setBrush(QBrush(QColor(0xFF, 0xFF, 0xCF)));
    }
    else
    {
        painter.setBrush(QBrush(QColor(0xD0, 0xD0, 0xD0)));
    }
    painter.drawRect(0, 0, width, height);

    // now draw the lines that make the stone look "3d"
    for (int i = 0; i < 3; ++i)
    {
        // Drawing left side
        painter.setPen(QPen(Qt::white));
        painter.drawLine(i, 0, i, height - i);

        // Drawing top
        painter.drawLine(0, i, width - i, i);

        // Drawing right side
        painter.setPen(QPen(Qt::lightGray));
        painter.drawLine(width - i, i, width - i, height);

        // Drawing bottom
        painter.drawLine(i, height - i, width, height - i);
    }

    // If the stone is still on heap, don't draw number or gamestone image
    if (stoneParent == HEAP)
    {
        return;
    }

    // the color of the number
    int showColor(0);
    switch (color)
    {
    case BLACK:
        showColor = 0x000000;
        break;
    case BLUE:
        showColor = 0x0000FF;
        break;
    case RED:
        showColor = 0xFF0000;
        break;
    case YELLOW:
        showColor = 0xCACA00;
        break;
    default:
        break; // Avoid warning
    }

    if (invalid)
    {
        showColor = showColor & 0xD0D0D0;
    }

    // Draw the colored number to the middle of the stone
    painter.setPen(
            QColor(showColor >> 16, (showColor >> 8) % 0x100,
                    showColor % 0x100));
    #ifdef _WIN32
        QFont font("SansSerif", 14, QFont::Normal);
    #else
        QFont font("Decorative", 14, QFont::Normal);
    #endif
    painter.setFont(font);
    QFontMetrics fontMetrics(font);
    QString value = QString("%1").arg(number);
    int textWidth = fontMetrics.width(value);
    int textHeight = fontMetrics.height() - 10;
    painter.drawText(QPoint((width - textWidth) / 2, (height + textHeight) / 2),
            value);
}

void Gamestone::dragEnterEvent(QDragEnterEvent *event)
{
    if (!event->mimeData()->hasFormat(GAMESTONE_MIMETYPE))
    {
        event->setDropAction(Qt::IgnoreAction);
        event->ignore();
        return;
    }

    // Check if current stone is being dragged over another stone
    // and if it can be dropped on the stone
    Q_ASSERT(stoneManager->draggedStone != NULL);
    Gamestone* draggingOver = stoneManager->draggedStone;
    if ((draggingOver->isJoker() || !this->acceptDropping(*draggingOver))
            && (!draggingOver->isJoker() || !draggingOver->acceptDropping(*this)))
    {
        event->setDropAction(Qt::IgnoreAction);
        event->ignore();
    }
    else
    {
        event->acceptProposedAction();
    }
}

void Gamestone::dragMoveEvent(QDragMoveEvent *event)
{
    this->moveAddedStones();
}

void Gamestone::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat(GAMESTONE_MIMETYPE)
            && !event->dropAction() == Qt::IgnoreAction)
    {
        // If not moving from heap to holder, check if stone is dragging over another
        Q_ASSERT(stoneManager->draggedStone != NULL);
        Gamestone* dragSource = stoneManager->draggedStone;

        // Do dropping
        // First try to drop depending on "hard" conditions (color, number etc.)
        if (!dragSource->appendStone(*this))
        {
            // No "hard" condition matched, so drop depending on mouse position
            if (this->pos().x() + event->pos().x()
                    > this->pos().x() + this->getWidth() / 2)
            {
                this->setNext(dragSource);
            }
            else
            {
                this->setPrev(dragSource);
            }
        }

        this->moveAddedStones();

        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void Gamestone::enterEvent(QEvent *)
{
    this->setCursor(Qt::OpenHandCursor);
}

void Gamestone::leaveEvent(QEvent *)
{
    this->setCursor(Qt::ArrowCursor);
}

void Gamestone::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        dragStartPosition = event->pos();
    }

    this->setCursor(Qt::ClosedHandCursor);
}

void Gamestone::mouseReleaseEvent(QMouseEvent* event)
{
    // If clicked, move stone from heap to holder
    if (this->stoneParent == HEAP)
    {
        // If the stones can not be moved, the parent can not be changed
        if (!Gamestone::canMoveStones)
        {
            return;
        }

        this->setParent(HOLDER);
    }
}

void Gamestone::mouseMoveEvent(QMouseEvent* event)
{
	if (this->stoneParent == HEAP || !Gamestone::canMoveStones)
	{
		return;
	}
	if (!(event->buttons() & Qt::LeftButton))
	{
		return;
	}
	if ((event->pos() - dragStartPosition).manhattanLength()
			< QApplication::startDragDistance())
	{
		return;
	}

	// Just do anything if we can move the stones. We must be dragging, since qt just sends
	// the events if left curser is pressed
	if (this->stoneParent != HEAP && Gamestone::canMoveStones)
	{
		// If not all stones should be moved, remove previous and next stone
		if (!stoneManager->moveAllStonesInRow)
		{
			this->setPrev(NULL);
			this->setNext(NULL);
		}

		// render the current gamestone to pixmap
		QPixmap *widgetPixmap = new QPixmap(this->size());
		this->render(widgetPixmap);

		// Create drag object and store the tragged stone
		QDrag *drag = new QDrag(this);
		QMimeData *mimeData = new QMimeData;
		mimeData->setData(GAMESTONE_MIMETYPE, NULL);

		drag->setMimeData(mimeData);
		drag->setPixmap(*widgetPixmap);
		drag->setHotSpot(dragStartPosition);

		stoneManager->draggedStone = this;

		// Move actions display that we are dragging over a gamestone,
		// copy actions display that we are dragging over a window.
		Qt::DropAction dropResult = drag->exec(Qt::MoveAction | Qt::CopyAction);

		std::cout << dropResult << std::endl;

		if (dropResult == Qt::CopyAction)
		{
			std::cout << "Dropping on window" << std::endl;
		}
		else if (dropResult == Qt::MoveAction)
		{
			std::cout << "Dropping" << std::endl;

			// If not moving from heap to holder, check if stone is dragging over another
			Gamestone* draggingOver = (Gamestone*) drag->target();

			// If we are dragging over a joker, check from jokers side if can be dropped
			if (((!draggingOver->isJoker() && this->acceptDropping(*draggingOver))
					|| (draggingOver->isJoker()
							&& draggingOver->acceptDropping(*this))))
			{
				// Do dropping
				// First try to drop depending on "hard" conditions (color, number etc.)
				if (!this->appendStone(*draggingOver))
				{
					// No "hard" condition matched, so drop depending on mouse position
					if (this->pos().x() + event->pos().x()
							> draggingOver->pos().x()
									+ draggingOver->getWidth() / 2)
					{
						draggingOver->setNext(this);
					}
					else
					{
						draggingOver->setPrev(this);
					}
				}

				draggingOver->moveAddedStones();
			}

			stoneManager->draggedStone = NULL;

			this->setCursor(Qt::OpenHandCursor);
		}
	}
}
