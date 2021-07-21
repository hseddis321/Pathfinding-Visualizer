#ifndef ASTARSEARCH_H
#define ASTARSEARCH_H
#include "bfssearch.h"
#include <QMutex>



class AStarSearch : public Search
{
public:
    AStarSearch(QPoint start, QPoint end, std::vector<std::vector<int>> map);
    std::tuple<std::vector<QPoint>, std::vector<QPoint>> step();
private:


    std::priority_queue<Node> q;
    std::vector<QPoint> final_path;
    std::vector<std::vector<double>> visited;
    std::vector<std::vector<QPoint>> backtrace;
    double getHeuristic(QPoint point);
    QMutex mutex;
};

#endif // ASTARSEARCH_H
