#include "MainWidget.h"

#include <QFileDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QIODevice>
#include <QRegularExpression>
#include <QTimer>
#include <ranges>

#include "ViewLayout.h"


#define FRAME_HEAD                        \
    stream << static_cast<uint8_t>(0x5A); \
    stream << static_cast<uint8_t>(0x5A); \
    stream << static_cast<uint8_t>(0x77);

#define FRAME_END                         \
    stream << static_cast<uint8_t>(0x00); \
    stream << static_cast<uint8_t>(0x23);

MainWidget::MainWidget(QWidget* _parent) : QWidget{_parent}
{
    m_view = new ViewLayout{this};
    std::invoke(&MainWidget::init_widget, this);
    std::invoke(&MainWidget::connect_signal_slot_base, this);
    std::invoke(&MainWidget::connect_signal_slot_login, this);
    std::invoke(&MainWidget::connect_signal_slot_audio, this);
    std::invoke(&MainWidget::connect_signal_slot_light, this);
}

auto MainWidget::init_widget() noexcept -> void
{
    this->setWindowTitle("QianJue TestTool");
    this->setFont(QFont{"微软雅黑", 10});
    this->resize(960, 600);
}

auto MainWidget::connect_signal_slot_base() noexcept -> void
{
    connect(m_stack_layout, &QStackedLayout::currentChanged, this, [this](int _index) {
        for (int i{}; auto& btn : this->m_group_select | std::views::values)
        {
            if (i == _index)
            {
                btn->setEnabled(false);
            }
            else
            {
                btn->setEnabled(true);
            }
            ++i;
        }
    });
    emit m_stack_layout->currentChanged(0);

    connect(m_group_select.at("audio"), &QPushButton::clicked, this, [this] {
        m_stack_layout->setCurrentWidget(m_group.at("audio"));
    });

    connect(m_group_select.at("light"), &QPushButton::clicked, this, [this] {
        m_stack_layout->setCurrentWidget(m_group.at("light"));
    });

    connect(m_comm_pool.at("audio"), &CommUdp::recv_data_signal, this, &MainWidget::print_log);

    connect(m_comm_pool.at("light"), &CommUdp::recv_data_signal, this, &MainWidget::print_log);
}

auto MainWidget::connect_signal_slot_login() noexcept -> void
{
    connect(qobject_cast<QPushButton*>(this->m_net_config.at("Connect")), &QPushButton::clicked, this, [this] {
        bool    tag{};
        QString ip{};
        ushort  port_a{};
        ushort  port_l{};
        if (QRegularExpression regex{"^[0-9.]+$"}; qobject_cast<QPushButton*>(this->m_net_config.at("Connect"))->text() == "连接")
        {
            ip = qobject_cast<QLineEdit*>(this->m_net_config.at("IP"))->text();
            if (!regex.match(ip).hasMatch())
            {
                qobject_cast<QLineEdit*>(this->m_net_config.at("IP"))->setText("");
                return;
            }
            port_a = qobject_cast<QLineEdit*>(this->m_net_config.at("Port_audio"))->text().toUShort(&tag);
            if (!tag)
            {
                qobject_cast<QLineEdit*>(this->m_net_config.at("Port_audio"))->setText("");
                return;
            }
            port_l = qobject_cast<QLineEdit*>(this->m_net_config.at("Port_light"))->text().toUShort(&tag);
            if (!tag)
            {
                qobject_cast<QLineEdit*>(this->m_net_config.at("Port_light"))->setText("");
                return;
            }
            m_comm_pool.at("audio")->m_user.second = port_a;
            m_comm_pool.at("light")->m_user.second = port_l;
            for (auto& comm : m_comm_pool | std::views::values)
            {
                comm->m_user.first = ip;
                comm->m_udp_tag    = true;
            }
            QByteArray  tmp{};
            QDataStream stream{&tmp, QIODevice::WriteOnly};
            stream.setByteOrder(QDataStream::LittleEndian);
            FRAME_HEAD
            stream << static_cast<uint8_t>(0x00);
            stream << static_cast<uint8_t>(0x0C);
            stream << static_cast<uint8_t>(0x01);
            stream << static_cast<uint8_t>(0x00);
            stream << static_cast<uint8_t>(0x50);
            stream << static_cast<uint8_t>(0x51);
            stream << static_cast<uint8_t>(0x00);
            FRAME_END
            Q_EMIT m_comm_pool.at("audio")->send_requested(tmp);
            Q_EMIT m_comm_pool.at("light")->send_requested(tmp);
            qobject_cast<QPushButton*>(this->m_net_config.at("Connect"))->setText("断开");
        }
        else
        {
            for (auto& comm : m_comm_pool | std::views::values)
            {
                comm->m_udp_tag = false;
            }
            qobject_cast<QPushButton*>(this->m_net_config.at("Connect"))->setText("连接");
        }
    });
}

auto MainWidget::connect_signal_slot_light() noexcept -> void
{
    connect(qobject_cast<QPushButton*>(this->m_light_btns.at("灯光控制-关")), &QPushButton::clicked, this, [this] {
        QByteArray  tmp{};
        QDataStream stream{&tmp, QIODevice::WriteOnly};
        stream.setByteOrder(QDataStream::LittleEndian);
        FRAME_HEAD
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x0D);
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x62);
        stream << static_cast<uint8_t>(0x51);
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x00);
        FRAME_END
        Q_EMIT m_comm_pool.at("light")->send_requested(tmp);
    });

    connect(qobject_cast<QPushButton*>(this->m_light_btns.at("灯光控制-爆闪")), &QPushButton::clicked, this, [this] {
        QByteArray  tmp{};
        QDataStream stream{&tmp, QIODevice::WriteOnly};
        stream.setByteOrder(QDataStream::LittleEndian);
        FRAME_HEAD
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x0D);
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x62);
        stream << static_cast<uint8_t>(0x51);
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x01);
        FRAME_END
        Q_EMIT m_comm_pool.at("light")->send_requested(tmp);
    });

    connect(qobject_cast<QPushButton*>(this->m_light_btns.at("灯光控制-常亮")), &QPushButton::clicked, this, [this] {
        QByteArray  tmp{};
        QDataStream stream{&tmp, QIODevice::WriteOnly};
        stream.setByteOrder(QDataStream::LittleEndian);
        FRAME_HEAD
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x0D);
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x62);
        stream << static_cast<uint8_t>(0x51);
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x02);
        FRAME_END
        Q_EMIT m_comm_pool.at("light")->send_requested(tmp);
    });

    connect(qobject_cast<QPushButton*>(this->m_light_btns.at("灯光亮度")), &QPushButton::clicked, this, [this] {
        bool   tag{};
        ushort value_left{qobject_cast<QSpinBox*>(this->m_light_btns.at("亮度输入-左灯"))->text().toUShort(&tag)};
        if (!tag)
        {
            return;
        }
        ushort value_right{qobject_cast<QSpinBox*>(this->m_light_btns.at("亮度输入-右灯"))->text().toUShort(&tag)};
        if (!tag)
        {
            return;
        }
        QByteArray  tmp{};
        QDataStream stream{&tmp, QIODevice::WriteOnly};
        stream.setByteOrder(QDataStream::LittleEndian);
        FRAME_HEAD
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x0D);
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x63);
        stream << static_cast<uint8_t>(0x51);
        stream << static_cast<uint8_t>(value_left);
        stream << static_cast<uint8_t>(value_right);
        FRAME_END
        Q_EMIT m_comm_pool.at("light")->send_requested(tmp);
    });

    connect(qobject_cast<QPushButton*>(this->m_light_btns.at("云台俯仰+")), &QPushButton::clicked, this, [this] {
        bool   tag{};
        ushort value{qobject_cast<QSpinBox*>(this->m_light_btns.at("俯仰值"))->text().toUShort(&tag)};
        if (!tag)
        {
            return;
        }

        QByteArray  tmp{};
        QDataStream stream{&tmp, QIODevice::WriteOnly};
        stream.setByteOrder(QDataStream::LittleEndian);
        FRAME_HEAD
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x0E);
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x64);
        stream << static_cast<uint8_t>(0x51);
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint16_t>(value);
        FRAME_END
        Q_EMIT m_comm_pool.at("light")->send_requested(tmp);
        qDebug() << tmp.toHex(' ').toUpper();
    });

    connect(qobject_cast<QPushButton*>(this->m_light_btns.at("云台俯仰-")), &QPushButton::clicked, this, [this] {
        bool   tag{};
        ushort value{qobject_cast<QSpinBox*>(this->m_light_btns.at("俯仰值"))->text().toUShort(&tag)};
        if (!tag)
        {
            return;
        }
        QByteArray  tmp{};
        QDataStream stream{&tmp, QIODevice::WriteOnly};
        stream.setByteOrder(QDataStream::LittleEndian);
        FRAME_HEAD
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x0E);
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x64);
        stream << static_cast<uint8_t>(0x51);
        stream << static_cast<uint8_t>(0xff);
        stream << static_cast<uint16_t>(value);
        FRAME_END
        Q_EMIT m_comm_pool.at("light")->send_requested(tmp);
        qDebug() << tmp.toHex(' ').toUpper();
    });

    connect(qobject_cast<QPushButton*>(this->m_light_btns.at("云台航向+")), &QPushButton::clicked, this, [this] {
        bool   tag{};
        ushort value{qobject_cast<QSpinBox*>(this->m_light_btns.at("航向值"))->text().toUShort(&tag)};
        if (!tag)
        {
            return;
        }

        QByteArray  tmp{};
        QDataStream stream{&tmp, QIODevice::WriteOnly};
        stream.setByteOrder(QDataStream::LittleEndian);
        FRAME_HEAD
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x0E);
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x65);
        stream << static_cast<uint8_t>(0x51);
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint16_t>(value);
        FRAME_END
        Q_EMIT m_comm_pool.at("light")->send_requested(tmp);
        qDebug() << tmp.toHex(' ').toUpper();
    });

    connect(qobject_cast<QPushButton*>(this->m_light_btns.at("云台航向-")), &QPushButton::clicked, this, [this] {
        bool   tag{};
        ushort value{qobject_cast<QSpinBox*>(this->m_light_btns.at("航向值"))->text().toUShort(&tag)};
        if (!tag)
        {
            return;
        }
        QByteArray  tmp{};
        QDataStream stream{&tmp, QIODevice::WriteOnly};
        stream.setByteOrder(QDataStream::LittleEndian);
        FRAME_HEAD
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x0E);
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x65);
        stream << static_cast<uint8_t>(0x51);
        stream << static_cast<uint8_t>(0xff);
        stream << static_cast<uint16_t>(value);
        FRAME_END
        Q_EMIT m_comm_pool.at("light")->send_requested(tmp);
        qDebug() << tmp.toHex(' ').toUpper();
    });

    connect(qobject_cast<QPushButton*>(this->m_light_btns.at("灯光查询")), &QPushButton::clicked, this, [this] {
        QByteArray  tmp{};
        QDataStream stream{&tmp, QIODevice::WriteOnly};
        stream.setByteOrder(QDataStream::LittleEndian);
        FRAME_HEAD
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x0D);
        stream << static_cast<uint8_t>(0x01);
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x6F);
        stream << static_cast<uint8_t>(0x51);
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x00);
        FRAME_END
        Q_EMIT m_comm_pool.at("light")->send_requested(tmp);
    });

    connect(qobject_cast<QPushButton*>(this->m_light_btns.at("角度查询")), &QPushButton::clicked, this, [this] {
        QByteArray  tmp{};
        QDataStream stream{&tmp, QIODevice::WriteOnly};
        stream.setByteOrder(QDataStream::LittleEndian);
        FRAME_HEAD
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x0D);
        stream << static_cast<uint8_t>(0x01);
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x6F);
        stream << static_cast<uint8_t>(0x54);
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x00);
        FRAME_END
        Q_EMIT m_comm_pool.at("light")->send_requested(tmp);
    });
}

auto MainWidget::print_log(const QByteArray& _data) -> void
{
    QString timestamp = QDateTime::currentDateTime().toString("[HH:mm:ss] ");

    m_log->append(timestamp + _data.toHex(' ').toUpper());
}
// 576 380ms
auto MainWidget::connect_signal_slot_audio() noexcept -> void
{
    connect(qobject_cast<QPushButton*>(this->m_audio_btns.at("开启语音")), &QPushButton::clicked, this, [this] {
        QByteArray  tmp{};
        QDataStream stream{&tmp, QIODevice::WriteOnly};
        stream.setByteOrder(QDataStream::LittleEndian);
        FRAME_HEAD
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x0C);
        stream << static_cast<uint8_t>(0x01);
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x51);
        stream << static_cast<uint8_t>(0xB1);
        stream << static_cast<uint8_t>(0x00);
        FRAME_END
        Q_EMIT m_comm_pool.at("audio")->send_requested(tmp);
        QString filePath{QFileDialog::getOpenFileName(this, "选择 MP2 文件", "", "所有文件 (*.*)")};
        if (filePath.isEmpty())
        {
            return;
        }
        QFile file{filePath};
        if (!file.open(QIODevice::ReadOnly))
        {
            return;
        }
        // 跳过前6个字节
        file.read(6);
        // 准备分块数据
        QVector<QByteArray> chunks;
        while (!file.atEnd())
        {
            QByteArray chunk = file.read(576);
            if (!chunk.isEmpty())
            {
                chunks.append(chunk);
            }
        }
        file.close();
        // 定时发送
        QSharedPointer<QTimer> timer(new QTimer);
        int                    index = 0;
        connect(timer.data(), &QTimer::timeout, this, [=]() mutable {
            if (index < chunks.size())
            {
                Q_EMIT m_comm_pool.at("audio")->send_requested(chunks.at(index));
                ++index;
            }
            else
            {
                timer->stop();
            }
        });
        timer->start(380);
    });

    connect(qobject_cast<QPushButton*>(this->m_audio_btns.at("结束语音")), &QPushButton::clicked, this, [this] {
        QByteArray  tmp{};
        QDataStream stream{&tmp, QIODevice::WriteOnly};
        stream.setByteOrder(QDataStream::LittleEndian);
        FRAME_HEAD
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x0C);
        stream << static_cast<uint8_t>(0x01);
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x51);
        stream << static_cast<uint8_t>(0xB2);
        stream << static_cast<uint8_t>(0x00);
        FRAME_END
        Q_EMIT m_comm_pool.at("audio")->send_requested(tmp);
    });

    connect(qobject_cast<QPushButton*>(this->m_audio_btns.at("打开音频文件上传")), &QPushButton::clicked, this, [this] {
        QString filePath{QFileDialog::getOpenFileName(this, "选择 MP2 文件", "", "音频文件 (*.mp2);;所有文件 (*.*)")};
        if (filePath.isEmpty())
        {
            return;
        }
        QFile      file{filePath};
        QByteArray fileData{};
        if (file.open(QIODevice::ReadOnly))
        {
            fileData = file.readAll();
            file.close();
        }
        QFileInfo fileInfo{filePath};
        QString   fileName{fileInfo.fileName()};
        qobject_cast<QLineEdit*>(this->m_audio_btns.at("上传文件名输入"))->setText(fileName);
        QString     name{qobject_cast<QLineEdit*>(this->m_audio_btns.at("上传文件名输入"))->text()};
        uint8_t     text_size{11 + sizeof(name)};
        QByteArray  tmp{};
        QDataStream stream{&tmp, QIODevice::WriteOnly};
        stream.setByteOrder(QDataStream::LittleEndian);
        FRAME_HEAD
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(text_size);
        stream << static_cast<uint8_t>(0x01);
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x51);
        stream << static_cast<uint8_t>(0x01);
        if (qobject_cast<QLineEdit*>(this->m_audio_btns.at("上传文件名输入"))->text().isEmpty())
        {
            stream << static_cast<uint8_t>(0x00);
        }
        else
        {
            stream.writeRawData(name.toUtf8().data(), sizeof(name));
        }
        FRAME_END
        Q_EMIT m_comm_pool.at("audio")->send_requested(tmp);
        for (int i = 0; i < fileData.size(); i += 1024)
        {
            QByteArray chunk = fileData.mid(i, 1024);
            Q_EMIT m_comm_pool.at("audio")->send_requested(chunk);
        }
    });

    connect(qobject_cast<QPushButton*>(this->m_audio_btns.at("关闭音频文件上传")), &QPushButton::clicked, this, [this] {
        QByteArray  tmp{};
        QDataStream stream{&tmp, QIODevice::WriteOnly};
        stream.setByteOrder(QDataStream::LittleEndian);
        FRAME_HEAD
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x0C);
        stream << static_cast<uint8_t>(0x01);
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x51);
        stream << static_cast<uint8_t>(0x03);
        stream << static_cast<uint8_t>(0x00);
        FRAME_END
        Q_EMIT m_comm_pool.at("audio")->send_requested(tmp);
    });

    connect(qobject_cast<QPushButton*>(this->m_audio_btns.at("音频继续播放")), &QPushButton::clicked, this, [this] {
        QByteArray  tmp{};
        QDataStream stream{&tmp, QIODevice::WriteOnly};
        stream.setByteOrder(QDataStream::LittleEndian);
        FRAME_HEAD
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x0C);
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x52);
        stream << static_cast<uint8_t>(0x04);
        stream << static_cast<uint8_t>(0x00);
        FRAME_END
        Q_EMIT m_comm_pool.at("audio")->send_requested(tmp);
    });

    connect(qobject_cast<QPushButton*>(this->m_audio_btns.at("音频暂停播放")), &QPushButton::clicked, this, [this] {
        QByteArray  tmp{};
        QDataStream stream{&tmp, QIODevice::WriteOnly};
        stream.setByteOrder(QDataStream::LittleEndian);
        FRAME_HEAD
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x0C);
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x52);
        stream << static_cast<uint8_t>(0x05);
        stream << static_cast<uint8_t>(0x00);
        FRAME_END
        Q_EMIT m_comm_pool.at("audio")->send_requested(tmp);
    });

    connect(qobject_cast<QPushButton*>(this->m_audio_btns.at("音频停止播放")), &QPushButton::clicked, this, [this] {
        QByteArray  tmp{};
        QDataStream stream{&tmp, QIODevice::WriteOnly};
        stream.setByteOrder(QDataStream::LittleEndian);
        FRAME_HEAD
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x0C);
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x52);
        stream << static_cast<uint8_t>(0x06);
        stream << static_cast<uint8_t>(0x00);
        FRAME_END
        Q_EMIT m_comm_pool.at("audio")->send_requested(tmp);
    });

    connect(qobject_cast<QPushButton*>(this->m_audio_btns.at("音频上一曲")), &QPushButton::clicked, this, [this] {
        QByteArray  tmp{};
        QDataStream stream{&tmp, QIODevice::WriteOnly};
        stream.setByteOrder(QDataStream::LittleEndian);
        FRAME_HEAD
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x0C);
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x52);
        stream << static_cast<uint8_t>(0x07);
        stream << static_cast<uint8_t>(0x00);
        FRAME_END
        Q_EMIT m_comm_pool.at("audio")->send_requested(tmp);
    });

    connect(qobject_cast<QPushButton*>(this->m_audio_btns.at("音频下一曲")), &QPushButton::clicked, this, [this] {
        QByteArray  tmp{};
        QDataStream stream{&tmp, QIODevice::WriteOnly};
        stream.setByteOrder(QDataStream::LittleEndian);
        FRAME_HEAD
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x0C);
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x52);
        stream << static_cast<uint8_t>(0x08);
        stream << static_cast<uint8_t>(0x00);
        FRAME_END
        Q_EMIT m_comm_pool.at("audio")->send_requested(tmp);
    });

    connect(qobject_cast<QPushButton*>(this->m_audio_btns.at("调节音量")), &QPushButton::clicked, this, [this] {
        bool   tag{};
        ushort value{qobject_cast<QSpinBox*>(this->m_audio_btns.at("音量"))->text().toUShort(&tag)};
        if (!tag)
        {
            return;
        }
        QByteArray  tmp{};
        QDataStream stream{&tmp, QIODevice::WriteOnly};
        stream.setByteOrder(QDataStream::LittleEndian);
        FRAME_HEAD
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x0C);
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x52);
        stream << static_cast<uint8_t>(0x10);
        stream << static_cast<uint8_t>(value);
        FRAME_END
        Q_EMIT m_comm_pool.at("audio")->send_requested(tmp);
    });

    connect(qobject_cast<QPushButton*>(this->m_audio_btns.at("开启循环")), &QPushButton::clicked, this, [this] {
        QByteArray  tmp{};
        QDataStream stream{&tmp, QIODevice::WriteOnly};
        stream.setByteOrder(QDataStream::LittleEndian);
        FRAME_HEAD
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x0C);
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x55);
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x01);
        FRAME_END
        Q_EMIT m_comm_pool.at("audio")->send_requested(tmp);
    });

    connect(qobject_cast<QPushButton*>(this->m_audio_btns.at("关闭循环")), &QPushButton::clicked, this, [this] {
        QByteArray  tmp{};
        QDataStream stream{&tmp, QIODevice::WriteOnly};
        stream.setByteOrder(QDataStream::LittleEndian);
        FRAME_HEAD
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x0C);
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x55);
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x00);
        FRAME_END
        Q_EMIT m_comm_pool.at("audio")->send_requested(tmp);
    });

    connect(qobject_cast<QPushButton*>(this->m_audio_btns.at("播放指定音频")), &QPushButton::clicked, this, [this] {
        QByteArray  tmp{};
        QDataStream stream{&tmp, QIODevice::WriteOnly};
        stream.setByteOrder(QDataStream::LittleEndian);
        FRAME_HEAD
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x0C);
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x52);
        stream << static_cast<uint8_t>(0x10);
        if (qobject_cast<QLineEdit*>(this->m_audio_btns.at("播放指定音频文件名输入"))->text().isEmpty())
        {
            stream << static_cast<uint8_t>(0x00);
        }
        else
        {
            stream.writeRawData(qobject_cast<QLineEdit*>(this->m_audio_btns.at("播放指定音频文件名输入"))->text().toUtf8().data(), qobject_cast<QLineEdit*>(this->m_audio_btns.at("播放指定音频文件名输入"))->text().toUtf8().size());
        }
        FRAME_END
        Q_EMIT m_comm_pool.at("audio")->send_requested(tmp);
    });

    connect(qobject_cast<QPushButton*>(this->m_audio_btns.at("删除音频")), &QPushButton::clicked, this, [this] {
        QByteArray  tmp{};
        QDataStream stream{&tmp, QIODevice::WriteOnly};
        stream.setByteOrder(QDataStream::LittleEndian);
        FRAME_HEAD
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x0C);
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x54);
        stream << static_cast<uint8_t>(0x02);
        if (qobject_cast<QLineEdit*>(this->m_audio_btns.at("播放指定音频文件名输入"))->text().isEmpty())
        {
            stream << static_cast<uint8_t>(0x00);
        }
        else
        {
            stream.writeRawData(qobject_cast<QLineEdit*>(this->m_audio_btns.at("播放指定音频文件名输入"))->text().toUtf8().data(), qobject_cast<QLineEdit*>(this->m_audio_btns.at("播放指定音频文件名输入"))->text().toUtf8().size());
        }
        FRAME_END
        Q_EMIT m_comm_pool.at("audio")->send_requested(tmp);
    });

    connect(qobject_cast<QPushButton*>(this->m_audio_btns.at("删除录音")), &QPushButton::clicked, this, [this] {
        QByteArray  tmp{};
        QDataStream stream{&tmp, QIODevice::WriteOnly};
        stream.setByteOrder(QDataStream::LittleEndian);
        FRAME_HEAD
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x0C);
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x54);
        stream << static_cast<uint8_t>(0x03);
        if (qobject_cast<QLineEdit*>(this->m_audio_btns.at("播放指定音频文件名输入"))->text().isEmpty())
        {
            stream << static_cast<uint8_t>(0x00);
        }
        else
        {
            stream.writeRawData(qobject_cast<QLineEdit*>(this->m_audio_btns.at("播放指定音频文件名输入"))->text().toUtf8().data(), qobject_cast<QLineEdit*>(this->m_audio_btns.at("播放指定音频文件名输入"))->text().toUtf8().size());
        }
        FRAME_END
        Q_EMIT m_comm_pool.at("audio")->send_requested(tmp);
    });

    connect(qobject_cast<QPushButton*>(this->m_audio_btns.at("查询列表")), &QPushButton::clicked, this, [this] {
        QByteArray  tmp{};
        QDataStream stream{&tmp, QIODevice::WriteOnly};
        stream.setByteOrder(QDataStream::LittleEndian);
        FRAME_HEAD
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x0C);
        stream << static_cast<uint8_t>(0x01);
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x58);
        stream << static_cast<uint8_t>(0x01);
        stream << static_cast<uint8_t>(0x00);
        FRAME_END
        Q_EMIT m_comm_pool.at("audio")->send_requested(tmp);
    });

    connect(qobject_cast<QPushButton*>(this->m_audio_btns.at("查询音量")), &QPushButton::clicked, this, [this] {
        QByteArray  tmp{};
        QDataStream stream{&tmp, QIODevice::WriteOnly};
        stream.setByteOrder(QDataStream::LittleEndian);
        FRAME_HEAD
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x0C);
        stream << static_cast<uint8_t>(0x01);
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x53);
        stream << static_cast<uint8_t>(0x01);
        stream << static_cast<uint8_t>(0x00);
        FRAME_END
        Q_EMIT m_comm_pool.at("audio")->send_requested(tmp);
    });

    connect(qobject_cast<QPushButton*>(this->m_audio_btns.at("查询播放状态")), &QPushButton::clicked, this, [this] {
        QByteArray  tmp{};
        QDataStream stream{&tmp, QIODevice::WriteOnly};
        stream.setByteOrder(QDataStream::LittleEndian);
        FRAME_HEAD
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x0C);
        stream << static_cast<uint8_t>(0x01);
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x53);
        stream << static_cast<uint8_t>(0x02);
        stream << static_cast<uint8_t>(0x00);
        FRAME_END
        Q_EMIT m_comm_pool.at("audio")->send_requested(tmp);
    });

    connect(qobject_cast<QPushButton*>(this->m_audio_btns.at("查询云台角度")), &QPushButton::clicked, this, [this] {
        QByteArray  tmp{};
        QDataStream stream{&tmp, QIODevice::WriteOnly};
        stream.setByteOrder(QDataStream::LittleEndian);
        FRAME_HEAD
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x0C);
        stream << static_cast<uint8_t>(0x01);
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x53);
        stream << static_cast<uint8_t>(0x03);
        stream << static_cast<uint8_t>(0x00);
        FRAME_END
        Q_EMIT m_comm_pool.at("audio")->send_requested(tmp);
    });

    connect(qobject_cast<QPushButton*>(this->m_audio_btns.at("查询当前播放文件名")), &QPushButton::clicked, this, [this] {
        QByteArray  tmp{};
        QDataStream stream{&tmp, QIODevice::WriteOnly};
        stream.setByteOrder(QDataStream::LittleEndian);
        FRAME_HEAD
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x0C);
        stream << static_cast<uint8_t>(0x01);
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x53);
        stream << static_cast<uint8_t>(0x04);
        stream << static_cast<uint8_t>(0x00);
        FRAME_END
        Q_EMIT m_comm_pool.at("audio")->send_requested(tmp);
    });

    connect(qobject_cast<QPushButton*>(this->m_audio_btns.at("查询版本")), &QPushButton::clicked, this, [this] {
        QByteArray  tmp{};
        QDataStream stream{&tmp, QIODevice::WriteOnly};
        stream.setByteOrder(QDataStream::LittleEndian);
        FRAME_HEAD
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x0C);
        stream << static_cast<uint8_t>(0x01);
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x53);
        stream << static_cast<uint8_t>(0xF1);
        stream << static_cast<uint8_t>(0x00);
        FRAME_END
        Q_EMIT m_comm_pool.at("audio")->send_requested(tmp);
    });

    connect(qobject_cast<QPushButton*>(this->m_audio_btns.at("开始传输文本")), &QPushButton::clicked, this, [this] {
        QByteArray tmp{};

        QDataStream stream{&tmp, QIODevice::WriteOnly};
        stream.setByteOrder(QDataStream::LittleEndian);
        QString text{qobject_cast<QLineEdit*>(this->m_audio_btns.at("传输文本内容"))->text()};
        uint8_t texi_size{12 + sizeof(text)};
        FRAME_HEAD
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x0C);
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x57);
        stream << static_cast<uint8_t>(0x03);
        stream << static_cast<uint8_t>(0x00);
        FRAME_END
        Q_EMIT m_comm_pool.at("audio")->send_requested(tmp);
        Q_EMIT m_comm_pool.at("audio")->send_requested(text.toUtf8());
    });

    connect(qobject_cast<QPushButton*>(this->m_audio_btns.at("结束传输并播放")), &QPushButton::clicked, this, [this] {
        QByteArray  tmp{};
        QDataStream stream{&tmp, QIODevice::WriteOnly};
        stream.setByteOrder(QDataStream::LittleEndian);
        FRAME_HEAD
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x0C);
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x00);
        stream << static_cast<uint8_t>(0x57);
        stream << static_cast<uint8_t>(0x04);
        stream << static_cast<uint8_t>(0x00);
        FRAME_END
        Q_EMIT m_comm_pool.at("audio")->send_requested(tmp);
    });
}
