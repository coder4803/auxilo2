#include <QDateTime>

#include "messagefilterproxymodel.h"

namespace MessageTool {

MessageFilterProxyModel::MessageFilterProxyModel(QObject* parent) :
   QSortFilterProxyModel(parent)
{
}

void MessageFilterProxyModel::setGroupFilterList(QStringList groups)
{
   m_groups = groups;
}

QVariant MessageFilterProxyModel::data(const QModelIndex &index, int role) const
{
   QVariant value = QSortFilterProxyModel::data(index, role);

   if (index.column() == 0 && role == Qt::DisplayRole) {
      return QDateTime::fromMSecsSinceEpoch(value.toLongLong()).
            toString("hh:mm:ss.zzz");
   }

   return value;
}

bool MessageFilterProxyModel::filterAcceptsRow(int sourceRow,
                                               const QModelIndex &sourceParent)
                                               const
{
   Q_UNUSED(sourceParent)

   if (m_groups.isEmpty()) {
      return true;
   }

   QString group = sourceModel()->index(sourceRow, 1).data().toString();
   if (m_groups.contains(group)) {
      return true;
   }

   return false;
}

} // MessageTool
