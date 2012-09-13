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

#ifndef ClickableScene_H
#define ClickableScene_H

#include <QGraphicsScene>
#include <QMouseEvent>

class ClickableScene : public QGraphicsScene
{
    Q_OBJECT

public:
    ClickableScene(QWidget *parent = 0);
  
    void mousePressEvent(QGraphicsSceneMouseEvent* pMouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* pMouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* pMouseEvent);

signals:
    void AddedLine(const QLineF& Line);
  
protected:
    bool MouseIsDown;
    bool Drawing;
    QPointF LastClick;
    QPointF NextValidX;
};

#endif
