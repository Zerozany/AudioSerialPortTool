_Pragma("once");
#include <QHBoxLayout>
#include <QWidget>

class MainWidget;

class ViewLayout
{
public:
    explicit(true) ViewLayout(MainWidget* _widget);
    ~ViewLayout() noexcept;

private:
    auto layout_widget() noexcept -> void;

    auto layout_user() noexcept -> void;

    auto layout_select() noexcept -> void;

    auto layout_stack() noexcept -> void;

    auto stack_layout_audio() noexcept -> void;

    auto stack_layout_light() noexcept -> void;

private:
    MainWidget* m_widget{nullptr};

    QHBoxLayout* m_login_layout{new QHBoxLayout{}};

    QHBoxLayout* m_select_layout{new QHBoxLayout{}};
};