#ifndef MESSAGEFILTERPROXYMODEL_H
#define MESSAGEFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>

class MessageFilterProxyModel : public QSortFilterProxyModel
{
public:
   explicit MessageFilterProxyModel(QObject* parent);

   void setGroupFilterList(QStringList groups);

   QVariant data(const QModelIndex &index, int role) const;

protected:
   bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

private:
   QStringList m_groups;
};

#endif
