#include "bfssearch.h"

std::vector<std::pair<QPoint, double>> Search::getNeighbors(QPoint curr_point){
    std::vector<std::pair<QPoint, double>> neighbors;
    //Check if top right point is on map and if it isn't a wall
    if(curr_point.x() + 1 < map.size() && curr_point.y() > 0 && (map[curr_point.x()+1][curr_point.y()-1] != 1
                                                                 && (map[curr_point.x()+1][curr_point.y()] != 1 && map[curr_point.x()][curr_point.y()-1] != 1))){
        neighbors.push_back(std::make_pair(QPoint(curr_point.x()+1, curr_point.y()-1),1.414));
    }
    //Check if right point is on map and if it isn't a wall
    if(curr_point.x() + 1 < map.size() && map[curr_point.x()+1][curr_point.y()] != 1){
        neighbors.push_back(std::make_pair(QPoint(curr_point.x()+1, curr_point.y()),1.0));
    }
    //Check if bottom right point is on map and if it isn't a wall
    if(curr_point.x() + 1 < map.size() && curr_point.y() + 1 < map[0].size() && (map[curr_point.x()+1][curr_point.y()+1] != 1
                                                                                 && (map[curr_point.x()+1][curr_point.y()] != 1 && map[curr_point.x()][curr_point.y()+1] != 1))){
        neighbors.push_back(std::make_pair(QPoint(curr_point.x()+1, curr_point.y()+1),1.414));
    }
    //Check if lower point is on map and if it isn't a wall
    if(curr_point.y() + 1 < map[0].size() && map[curr_point.x()][curr_point.y()+1] != 1){
        neighbors.push_back(std::make_pair(QPoint(curr_point.x(), curr_point.y()+1), 1.0));
    }
     //Check if bottom left point is on map and if it isn't a wall
    if(curr_point.x() > 0 && curr_point.y() + 1 < map[0].size() && (map[curr_point.x()-1][curr_point.y()+1] != 1
                                                                    && (map[curr_point.x()-1][curr_point.y()] != 1 && map[curr_point.x()][curr_point.y()+1] != 1))){
        neighbors.push_back(std::make_pair(QPoint(curr_point.x()-1, curr_point.y()+1),1.414));
    }
    //Check if left point is on map and if it isn't a wall
    if(curr_point.x() > 0 && map[curr_point.x()-1][curr_point.y()] != 1){
        neighbors.push_back(std::make_pair(QPoint(curr_point.x()-1, curr_point.y()), 1.0));

    }
    //Check if top left point is on map and if it isn't a wall
   if(curr_point.x() > 0 && curr_point.y() > 0 && (map[curr_point.x()-1][curr_point.y()-1] != 1
                                                   && (map[curr_point.x()-1][curr_point.y()] != 1 && map[curr_point.x()][curr_point.y()-1] != 1))){
       neighbors.push_back(std::make_pair(QPoint(curr_point.x()-1, curr_point.y()-1),1.414));
   }

    //Check if above point is on map and if it isn't a wall
    if(curr_point.y() > 0 && map[curr_point.x()][curr_point.y()-1] != 1){
        neighbors.push_back(std::make_pair(QPoint(curr_point.x(), curr_point.y()-1), 1.0));
    }
    return neighbors;
}
std::vector<QPoint> Search::getNeighborsWithoutWeights(QPoint curr_point){
    std::vector<std::pair<QPoint, double>> neighbors = getNeighbors(curr_point);
    std::vector<QPoint> ret_neighbors;
    for(int i = 0; i < neighbors.size(); i++){
        ret_neighbors.push_back(neighbors[i].first);
    }
    return ret_neighbors;
}

BFSSearch::BFSSearch(QPoint start, QPoint end, std::vector<std::vector<int>> map)
{
    this->start = start;
    this->end = end;
    this->map = map;
    foundPath = false;
    q.push(Node(start,0,0));
    visited = std::vector<std::vector<double>>(map.size(), std::vector<double> (map[0].size(), -1));
    backtrace = std::vector<std::vector<QPoint>>(map.size(), std::vector<QPoint> (map[0].size(), QPoint(-1,-1)));
    visited[start.x()][start.y()] = 1;
}


std::tuple<std::vector<QPoint>, std::vector<QPoint>> BFSSearch::step(){
    if(foundPath){
        return {final_path, std::vector<QPoint>()};
    }
    std::vector<QPoint> frontier;
    int number_of_nodes = q.size();
    for(int length = 0; length < number_of_nodes; length++){
        Node current_node = q.top();
        if(current_node.point == end){
            foundPath = true;
            QPoint backtrace_point = end;
            while(backtrace_point != start){
                final_path.push_back(backtrace_point);
                 backtrace_point = backtrace[backtrace_point.x()][backtrace_point.y()];
            }

            return {final_path, frontier};
        }
        q.pop();
        std::vector<std::pair<QPoint,double>> neighbors = getNeighbors(current_node.point);
        for (size_t i = 0; i < neighbors.size(); i++){
            QPoint cp = neighbors[i].first;
            double dist_to_neighbor =  current_node.distance_traveled + neighbors[i].second;
            if(visited[cp.x()][cp.y()] == -1 || visited[cp.x()][cp.y()] > dist_to_neighbor){
                frontier.push_back(cp);
                q.push(Node(cp,0,dist_to_neighbor));
                visited[cp.x()][cp.y()] = dist_to_neighbor;
                backtrace[cp.x()][cp.y()] = current_node.point;
            }
        }
    }
    return {std::vector<QPoint>(), frontier};
}
