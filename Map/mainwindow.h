#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QTimer>
#include <QButtonGroup>
#include "gridscene.h"
#include "tile.h"
#include <string>
#include <fstream>
#include <streambuf>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
private slots:
    void on_next_button_clicked();

    void on_previous_button_clicked();

    void on_wall_button_clicked();

    void on_start_button_clicked();

    void on_end_button_clicked();

    void search_complete();

    void on_start_search_button_clicked();

    void on_clear_button_clicked();

    void on_generate_button_clicked();
    void updateSearch();


    void on_clear_walls_button_clicked();

private:
    Ui::MainWindow *ui;
    QButtonGroup * buttonGroup;
    GridScene *scene;
    QTimer *search_timer;
    std::map<int, GridScene::SearchType> stateToSearch = {{0,GridScene::SearchType::BFS},
                                                {1,GridScene::SearchType::DFS},
                                                {2,GridScene::SearchType::GreedyF},
                                                {3,GridScene::SearchType::AStar}};

    std::string bfsString = "<h1>Intro</h1>"
"<p>This tool can be used to explore different search algorithms in a static space. In the bottom left you can click on the buttons to either move the start node, the end node or add a wall with a left-click and remove a wall with a right-click. The generate button randomly places walls while the clear walls button removes all walls. The search algorithm can go in all 4 directions and in the diagonal direction at 1.44x the cost if there are no wall tiles between the two points.</p>"
"<h1>BFS/Dijkstra</h1>"
"<p>The first search is called Breadth-First Search (BFS). In this algorithm, we explore the node closest to our starting point. Since we allow diagonal movements at 1.44x the cost of a sideways movement, this is technically Dijkstra's algorithm. While in BFS we would simply use a queue to go through our nodes on the frontier, in Dijkstra's algorithm we need to keep track of how far each node is from the start. Dijkstra's algorithm is guaranteed to find the shortest path between two nodes, but many other states are also explored that need not be. Since we know where the end node is we should be able to optimize this search.</p>";
    std::string dfsString = "<h1>DFS</h1>"
"<p>Depth-First Search (DFS) is another one of the base search algorithms. In DFS the node we have explored is continuously explored until it hits a dead-end or the goal node. In a finite search space such as this, a path is guaranteed. This path may not be the shortest path but finding this path requires a lot less memory then BFS</p>";
    std::string gfString = "<h1>Greedy-First Search</h1>"
"<p>Greedy-First Search takes the concept of DFS and adds a heuristic to it. Instead of arbitrarily picking a new node to explore, it expands the node closest to the end node. This will find a path between the start and the end node, however, it isn't guaranteed to be the shortest. In a sparse map, this will likely find a path very quickly. However, because Greedy-First doesn't account for how far it travels, it can get caught easily and create a significantly longer path.</p>";
    std::string asString = "<h1>A* Search</h1>"
"<p>A* Search combines the best of Dijkstra's Algorithm and Greedy-First Search. A* search expands nodes based on the sum of how far away it is from the start node and how close it is to the end node. It determines the distance to the end node based on the diagonal distance heuristic. This heuristic is the sum of the x and y distance between the current node and the end node minus the distance saved by going diagonally. This search is guaranteed to find the shortest path and takes less space than BFS because it is optimized towards the end node.</p>";
};
#endif // MAINWINDOW_H
