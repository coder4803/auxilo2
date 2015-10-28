#ifndef DETAILFILTER_H
#define DETAILFILTER_H

#include <QSortFilterProxyModel>

class DetailFilter : public QSortFilterProxyModel
{
   Q_OBJECT
public:
   explicit DetailFilter(QObject *parent = 0);

   Qt::ItemFlags flags(const QModelIndex &index) const;
};

#endif // DETAILFILTER_H
