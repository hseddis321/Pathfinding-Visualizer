#ifndef GRIDSCENE_H
#define GRIDSCENE_H

#include <QObject>
#include <QMutex>
#include <QPoint>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <vector>
#include "tile.h"
#include "bfssearch.h"
#include "dfssearch.h"
#include "greedyfirst.h"
#include "astarsearch.h"

class GridScene : public QGraphicsScene
{
    Q_OBJECT

public:

    GridScene(int x_size, int y_size, QObject *parent = nullptr);

    enum SearchType {AStar, BFS, DFS, GreedyF};
public slots:
    void setTileType(Tile::TileType tileType);
    void startSearch(SearchType searchType);
    int updateSearch();
    void clearSearch();
    void clearWalls();
    void generateRandom(double chance);
signals:
    void search_complete();

protected:

    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    //void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    //void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) override;

private:
    Tile::TileType  tileType = Tile::Wall;
    std::vector<std::vector<int>> grid;
    std::vector<std::vector<Tile*>> tileGrid;
    static const int TILE_SIZE = 20;
    QPoint startPoint;
    QPoint endPoint;

    void clearSearchHelper();


    Search* currentSearch;
    std::vector<Tile*> lastExploredStep;
    std::vector<Tile*> totalSearchNodes;
    std::vector<Tile*> tilePath;
    bool isSearching;
    int statesExplored;
    QMutex mutex;
};

#endif // GRIDSCENE_H
