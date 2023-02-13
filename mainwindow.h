#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "inventory.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  void on_Begin_clicked();

  void on_to_main_menu_clicked();

private:
  Ui::MainWindow *ui;
  QWidget* widgets_to_disable;
  inventory *inventory_in;
  apple_generator *apple_gentrator_in;
};
#endif // MAINWINDOW_H
