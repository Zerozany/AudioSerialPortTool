_Pragma("once");
#include <bitset>
#include <cstddef>
#include <cstdint>

namespace QJ
{
    struct Speed
    {
        constexpr static inline uint32_t update{10};                     // ms
        constexpr static inline uint32_t baud_rates[2]{230400, 921600};  // kbps
    };

    struct ProductCheckResult
    {
        std::bitset<2> self_status{};               // 光电自检状态
        std::bitset<1> top_status{};                // 陀螺启动状态
        std::bitset<1> optic_axis_status{};         // 光轴归零状态
        std::bitset<1> tv_detector{};               // 电视探测器状态
        std::bitset<1> infrared_detector_status{};  // 红外探测器状态
        std::bitset<1> laser_ranging_status{};      // 激光测距状态
        std::bitset<1> standby_7{};                 // 备用
    };

    struct ProjectWorkStatus
    {
        std::bitset<1> light_type{};         // 光类型
        std::bitset<1> lock_status{};        // 电锁状态
        std::bitset<1> frame_move_status{};  // 框架移动状态
        std::bitset<1> standby_3{};          // 预留
        std::bitset<1> standby_4{};          // 预留
        std::bitset<1> standby_5{};          // 预留
        std::bitset<1> standby_6{};          // 预留
        std::bitset<1> standby_7{};          // 预留
    };

    struct RecvAngleInfo
    {
        std::int16_t pitch_frame_angle{};            // 俯仰框架角
        std::int16_t yaw_frame_angle{};              // 偏航框架角
        std::int16_t gyroscope_pitch_angle_speed{};  // 陀螺仪俯仰角速度
        std::int16_t gyroscope_yaw_angle_speed{};    // 陀螺仪偏航角速度
        std::int16_t pitch_sight_angle_speed{};      // 俯仰视线角速度
        std::int16_t yaw_sight_angle_speed{};        // 偏航视线角速度
        std::int16_t pitch_misalignment_angle{};     // 俯仰失调角
        std::int16_t yaw_misalignment_angle{};       // 偏航失调角
        std::byte standby[3]{};                      // 备用
        std::int8_t checksum{};                      // 校验和
    };

    struct SendAngleInfo
    {
        std::int16_t b_frame_pitch_command{};  // 波门/框架俯仰指令
        std::int16_t b_frame_yaw_command{};    // 波门/框架偏航指令
        std::int8_t standby_5{};               // 预留
        std::int16_t pitch_angle{};            // 俯仰角
        std::int16_t roll_angle{};             // 滚转角
        std::int16_t yaw_angle{};              // 偏航角
        std::int16_t x_axis_angle_speed{};     // 载体系X轴向角速度
        std::int16_t y_axis_angle_speed{};     // 载体系Y轴向角速度
        std::int16_t z_axis_angle_speed{};     // 载体系Z轴向角速度
        std::int8_t standby_12{};              // 预留
    };

    struct ProjectTag
    {
        std::bitset<1> gun_status{};          // 炮口查询标志
        std::bitset<1> attack_status{};       // 攻击状态
        std::bitset<1> positioning_status{};  // 定位结束标志
        std::bitset<1> laser_status{};        // 激光判断标志位
        std::bitset<1> standby_4{};           // 备用
        std::bitset<1> standby_5{};           // 备用
        std::bitset<1> standby_6{};           // 备用
        std::bitset<1> standby_7{};           // 备用
    };

    struct GeographicInfo
    {
        std::int32_t longitude{};  // 经度
        std::int32_t latitude{};   // 纬度
        std::int32_t height{};     // 高度
    };

    struct ProjectStatus
    {
        std::bitset<2> gaze_track{};    // 凝视状态
        std::bitset<3> track_status{};  // 跟踪状态
        std::bitset<3> standby_5_7{};   // 预留
    };

    struct SendTargetInfo
    {
        std::int8_t capture_target_status{};  // 捕获目标状态
        std::int16_t capture_target_x{};      // 捕获目标X坐标
        std::int16_t capture_target_y{};      // 捕获目标Y坐标
        std::int32_t track_pitch_diff{};      // 跟踪俯仰偏差
        std::int32_t track_yaw_diff{};        // 跟踪偏航偏差
        std::int16_t pitch_frame_angle{};     // 俯仰框架角
        std::int16_t yaw_frame_angle{};       // 偏航框架角
        std::byte standby[16]{};              // 备用
        std::int8_t checksum{};               // 校验和
    };

}  // namespace QJ
