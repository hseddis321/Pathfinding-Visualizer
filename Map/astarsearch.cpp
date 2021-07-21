#include "astarsearch.h"

AStarSearch::AStarSearch(QPoint start, QPoint end, std::vector<std::vector<int>> map)
{
    this->start = start;
    this->end = end;
    this->map = map;
    foundPath = false;
    q.push(Node(start,0,0));
    visited = std::vector<std::vector<double>>(map.size(), std::vector<double> (map[0].size(), -1));
    backtrace = std::vector<std::vector<QPoint>>(map.size(), std::vector<QPoint> (map[0].size(), QPoint(-1,-1)));
    visited[start.x()][start.y()] = 0;
}

std::tuple<std::vector<QPoint>, std::vector<QPoint>> AStarSearch::step(){
    if(foundPath){
        return {final_path, std::vector<QPoint>()};
    }
    std::vector<QPoint> frontier;
    int number_of_nodes = q.size();
    for(int length = 0; length < 10; length++){
        Node current_node = q.top();
        frontier.push_back(current_node.point);
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
                q.push(Node(cp,getHeuristic(cp),dist_to_neighbor));
                visited[cp.x()][cp.y()] = dist_to_neighbor;
                backtrace[cp.x()][cp.y()] = current_node.point;
            }
        }
    }
    return {std::vector<QPoint>(), frontier};
}

double AStarSearch::getHeuristic(QPoint point){
    double dx = abs(point.x() - end.x());
    double dy = abs(point.y() - end.y());
    //int distance = abs(this->end.x() - point.x()) + abs(this->end.y() - point.y());
    double distance = (dx + dy) + (1.414 - 2) * std::min(dx, dy);
   // double distance = (pow(pow(point.x() - end.x(), 2.0) + pow(point.y() - end.y(), 2.0), 0.5));
    return distance;
}

