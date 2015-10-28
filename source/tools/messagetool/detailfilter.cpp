#include "detailfilter.h"

DetailFilter::DetailFilter(QObject *parent) :
   QSortFilterProxyModel(parent)
{
}

Qt::ItemFlags DetailFilter::flags(const QModelIndex &index) const
{
   Qt::ItemFlags indexFlags = Qt::ItemIsSelectable | Qt::ItemIsEnabled;

   if (index.column() == 0 && index.data(Qt::UserRole).toBool()) {
      indexFlags |= Qt::ItemIsEditable;
   } else if (index.column() == 1) {
      indexFlags |= Qt::ItemIsEditable;
   }

   return indexFlags;
}
