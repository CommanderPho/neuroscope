/***************************************************************************
                          itemiconview.h  -  description
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

#ifndef ITEMICONVIEW_H
#define ITEMICONVIEW_H

//QT include files
#include <qwidget.h>
#include <QListWidget>
#include <qwidget.h>
//Added by qt3to4:
#include <QMouseEvent>
#include <QWheelEvent>


/**Utilitary class used to build the cluster and event palettes.
  *@author Lynn Hazan
  */

class ItemIconView : public QListWidget {
    Q_OBJECT
public:
    explicit ItemIconView(const QColor &backgroundColor, ViewMode mode, int gridX, int gridY, QWidget* parent = 0, const QString& name = QString());
    ~ItemIconView(){}

    enum IndexItem { INDEXICON = Qt::UserRole + 1 };

protected:
    void mousePressEvent ( QMouseEvent * event );
    void wheelEvent ( QWheelEvent * e );
    void mouseReleaseEvent ( QMouseEvent * event );

Q_SIGNALS:
    void mousePressWoModificators(QString sourceGroup);
    void mousePressMiddleButton(const QString& sourceGroup,QListWidgetItem*);
    void mousePressWAltButton(QString sourceGroup,int index);
    void mouseReleased(QString sourceGroup);
    void mousePressMiddleButton(QListWidgetItem*);

};

#endif
