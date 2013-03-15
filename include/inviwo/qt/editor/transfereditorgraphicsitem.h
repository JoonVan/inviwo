#ifndef IVW_TRANSFEREDITORGRAPHICSITEM_H
#define IVW_TRANSFEREDITORGRAPHICSITEM_H

#include <inviwo/qt/editor/inviwoqteditordefine.h>
#include <QGraphicsLineItem>
#include <QGraphicsScene>
#include <QPainterPath>

#include <inviwo/core/ports/port.h>
#include <inviwo/qt/editor/processorgraphicsitem.h>

namespace inviwo {

    class IVW_QTEDITOR_API TransferEditorGraphicsItem : public QGraphicsItem {

    public:
        TransferEditorGraphicsItem(QPointF startPoint);
        TransferEditorGraphicsItem(float x, float y);
        TransferEditorGraphicsItem();
        ~TransferEditorGraphicsItem();

        //override for qgraphicsitem_cast (refer qt documentation)
        enum { Type = UserType + ConnectionGraphicsType };
        int type() const  {return Type; }

        /**
        * Overloaded paint method from QGraphicsItem. Here the actual representation is drawn.
        */
        virtual void paint(QPainter* p, const QStyleOptionGraphicsItem* options, QWidget* widget);
        QRectF boundingRect() const;

        int getId();
        void setId(int id);
        QPointF position();


    protected:
        void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
        void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

    private:
        static const std::string logSource_;
        int id;
    };

}// namespace
#endif // IVW_TRANSFEREDITORGRAPHICSITEM_H