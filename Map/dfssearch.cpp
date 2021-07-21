#include "dfssearch.h"

DFSSearch::DFSSearch(QPoint start, QPoint end, std::vector<std::vector<int>> map)
{
    this->start = start;
    this->end = end;
    this->map = map;
    visited = std::vector<std::vector<int>>(map.size(), std::vector<int> (map[0].size(), 0));
    visited[start.x()][start.y()] = 1;
    final_path.push_back(start);
}

std::tuple<std::vector<QPoint>, std::vector<QPoint>> DFSSearch::step(){
    if(foundPath){
        return {final_path, std::vector<QPoint>()};
    }


    while(final_path.size() > 0){
        QPoint curr_point = final_path.back();

        std::vector<QPoint> neighbors = getNeighborsWithoutWeights(curr_point);
        for(int i = 0; i < neighbors.size(); i++){
            if(visited[neighbors[i].x()][neighbors[i].y()] != 1){
                final_path.push_back(neighbors[i]);
                visited[neighbors[i].x()][neighbors[i].y()] = 1;
                if(neighbors[i] == end){
                    foundPath = true;
                    return {final_path, std::vector<QPoint>()};
                }
                return {std::vector<QPoint>(), std::vector<QPoint>(1, neighbors[i])};
            }
        }
        final_path.pop_back();
    }
    return {std::vector<QPoint>(), std::vector<QPoint>()};

}
