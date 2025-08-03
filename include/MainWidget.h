_Pragma("once");
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QStackedLayout>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>
#include <map>

#include "CommUdp.h"

class ViewLayout;

class MainWidget : public QWidget
{
    Q_OBJECT
    friend class ViewLayout;

public:
    explicit(true) MainWidget(QWidget* _parent = nullptr);
    ~MainWidget() noexcept = default;

private:
    auto init_widget() noexcept -> void;

    auto connect_signal_slot_base() noexcept -> void;

    auto connect_signal_slot_login() noexcept -> void;

    auto connect_signal_slot_audio() noexcept -> void;

    auto connect_signal_slot_light() noexcept -> void;

public Q_SLOTS:
    auto print_log(const QByteArray& _data) -> void;

private:
    QVBoxLayout*    m_widget_layout{new QVBoxLayout{this}};
    QStackedLayout* m_stack_layout{new QStackedLayout{}};
    ViewLayout*     m_view{nullptr};
    QTextEdit*      m_log{new QTextEdit{this}};

    std::map<QString, CommUdp*> m_comm_pool{
        {"audio", new CommUdp{}},
        {"light", new CommUdp{}},
    };

    std::map<QString, QWidget*> m_net_config{
        {"IP", new QLineEdit{}},
        {"Port_audio", new QLineEdit{}},
        {"Port_light", new QLineEdit{}},
        {"Connect", new QPushButton{"连接"}},
    };

    std::map<QString, QPushButton*> m_group_select{
        {"audio", new QPushButton{"音频功能", this}},
        {"light", new QPushButton{"灯光控制", this}},
    };

    std::map<QString, QGroupBox*> m_group{
        {"audio", new QGroupBox{"音频功能", this}},
        {"light", new QGroupBox{"灯光控制", this}},
    };

    std::map<QString, QWidget*> m_light_btns{
        {"灯光控制-关", new QPushButton{"灯光控制-关"}},
        {"灯光控制-爆闪", new QPushButton{"灯光控制-爆闪"}},
        {"灯光控制-常亮", new QPushButton{"灯光控制-常亮"}},
        {"亮度输入-左灯", new QSpinBox{}},
        {"亮度输入-右灯", new QSpinBox{}},
        {"灯光亮度", new QPushButton{"灯光亮度"}},
        {"俯仰值", new QSpinBox{}},
        {"云台俯仰+", new QPushButton{"云台俯仰+"}},
        {"云台俯仰-", new QPushButton{"云台俯仰-"}},
        {"航向值", new QSpinBox{}},
        {"云台航向+", new QPushButton{"云台航向+"}},
        {"云台航向-", new QPushButton{"云台航向-"}},
        {"灯光查询", new QPushButton{"灯光查询"}},
        {"角度查询", new QPushButton{"角度查询"}},
    };

    std::map<QString, QWidget*> m_audio_btns{
        {"开启语音", new QPushButton{"开启语音"}},
        {"结束语音", new QPushButton{"结束语音"}},
        {"上传文件名输入", new QLineEdit{}},
        {"打开音频文件上传", new QPushButton{"打开音频文件上传"}},
        {"关闭音频文件上传", new QPushButton{"关闭音频文件上传"}},
        {"播放指定音频文件名输入", new QLineEdit{}},
        {"播放指定音频", new QPushButton{"播放指定音频"}},
        {"音频继续播放", new QPushButton{"音频继续播放"}},
        {"音频暂停播放", new QPushButton{"音频暂停播放"}},
        {"音频停止播放", new QPushButton{"音频停止播放"}},
        {"音频上一曲", new QPushButton{"音频上一曲"}},
        {"音频下一曲", new QPushButton{"音频下一曲"}},
        {"音量", new QSpinBox{}},
        {"调节音量", new QPushButton{"调节音量"}},
        {"开启循环", new QPushButton{"开启循环"}},
        {"关闭循环", new QPushButton{"关闭循环"}},
        {"删除音频", new QPushButton{"删除音频"}},
        {"删除录音", new QPushButton{"删除录音"}},
        {"查询列表", new QPushButton{"查询列表"}},
        {"传输文本内容", new QLineEdit{}},
        {"开始传输文本", new QPushButton{"开始传输文本"}},
        {"结束传输并播放", new QPushButton{"结束传输并播放"}},
        {"查询音量", new QPushButton{"查询音量"}},
        {"查询播放状态", new QPushButton{"查询播放状态"}},
        {"查询云台角度", new QPushButton{"查询云台角度"}},
        {"查询当前播放文件名", new QPushButton{"查询当前播放文件名"}},
        {"查询版本", new QPushButton{"查询版本"}},
    };
};
