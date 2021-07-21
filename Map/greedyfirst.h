#ifndef GREEDYFIRST_H
#define GREEDYFIRST_H
#include "bfssearch.h"

class GreedyFirst : public Search
{
public:
    GreedyFirst(QPoint start, QPoint end, std::vector<std::vector<int>> map);
    std::tuple<std::vector<QPoint>, std::vector<QPoint>> step();
private:
    std::vector<QPoint> final_path;
    std::vector<std::vector<int>> visited;
    void sortNeighborsHeuristic(std::vector<QPoint>& neighbors);
};

#endif // DFSSEARCH_H
