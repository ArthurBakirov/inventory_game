
#include "inventory.h"




item::item(){}

QString item::get_type() const
{
  return type;
}

QString item::get_icon_path() const
{
  return icon_path;
}

apple::apple()
{
  type = "apple";
  icon_path = ":/icons/apple.jpg";
}

basket::basket()
{
  setTextAlignment(Qt::AlignBottom);
}

basket::basket(item item_new, uint amount_new)
{
  item_in = item_new;
  amount = amount_new;
  setTextAlignment(Qt::AlignBottom);
  set_picture();
}

void basket::add_item(const basket &other)
{
  if(item_in.get_type() == other.item_in.get_type())
  {
    amount += other.amount;
  }
  else
  {
    item_in = other.item_in;
    amount = other.amount;
  }
  set_picture();
}

void basket::set_item(item item_new, uint amount_new)
{
  item_in = item_new;
  amount = amount_new;
  set_picture();
}

void basket::move_item(basket *other)
{
  if(item_in.get_type() == other->item_in.get_type())
  {
    amount += other->amount;
  }
  else
  {
    item_in = other->item_in;
    amount = other->amount;
  }
  other->amount = 0;
  other->item_in = item();
  set_picture();
  other->set_picture();
}

const item basket::get_item_type() const
{
  return item_in;
}

const uint basket::get_item_amount() const
{
  return amount;
}

void basket::decrement_item()
{
  if(amount > 1)
  {
    amount--;
  }
  else if(amount == 1)
  {
    amount = 0;
    item_in = item();
  }
  set_picture();
}

void basket::set_picture()
{
  setIcon(QIcon(item_in.get_icon_path()));
  setText(amount ? QString::number(amount) : "");
}

generator::generator(QWidget *parent) : QTableWidget(1, 1, parent)
{
  setMaximumSize(QSize(150, 150));
  setDefaultDropAction(Qt::DropAction::IgnoreAction);
  setDragDropMode(QAbstractItemView::DragOnly);
  horizontalHeader()->setVisible(0);
  verticalHeader()->setVisible(0);
  verticalHeader()->setDefaultSectionSize(150);
  horizontalHeader()->setDefaultSectionSize(150);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setIconSize(QSize(150,150));
  setSelectionMode(QAbstractItemView::SingleSelection);
  setEditTriggers(QAbstractItemView::NoEditTriggers);

}

const basket generator::get_generating_item() const
{
  return generating_item;
}

apple_generator::apple_generator(QWidget *parent) : generator(parent)
{
  generating_item.set_item(apple(), 1);
  setItem(0, 0, &generating_item);
  generating_item.setText("");
}

inventory::inventory(int rows, int columns, QWidget *parent) : QTableWidget(rows, columns, parent), rows(rows), columns(columns),
                                                               sql_manager("sql_database", rows, columns)
{
  setMaximumSize(QSize(columns * 150, rows * 150));
  setMinimumSize(QSize(columns * 150, rows * 150));
  horizontalHeader()->setVisible(0);
  verticalHeader()->setVisible(0);
  setDefaultDropAction(Qt::DropAction::IgnoreAction);
  setDragDropMode(QAbstractItemView::DragDrop);


  verticalHeader()->setDefaultSectionSize(150);
  horizontalHeader()->setDefaultSectionSize(150);

  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setIconSize(QSize(150,150));
  setSelectionMode(QAbstractItemView::SingleSelection);
  setEditTriggers(QAbstractItemView::NoEditTriggers);
  crunch_sound.setSource(QUrl::fromLocalFile(":/sounds/apple_crunch.wav"));
  baskets.resize(rows);
  for(int i = 0; i < rows; i++)
  {
    baskets[i].resize(columns);
  }


  for(int i = 0; i < rows; i++)
  {
    for(int j = 0; j < columns; j++)
    {
      setItem(i, j, &baskets[i][j]);
    }
  }

  sql_manager.print_inventory();

}

inventory::~inventory()
{
 sql_manager.clear_inventory();
}

uint inventory::get_cell_amount() const
{
  return rows * columns;
}

void inventory::clear_inventory()
{
  for(uint i = 0; i < rows; i++)
  {
    for(uint j = 0; j < columns; j++)
    {
      class item blank;
      baskets[i][j].set_item(blank, 0);
    }
  }
  sql_manager.clear_inventory();
}


void inventory::dropEvent(QDropEvent *event)
{
  basket* drop_basket = reinterpret_cast<basket*>(itemAt(event->position().toPoint()));

  if(drop_basket != selected_basket)
  {
    drop_basket->move_item(selected_basket);

    sql_manager.set_items_to_inventory(drop_basket->get_item_type().get_type(), drop_basket->get_item_amount(), drop_basket->row(),  drop_basket->column());
    sql_manager.set_items_to_inventory(selected_basket->get_item_type().get_type(), selected_basket->get_item_amount(), selected_basket->row(),  selected_basket->column());
    sql_manager.add_item(drop_basket->get_item_type().get_type(), drop_basket->get_item_type().get_icon_path());
    sql_manager.print_all_items();
    sql_manager.print_inventory();
  }


  event->accept();

}

void inventory::dragEnterEvent(QDragEnterEvent *event)
{
  if(event->source() != this)
  {
    generator* source = reinterpret_cast<generator*>(event->source());
    clearSelection();
    buffer_basket = source->get_generating_item();
    selected_basket = &buffer_basket;
  }
  QAbstractItemView::dragEnterEvent(event);
}

void inventory::mousePressEvent(QMouseEvent *event)
{
  selected_basket = reinterpret_cast<basket*>(itemAt(event->position().toPoint()));



  if(selected_basket->get_item_type().get_type().isEmpty())
  {
    event->ignore();
    return;
  }

  if(event->button() == Qt::RightButton)
  {
    selected_basket->decrement_item();
    sql_manager.set_items_to_inventory(selected_basket->get_item_type().get_type(), selected_basket->get_item_amount(), selected_basket->row(),  selected_basket->column());

    crunch_sound.play();
    return;
  }


  QAbstractItemView::mousePressEvent(event);

}

void inventory::mouseReleaseEvent(QMouseEvent *event)
{
  clearSelection();
  selected_basket = nullptr;
  QAbstractItemView::mouseReleaseEvent(event);
}
