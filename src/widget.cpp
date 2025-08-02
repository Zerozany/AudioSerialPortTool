#include "widget.h"

Widget::Widget(QWidget* parent) : QWidget{parent}
{
    std::invoke(&Widget::initUi, this);
    std::invoke(&Widget::signalToSlot, this);
    std::invoke(static_cast<void (QTimer::*)(int)>(&QTimer::start), m_hotPlugTimer, 1000);
}

void Widget::initUi() noexcept
{
    std::invoke(&Widget::initWindowInfo, this);
    std::invoke(&Widget::initWindowLayout, this);
}

void Widget::initWindowInfo() noexcept
{
    this->setWindowTitle("串口通信测试工具");
    this->resize(QSize{WIN_WIDTH, WIN_HEIGHT});
    this->setWindowState(Qt::WindowNoState);
    this->setWindowIconText("串口通信测试工具");
    this->setContentsMargins(QMargins{5, 5, 5, 5});
    this->setLayoutDirection(Qt::LeftToRight);
    this->setWindowIcon(QIcon{":/resources/app-icon.ico"});
    this->setFont(QFont{"微软雅黑"});
}

void Widget::initWindowLayout() noexcept
{
    QHBoxLayout* windowUiLayout{new QHBoxLayout{this}};
    QVBoxLayout* serialPortAndRecvLayout{new QVBoxLayout{}};
    QVBoxLayout* logAndSendLayout{new QVBoxLayout{}};
    serialPortAndRecvLayout->addWidget(std::invoke(&Widget::initSerialPortLayout, this), 1);
    serialPortAndRecvLayout->addWidget(std::invoke(&Widget::initRecvLayout, this), 3);
    logAndSendLayout->addWidget(std::invoke(&Widget::initSendLayout, this), 1);
    logAndSendLayout->addWidget(std::invoke(&Widget::initDataLogLayout, this), 1);
    windowUiLayout->addLayout(serialPortAndRecvLayout, 1);
    windowUiLayout->addLayout(logAndSendLayout, 4);
}

QGroupBox* Widget::initSerialPortLayout() noexcept
{
    QGroupBox* serialPortBox{new QGroupBox{"串口设置", this}};
    QVBoxLayout* serialportAndButtonLayout{new QVBoxLayout{serialPortBox}};
    QFormLayout* serialPortInofLayout{new QFormLayout{}};
    QHBoxLayout* serialPortButtonLayout{new QHBoxLayout{}};
    for (auto& [key, value] : m_serialPortInfo)
    {
        QLabel* label{new QLabel{key, this}};
        label->setStyleSheet("font-size: 14px;");
        value->setStyleSheet("font-size: 14px; border: 1px solid #ABCDA0; border-radius: 5px; background-color:rgb(238, 239, 238)");
        label->setFixedSize(80, 25);
        value->setFixedSize(120, 25);
        if (auto edit{qobject_cast<QLineEdit*>(value)})
        {
            edit->setMaxLength(1);
        }
        serialPortInofLayout->addRow(label, value);
    }
    qobject_cast<QComboBox*>(m_serialPortInfo["波特率"])->addItem(QString::number(QJ::Speed::baud_rates[0]));
    qobject_cast<QComboBox*>(m_serialPortInfo["波特率"])->addItem(QString::number(QJ::Speed::baud_rates[1]));
    qobject_cast<QLineEdit*>(m_serialPortInfo["数据位"])->setText("8");
    qobject_cast<QLineEdit*>(m_serialPortInfo["停止位"])->setText("1");
    qobject_cast<QLineEdit*>(m_serialPortInfo["校验位"])->setText("0");
    m_toolButton["连接"]->setFixedSize(80, 35);
    m_toolButton["断开"]->setFixedSize(80, 35);
    serialPortButtonLayout->addWidget(m_toolButton["连接"]);
    serialPortButtonLayout->addWidget(m_toolButton["断开"]);
    serialPortButtonLayout->addWidget(m_toolButton["连接状态"]);
    serialportAndButtonLayout->addLayout(serialPortInofLayout);
    serialportAndButtonLayout->addLayout(serialPortButtonLayout);
    return serialPortBox;
}

QGroupBox* Widget::initRecvLayout() noexcept
{
    QGroupBox* recvDataBox{new QGroupBox{"接收信息", this}};
    QFormLayout* recvDataLayout{new QFormLayout{recvDataBox}};
    recvDataLayout->setSpacing(5);
    for (auto& [key, value] : m_recvDataInfo)
    {
        QLabel* label{new QLabel{key, this}};
        label->setStyleSheet("font-size: 14px;");
        value->setStyleSheet("font-size: 14px; border: 1px solid #ABCDA0; border-radius: 5px;");
        label->setFixedSize(120, 25);
        value->setFixedSize(120, 25);
        recvDataLayout->addRow(label, value);
    }
    return recvDataBox;
}

QGroupBox* Widget::initSendLayout() noexcept
{
    QStringList switchControlList{
        "无效",
        "摇杆波门/框架移动(框架角)",
        "光电产品电锁解除指令",
        "框架角重置",
        "探测器切换(红外、可见光)",
        "框架移动模式打开",
        "框架移动模式关闭(默认关闭)",
    };

    QStringList productStatusList_1{
        "波门凝视",
        "框架凝视",
        "跟踪",
    };

    QStringList productStatusList_2{
        "初值",
        "稳定跟踪",
        "记忆跟踪",
        "跟踪失败",
        "跟踪盲区",
    };

    QStringList gunStatusList{
        "未进入炮口",
        "进入炮口",
    };

    QStringList attackStatusList{
        "未攻击",
        "攻击",
    };

    QStringList positioningTagList{
        "未定位",
        "已定位",
    };

    qobject_cast<QComboBox*>(m_sendComboxInfo["开关控制量"])->addItems(switchControlList);
    qobject_cast<QComboBox*>(m_sendComboxInfo["光电产品状态1"])->addItems(productStatusList_1);
    qobject_cast<QComboBox*>(m_sendComboxInfo["光电产品状态2"])->addItems(productStatusList_2);
    qobject_cast<QComboBox*>(m_sendComboxInfo["炮口状态"])->addItems(gunStatusList);
    qobject_cast<QComboBox*>(m_sendComboxInfo["攻击状态"])->addItems(attackStatusList);
    qobject_cast<QComboBox*>(m_sendComboxInfo["定位标志"])->addItems(positioningTagList);

    QGroupBox* sendDataBox{new QGroupBox{"发送信息", this}};
    QHBoxLayout* sendDataLayout{new QHBoxLayout{sendDataBox}};
    QFormLayout* sendAngleLayout{new QFormLayout{}};
    QFormLayout* sendTargetLayout{new QFormLayout{}};
    QFormLayout* sendCoordinateAndStatusLayout{new QFormLayout{}};
    sendAngleLayout->setContentsMargins(5, 5, 5, 5);
    sendTargetLayout->setContentsMargins(5, 5, 5, 5);
    sendCoordinateAndStatusLayout->setContentsMargins(5, 5, 5, 5);
    sendAngleLayout->setSpacing(15);
    sendTargetLayout->setSpacing(15);
    sendCoordinateAndStatusLayout->setSpacing(15);

    auto lambda{[&, this](const std::unordered_map<QString, QWidget*>& _map, QFormLayout* _layout) -> void {
        for (auto& [key, value] : _map)
        {
            QLabel* label{new QLabel{key, this}};
            label->setStyleSheet("font-size: 14px;");
            value->setStyleSheet("font-size: 14px; border: 1px solid #ABCDA0; border-radius: 5px; background-color:rgb(238, 239, 238)");
            if (_map == m_sendAngleInfo)
            {
                label->setFixedSize(130, 25);
                value->setFixedSize(100, 25);
            }
            else if (_map == m_sendTargetInfo)
            {
                label->setFixedSize(95, 25);
                value->setFixedSize(100, 25);
            }
            else
            {
                label->setFixedSize(95, 25);
                value->setFixedSize(200, 25);
            }
            _layout->addRow(label, value);
        }
    }};

    std::invoke(lambda, m_sendAngleInfo, sendAngleLayout);
    std::invoke(lambda, m_sendTargetInfo, sendTargetLayout);
    std::invoke(lambda, m_sendComboxInfo, sendCoordinateAndStatusLayout);
    std::invoke(lambda, m_sendCoordinateInfo, sendCoordinateAndStatusLayout);
    sendDataLayout->addLayout(sendAngleLayout);
    sendDataLayout->addLayout(sendTargetLayout);
    sendDataLayout->addLayout(sendCoordinateAndStatusLayout);
    return sendDataBox;
}

QGroupBox* Widget::initDataLogLayout() noexcept
{
    QGroupBox* dataLogBox{new QGroupBox{"日志信息", this}};
    QVBoxLayout* dataInfoLayout{new QVBoxLayout{dataLogBox}};
    QHBoxLayout* dataLogLayout{new QHBoxLayout{}};
    QHBoxLayout* dataControlLayout{new QHBoxLayout{}};
    qobject_cast<QTextEdit*>(m_dataLogInfo["日志信息"])->setReadOnly(true);
    m_dataLogInfo["日志信息"]->setStyleSheet("font-size: 14px");
    qobject_cast<QLabel*>(m_dataLogInfo["更新速率"])->setText("更新速率 10ms");
    qobject_cast<QLabel*>(m_dataLogInfo["更新速率"])->setStyleSheet("font-size: 14px;");
    qobject_cast<QPushButton*>(m_dataLogInfo["发送"])->setStyleSheet("font-size: 14px;");
    qobject_cast<QPushButton*>(m_dataLogInfo["清空"])->setStyleSheet("font-size: 14px;");
    m_dataLogInfo["发送"]->setFixedSize(80, 35);
    m_dataLogInfo["清空"]->setFixedSize(80, 35);
    dataLogLayout->addWidget(m_dataLogInfo["日志信息"]);
    dataControlLayout->addWidget(m_dataLogInfo["更新速率"]);
    dataControlLayout->addStretch();
    dataControlLayout->addWidget(m_dataLogInfo["发送"]);
    dataControlLayout->addWidget(m_dataLogInfo["清空"]);
    dataInfoLayout->addLayout(dataLogLayout);
    dataInfoLayout->addLayout(dataControlLayout);
    return dataLogBox;
}

void Widget::signalToSlot() noexcept
{
    // 串口监听
    connect(m_hotPlugTimer, &QTimer::timeout, &m_serial_port, [&, this] {
        const auto port_status{std::invoke(&SerialPort::setAvailabePortNames, &m_serial_port)};
        if (port_status == SerialPortStatus::INITSERIALPORT || port_status == SerialPortStatus::NEWSERIALPORT)
        {
            qobject_cast<QComboBox*>(m_serialPortInfo["串口名称"])->clear();
            qobject_cast<QComboBox*>(m_serialPortInfo["串口名称"])->addItems(m_serial_port.getAvailabePortNames());
            qobject_cast<QComboBox*>(m_serialPortInfo["串口名称"])->setCurrentText(m_serial_port.getCurrentPortName());
        }
        std::invoke(&Widget::showSeriaoPortStatus, this);
    });

    // 串口通信连接按钮
    connect(qobject_cast<QPushButton*>(m_toolButton["连接"]), &QPushButton::clicked, this, [&, this] {
        {
            std::lock_guard<std::mutex> lock{m_mtx};
            SerialPortConfig::serial_name = qobject_cast<QComboBox*>(m_serialPortInfo["串口名称"])->currentText();
            SerialPortConfig::baud_rate = static_cast<qint32>(qobject_cast<QComboBox*>(m_serialPortInfo["波特率"])->currentText().toInt());
            SerialPortConfig::data_bit = static_cast<uint8_t>(qobject_cast<QLineEdit*>(m_serialPortInfo["数据位"])->text().toInt());
            SerialPortConfig::stop_bit = static_cast<uint8_t>(qobject_cast<QLineEdit*>(m_serialPortInfo["停止位"])->text().toInt());
            SerialPortConfig::check_bit = static_cast<uint8_t>(qobject_cast<QLineEdit*>(m_serialPortInfo["校验位"])->text().toInt());
        }
        std::invoke(&SerialPort::initSerialPort, &m_serial_port, qobject_cast<QTextEdit*>(m_dataLogInfo["日志信息"]));
    });

    // 串口通信断开连接按钮
    connect(qobject_cast<QPushButton*>(m_toolButton["断开"]), &QPushButton::clicked, this, [&, this] {
        std::invoke(&SerialPort::serialPortClose, &m_serial_port);
    });

    // 发送按钮
    connect(qobject_cast<QPushButton*>(m_dataLogInfo["发送"]), &QPushButton::clicked, this, [&, this] {
        if (m_serial_port.getSerialPortStatus())
        {
            std::invoke(&Widget::uiToData, this);
        }
    });

    // 清空日志
    connect(qobject_cast<QPushButton*>(m_dataLogInfo["清空"]), &QPushButton::clicked, this, [&, this] {
        qobject_cast<QTextEdit*>(m_dataLogInfo["日志信息"])->clear();
    });

    // 接收数据定时器
    connect(m_readBinaryTimer, &QTimer::timeout, this, [&, this] {
        qobject_cast<QTextEdit*>(m_dataLogInfo["日志信息"])->append(QDateTime::fromSecsSinceEpoch(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now().time_since_epoch()).count()).toString("HH:mm:ss") + " : ");
        qobject_cast<QTextEdit*>(m_dataLogInfo["日志信息"])->setTextColor(Qt::red);
        if (m_serial_port.getBinaryData().size() <= 25)
        {
            qobject_cast<QTextEdit*>(m_dataLogInfo["日志信息"])->append(m_serial_port.getBinaryData().toHex(' '));
        }
        qobject_cast<QTextEdit*>(m_dataLogInfo["日志信息"])->setTextColor(Qt::black);
        if (m_serial_port.getSerialPortStatus())
        {
            std::invoke(&Widget::dataToUi, this, true);
        }
        else
        {
            std::invoke(&Widget::dataToUi, this, false);
        }
    });

    // 发送数据定时器
    connect(m_sendBinaryTimer, &QTimer::timeout, this, [&, this] {
        std::invoke(&SerialPort::writeDate, &m_serial_port, this->m_sendByteArray);
        qobject_cast<QTextEdit*>(m_dataLogInfo["日志信息"])->append(QDateTime::fromSecsSinceEpoch(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now().time_since_epoch()).count()).toString("HH:mm:ss") + " : ");
        qobject_cast<QTextEdit*>(m_dataLogInfo["日志信息"])->setTextColor(Qt::blue);
        qobject_cast<QTextEdit*>(m_dataLogInfo["日志信息"])->append(this->m_sendByteArray.toHex(' '));
        qobject_cast<QTextEdit*>(m_dataLogInfo["日志信息"])->setTextColor(Qt::black);
    });
}

void Widget::showSeriaoPortStatus() noexcept
{
    if (m_serial_port.getSerialPortStatus())
    {
        qobject_cast<QLabel*>(m_toolButton["连接状态"])->setStyleSheet("font-size: 30px; color:rgb(7, 245, 35)");
        m_readBinaryTimer->start(10);
        m_sendBinaryTimer->start(10);
    }
    else
    {
        qobject_cast<QLabel*>(m_toolButton["连接状态"])->setStyleSheet("font-size: 30px; color:rgb(243, 7, 26)");
        m_readBinaryTimer->stop();
        m_sendBinaryTimer->stop();
    }
}

void Widget::dataToUi(const bool& _tag) noexcept
{
    if (!_tag)
    {
        qobject_cast<QLabel*>(m_recvDataInfo["光电产品自检结果"])->clear();
        qobject_cast<QLabel*>(m_recvDataInfo["陀螺启动状态"])->clear();
        qobject_cast<QLabel*>(m_recvDataInfo["光轴归零状态"])->clear();
        qobject_cast<QLabel*>(m_recvDataInfo["电视探测器状态"])->clear();
        qobject_cast<QLabel*>(m_recvDataInfo["红外探测器状态"])->clear();
        qobject_cast<QLabel*>(m_recvDataInfo["激光测距状态"])->clear();
        qobject_cast<QLabel*>(m_recvDataInfo["光类型"])->clear();
        qobject_cast<QLabel*>(m_recvDataInfo["电锁状态"])->clear();
        qobject_cast<QLabel*>(m_recvDataInfo["框架移动状态"])->clear();
        qobject_cast<QLabel*>(m_recvDataInfo["俯仰框架角"])->clear();
        qobject_cast<QLabel*>(m_recvDataInfo["偏航框架角"])->clear();
        qobject_cast<QLabel*>(m_recvDataInfo["陀螺仪俯仰角速度"])->clear();
        qobject_cast<QLabel*>(m_recvDataInfo["陀螺仪偏航角速度"])->clear();
        qobject_cast<QLabel*>(m_recvDataInfo["俯仰视线角速度"])->clear();
        qobject_cast<QLabel*>(m_recvDataInfo["偏航视线角速度"])->clear();
        qobject_cast<QLabel*>(m_recvDataInfo["俯仰失调角"])->clear();
        qobject_cast<QLabel*>(m_recvDataInfo["偏航失调角"])->clear();
        return;
    }
    if (m_serial_port.m_projectCheckResult.self_status.to_ulong() == 0b01)
    {
        qobject_cast<QLabel*>(m_recvDataInfo["光电产品自检结果"])->setText("自检正常");
    }
    else if (m_serial_port.m_projectCheckResult.self_status.to_ulong() == 0b10)
    {
        qobject_cast<QLabel*>(m_recvDataInfo["光电产品自检结果"])->setText("自检故障");
    }
    else if (m_serial_port.m_projectCheckResult.self_status.to_ulong() == 0b11)
    {
        qobject_cast<QLabel*>(m_recvDataInfo["光电产品自检结果"])->setText("正在自检");
    }
    else
    {
        qobject_cast<QLabel*>(m_recvDataInfo["光电产品自检结果"])->setText("");
    }
    if (m_serial_port.m_projectCheckResult.top_status.to_ulong() == 0b0)
    {
        qobject_cast<QLabel*>(m_recvDataInfo["陀螺启动状态"])->setText("陀螺启动正常");
    }
    else
    {
        qobject_cast<QLabel*>(m_recvDataInfo["陀螺启动状态"])->setText("陀螺启动故障");
    }
    if (m_serial_port.m_projectCheckResult.optic_axis_status.to_ulong() == 0b0)
    {
        qobject_cast<QLabel*>(m_recvDataInfo["光轴归零状态"])->setText("光轴归零正常");
    }
    else
    {
        qobject_cast<QLabel*>(m_recvDataInfo["光轴归零状态"])->setText("光轴归零故障");
    }

    if (m_serial_port.m_projectCheckResult.tv_detector.to_ulong() == 0b0)
    {
        qobject_cast<QLabel*>(m_recvDataInfo["电视探测器状态"])->setText("电视探测器正常");
    }
    else
    {
        qobject_cast<QLabel*>(m_recvDataInfo["电视探测器状态"])->setText("电视探测器故障");
    }

    if (m_serial_port.m_projectCheckResult.infrared_detector_status.to_ulong() == 0b0)
    {
        qobject_cast<QLabel*>(m_recvDataInfo["红外探测器状态"])->setText("红外探测器正常");
    }
    else
    {
        qobject_cast<QLabel*>(m_recvDataInfo["红外探测器状态"])->setText("红外探测器故障");
    }

    if (m_serial_port.m_projectCheckResult.laser_ranging_status.to_ulong() == 0b0)
    {
        qobject_cast<QLabel*>(m_recvDataInfo["激光测距状态"])->setText("激光测距正常");
    }
    else
    {
        qobject_cast<QLabel*>(m_recvDataInfo["激光测距状态"])->setText("激光测距故障");
    }

    if (m_serial_port.m_projectWorkStatus.light_type.to_ulong() == 0b0)
    {
        qobject_cast<QLabel*>(m_recvDataInfo["光类型"])->setText("可见光");
    }
    else
    {
        qobject_cast<QLabel*>(m_recvDataInfo["光类型"])->setText("红外");
    }

    if (m_serial_port.m_projectWorkStatus.lock_status.to_ulong() == 0b0)
    {
        qobject_cast<QLabel*>(m_recvDataInfo["电锁状态"])->setText("电锁");
    }
    else
    {
        qobject_cast<QLabel*>(m_recvDataInfo["电锁状态"])->setText("电锁解除");
    }

    if (m_serial_port.m_projectWorkStatus.frame_move_status.to_ulong() == 0b0)
    {
        qobject_cast<QLabel*>(m_recvDataInfo["框架移动状态"])->setText("框架移动打开");
    }
    else
    {
        qobject_cast<QLabel*>(m_recvDataInfo["框架移动状态"])->setText("框架移动关闭");
    }

    qobject_cast<QLabel*>(m_recvDataInfo["俯仰框架角"])->setText(QString::number(m_serial_port.m_recvAngleInfo.pitch_frame_angle / 100) + QString{" °"});
    qobject_cast<QLabel*>(m_recvDataInfo["偏航框架角"])->setText(QString::number(m_serial_port.m_recvAngleInfo.yaw_frame_angle / 100) + QString{" °"});
    qobject_cast<QLabel*>(m_recvDataInfo["陀螺仪俯仰角速度"])->setText(QString::number(m_serial_port.m_recvAngleInfo.gyroscope_pitch_angle_speed / 100) + QString{" ° s"});
    qobject_cast<QLabel*>(m_recvDataInfo["陀螺仪偏航角速度"])->setText(QString::number(m_serial_port.m_recvAngleInfo.gyroscope_yaw_angle_speed / 100) + QString{" ° s"});
    qobject_cast<QLabel*>(m_recvDataInfo["俯仰视线角速度"])->setText(QString::number(m_serial_port.m_recvAngleInfo.pitch_sight_angle_speed / 100) + QString{" ° s"});
    qobject_cast<QLabel*>(m_recvDataInfo["偏航视线角速度"])->setText(QString::number(m_serial_port.m_recvAngleInfo.yaw_sight_angle_speed / 100) + QString{" ° s"});
    qobject_cast<QLabel*>(m_recvDataInfo["俯仰失调角"])->setText(QString::number(m_serial_port.m_recvAngleInfo.pitch_misalignment_angle / 100) + QString{" °"});
    qobject_cast<QLabel*>(m_recvDataInfo["偏航失调角"])->setText(QString::number(m_serial_port.m_recvAngleInfo.yaw_misalignment_angle / 100) + QString{" °"});
}

void Widget::uiToData() noexcept
{
    auto lambda_count{[&, this](const QByteArray& _send_data) -> std::int8_t {
        unsigned char checksum{};
        for (std::size_t i = 3; i < _send_data.size() - 1; ++i)
        {
            checksum += static_cast<unsigned char>(_send_data[i]);
        }
        return static_cast<std::int8_t>(checksum & 0xFF);
    }};

    auto lambda{[&, this]() -> std::int8_t {
        std::int8_t byte{};
        switch (qobject_cast<QComboBox*>(m_sendComboxInfo["开关控制量"])->currentIndex())
        {
            case 0:
                byte = 0x00;
                break;
            case 1:
                byte = 0xB1;
                break;
            case 2:
                byte = 0x9E;
                break;
            case 3:
                byte = 0xE9;
                break;
            case 4:
                byte = 0x53;
                break;
            case 5:
                byte = 0x6A;
                break;
            case 6:
                byte = 0xA6;
                break;
            default:
                byte = 0x00;
                break;
        }
        return byte;
    }};

    QJ::SendAngleInfo sendAngleInfo{
        .b_frame_pitch_command = static_cast<std::int16_t>(qobject_cast<QLineEdit*>(m_sendAngleInfo["波门/框架角俯仰指令"])->text().toFloat() * 10.0f),
        .b_frame_yaw_command = static_cast<std::int16_t>(qobject_cast<QLineEdit*>(m_sendAngleInfo["波门/框架角偏航指令"])->text().toFloat() * 10.0f),
        .pitch_angle = static_cast<std::int16_t>(qobject_cast<QLineEdit*>(m_sendAngleInfo["俯仰角"])->text().toFloat()),
        .roll_angle = static_cast<std::int16_t>(qobject_cast<QLineEdit*>(m_sendAngleInfo["滚转角"])->text().toFloat() * 100.0f),
        .yaw_angle = static_cast<std::int16_t>(qobject_cast<QLineEdit*>(m_sendAngleInfo["航向角"])->text().toFloat() * 100.0f),
        .x_axis_angle_speed = static_cast<std::int16_t>(qobject_cast<QLineEdit*>(m_sendAngleInfo["载体系X轴向角速度"])->text().toFloat() * 100.0f),
        .y_axis_angle_speed = static_cast<std::int16_t>(qobject_cast<QLineEdit*>(m_sendAngleInfo["载体系Y轴向角速度"])->text().toFloat() * 100.0f),
        .z_axis_angle_speed = static_cast<std::int16_t>(qobject_cast<QLineEdit*>(m_sendAngleInfo["载体系Z轴向角速度"])->text().toFloat() * 100.0f),
    };

    QJ::ProjectTag projectTag{};
    if (qobject_cast<QComboBox*>(m_sendComboxInfo["炮口状态"])->currentIndex() == 0)
    {
        projectTag.gun_status = 0b0;
    }
    else
    {
        projectTag.gun_status = 0b1;
    }

    if (qobject_cast<QComboBox*>(m_sendComboxInfo["攻击状态"])->currentIndex() == 0)
    {
        projectTag.attack_status = 0b0;
    }
    else
    {
        projectTag.attack_status = 0b1;
    }

    if (qobject_cast<QComboBox*>(m_sendComboxInfo["定位标志"])->currentIndex() == 0)
    {
        projectTag.positioning_status = 0b0;
    }
    else
    {
        projectTag.positioning_status = 0b1;
    }

    QJ::GeographicInfo geographicInfo{
        .longitude = static_cast<std::int32_t>(qobject_cast<QLineEdit*>(m_sendCoordinateInfo["经度"])->text().toFloat() * static_cast<float>(std::pow(10, 7))),
        .latitude = static_cast<std::int32_t>(qobject_cast<QLineEdit*>(m_sendCoordinateInfo["纬度"])->text().toFloat() * static_cast<float>(std::pow(10, 7))),
        .height = static_cast<std::int32_t>(qobject_cast<QLineEdit*>(m_sendCoordinateInfo["高度"])->text().toFloat() * 1000.0f),
    };

    QJ::ProjectStatus projectStatus{};
    if (qobject_cast<QComboBox*>(m_sendComboxInfo["光电产品状态1"])->currentIndex() == 0)
    {
        projectStatus.gaze_track = 0b00;
    }
    else if (qobject_cast<QComboBox*>(m_sendComboxInfo["光电产品状态1"])->currentIndex() == 1)
    {
        projectStatus.gaze_track = 0b01;
    }
    else if (qobject_cast<QComboBox*>(m_sendComboxInfo["光电产品状态1"])->currentIndex() == 2)
    {
        projectStatus.gaze_track = 0b11;
    }

    if (qobject_cast<QComboBox*>(m_sendComboxInfo["光电产品状态2"])->currentIndex() == 0)
    {
        projectStatus.track_status = 0b110;
    }
    else if (qobject_cast<QComboBox*>(m_sendComboxInfo["光电产品状态2"])->currentIndex() == 1)
    {
        projectStatus.track_status = 0b000;
    }
    else if (qobject_cast<QComboBox*>(m_sendComboxInfo["光电产品状态2"])->currentIndex() == 2)
    {
        projectStatus.track_status = 0b100;
    }
    else if (qobject_cast<QComboBox*>(m_sendComboxInfo["光电产品状态2"])->currentIndex() == 3)
    {
        projectStatus.track_status = 0b101;
    }
    else if (qobject_cast<QComboBox*>(m_sendComboxInfo["光电产品状态2"])->currentIndex() == 4)
    {
        projectStatus.track_status = 0b111;
    }

    QJ::SendTargetInfo sendTargetInfo{
        .capture_target_status = static_cast<std::int8_t>(qobject_cast<QLineEdit*>(m_sendTargetInfo["捕获目标状态"])->text().toFloat()),
        .capture_target_x = static_cast<std::int16_t>(qobject_cast<QLineEdit*>(m_sendTargetInfo["捕获目标X坐标"])->text().toFloat()),
        .capture_target_y = static_cast<std::int16_t>(qobject_cast<QLineEdit*>(m_sendTargetInfo["捕获目标Y坐标"])->text().toFloat()),
        .track_pitch_diff = static_cast<std::int32_t>(qobject_cast<QLineEdit*>(m_sendTargetInfo["跟踪俯仰偏差"])->text().toFloat()),
        .track_yaw_diff = static_cast<std::int32_t>(qobject_cast<QLineEdit*>(m_sendTargetInfo["跟踪偏航偏差"])->text().toFloat()),
        .pitch_frame_angle = static_cast<std::int16_t>(qobject_cast<QLineEdit*>(m_sendTargetInfo["俯仰框架角"])->text().toFloat()),
        .yaw_frame_angle = static_cast<std::int16_t>(qobject_cast<QLineEdit*>(m_sendTargetInfo["偏航框架角"])->text().toFloat()),
    };

    QByteArray tmp{};
    QDataStream stream{&tmp, QIODevice::WriteOnly};
    stream.setByteOrder(QDataStream::LittleEndian);
    std::uint32_t head{0xAA122D};
    stream << static_cast<std::uint8_t>((head >> 16) & 0xFF);
    stream << static_cast<std::uint8_t>((head >> 8) & 0xFF);
    stream << static_cast<std::uint8_t>(head & 0xFF);
    stream << lambda();
    stream << sendAngleInfo.b_frame_pitch_command;
    stream << sendAngleInfo.b_frame_yaw_command;
    stream << sendAngleInfo.standby_5;
    stream << sendAngleInfo.pitch_angle;
    stream << sendAngleInfo.roll_angle;
    stream << sendAngleInfo.yaw_angle;
    stream << sendAngleInfo.x_axis_angle_speed;
    stream << sendAngleInfo.y_axis_angle_speed;
    stream << sendAngleInfo.z_axis_angle_speed;
    stream << sendAngleInfo.standby_12;
    std::uint8_t tag_byte{};
    tag_byte |= (projectTag.gun_status.to_ulong() << 0);
    tag_byte |= (projectTag.attack_status.to_ulong() << 1);
    tag_byte |= (projectTag.positioning_status.to_ulong() << 2);
    tag_byte |= (projectTag.laser_status.to_ulong() << 3);
    tag_byte |= (projectTag.standby_4.to_ulong() << 4);
    tag_byte |= (projectTag.standby_5.to_ulong() << 5);
    tag_byte |= (projectTag.standby_6.to_ulong() << 6);
    tag_byte |= (projectTag.standby_7.to_ulong() << 7);
    stream << tag_byte;
    stream << geographicInfo.longitude;
    stream << geographicInfo.latitude;
    stream << geographicInfo.height;
    std::uint8_t status_byte{};
    status_byte |= (projectStatus.gaze_track.to_ulong() << 0);
    status_byte |= (projectStatus.track_status.to_ulong() << 2);
    status_byte |= (projectStatus.standby_5_7.to_ulong() << 5);
    stream << status_byte;
    stream << sendTargetInfo.capture_target_status;
    stream << sendTargetInfo.capture_target_x;
    stream << sendTargetInfo.capture_target_y;
    stream << sendTargetInfo.track_pitch_diff;
    stream << sendTargetInfo.track_yaw_diff;
    stream << sendTargetInfo.pitch_frame_angle;
    stream << sendTargetInfo.yaw_frame_angle;
    stream << sendTargetInfo.standby[0];
    stream << sendTargetInfo.standby[1];
    stream << sendTargetInfo.standby[2];
    stream << sendTargetInfo.standby[3];
    stream << sendTargetInfo.standby[4];
    stream << sendTargetInfo.standby[5];
    stream << sendTargetInfo.standby[6];
    stream << sendTargetInfo.standby[7];
    stream << sendTargetInfo.standby[8];
    stream << sendTargetInfo.standby[9];
    stream << sendTargetInfo.standby[10];
    stream << sendTargetInfo.standby[11];
    stream << sendTargetInfo.standby[12];
    stream << sendTargetInfo.standby[13];
    stream << sendTargetInfo.standby[14];
    stream << sendTargetInfo.standby[15];
    stream << lambda_count(tmp);
    std::lock_guard<std::mutex> lock{m_mtx};
    m_sendByteArray = tmp;
}

void Widget::mousePressEvent(QMouseEvent* event)
{
    auto lambda{[=, this](std::unordered_map<QString, QWidget*>& _map) -> void {
        for (auto& [key, value] : _map)
        {
            if (event->button() == Qt::LeftButton && event->pos() != value->pos())
            {
                value->clearFocus();
            }
        }
    }};
    std::invoke(lambda, m_serialPortInfo);
    std::invoke(lambda, m_sendAngleInfo);
    std::invoke(lambda, m_sendCoordinateInfo);
    std::invoke(lambda, m_sendTargetInfo);
}
