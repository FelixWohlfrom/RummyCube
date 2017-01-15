/*
 * Joker.cpp
 *
 *  Created on: 04.01.2012
 *      Author: Felix Wohlfrom
 */

#include "Joker.h"

#include <QPen>
#include <QPainter>

/****************************************/
/* GENERAL STUFF, PLATTFORM INDEPENDEND */
/****************************************/
// Constructors //
Joker::Joker() :
    Gamestone(), oldColor(UNKNOWN), oldNumber(-1)
{}

Joker::Joker(QWidget* parent, StoneManager* stoneManager, bool isFirst) :
    Gamestone(parent, stoneManager, isFirst, UNKNOWN, -1), jokerIcon(":images/joker"),
    oldColor(UNKNOWN), oldNumber(-1)
{
    // Create transparency mask
    transparentMask = QBitmap(this->getWidth(), this->getHeight());
    QPainter transparencyPainter(&transparentMask);

    int x((this->getWidth() - jokerIcon.width()) / 2);
    int y((this->getHeight() - jokerIcon.height()) / 2);
    transparencyPainter.drawImage(QPoint(x, y), jokerIcon.createHeuristicMask());
}

// Getters and setters //
bool Joker::isJoker() const
{
    return true;
}

Gamestone::StoneColor Joker::getColor()
{
    this->update();
    return color;
}

int Joker::getNumber()
{
    this->update();
    return number;
}

void Joker::update()
{
    this->color = UNKNOWN;
    this->number = -1;

    // If we are left of a 1 or right of a 13, the number can be set
    if (this->next != NULL && !this->next->isJoker() && this->next->getNumber() == 1)
    {
        this->number = 1;
    }
    if (this->prev != NULL && !this->prev->isJoker() && this->prev->getNumber() == 13)
    {
        this->number = 13;
    }

    // The two stones which should be checked for same stone color or number
    Gamestone* firstStone(NULL);
    Gamestone* secondStone(NULL);

    // The difference between first stone and current joker
    int diff(0);

    // First try the stones before
    if (this->prev != NULL)
    {
        firstStone = this->prev;
        diff = -1;

        if (firstStone->isJoker())
        {
            firstStone = firstStone->prev;
            diff = -2;
        }

        if (firstStone != NULL)
        {
            // The second stone can now be either the first non-joker before the first stone ...
            secondStone = firstStone->prev;
            if (secondStone != NULL && secondStone->isJoker())
            {
                secondStone = secondStone->prev;
            }

            // ... or the first non-joker after the current stone
            if (secondStone == NULL)
            {
                secondStone = this->next;
                if (secondStone != NULL && secondStone->isJoker())
                {
                    secondStone = secondStone->next;
                }
            }
        }
    }
    // Now the same stuff with stones after
    else if (this->next != NULL)
    {
        firstStone = this->next;
        diff = 1;
        if (firstStone->isJoker())
        {
            firstStone = firstStone->next;
            diff = 2;
        }

        if (firstStone != NULL)
        {
            // The second stone is now the next non-joker after the first stone
            // Stones that are before current stone are handled in first case
            secondStone = firstStone->next;
            if (secondStone != NULL && secondStone->isJoker())
            {
                secondStone = secondStone->next;
            }
        }
    }
    
    // If we don't have a second stone to check with, the color can not be determined
    if (secondStone == NULL)
    {
        return;
    }

    // If is same color, then is row with same color and we can take that color for the joker
    // and calculate the number from diff
    if (firstStone->isSameColor(secondStone))
    {
        this->color = firstStone->getColor();
        this->number = firstStone->getNumber() - diff;
    }
    // Otherwise row with same number and we can take that number for the joker
    else 
    {
        this->number = firstStone->getNumber();

        // Check if we just have one more color left, so the joker has to have this color
        if (this->countStonesInRow() == 4)
        {
            if (!this->colorIsAlreadyInRow(BLACK))
            {
                this->color = BLACK;
            }
            else if (!this->colorIsAlreadyInRow(BLUE))
            {
                this->color = BLUE;
            }
            else if (!this->colorIsAlreadyInRow(YELLOW))
            {
                this->color = YELLOW;
            }
            else if (!this->colorIsAlreadyInRow(RED))
            {
                this->color = RED;
            }
        }
    }
}

bool Joker::colorIsAlreadyInRow(int color) const
{
    Gamestone* prev(this->prev);
    while (prev != NULL)
    {
        if (prev->color == color)
        {
            return true;
        }
        prev = prev->prev;
    }

    Gamestone* next(this->next);
    while (next != NULL)
    {
        if (next->color == color)
        {
            return true;
        }
        next = next->next;
    }

    return false;
}

void Joker::storeStone()
{
    Gamestone::storeStone();
    this->oldNumber = number;
    this->oldColor = color;
}

void Joker::restoreStone()
{
    Gamestone::restoreStone();
    this->number = oldNumber;
    this->color = oldColor;
}

// Private methods //
bool Joker::acceptDropping(Gamestone& otherStone)
{
    // Never accept if dragged stone or stone dragging over is in middle of row
    // TODO Accept if joker can be replaced by the gamestone
    if ((!this->isFirstInRow() && !this->isLastInRow()) ||
            (!otherStone.isFirstInRow() && !otherStone.isLastInRow()))
    {
        return false;
    }

    // Also don't accept if a first stone is dragged over a first or a last is dragged over a last
    // Just fail, if a row is dragged and not single stones
    if ((this->isFirstInRow() && otherStone.isFirstInRow()
            && !(this->isLastInRow() || otherStone.isLastInRow())
        ) || (
            this->isLastInRow() && otherStone.isLastInRow()
            && !(this->isFirstInRow() || otherStone.isFirstInRow()))
        )
    {
        return false;
    }

    // Accept, if is same color and a row with same color
    if (this->isSameColor(&otherStone) && this->isRowWithSameColor()
            && otherStone.isRowWithSameColor()
            && ((this->isFirstInRow() && otherStone.isLastInRow() && this->isSucc(&otherStone)
        ) || (
            this->isLastInRow() && otherStone.isFirstInRow() && this->isPred(&otherStone)))
        )
    {
        return true;
    }

    // Also accept if is same number and various color and row with same number
    if  (this->isSameNumber(&otherStone) && this->isRowWithSameNumber()
            && otherStone.isRowWithSameNumber()
            // Needed, because next test doesn't fail if a joker is in the middle of a row with
            // already 4 stones
            && this->countStonesInRow() + otherStone.countStonesInRow() <= 4
            && !otherStone.colorIsAlreadyInRow(this))
    {
        return true;
    }

    return false;
}

bool Joker::isSucc(Gamestone* otherStone)
{
    // If the number is defined, we can use default tests
    if (this->getNumber() != -1)
    {
        return Gamestone::isSucc(otherStone);
    }
    
    // Otherwise check if we have a non-joker before current stone
    // No need to check for non-jokers after current stone, because then the stone can
    // not be dropped
    Gamestone* toTestEnd(this->next);
    Gamestone* toTestStart(otherStone);
    int diff(2);

    // If our stone is connected to a joker we need to adjust the difference
    if (toTestEnd != NULL && toTestEnd->isJoker())
    {
        toTestEnd = toTestEnd->next;
        diff = 3;
    }
    // Same if otherStone is a joker
    else if (toTestStart->isJoker())
    {
        toTestStart = toTestStart->prev;
        diff = 3;
    }

    // We are no successor if we are dropped left of a 1 in a row with same color
    return ((otherStone->isFirstInRow() || otherStone->getNumber() < 13)
            && ((toTestStart == NULL) || (toTestEnd == NULL)
                || (toTestEnd->getNumber() == toTestStart->getNumber() + diff)));
}

bool Joker::isPred(Gamestone* otherStone)
{
    // If the number is defined, we can use default tests
    if (this->getNumber() != -1)
    {
        return Gamestone::isPred(otherStone);
    }
    
    // Otherwise check if we have a non-joker after current stone
    // No need to check for non-jokers before current stone, because then the stone can
    // not be dropped
    Gamestone* toTestStart(this->prev);
    Gamestone* toTestEnd(otherStone);
    int diff(2);

    // If our stone is connected to a joker we need to adjust the difference
    if (toTestStart != NULL && toTestStart->isJoker())
    {
        toTestStart = toTestStart->prev;
        diff = 3;
    }
    // Same if otherStone is a joker
    else if (toTestEnd->isJoker())
    {
        toTestEnd = toTestEnd->next;
        diff = 3;
    }

    // We are no predessor if we are dropped left of a 1 in a row with same color
    return ((otherStone->isLastInRow() || otherStone->getNumber() > 1)
            && ((toTestStart == NULL) || (toTestEnd == NULL)
                || (toTestStart->getNumber() == toTestEnd->getNumber() - diff)));
}

bool Joker::isSameNumber(Gamestone* otherStone)
{
    // If the number is defined, we can use default tests
    if (this->getNumber() != -1)
    {
        return Gamestone::isSameNumber(otherStone);
    }
    
    // Otherwise check if we have a non-joker after current stone
    Gamestone* toTest(this->next);

    if (toTest != NULL && toTest->isJoker())
    {
        toTest = toTest->next;
    }

    // If not, check if we have a non-joker before current stone
    if (toTest == NULL)
    {
        toTest = this->prev;
    }

    if (toTest != NULL && toTest->isJoker())
    {
        toTest = toTest->prev;
    }

    return (toTest == NULL || toTest->isSameNumber(otherStone));
}

bool Joker::isSameColor(Gamestone* otherStone)
{
    // If the color is defined, we can use default tests
    if (this->getColor() != UNKNOWN)
    {
        return Gamestone::isSameColor(otherStone);
    }
    
    // Otherwise check if we have a non-joker after current stone
    Gamestone* toTest(this->next);

    if (toTest != NULL && toTest->isJoker())
    {
        toTest = toTest->next;
    }

    // If not, check if we have a non-joker before current stone
    if (toTest == NULL)
    {
        toTest = this->prev;
    }

    if (toTest != NULL && toTest->isJoker())
    {
        toTest = toTest->prev;
    }

    return (toTest == NULL || toTest->isSameColor(otherStone));
}

bool Joker::isRowWithSameNumber()
{
    // If we have a defined number we can do the normal check
    if (this->getNumber() != -1)
    {
        return Gamestone::isRowWithSameNumber();
    }

    // If the number is undefined, we can assume that is row with same number
    return true;
}

bool Joker::isRowWithSameColor()
{
    // If we have a defined color we can do the normal check
    if (this->getColor() != UNKNOWN)
    {
        return Gamestone::isRowWithSameColor();
    }

    // Check, if we have two stones next to us and if they have the same color
    if (this->getNext() != NULL && this->getNext()->getNext() != NULL)
    {
        return this->getNext()->isSameColor(this->getNext()->getNext());
    }

    // otherwise assume that is row with same color
    return true;
}

/*************/
/* Operators */
/*************/
QXmlStreamReader &operator>>(QXmlStreamReader &input, Joker* stone)
{
    // Restore joker specific settings
    int oldColor = input.attributes().value("oldColor").toInt();
    int oldNumber = input.attributes().value("oldNumber").toInt();
    stone->oldColor = (Gamestone::StoneColor)oldColor;
    stone->oldNumber = oldNumber;

    if (!input.readNextStartElement() || input.name() != "stone")
    {
        if (!input.hasError())
        {
            input.raiseError(QObject::tr("Expected 'stone' Tag, "
                    "instead found '%1'.").arg(input.name().toString()));
        }
        return input;
    }

    // Restore general gamestone settings
    input >> (Gamestone*)stone;

    input.skipCurrentElement(); // Finish handling of "stone" element

    stone->update();

    return input;
}

QXmlStreamWriter &operator<<(QXmlStreamWriter &output, Joker* stone)
{
    output.writeStartElement("joker");
    // Store joker specific settings
    output.writeAttribute("oldColor", QString::number(stone->oldColor));
    output.writeAttribute("oldNumber", QString::number(stone->oldNumber));
    output.writeAttribute("val", QString::number(stone->asInt()));

    // Store general gamestone settings
    output << (Gamestone*)stone;

    output.writeEndElement();

    return output;
}

#ifdef _DEBUG
std::ostream &operator<<(std::ostream &stream, Joker* stone)
{
    stream << (Gamestone*)stone << " (Joker)";

    return stream;
}
#endif

/******************************************/
/*    HERE COMES THE QT SPECIFIC STUFF    */
/******************************************/
void Joker::paintEvent(QPaintEvent *)
{
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

    // If the stone is still on heap, don't draw joker image
    if (stoneParent == HEAP)
    {
        return;
    }

    // otherwise draw joker image
    int x((this->getWidth() - jokerIcon.width()) / 2);
    int y((this->getHeight() - jokerIcon.height()) / 2);
    painter.setClipRegion(QRegion(transparentMask));
    painter.drawImage(QPoint(x, y), jokerIcon);
}

#ifdef _DEBUG
void Joker::enterEvent(QEvent *)
{
    this->update();
    const QString colorNames[] = { "Unknown", "Black", "Blue", "Yellow", "Red" };
    QString out = QString("%1 (%2) %3").arg(QString::number(this->number),
            colorNames[this->color + 1], QString::number(this->first));
    this->setToolTip(out);
}
#endif
