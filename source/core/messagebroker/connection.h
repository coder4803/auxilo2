#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include <QTcpSocket>

namespace Core {

class Connection : public QObject
{
   Q_OBJECT
public:
   explicit Connection(QTcpSocket* socket, QObject* parent = NULL);

signals:
   void joinToGroup(QString name);
   void publishMessage(QByteArray payload, QString group);
   void deliveMessage(QByteArray payload, QString group);
   void disconnected();

public slots:
   void sendMessage(QByteArray payload);

private slots:
   void onConnected();
   void onDisconnected();
   void onReadyRead();

private:
   void handleReceivedData(const QByteArray& data);

   bool handleJoinMessage(QDataStream& stream);
   bool handlePublishMessage(QDataStream& stream);
   bool handleDeliveMessage(QDataStream& stream);

   QTcpSocket* m_socket;
   QByteArray m_buffer;
};

} // Core

#endif
