_Pragma("once");

#include <QComboBox>
#include <QFormLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMouseEvent>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>
#include <cmath>
#include <functional>
#include <unordered_map>

#include "SerialPort.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget* parent = nullptr);
    ~Widget() = default;

private:
    /// @brief 界面ui
    void initUi() noexcept;

    /// @brief 窗口信息
    void initWindowInfo() noexcept;

    /// @brief 总布局
    void initWindowLayout() noexcept;

    /// @brief 串口设置布局
    QGroupBox* initSerialPortLayout() noexcept;

    /// @brief 接收窗口布局
    QGroupBox* initRecvLayout() noexcept;

    /// @brief 发送窗口布局
    QGroupBox* initSendLayout() noexcept;

    /// @brief 日志信息布局
    QGroupBox* initDataLogLayout() noexcept;

    /// @brief 连接信号与槽
    void signalToSlot() noexcept;

    /// @brief 显示连接状态
    void showSeriaoPortStatus() noexcept;

    /// @brief 接收数据交互界面
    void dataToUi(const bool& _tag) noexcept;

    /// @brief 发送数据交互界面
    void uiToData() noexcept;

protected:
    void mousePressEvent(QMouseEvent* event) override;

private:
    constexpr static inline uint16_t WIN_WIDTH{1200};
    constexpr static inline uint16_t WIN_HEIGHT{720};

    std::unordered_map<QString, QWidget*> m_serialPortInfo{
        {"串口名称", new QComboBox{this}},
        {"波特率", new QComboBox{this}},
        {"数据位", new QLineEdit{this}},
        {"停止位", new QLineEdit{this}},
        {"校验位", new QLineEdit{this}},
    };

    std::unordered_map<QString, QWidget*> m_toolButton{
        {"连接", new QPushButton{"连接", this}},
        {"断开", new QPushButton{"断开", this}},
        {"连接状态", new QLabel{"●", this}},
    };

    std::unordered_map<QString, QWidget*> m_recvDataInfo{
        {"光电产品自检结果", new QLabel{this}},
        {"陀螺启动状态", new QLabel{this}},
        {"光轴归零状态", new QLabel{this}},
        {"电视探测器状态", new QLabel{this}},
        {"红外探测器状态", new QLabel{this}},
        {"激光测距状态", new QLabel{this}},
        {"光类型", new QLabel{this}},
        {"电锁状态", new QLabel{this}},
        {"框架移动状态", new QLabel{this}},
        {"俯仰框架角", new QLabel{this}},
        {"偏航框架角", new QLabel{this}},
        {"陀螺仪俯仰角速度", new QLabel{this}},
        {"陀螺仪偏航角速度", new QLabel{this}},
        {"俯仰视线角速度", new QLabel{this}},
        {"偏航视线角速度", new QLabel{this}},
        {"俯仰失调角", new QLabel{this}},
        {"偏航失调角", new QLabel{this}},
    };

    std::unordered_map<QString, QWidget*> m_sendAngleInfo{
        {"波门/框架角俯仰指令", new QLineEdit{"0", this}},
        {"波门/框架角偏航指令", new QLineEdit{"0", this}},
        {"俯仰角", new QLineEdit{"0", this}},
        {"滚转角", new QLineEdit{"0", this}},
        {"航向角", new QLineEdit{"0", this}},
        {"载体系X轴向角速度", new QLineEdit{"0", this}},
        {"载体系Y轴向角速度", new QLineEdit{"0", this}},
        {"载体系Z轴向角速度", new QLineEdit{"0", this}},
    };

    std::unordered_map<QString, QWidget*> m_sendTargetInfo{
        {"捕获目标状态", new QLineEdit{"0", this}},
        {"捕获目标X坐标", new QLineEdit{"0", this}},
        {"捕获目标Y坐标", new QLineEdit{"0", this}},
        {"跟踪俯仰偏差", new QLineEdit{"0", this}},
        {"跟踪偏航偏差", new QLineEdit{"0", this}},
        {"俯仰框架角", new QLineEdit{"0", this}},
        {"偏航框架角", new QLineEdit{"0", this}},
    };

    std::unordered_map<QString, QWidget*> m_sendCoordinateInfo{
        {"经度", new QLineEdit{"0", this}},
        {"纬度", new QLineEdit{"0", this}},
        {"高度", new QLineEdit{"0", this}},
    };

    std::unordered_map<QString, QWidget*> m_sendComboxInfo{
        {"开关控制量", new QComboBox{this}},
        {"光电产品状态1", new QComboBox{this}},
        {"光电产品状态2", new QComboBox{this}},
        {"炮口状态", new QComboBox{this}},
        {"攻击状态", new QComboBox{this}},
        {"定位标志", new QComboBox{this}},
    };

    std::unordered_map<QString, QWidget*> m_dataLogInfo{
        {"日志信息", new QTextEdit{this}},
        {"更新速率", new QLabel{this}},
        {"发送", new QPushButton{"发送", this}},
        {"清空", new QPushButton{"清空", this}},
    };

    // 串口通信
    SerialPort m_serial_port{};

    // 热插拔监听
    QTimer* m_hotPlugTimer{new QTimer{this}};

    // 接收信息定时器
    QTimer* m_readBinaryTimer{new QTimer{this}};

    // 发送信息定时器
    QTimer* m_sendBinaryTimer{new QTimer{this}};

    // 发送数据缓存区
    QByteArray m_sendByteArray{};

    std::mutex m_mtx{};
};
