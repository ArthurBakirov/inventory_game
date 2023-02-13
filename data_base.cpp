#include "data_base.h"


sql_inventory::sql_inventory(const QString &path, uint rows, uint columns) : rows_amount(rows), columns_amount(columns),
  sql_inventory_name("inventory_" + QString::number(rows_amount) + "x" + QString::number(columns_amount))
{
  m_db = QSqlDatabase::addDatabase("QSQLITE");
  m_db.setDatabaseName(path);

  if (!m_db.open())
  {
    qDebug() << "Error: connection with database failed";
  }
  else
  {
    qDebug() << "Database: connection ok";
  }


  create_inventory_table();

}

bool sql_inventory::add_item(const QString &type, const QString &icon_path)
{
  bool success = false;

  QSqlQuery query("CREATE TABLE IF NOT EXISTS items ("
                  "Type varchar(255) NOT NULL PRIMARY KEY,"
                  "Icon_path varchar(255)"
                  ");");

  query.exec();
  query.clear();
  query.prepare("INSERT INTO items (Type, Icon_path) VALUES (:Type, :Icon_path);");
  query.bindValue(":Type", type);
  query.bindValue(":Icon_path", icon_path);
  if(query.exec())
  {
    success = true;
  }
  else
  {
    qDebug() << "add_item error:"
             << query.lastError();
  }


  return success;
}

void sql_inventory::print_all_items() const
{

  qDebug() << "items table:";

  QSqlQuery query("SELECT * FROM items;");

  QDebug qDebug_no_newline = qDebug().nospace();

  while (query.next())
  {
    for(int i = 0; i < query.record().count(); i++)
    {
      qDebug_no_newline << query.value(i).toString() << " ";
    }
    qDebug_no_newline << "\n";
  }
}

void sql_inventory::print_inventory() const
{
  qDebug() << sql_inventory_name << " table:";

  QSqlQuery query("SELECT * FROM " + sql_inventory_name + ";");

  QDebug qDebug_no_newline = qDebug().nospace();

  while (query.next())
  {
    for(int i = 0; i < query.record().count(); i++)
    {
      qDebug_no_newline << query.value(i).toString() << " ";
    }
    qDebug_no_newline << "\n";
  }
}

bool sql_inventory::set_items_to_inventory(const QString &type, uint amount_of_items, uint row, uint column)
{

  bool success = false;

  QSqlQuery query;
  query.prepare(QString("UPDATE %1 "
                        "SET %2 = (:items) "
                        "WHERE row_name = (:row_name);").arg(sql_inventory_name, "column" + QString::number(column + 1)));

  query.bindValue(":items", amount_of_items ? (QString::number(amount_of_items) + " " + type) : "empty");
  query.bindValue(":row_name", "row" + QString::number(row + 1));


  if(query.exec())
  {
    success = true;
  }
  else
  {
    qDebug() << "set_items_to_inventory error:"
             << query.lastError();
    success = false;
  }

  return success;
}

void sql_inventory::create_inventory_table()
{
  QSqlQuery query;
  QString query_line;


  query.prepare("SELECT name FROM sqlite_master WHERE type='table' AND name='"+ sql_inventory_name +"';");
  query.exec();

  //query.last() - returns true if a table exists
  if(!query.last())
  {
    query.clear();

    //creating command by cycle, because cell amount is not predetermined
    query_line.append( "CREATE TABLE IF NOT EXISTS " + sql_inventory_name + " (row_name varchar(255), ");
    for(uint i = 1; i <= columns_amount; i++)
    {
      query_line.append("column" + QString::number(i) + " varchar(255), ");
    }

    query_line.chop(2);
    query_line.append(");");


    query.prepare(query_line);


    qDebug() <<  query.exec()  << " creation initial";


    query.clear();
    query_line.clear();



    for(uint i = 1; i <= rows_amount; i++)
    {
      query_line.append( "INSERT INTO " + sql_inventory_name + " VALUES (");
      query_line.append("'row" + QString::number(i) + "', ");
      for(uint j = 1; j <= columns_amount; j++)
      {
        query_line.append("'empty', ");
      }
      query_line.chop(2);
      query_line.append(");");
      query.prepare(query_line);
      query.exec();
      query.clear();
      query_line.clear();
    }
  }

}

void sql_inventory::clear_inventory()
{

  QSqlQuery query;
  QString query_line;
  for(uint i = 1; i <= rows_amount; i++)
  {

    query_line.append( "UPDATE " + sql_inventory_name + " SET ");

    for(uint j = 1; j <= columns_amount; j++)
    {
      query_line.append("column" + QString::number(j) + " = 'empty', ");
    }
    query_line.chop(2);
    query_line.append(" WHERE row_name = 'row" + QString::number(i));
    query_line.append("'");
    query.prepare(query_line);
    query.exec();

    query.clear();
    query_line.clear();
  }
}
