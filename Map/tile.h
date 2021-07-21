#ifndef TILE_H
#define TILE_H



#include <QGraphicsPolygonItem>
#include <QGraphicsSceneMouseEvent>

class Tile : public QGraphicsPolygonItem
{

public:
    enum TileType {Wall, Start, End, Path};
    Tile( TileType tileType,int tileSize, QGraphicsItem *parent = nullptr);

private:
    TileType tileType;
    QPolygonF myPolygon;

};
#endif // TILE_H
