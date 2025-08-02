#include "SerialPort.h"

SerialPort::SerialPort(QObject* parent) : QObject{parent}
{
    // 连接信号与槽
    std::invoke(&SerialPort::signalToSlot, this);
}

SerialPort::~SerialPort() noexcept
{
    if (m_serialport->isOpen())
    {
        m_serialport->close();
    }
}

SerialPortStatus SerialPort::setAvailabePortNames() noexcept
{
    QStringList tmp{};
    for (const auto& port : QSerialPortInfo::availablePorts())
    {
        tmp.append(port.portName());
    }
    if (m_serialport_names.size() == 0)
    {
        m_serialport_names = tmp;
        std::invoke(&SerialPort::setSerialPortStatus, this, false);
        return SerialPortStatus::INITSERIALPORT;
    }
    else if (m_serialport_names != tmp)
    {
        m_serialport_names = tmp;
        std::invoke(&SerialPort::setSerialPortStatus, this, false);
        return SerialPortStatus::NEWSERIALPORT;
    }
    else
    {
        return SerialPortStatus::NOSERIALPORTCHANGED;
    }
}

QStringList SerialPort::getAvailabePortNames() const noexcept
{
    return m_serialport_names;
}

void SerialPort::initSerialPort(QTextEdit* _log_edit) noexcept
{
    if (m_serialport->isOpen())
    {
        m_serialport->clear();
        m_serialport->close();
    }
    m_serialport->setPortName(SerialPortConfig::serial_name);
    m_serialport->setBaudRate(SerialPortConfig::baud_rate);
    m_serialport->setDataBits(static_cast<QSerialPort::DataBits>(SerialPortConfig::data_bit));
    m_serialport->setStopBits(static_cast<QSerialPort::StopBits>(SerialPortConfig::stop_bit));
    m_serialport->setParity(static_cast<QSerialPort::Parity>(SerialPortConfig::check_bit));
    m_serialport->setFlowControl(QSerialPort::NoFlowControl);
    if (!m_serialport->open(QIODevice::ReadWrite))
    {
        _log_edit->append(QDateTime::fromSecsSinceEpoch(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now().time_since_epoch()).count()).toString("HH:mm:ss") + " 串口配置" + m_serialport->errorString());
        return;
    }
    m_serialport_status.store(true);
    std::invoke(&SerialPort::setCurrentPortName, this, m_serialport->portName());
}

void SerialPort::serialPortClose() noexcept
{
    if (m_serialport->isOpen())
    {
        m_serialport->close();
    }
    m_serialport_status.store(false);
}

void SerialPort::writeDate(const QByteArray& _data) noexcept
{
    if (m_serialport->isOpen() && this->getSerialPortStatus())
    {
        m_serialport->write(_data);
    }
}

void SerialPort::readData() noexcept
{
    if (m_serialport->isOpen() && m_serialport->bytesAvailable() > 0)
    {
        std::lock_guard<std::mutex> lock{m_mtx};
        if (m_read_data.size() >= 25)
        {
            std::invoke(&SerialPort::handleData, this, m_read_data);
            m_read_data.clear();
        }
        m_read_data.append(m_serialport->readAll());
    }
}

void SerialPort::handleData(QByteArray _read_data) noexcept
{
    auto lambda{[_read_data, this]() -> bool {
        uint8_t ret{0};
        for (std::size_t i{3}; i < (_read_data.size() - 4); i++)
        {
            ret += _read_data[i];
        }
        return (ret & 0xFF) == static_cast<unsigned char>(_read_data[_read_data.size() - 1]);
    }()};
    if (_read_data[1] != 0x5A || !lambda)
    {
        return;
    }

    QByteArray tmp{_read_data.mid(3, 21)};
    QDataStream stream{&tmp, QIODevice::ReadOnly};
    stream.setByteOrder(QDataStream::LittleEndian);
    QJ::ProductCheckResult productCheckResult{};
    QJ::ProjectWorkStatus projectWorkStatus{};
    QJ::RecvAngleInfo recvAngleInfo{};
    uint8_t byte_data_1{};
    uint8_t byte_data_2{};

    stream >> byte_data_1;
    stream >> byte_data_2;
    stream >> recvAngleInfo.pitch_frame_angle;
    stream >> recvAngleInfo.yaw_frame_angle;
    stream >> recvAngleInfo.gyroscope_pitch_angle_speed;
    stream >> recvAngleInfo.gyroscope_yaw_angle_speed;
    stream >> recvAngleInfo.pitch_sight_angle_speed;
    stream >> recvAngleInfo.yaw_sight_angle_speed;
    stream >> recvAngleInfo.pitch_misalignment_angle;
    stream >> recvAngleInfo.yaw_misalignment_angle;
    stream >> recvAngleInfo.standby[0];
    stream >> recvAngleInfo.standby[1];
    stream >> recvAngleInfo.standby[2];
    stream >> recvAngleInfo.checksum;

    productCheckResult.self_status = std::bitset<2>((byte_data_1 >> 0) & 0b11);

    productCheckResult.top_status = std::bitset<1>((byte_data_1 >> 2) & 0b1);

    productCheckResult.optic_axis_status = std::bitset<1>((byte_data_1 >> 3) & 0b1);

    productCheckResult.tv_detector = std::bitset<1>((byte_data_1 >> 4) & 0b1);

    productCheckResult.infrared_detector_status = std::bitset<1>((byte_data_1 >> 5) & 0b1);

    productCheckResult.laser_ranging_status = std::bitset<1>((byte_data_1 >> 6) & 0b1);

    productCheckResult.standby_7 = std::bitset<1>((byte_data_1 >> 7) & 0b1);

    projectWorkStatus.light_type = std::bitset<1>((byte_data_2 >> 0) & 0b1);

    projectWorkStatus.lock_status = std::bitset<1>((byte_data_2 >> 1) & 0b1);

    projectWorkStatus.frame_move_status = std::bitset<1>((byte_data_2 >> 2) & 0b1);

    projectWorkStatus.standby_3 = std::bitset<1>((byte_data_2 >> 3) & 0b1);

    projectWorkStatus.standby_4 = std::bitset<1>((byte_data_2 >> 4) & 0b1);

    projectWorkStatus.standby_5 = std::bitset<1>((byte_data_2 >> 5) & 0b1);

    projectWorkStatus.standby_6 = std::bitset<1>((byte_data_2 >> 6) & 0b1);

    projectWorkStatus.standby_7 = std::bitset<1>((byte_data_2 >> 7) & 0b1);
    m_projectCheckResult = productCheckResult;
    m_projectWorkStatus = projectWorkStatus;
    m_recvAngleInfo = recvAngleInfo;
}

void SerialPort::setSerialPortStatus(const bool& _tag) noexcept
{
    if (!_tag)
    {
        for (const auto& name : m_serialport_names)
        {
            if (name == m_post_name)
            {
                return;
            }
        }
    }
    m_serialport_status.store(_tag);
}

std::atomic<bool> SerialPort::getSerialPortStatus() const noexcept
{
    return m_serialport_status.load();
}

void SerialPort::setCurrentPortName(const QString& _port_name) noexcept
{
    m_post_name = _port_name;
}

QString SerialPort::getCurrentPortName() const noexcept
{
    return m_post_name;
}

QByteArray SerialPort::getBinaryData() const noexcept
{
    return m_read_data;
}

void SerialPort::signalToSlot() noexcept
{
    // 连接串口读取事件
    connect(m_serialport, &QSerialPort::readyRead, this, &SerialPort::readData);
}
