#include "keyboard.h"

namespace application
{
    keyboard_key_data::keyboard_key_data(SDL_Scancode scancode) : scancode(scancode) { }

    void keyboard::on_key_pressed(SDL_Event event)
    {
        if (is_key_released(event.key.keysym.scancode))
        {
            keys[event.key.keysym.scancode] = keyboard_key_data(event.key.keysym.scancode);
        }
        else
        {
            keys.at(event.key.keysym.scancode).number_of_repeats += 1;
        }
    }

    void keyboard::on_key_released(SDL_Event event)
    {
        keys.erase(event.key.keysym.scancode);
    }

    bool keyboard::is_key_pressed(const SDL_Scancode scancode) const
    {
        return keys.contains(scancode);
    }

    bool keyboard::is_key_released(const SDL_Scancode scancode) const
    {
        return !is_key_pressed(scancode);
    }

    bool keyboard::is_key_held(const SDL_Scancode scancode) const
    {
        return keys.contains(scancode);
    }
}