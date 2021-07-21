#ifndef DFSSEARCH_H
#define DFSSEARCH_H
#include "bfssearch.h"

class DFSSearch : public Search
{
public:
    DFSSearch(QPoint start, QPoint end, std::vector<std::vector<int>> map);
    std::tuple<std::vector<QPoint>, std::vector<QPoint>> step();
private:
    std::vector<QPoint> final_path;
    std::vector<std::vector<int>> visited;
};

#endif // DFSSEARCH_H
