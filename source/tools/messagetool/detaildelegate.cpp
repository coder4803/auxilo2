#include <QComboBox>

#include "detaildelegate.h"
#include "detailmodel.h"

namespace MessageTool {

DetailDelegate::DetailDelegate(QObject* parent) :
   QItemDelegate(parent)
{
}

QWidget* DetailDelegate::createEditor(QWidget *parent,
                                      const QStyleOptionViewItem &option,
                                      const QModelIndex &index) const
{
   // Create Drow-down box for rows that have list of value options.
   if (index.column() == DetailModel::COLUMN_VALUE) {
      if (!index.data(Qt::UserRole).toString().isEmpty()) {
         // List of options is stored in user role of value column.
         QStringList options = index.data(Qt::UserRole).toString().split(",");
         QComboBox* editor = new QComboBox(parent);
         editor->addItems(options);

         int id = editor->findText(index.data().toString());
         if (id >= 0) {
            editor->setCurrentIndex(id);
         }

         return editor;
      }
   }

   // Default editor is used for others.
   return QItemDelegate::createEditor(parent, option, index);
}

} // MessageTool
