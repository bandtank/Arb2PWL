/*=========================================================================
 *
 *  Copyright Anthony Andriano, David Doria 2012
 *  anthonyandriano@gmail.com daviddoria@gmail.com
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

#include "ClickableScene.h"
#include <QGraphicsSceneMouseEvent>
#include <QMouseEvent>
#include <iostream>

ClickableScene::ClickableScene(QWidget *parent) : QGraphicsScene()
{
    this->MouseIsDown = false;
    this->NextValidX.setX(0); //y unsued for now
    this->Drawing = false;
}

void ClickableScene::mousePressEvent(QGraphicsSceneMouseEvent* pMouseEvent)
{
    if(this->NextValidX.x() < pMouseEvent->scenePos().x())
    {
        //If the x location is valid, we are drawing
        this->Drawing = true;

        //Set next valid x location as the column to the right
        this->NextValidX.setX(pMouseEvent->scenePos().x() + 1);

        //Set the mouse down flag
        this->MouseIsDown = true;

        //Create a dot at this point
        QRectF Dot(pMouseEvent->scenePos().x(),pMouseEvent->scenePos().y(),0,0);
        addRect(Dot);

        //Create the first point in a line
        this->LastClick.setX(pMouseEvent->scenePos().x());
        this->LastClick.setY(pMouseEvent->scenePos().y());
    }
    else
        this->Drawing = false;
}

void ClickableScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* pMouseEvent)
{
    this->MouseIsDown = false;

    //Set next valid x location as the column to the right
    if(this->Drawing == true)
    {
        this->NextValidX.setX(pMouseEvent->scenePos().x() + 1);
        this->Drawing = false;
    }
}

void ClickableScene::mouseMoveEvent(QGraphicsSceneMouseEvent* pMouseEvent)
{
    if(this->MouseIsDown == true && this->Drawing == true)
    {
        qreal x;
        if(pMouseEvent->scenePos().x() < this->LastClick.x())
            x = this->LastClick.x();
        else
            x = pMouseEvent->scenePos().x();


        qreal y = pMouseEvent->scenePos().y();

        QLineF Line(this->LastClick.x(),this->LastClick.y(),x,y);
        addLine(Line);

        this->LastClick.setX(x);
        this->LastClick.setY(y);
    }
}
