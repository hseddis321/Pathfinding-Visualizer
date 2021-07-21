#include "mainwindow.h"
#include "ui_mainwindow.h"




static const int MAX_STATES = 4;
int state = 0;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->text_out->setReadOnly(true);

    int x_size = 900;
    int y_size = 540;


    scene = new GridScene(x_size, y_size, this);

    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);

    scene->setSceneRect(0, 0, x_size, y_size);

    QLineF topLine(scene->sceneRect().topLeft(),
                   scene->sceneRect().topRight());
    QLineF leftLine(scene->sceneRect().topLeft(),
                   scene->sceneRect().bottomLeft());
    QLineF rightLine(scene->sceneRect().topRight(),
                   scene->sceneRect().bottomRight());
    QLineF bottomLine(scene->sceneRect().bottomLeft(),
                   scene->sceneRect().bottomRight());

    QPen myPen = QPen(Qt::red);

    scene->addLine(topLine, myPen);
    scene->addLine(leftLine, myPen);
    scene->addLine(rightLine, myPen);
    scene->addLine(bottomLine, myPen);
    connect(scene, &GridScene::search_complete, this, &MainWindow::search_complete);
    buttonGroup = new QButtonGroup;
    buttonGroup->addButton(ui->start_button);
    buttonGroup->addButton(ui->end_button);
    buttonGroup->addButton(ui->wall_button);

    search_timer = nullptr;

    QTextEdit *txt = ui->text_out;
    txt->setHtml(&bfsString[0]);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_next_button_clicked()
{
    if(state < MAX_STATES - 1){
        state++;
        QTextEdit *txt = ui->text_out;
        GridScene::SearchType searchType = stateToSearch[state];
        switch(searchType){
            case GridScene::BFS:
                txt->setHtml(&bfsString[0]);
                break;
            case GridScene::DFS:
                txt->setHtml(&dfsString[0]);
                break;
            case GridScene::GreedyF:
                txt->setHtml(&gfString[0]);
                break;
            case GridScene::AStar:
                txt->setHtml(&asString[0]);
                break;
            default:
                break;
        }
    }
}


void MainWindow::on_previous_button_clicked()
{
    if(state > 0){
        state--;
        QTextEdit *txt = ui->text_out;
        GridScene::SearchType searchType = stateToSearch[state];
        switch(searchType){
            case GridScene::BFS:
                txt->setHtml(&bfsString[0]);
                break;
            case GridScene::DFS:
                txt->setHtml(&dfsString[0]);
                break;
            case GridScene::GreedyF:
                txt->setHtml(&gfString[0]);
                break;
            case GridScene::AStar:
                txt->setHtml(&asString[0]);
                break;
            default:
                break;
        }
    }
}



void MainWindow::on_wall_button_clicked()
{
    scene->setTileType(Tile::Wall);
}


void MainWindow::on_start_button_clicked()
{
    scene->setTileType(Tile::Start);
}


void MainWindow::on_end_button_clicked()
{
    scene->setTileType(Tile::End);
}

void MainWindow::search_complete(){
    search_timer->stop();
}

void MainWindow::on_start_search_button_clicked()
{
    if(search_timer != nullptr and search_timer->isActive()){
        search_timer->stop();
    }
    scene->startSearch(stateToSearch[state]);

    search_timer = new QTimer(this);
    connect(search_timer, &QTimer::timeout, this, &MainWindow::updateSearch);
    search_timer->start(50);
}
void MainWindow::updateSearch(){
    int noExploredStates = scene->updateSearch();
    QLabel *label = ui->states_explored;
    char buff[100];
    snprintf(buff, sizeof(buff), "States Explored: %d", noExploredStates);
    label->setText(buff);
}

void MainWindow::on_clear_button_clicked()
{
    scene->clearSearch();
    if(search_timer != nullptr and search_timer->isActive()){
        search_timer->stop();
    }
}


void MainWindow::on_generate_button_clicked()
{
    scene->generateRandom(0.25);
}


void MainWindow::on_clear_walls_button_clicked()
{
    scene->clearWalls();
}

