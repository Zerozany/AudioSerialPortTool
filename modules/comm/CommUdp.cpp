#include "CommUdp.h"

#include <QHostAddress>
#include <QRegularExpression>
#include <QtNetwork>

CommUdp::CommUdp(QObject* _parent) : QObject{_parent}
{
    std::invoke(&CommUdp::connect_signal_slot, this);
}

auto CommUdp::connect_signal_slot() noexcept -> void
{
    connect(this, &CommUdp::send_requested, this, &CommUdp::send_data, Qt::QueuedConnection);
    connect(m_udp, &QUdpSocket::readyRead, this, &CommUdp::recv_data, Qt::QueuedConnection);
}

auto CommUdp::send_data(const QByteArray& _data) noexcept -> bool
{
    qint64 bytes{};
    if (m_udp_tag)
    {
        bytes = m_udp->writeDatagram(_data, QHostAddress{m_user.first}, m_user.second);
        // qDebug() << _data.toHex(' ').toUpper();
    }
    if (bytes == -1)
    {
        return false;
    }
    return true;
}

auto CommUdp::recv_data() noexcept -> void
{
    while (m_udp->hasPendingDatagrams())
    {
        QByteArray datagram{};
        datagram.resize(m_udp->pendingDatagramSize());
        QHostAddress sender{};
        quint16      port{};
        m_udp->readDatagram(datagram.data(), datagram.size(), &sender, &port);
        emit recv_data_signal(datagram);
    }
}
