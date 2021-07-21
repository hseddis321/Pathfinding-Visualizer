#ifndef BFSSEARCH_H
#define BFSSEARCH_H
#include <vector>
#include <QPoint>
#include <queue>
#include <iostream>
class Search{
    public:
        virtual std::tuple<std::vector<QPoint>, std::vector<QPoint>> step() = 0;
    protected:
        std::vector<std::pair<QPoint, double>> getNeighbors(QPoint curr_point);
        std::vector<QPoint> getNeighborsWithoutWeights(QPoint curr_point);
        QPoint start;
        QPoint end;
        std::vector<std::vector<int>> map;
        bool foundPath;
};
class Node{
    public:
        Node(QPoint point, double heuristic, double distance_traveled){
            this->point = point;
            this->heuristic= heuristic;
            this->distance_traveled = distance_traveled;
        }
        double heuristic;
        double distance_traveled;
        QPoint point;
};

static inline bool operator <(const Node& d, const Node& d2) {
    return d.heuristic +d.distance_traveled > d2.heuristic + d2.distance_traveled;
}
class BFSSearch : public Search
{
public:
    BFSSearch(QPoint start, QPoint end, std::vector<std::vector<int>> map);
    std::tuple<std::vector<QPoint>, std::vector<QPoint>> step();
private:
    std::priority_queue<Node> q;
    std::vector<QPoint> final_path;
    std::vector<std::vector<double>> visited;
    std::vector<std::vector<QPoint>> backtrace;
};

#endif // BFSSEARCH_H
