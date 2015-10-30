#ifndef DETAILFILTER_H
#define DETAILFILTER_H

#include <QSortFilterProxyModel>

namespace MessageTool {

class DetailFilter : public QSortFilterProxyModel
{
   Q_OBJECT
public:
   //! Constructor.
   explicit DetailFilter(QObject *parent = 0);

   //! Controls editing mode of fields.
   Qt::ItemFlags flags(const QModelIndex &index) const;
};

} // MessageTool

#endif
