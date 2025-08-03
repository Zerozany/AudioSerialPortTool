_Pragma("once");
#include <QUdpSocket>
#include <QWidget>
#include <utility>

class CommUdp : public QObject
{
    Q_OBJECT
    friend class MainWidget;

public:
    explicit(true) CommUdp(QObject* _parent = nullptr);
    ~CommUdp() noexcept = default;

private:
    auto connect_signal_slot() noexcept -> void;

    auto send_data(const QByteArray& _data) noexcept -> bool;

    auto recv_data() noexcept -> void;

Q_SIGNALS:
    auto send_requested(const QByteArray& _data) -> void;

    auto recv_data_signal(const QByteArray& _data) -> void;

private:
    QUdpSocket* m_udp{new QUdpSocket{this}};

    std::pair<QString, quint16> m_user{};

    bool m_udp_tag{false};
};