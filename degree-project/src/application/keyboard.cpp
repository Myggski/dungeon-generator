#include "Keyboard.h"

#include <stdexcept>

namespace Application
{
    std::unique_ptr<Keyboard> Keyboard::Instance = nullptr;

    KeyboardKeyData::KeyboardKeyData(SDL_Scancode Scancode) : Scancode(Scancode) { }

    void Keyboard::CreateInstance()
    {
        if (Instance != nullptr) {
            throw std::logic_error("Instance of Keyboard has already been created!");
        }

        Instance = std::make_unique<Keyboard>(Keyboard());
    }

    Keyboard& Keyboard::GetInstance()
    {
        if (Instance == nullptr)
        {
            throw std::logic_error("Instance of Keyboard has not been created yet, or has been destroyed!");
        }

        return *Instance;
    }

    void Keyboard::OnKeyPressed(SDL_Event Event)
    {
        if (IsKeyReleased(Event.key.keysym.scancode))
        {
            Keys[Event.key.keysym.scancode] = KeyboardKeyData(Event.key.keysym.scancode);
        }
        else
        {
            Keys.at(Event.key.keysym.scancode).NumberOfRepeats += 1;
        }
    }

    void Keyboard::OnKeyReleased(SDL_Event Event)
    {
        Keys.erase(Event.key.keysym.scancode);
    }

    bool Keyboard::IsKeyPressed(const SDL_Scancode Scancode) const
    {
        return Keys.contains(Scancode);
    }

    bool Keyboard::IsKeyPressedOnce(const SDL_Scancode Scancode)
    {
        if (!IsKeyPressed(Scancode))
        {
            return false;
        }

        Keys.at(Scancode).NumberOfRepeats += 1;

        return IsKeyPressed(Scancode) && Keys.at(Scancode).NumberOfRepeats == 1;
    }


    bool Keyboard::IsKeyReleased(const SDL_Scancode Scancode) const
    {
        return !IsKeyPressed(Scancode);
    }

    bool Keyboard::IsKeyHeld(const SDL_Scancode Scancode) const
    {
        return Keys.contains(Scancode);
    }
}
