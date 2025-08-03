#include "ViewLayout.h"

#include <QLabel>
#include <ranges>

#include "MainWidget.h"

ViewLayout::ViewLayout(MainWidget* _widget) : m_widget{_widget}
{
    std::invoke(&ViewLayout::layout_widget, this);
}

ViewLayout::~ViewLayout() noexcept
{
    delete m_login_layout;
    delete m_select_layout;
}

auto ViewLayout::layout_widget() noexcept -> void
{
    std::invoke(&ViewLayout::layout_user, this);
    std::invoke(&ViewLayout::layout_select, this);
    std::invoke(&ViewLayout::layout_stack, this);

    m_widget->m_log->setReadOnly(true);

    m_widget->m_widget_layout->addLayout(m_login_layout, Qt::AlignTop);
    m_widget->m_widget_layout->addLayout(m_select_layout, 0);
    m_widget->m_widget_layout->addLayout(m_widget->m_stack_layout, 2);
    m_widget->m_widget_layout->addWidget(m_widget->m_log, 1);
}

auto ViewLayout::layout_user() noexcept -> void
{
    qobject_cast<QLineEdit*>(m_widget->m_net_config.at("IP"))->setText("192.168.1.201");
    qobject_cast<QLineEdit*>(m_widget->m_net_config.at("Port_audio"))->setText("23001");
    qobject_cast<QLineEdit*>(m_widget->m_net_config.at("Port_light"))->setText("23002");
    m_widget->m_net_config.at("IP")->setFixedSize(180, 30);
    m_widget->m_net_config.at("Port_audio")->setFixedSize(140, 30);
    m_widget->m_net_config.at("Port_light")->setFixedSize(140, 30);
    m_widget->m_net_config.at("Connect")->setFixedSize(100, 35);
    m_login_layout->addWidget(new QLabel{"IP地址", m_widget}, 0);
    m_login_layout->addWidget(m_widget->m_net_config.at("IP"), 2);
    m_login_layout->addWidget(new QLabel{"喊话器端口", m_widget}, 0);
    m_login_layout->addWidget(m_widget->m_net_config.at("Port_audio"), 1);
    m_login_layout->addWidget(new QLabel{"探照灯端口", m_widget}, 0);
    m_login_layout->addWidget(m_widget->m_net_config.at("Port_light"), 1);
    m_login_layout->addWidget(m_widget->m_net_config.at("Connect"), 1);
    m_login_layout->addStretch(1);
}

auto ViewLayout::layout_select() noexcept -> void
{
    m_select_layout->setSpacing(0);
    for (auto& btn : m_widget->m_group_select | std::views::values)
    {
        btn->setFixedHeight(30);
        m_select_layout->addWidget(btn);
    }
    m_select_layout->addStretch();
}

auto ViewLayout::layout_stack() noexcept -> void
{
    std::invoke(&ViewLayout::stack_layout_audio, this);
    std::invoke(&ViewLayout::stack_layout_light, this);
    for (auto& group : m_widget->m_group | std::views::values)
    {
        m_widget->m_stack_layout->addWidget(group);
    }
}

auto ViewLayout::stack_layout_audio() noexcept -> void
{
    for (auto& btn : m_widget->m_audio_btns | std::views::values)
    {
        btn->setFixedSize(140, 35);
        btn->setParent(m_widget);
    }
    QVBoxLayout* audio_layout{new QVBoxLayout{m_widget->m_group.at("audio")}};
    QHBoxLayout* voice_layout{new QHBoxLayout{}};
    QHBoxLayout* upload_layout{new QHBoxLayout{}};
    QHBoxLayout* play_layout{new QHBoxLayout{}};
    QHBoxLayout* handle_layout{new QHBoxLayout{}};
    QHBoxLayout* appoint_layout{new QHBoxLayout{}};
    QHBoxLayout* query_layout{new QHBoxLayout{}};

    voice_layout->addWidget(m_widget->m_audio_btns.at("开启语音"));
    voice_layout->addWidget(m_widget->m_audio_btns.at("结束语音"));
    voice_layout->addWidget(new QLabel{"文本内容", m_widget});
    voice_layout->addWidget(m_widget->m_audio_btns.at("传输文本内容"));
    voice_layout->addWidget(m_widget->m_audio_btns.at("开始传输文本"));
    voice_layout->addWidget(m_widget->m_audio_btns.at("结束传输并播放"));
    voice_layout->addStretch();

    upload_layout->addWidget(new QLabel{"文件名称", m_widget});
    upload_layout->addWidget(m_widget->m_audio_btns.at("上传文件名输入"));
    upload_layout->addWidget(m_widget->m_audio_btns.at("打开音频文件上传"));
    upload_layout->addWidget(m_widget->m_audio_btns.at("关闭音频文件上传"));
    upload_layout->addStretch();

    appoint_layout->addWidget(new QLabel{"文件名称", m_widget});
    appoint_layout->addWidget(m_widget->m_audio_btns.at("播放指定音频文件名输入"));
    appoint_layout->addWidget(m_widget->m_audio_btns.at("播放指定音频"));
    appoint_layout->addWidget(m_widget->m_audio_btns.at("删除音频"));
    appoint_layout->addWidget(m_widget->m_audio_btns.at("删除录音"));
    appoint_layout->addStretch();

    query_layout->addWidget(m_widget->m_audio_btns.at("查询列表"));
    query_layout->addWidget(m_widget->m_audio_btns.at("查询音量"));
    query_layout->addWidget(m_widget->m_audio_btns.at("查询播放状态"));
    query_layout->addWidget(m_widget->m_audio_btns.at("查询云台角度"));
    query_layout->addWidget(m_widget->m_audio_btns.at("查询当前播放文件名"));
    query_layout->addWidget(m_widget->m_audio_btns.at("查询版本"));
    query_layout->addStretch();

    play_layout->addWidget(m_widget->m_audio_btns.at("音频继续播放"));
    play_layout->addWidget(m_widget->m_audio_btns.at("音频暂停播放"));
    play_layout->addWidget(m_widget->m_audio_btns.at("音频停止播放"));
    play_layout->addWidget(m_widget->m_audio_btns.at("音频上一曲"));
    play_layout->addWidget(m_widget->m_audio_btns.at("音频下一曲"));
    play_layout->addStretch();

    handle_layout->addWidget(new QLabel{"音量", m_widget});
    handle_layout->addWidget(m_widget->m_audio_btns.at("音量"));
    handle_layout->addWidget(m_widget->m_audio_btns.at("调节音量"));
    handle_layout->addWidget(m_widget->m_audio_btns.at("开启循环"));
    handle_layout->addWidget(m_widget->m_audio_btns.at("关闭循环"));
    handle_layout->addStretch();

    audio_layout->addLayout(voice_layout);
    audio_layout->addLayout(upload_layout);
    audio_layout->addLayout(appoint_layout);
    audio_layout->addLayout(query_layout);
    audio_layout->addLayout(play_layout);
    audio_layout->addLayout(handle_layout);
    audio_layout->addStretch();
}

auto ViewLayout::stack_layout_light() noexcept -> void
{
    for (auto& btn : m_widget->m_light_btns | std::views::values)
    {
        btn->setFixedSize(140, 35);
        btn->setParent(m_widget);
    }
    qobject_cast<QSpinBox*>(m_widget->m_light_btns.at("亮度输入-左灯"))->setRange(0, 100);
    qobject_cast<QSpinBox*>(m_widget->m_light_btns.at("亮度输入-右灯"))->setRange(0, 100);
    qobject_cast<QSpinBox*>(m_widget->m_light_btns.at("俯仰值"))->setRange(0, 1800);
    qobject_cast<QSpinBox*>(m_widget->m_light_btns.at("航向值"))->setRange(0, 1800);
    QVBoxLayout* light_layout{new QVBoxLayout{m_widget->m_group.at("light")}};
    QHBoxLayout* base_layout{new QHBoxLayout{}};
    QHBoxLayout* brightness_layout{new QHBoxLayout{}};
    QHBoxLayout* control_layout_pitch{new QHBoxLayout{}};
    QHBoxLayout* control_layout_direction{new QHBoxLayout{}};
    QHBoxLayout* light_quert_layout{new QHBoxLayout{}};

    base_layout->addWidget(m_widget->m_light_btns.at("灯光控制-关"));
    base_layout->addWidget(m_widget->m_light_btns.at("灯光控制-爆闪"));
    base_layout->addWidget(m_widget->m_light_btns.at("灯光控制-常亮"));
    base_layout->addStretch();

    brightness_layout->addWidget(new QLabel{"左灯", m_widget});
    brightness_layout->addWidget(m_widget->m_light_btns.at("亮度输入-左灯"));
    brightness_layout->addWidget(new QLabel{"右灯", m_widget});
    brightness_layout->addWidget(m_widget->m_light_btns.at("亮度输入-右灯"));
    brightness_layout->addWidget(m_widget->m_light_btns.at("灯光亮度"));
    brightness_layout->addStretch();

    control_layout_pitch->addWidget(new QLabel{"俯仰值", m_widget});
    control_layout_pitch->addWidget(m_widget->m_light_btns.at("俯仰值"));
    control_layout_pitch->addWidget(m_widget->m_light_btns.at("云台俯仰+"));
    control_layout_pitch->addWidget(m_widget->m_light_btns.at("云台俯仰-"));
    control_layout_pitch->addStretch();

    control_layout_direction->addWidget(new QLabel{"航向值", m_widget});
    control_layout_direction->addWidget(m_widget->m_light_btns.at("航向值"));
    control_layout_direction->addWidget(m_widget->m_light_btns.at("云台航向+"));
    control_layout_direction->addWidget(m_widget->m_light_btns.at("云台航向-"));
    control_layout_direction->addStretch();

    light_quert_layout->addWidget(m_widget->m_light_btns.at("灯光查询"));
    light_quert_layout->addWidget(m_widget->m_light_btns.at("角度查询"));
    light_quert_layout->addStretch();

    light_layout->addLayout(base_layout);
    light_layout->addLayout(brightness_layout);
    light_layout->addLayout(control_layout_pitch);
    light_layout->addLayout(control_layout_direction);
    light_layout->addLayout(light_quert_layout);
    light_layout->addStretch();
}
