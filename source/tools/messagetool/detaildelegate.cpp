#include <QComboBox>

#include "detaildelegate.h"

DetailDelegate::DetailDelegate(QObject* parent) :
   QItemDelegate(parent)
{
}

QWidget* DetailDelegate::createEditor(QWidget *parent,
                                      const QStyleOptionViewItem &option,
                                      const QModelIndex &index) const
{
   if (index.column() == 1) {
      if (!index.data(Qt::UserRole).toString().isEmpty()) {
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

   return QItemDelegate::createEditor(parent, option, index);
}
