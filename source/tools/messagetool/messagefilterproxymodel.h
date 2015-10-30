#ifndef MESSAGEFILTERPROXYMODEL_H
#define MESSAGEFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>

namespace MessageTool {

/*!
 * \brief The MessageFilterProxyModel class
 * Class is used to show messages only from specified groups.
 */
class MessageFilterProxyModel : public QSortFilterProxyModel
{
public:
   //! Constructor.
   explicit MessageFilterProxyModel(QObject* parent);

   /*!
    * \brief Sets list of groups to show the messages from.
    * \param groups List of groups.
    */
   void setGroupFilterList(QStringList groups);

   /*!
    * \brief Returns data from given index.
    * This method converts timestamp to human readable format.
    * \param index Index to return data from.
    * \param role Role of the index.
    * \return Data from given index and role.
    */
   QVariant data(const QModelIndex &index, int role) const;

protected:
   //! Overloaded method that filters out unwanted messages.
   bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

private:
   //! List of groups to show messages from.
   QStringList m_groups;
};

} // MessageTool

#endif
