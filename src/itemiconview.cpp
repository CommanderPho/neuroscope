/***************************************************************************
                          itemiconview.cpp  -  description
                             -------------------
    begin                : Fri Mar 5 2004
    copyright            : (C) 2004 by Lynn Hazan
    email                : lynn.hazan.myrealbox.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
// application specific includes
#include "itemiconview.h"





// include files for Qt
#include <QCursor>
#include <QTextCodec>
//Added by qt3to4:
#include <QMouseEvent>
#include <QFrame>


ItemIconView::ItemIconView(const QColor& backgroundColor,QListView::ViewMode mode,int gridX,int gridY,QWidget* parent, const QString& name):
    QListWidget(parent)
{
    setObjectName(name);

    QFont font( "Helvetica",8);
    setFont(font);
    setSpacing(4);
    setFrameStyle(QFrame::NoFrame);
    //setArrangement(LeftToRight);
    setResizeMode(QListWidget::Adjust);
    setViewMode(mode);
    setGridSize(QSize(gridX,gridY));
    //arrangeItemsInGrid();
    setWordWrap(false);
    //setDragAutoScroll(false);
    setAutoFillBackground(true);
    //Set the iconView color, the foreground color depends on the background color
    QPalette palette;
    palette.setColor(backgroundRole(), backgroundColor);
    int h;
    int s;
    int v;
    backgroundColor.getHsv(&h,&s,&v);
    QColor legendColor;
    if(s <= 80 && v >= 240 || (s <= 40 && v >= 220))
        legendColor = Qt::black;
    else
        legendColor = Qt::white;
    palette.setColor(foregroundRole(), legendColor); 
    setPalette(palette);
    setSelectionMode(QAbstractItemView::ExtendedSelection);

    setMovement(QListView::Static);

    setSpacing(4);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setFrameStyle(QFrame::Box | QFrame::Plain);
    setLineWidth(1);
}

void ItemIconView::wheelEvent ( QWheelEvent * event )
{
    event->accept();
}

void ItemIconView::mousePressEvent ( QMouseEvent * event )
{
    QListWidgetItem *item = itemAt(event->pos());
    if(!item)
        return;
    if(event->button() == Qt::LeftButton && (event->modifiers() & Qt::AltModifier) && (event->modifiers() & Qt::ControlModifier)){
        emit mousePressWAltButton(this->objectName(),item->data(INDEXICON).toInt());
    } else if(event->button() == Qt::MiddleButton) {
        emit mousePressMiddleButton(this->objectName(),item);
        emit mousePressMiddleButton(item);
    }
    QListWidget::mousePressEvent(event);
}

void ItemIconView::mouseReleaseEvent ( QMouseEvent * event ) {
    QListWidget::mouseReleaseEvent(event);
    emit mouseReleased(this->objectName());
}


#include "itemiconview.moc"
