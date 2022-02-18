#pragma once

struct PlayerInput {
    float axis_forward = 0;
    float axis_right = 0;
    float axis_up = 0;
    float mouse_delta_x = 0;
    float mouse_delta_y = 0;

    void clear_deltas() {
        this->mouse_delta_x = 0;
        this->mouse_delta_y = 0;
    }
};
