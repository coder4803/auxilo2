#include "detailfilter.h"
#include "detailmodel.h"

namespace MessageTool {

DetailFilter::DetailFilter(QObject *parent) :
   QSortFilterProxyModel(parent)
{
}

Qt::ItemFlags DetailFilter::flags(const QModelIndex &index) const
{
   Qt::ItemFlags indexFlags = Qt::ItemIsSelectable | Qt::ItemIsEnabled;

   // Name column is ductile only if manually set.
   if (index.column() == DetailModel::COLUMN_NAME && index.data(Qt::UserRole).toBool()) {
      indexFlags |= Qt::ItemIsEditable;
   // Value column is always ductile
   } else if (index.column() == DetailModel::COLUMN_VALUE) {
      indexFlags |= Qt::ItemIsEditable;
   }

   return indexFlags;
}

} // MessageTool
