_Pragma("once");
#include <QDateTime>
#include <QDebug>
#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTextEdit>
#include <QTimer>
#include <atomic>
#include <chrono>
#include <functional>
#include <mutex>

#include "DataStruct.h"

/// @brief 串口列表状态
enum struct SerialPortStatus : uint8_t
{
    INITSERIALPORT = 0x00,       // 初始化串口
    NEWSERIALPORT = 0x01,        // 串口列表变化
    NOSERIALPORTCHANGED = 0x02,  // 串口列表状态无变化
};

struct SerialPortConfig
{
    // 串口名称
    static inline QString serial_name{};
    // 波特率
    static inline qint32 baud_rate{};
    // 数据位
    static inline uint8_t data_bit{};
    // 停止位
    static inline uint8_t stop_bit{};
    // 校验位
    static inline uint8_t check_bit{};
};

class SerialPort : public QObject
{
    Q_OBJECT
public:
    explicit SerialPort(QObject* parent = nullptr);
    ~SerialPort() noexcept;

    /// @brief 获取可用串口
    QStringList getAvailabePortNames() const noexcept;

    /// @brief 设置可用串口
    SerialPortStatus setAvailabePortNames() noexcept;

    /// @brief 初始化串口
    void initSerialPort(QTextEdit* _log_edit) noexcept;

    /// @brief 关闭串口通信
    void serialPortClose() noexcept;

    /// @brief 写数据
    void writeDate(const QByteArray& _data) noexcept;

    /// @brief 获取串口连接状态
    std::atomic<bool> getSerialPortStatus() const noexcept;

    /// @brief 获取当前连接串口名称
    QString getCurrentPortName() const noexcept;

    /// @brief 获取日志打印的原始数据
    QByteArray getBinaryData() const noexcept;

private:
    /// @brief 连接信号与槽
    void
    signalToSlot() noexcept;

    /// @brief 设置串口连接状态
    void setSerialPortStatus(const bool& _tag) noexcept;

    /// @brief 设置当前连接串口缓存名称
    void setCurrentPortName(const QString& _port_name) noexcept;

    /// @brief 读数据
    void readData() noexcept;

    /// @brief 处理读取数据
    void handleData(QByteArray _read_data) noexcept;

public:
    // 产品自检结果结构体
    QJ::ProductCheckResult m_projectCheckResult{};

    // 产品工作状态结构体
    QJ::ProjectWorkStatus m_projectWorkStatus{};

    // 接收角度信息结构体
    QJ::RecvAngleInfo m_recvAngleInfo{};

private:
    // 串口类
    QSerialPort* m_serialport{new QSerialPort{this}};

    // 可用串口名称
    QStringList m_serialport_names{};

    // 读取数据缓存
    QByteArray m_read_data{};

    // 连接状态
    std::atomic<bool> m_serialport_status{false};

    // 串口名称缓存
    QString m_post_name{};

    std::mutex m_mtx{};
};
