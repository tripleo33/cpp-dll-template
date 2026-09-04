#include <windows.h>
#include <cmath>
#include <algorithm>
#include <numbers>

// Improved Airstrafe Implementation
namespace movement {

    class ImprovedAirstrafe {
    private:
        float m_velocity[3] = {0, 0, 0};
        float m_last_angles[3] = {0, 0, 0};
        bool m_strafe_enabled = true;
        float m_strafe_sensitivity = 1.0f;
        float m_max_speed = 250.0f;
        float m_air_accel = 12.0f;

    public:
        void on_create_move(void* cmd, float* current_angles) {
            if (!should_strafe(cmd)) {
                return;
            }

            // Get current velocity from game state
            get_velocity(m_velocity);
            
            float speed_2d = std::sqrtf(m_velocity[0] * m_velocity[0] + m_velocity[1] * m_velocity[1]);
            
            if (speed_2d < 5.0f) {
                return;  // Too slow to strafe
            }

            // Calculate optimal strafe direction
            float target_yaw = calculate_strafe_angle(current_angles[1], speed_2d);
            
            // Apply smooth movement
            apply_movement_input(cmd, target_yaw, current_angles[1], speed_2d);
        }

    private:
        bool should_strafe(void* cmd) {
            if (!m_strafe_enabled) return false;
            
            // Check if in air
            if (is_on_ground()) return false;
            
            // Check if jumping (W key held or recently pressed)
            if (get_buttons() & 0x04) return false;  // IN_JUMP
            
            return true;
        }

        float calculate_strafe_angle(float view_yaw, float speed) {
            // Dynamic strafe angle based on speed
            // At max speed, strafe more aggressively
            float speed_ratio = std::clamp(speed / m_max_speed, 0.0f, 1.0f);
            
            // Base strafing angle: 90 degrees
            // Increase up to 135 degrees at max speed for better acceleration
            float strafe_angle = 90.0f + (speed_ratio * 45.0f);
            
            // Alternate sides each frame for classic AA bypass
            static bool strafe_left = true;
            float direction = strafe_left ? 1.0f : -1.0f;
            strafe_left = !strafe_left;
            
            return view_yaw + (strafe_angle * direction);
        }

        void apply_movement_input(void* cmd, float target_yaw, float view_yaw, float speed) {
            // Calculate angle difference
            float angle_diff = target_yaw - view_yaw;
            
            // Normalize to [-180, 180]
            while (angle_diff > 180.0f) angle_diff -= 360.0f;
            while (angle_diff < -180.0f) angle_diff += 360.0f;
            
            // Smooth transition based on angle
            float forward_move = 0.0f;
            float side_move = 0.0f;
            
            if (std::fabsf(angle_diff) < 90.0f) {
                forward_move = 1.0f * (1.0f - std::fabsf(angle_diff) / 90.0f);
                side_move = (angle_diff > 0.0f) ? -1.0f : 1.0f;
            } else {
                side_move = (angle_diff > 0.0f) ? -1.0f : 1.0f;
            }
            
            // Apply movement with smoothing
            set_move_input(cmd, forward_move * m_strafe_sensitivity, side_move * m_strafe_sensitivity);
        }

        void get_velocity(float* out_vel) {
            // TODO: Read velocity from game memory
            // out_vel[0] = vel_x
            // out_vel[1] = vel_y
            // out_vel[2] = vel_z
        }

        bool is_on_ground() {
            // TODO: Check entity flags
            return false;
        }

        int get_buttons() {
            // TODO: Read button state
            return 0;
        }

        void set_move_input(void* cmd, float forward, float side) {
            // TODO: Write movement to usercmd
        }
    };
}
