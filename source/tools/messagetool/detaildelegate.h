#ifndef DETAILDELEGATE_H
#define DETAILDELEGATE_H

#include <QItemDelegate>

namespace MessageTool {

/*!
 * \brief The DetailDelegate class.
 * This class takes care of providing right editor for message field values.
 */
class DetailDelegate : public QItemDelegate
{
   Q_OBJECT
public:
   //! Constructor.
   explicit DetailDelegate(QObject* parent = NULL);

   //! Creates dropdown editor for enum values.
   QWidget* createEditor(QWidget *parent,
                         const QStyleOptionViewItem &option,
                         const QModelIndex &index) const;
};

} // MessageTool

#endif
