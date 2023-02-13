#ifndef DATA_BASE_H
#define DATA_BASE_H


#include <QtSql>
#include <QString>



class sql_inventory
{
public:
  //connects to the path + name from .exe (e.g database/sql.database) otherwise creates one
  sql_inventory(const QString& path, uint rows, uint columns);

  //adds item if not already exists to the table of list of all items, also check if there is already
  //a table created
  bool add_item(const QString& type, const QString& icon_path);

  void print_all_items() const;

  void print_inventory() const;


  //sets items to the specific cell in table
  bool set_items_to_inventory(const QString& type, uint amount_of_items, uint row, uint column);

  //creates table inventory(rows)x(columns) if not exists
  void create_inventory_table();

  //sets all cells to empty
  void clear_inventory();

private:
  uint rows_amount;
  uint columns_amount;
  QString sql_inventory_name;
  QSqlDatabase m_db;
};


#endif // DATA_BASE_H
