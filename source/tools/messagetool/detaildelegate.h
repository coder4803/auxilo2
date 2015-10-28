#ifndef DETAILDELEGATE_H
#define DETAILDELEGATE_H

#include <QItemDelegate>

class DetailDelegate : public QItemDelegate
{
   Q_OBJECT
public:
   DetailDelegate(QObject* parent = NULL);

   QWidget* createEditor(QWidget *parent,
                         const QStyleOptionViewItem &option,
                         const QModelIndex &index) const;
};

#endif
