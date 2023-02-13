#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHBoxLayout>
#include <QGridLayout>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  this->setWindowTitle("inventory game");


  QVBoxLayout* verticalLayout = new QVBoxLayout(this);

  widgets_to_disable = new QWidget(this);
  verticalLayout->setDirection(QBoxLayout::BottomToTop);
  verticalLayout->addWidget(widgets_to_disable);



  QHBoxLayout *horizontalLayout = new QHBoxLayout(widgets_to_disable);

  inventory_in = new inventory(3,3,widgets_to_disable);
  apple_gentrator_in = new apple_generator(widgets_to_disable);

  horizontalLayout->addWidget(widgets_to_disable);
  horizontalLayout->addWidget(inventory_in);

  QGridLayout* gridLayout = new QGridLayout(widgets_to_disable);
  gridLayout->addWidget(apple_gentrator_in, 0, 0);
  gridLayout->addWidget(ui->to_main_menu, 1, 1);
  horizontalLayout->addLayout(gridLayout);


  verticalLayout->addWidget(ui->Main_menu);
  verticalLayout->addLayout(horizontalLayout);





  QWidget *window = new QWidget();
  window->setLayout(verticalLayout);
  setCentralWidget(window);

   widgets_to_disable->setEnabled(false);
}

MainWindow::~MainWindow()
{
  delete ui;
}


void MainWindow::on_Begin_clicked()
{
  widgets_to_disable->setEnabled(true);
  inventory_in->clear_inventory();
  ui->Main_menu->scroll(0, -100);
}


void MainWindow::on_to_main_menu_clicked()
{
  apple_gentrator_in->clearSelection();
  inventory_in->clearSelection();
  widgets_to_disable->setEnabled(false);
  ui->Main_menu->scroll(0, 100);
}

