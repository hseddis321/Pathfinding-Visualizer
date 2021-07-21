#include "tile.h"
#include <QPainter>
#include <QTimer>

Tile::Tile(TileType tileType, int tileSize,
           QGraphicsItem *parent)
: QGraphicsPolygonItem(parent)
{
    this->tileType = tileType;
    myPolygon << QPointF(0,0) << QPointF(tileSize,0)
                         << QPointF(tileSize, tileSize) << QPointF(0, tileSize)
                         << QPointF(0,0);
    setPolygon(myPolygon);
}
