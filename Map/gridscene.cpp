#include "gridscene.h"

GridScene::GridScene(int x_size, int y_size, QObject *parent)
    : QGraphicsScene(parent)
{
    grid = std::vector<std::vector<int>>(x_size/TILE_SIZE, std::vector<int> (y_size/TILE_SIZE, 0));
    tileGrid = std::vector<std::vector<Tile*>>(x_size/TILE_SIZE, std::vector<Tile*> (y_size/TILE_SIZE, 0));
    endPoint = QPoint(x_size/TILE_SIZE - 1,y_size/TILE_SIZE - 1);
    startPoint = QPoint(0,0);
    Tile* startTile = new Tile(Tile::Start, TILE_SIZE);
    startTile->setBrush(Qt::green);
    addItem(startTile);
    startTile->setPos(startPoint.x()*TILE_SIZE,startPoint.y()*TILE_SIZE);
    tileGrid[startPoint.x()][startPoint.y()] = startTile;
    grid[startPoint.x()][startPoint.y()] = 2;

    Tile* endTile = new Tile(Tile::Start, TILE_SIZE);
    endTile->setBrush(Qt::red);
    addItem(endTile);
    endTile->setPos(endPoint.x()*TILE_SIZE,endPoint.y()*TILE_SIZE);
    tileGrid[endPoint.x()][endPoint.y()] = endTile;
    grid[endPoint.x()][endPoint.y()] = 3;

    isSearching = false;

}

void GridScene::setTileType(Tile::TileType tileType){
    mutex.lock();
    this->tileType = tileType;
    mutex.unlock();
}
void GridScene::clearSearchHelper(){
    for (size_t i = 0; i < totalSearchNodes.size(); i++){
        Tile* tile_pt = totalSearchNodes[i];
        removeItem(tile_pt);
        free(tile_pt);
    }
    for (size_t i = 0; i < lastExploredStep.size(); i++){
        Tile* tile_pt = lastExploredStep[i];
        removeItem(tile_pt);
        free(tile_pt);
    }
    for (size_t i = 0; i < tilePath.size(); i++){
        Tile* tile_pt = tilePath[i];
        removeItem(tile_pt);
        free(tile_pt);
    }
    totalSearchNodes.clear();
    lastExploredStep.clear();
    tilePath.clear();

}

void GridScene::clearSearch(){
    mutex.lock();
    isSearching = false;
    clearSearchHelper();
    mutex.unlock();
}
void GridScene::startSearch(SearchType searchType){
    mutex.lock();
    statesExplored = 0;
    clearSearchHelper();
    switch(searchType){
        case BFS:
            currentSearch = new BFSSearch(startPoint, endPoint, grid);
            break;
        case DFS:
            currentSearch = new DFSSearch(startPoint, endPoint, grid);
            break;
        case GreedyF:
            currentSearch = new GreedyFirst(startPoint, endPoint, grid);
            break;
        case AStar:
            currentSearch = new AStarSearch(startPoint, endPoint, grid);
            break;
        default:
            break;
    }
    isSearching = true;
    mutex.unlock();
}

int GridScene::updateSearch(){
    mutex.lock();
    auto[path, frontier] = currentSearch->step();
    statesExplored += frontier.size();
    for(size_t i=0; i < lastExploredStep.size(); i++){
        lastExploredStep[i]->setBrush(Qt::blue);
        totalSearchNodes.push_back(lastExploredStep[i]);
    }
    lastExploredStep.clear();

    if(path.size() == 0){
        //Potential Problem: creating tile that may overlap with another tile. This could lead to performance issues
        //if the frontier rechecks many nodes that it has already explored
        for(size_t i = 0; i < frontier.size(); i++){
            Tile* tile = new Tile(Tile::Path, TILE_SIZE);
            tile->setBrush(Qt::yellow);
            addItem(tile);
            tile->setPos(frontier[i].x()*TILE_SIZE,frontier[i].y()*TILE_SIZE);
            lastExploredStep.push_back(tile);
        }
    } else {
        isSearching = false;
        for(size_t i = 0; i < path.size(); i++){
            Tile* tile = new Tile(Tile::Path, TILE_SIZE);
            tile->setBrush(Qt::darkGreen);
            addItem(tile);
            tile->setPos(path[i].x()*TILE_SIZE,path[i].y()*TILE_SIZE);
            tilePath.push_back(tile);
        }
        emit search_complete();
    }
    mutex.unlock();
    return statesExplored;
}
void GridScene::clearWalls(){
    mutex.lock();
    if(isSearching){
        mutex.unlock();
        return;
    }
    for(int grid_x = 0; grid_x < grid.size(); grid_x++){
        for(int grid_y = 0; grid_y < grid[0].size(); grid_y++){
            if(grid[grid_x][grid_y] == 1){
                grid[grid_x][grid_y] = 0;
                Tile* tile_pt = tileGrid[grid_x][grid_y];
                tileGrid[grid_x][grid_y] = nullptr;
                removeItem(tile_pt);
                free(tile_pt);
            }
        }
    }
    mutex.unlock();
}
void GridScene::generateRandom(double chance){
    mutex.lock();
    if(isSearching){
        mutex.unlock();
        return;
    }
    for(int x = 0; x < grid.size(); x++){
        for(int y = 0; y < grid[0].size(); y++){
            double roll =  (double) rand()/RAND_MAX;
            if(grid[x][y] != 2 && grid[x][y] != 3){
                if(roll < chance && grid[x][y] == 0){
                    Tile* tile = new Tile(tileType, TILE_SIZE);
                    tile->setBrush(Qt::black);
                    addItem(tile);
                    tile->setPos(x*TILE_SIZE,y*TILE_SIZE);
                    grid[x][y] = 1;
                    tileGrid[x][y] = tile;
                }
                else if (roll >= chance && grid[x][y] == 1){
                    grid[x][y] = 0;
                    Tile* tile_pt = tileGrid[x][y];
                    tileGrid[x][y] = nullptr;
                    removeItem(tile_pt);
                    free(tile_pt);
                }

            }
        }
    }
    mutex.unlock();
}

void GridScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{

    mutex.lock();
    int grid_x = int(mouseEvent->scenePos().x()/TILE_SIZE);
    int grid_y = int(mouseEvent->scenePos().y()/TILE_SIZE);
    if(grid_x >= grid.size() || grid_y >= grid[0].size() || isSearching){
        mutex.unlock();
        return;
    }
    if(tileType == Tile::Wall){
        if(grid[grid_x][grid_y] == 0 && mouseEvent->button() == Qt::LeftButton){
            Tile* tile = new Tile(tileType, TILE_SIZE);
            tile->setBrush(Qt::black);
            addItem(tile);
            tile->setPos(grid_x*TILE_SIZE,grid_y*TILE_SIZE);
            grid[grid_x][grid_y] = 1;
            tileGrid[grid_x][grid_y] = tile;
        }
        else if (grid[grid_x][grid_y] == 1 && mouseEvent->button() == Qt::RightButton){
            grid[grid_x][grid_y] = 0;
            Tile* tile_pt = tileGrid[grid_x][grid_y];
            tileGrid[grid_x][grid_y] = nullptr;
            removeItem(tile_pt);
            free(tile_pt);
        }
    } else if(tileType == Tile::Start and mouseEvent->button() == Qt::LeftButton){
        if(grid[grid_x][grid_y] == 0){
            Tile* startTile = tileGrid[startPoint.x()][startPoint.y()];
            startTile->setPos(grid_x*TILE_SIZE,grid_y*TILE_SIZE);

            tileGrid[grid_x][grid_y] = startTile;
            grid[grid_x][grid_y] = 2;

            tileGrid[startPoint.x()][startPoint.y()] = 0;
            grid[startPoint.x()][startPoint.y()] = 0;

            startPoint.setX(grid_x);
            startPoint.setY(grid_y);
        }
    } else if (mouseEvent->button() == Qt::LeftButton){
        if(grid[grid_x][grid_y] == 0){
            Tile* endTile = tileGrid[endPoint.x()][endPoint.y()];
            endTile->setPos(grid_x*TILE_SIZE,grid_y*TILE_SIZE);
            tileGrid[grid_x][grid_y] = endTile;
            grid[grid_x][grid_y] = 3;

            tileGrid[endPoint.x()][endPoint.y()] = 0;
            grid[endPoint.x()][endPoint.y()] = 0;

            endPoint.setX(grid_x);
            endPoint.setY(grid_y);
        }
    }
    mutex.unlock();
}
