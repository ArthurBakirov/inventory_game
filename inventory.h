#ifndef INVENTORY_H
#define INVENTORY_H


#include <QTableWidget>
#include <QTableWidgetItem>
#include <QDropEvent>
#include <QDebug>
#include <QHeaderView>
#include <QSoundEffect>

#include "data_base.h"




class item
{
public:
  item();

  virtual ~item() = default;

  virtual QString get_type() const;

  virtual QString get_icon_path() const;

protected:
  QString type;
  QString icon_path;


};


class apple : public item
{
public:
  apple();

};




class basket : public QTableWidgetItem
{
public:
  basket();

  basket(item item_new, uint amount_new);

  //add all the items from other basket
  void add_item(const basket& other);

  //sets specific amount of specific type
  void set_item(item item_new, uint amount_new);

  //moves everything from other basket
  void move_item(basket* other);

  const item get_item_type() const;

  const uint get_item_amount() const;

  void decrement_item();

protected:
  void set_picture();

  item item_in;
  uint amount = 0;
};



class generator : public QTableWidget
{
public:
  generator(QWidget *parent = nullptr);

  virtual ~generator() = default;

  const basket get_generating_item() const;

protected:
  basket generating_item;
};


class apple_generator : public generator
{
public:
  apple_generator(QWidget *parent = nullptr);

};



class inventory : public QTableWidget
{
public:
  inventory(int rows, int columns, QWidget *parent = nullptr);

  virtual ~inventory();

  uint get_cell_amount() const;

  void clear_inventory();


private:
  //overriding standart drag-drop functionality to implement basket system

  void dropEvent(QDropEvent *event) override;


  void dragEnterEvent(QDragEnterEvent *event) override;


  void mousePressEvent(QMouseEvent *event) override;


  void mouseReleaseEvent(QMouseEvent *event) override;




private:
  QVector<QVector<basket>> baskets;
  uint rows;
  uint columns;
  basket* selected_basket = nullptr;
  basket buffer_basket;
  QSoundEffect crunch_sound;
  sql_inventory sql_manager;
};




#endif // INVENTORY_H
