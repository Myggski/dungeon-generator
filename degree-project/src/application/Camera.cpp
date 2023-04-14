#include "Camera.h"

#include <stdexcept>

#include "Keyboard.h"

namespace Application
{
    std::unique_ptr<Camera> Camera::Instance = nullptr;

    Camera::Camera()
	    : bMoveCamera(false),
		MouseStartPositionX(std::numeric_limits<float>::min()),
	    MouseStartPositionY(std::numeric_limits<float>::min()),
	    PositionX(-448),
	    PositionY(-232) { }

    void Camera::CreateInstance()
    {
        if (Instance != nullptr) {
            throw std::logic_error("Instance of Keyboard has already been created!");
        }

        Instance = std::make_unique<Camera>(Camera());
    }

    Camera& Camera::GetInstance()
    {
        if (Instance == nullptr)
        {
            throw std::logic_error("Instance of Keyboard has not been created yet, or has been destroyed!");
        }

        return *Instance;
    }

    void Camera::Update()
    {
        int MousePositionX { std::numeric_limits<int>::min() };
        int MousePositionY { std::numeric_limits<int>::min() };

        SDL_GetMouseState(&MousePositionX, &MousePositionY);

        if (bMoveCamera)
        {
            if (MouseStartPositionX == std::numeric_limits<float>::min() && MouseStartPositionY == std::numeric_limits<float>::min())
            {
                MouseStartPositionX = PositionX + static_cast<float>(MousePositionX);
                MouseStartPositionY = PositionY + static_cast<float>(MousePositionY);
            }
        }
        else
        {
            MouseStartPositionX = std::numeric_limits<float>::min();
            MouseStartPositionY = std::numeric_limits<float>::min();
        }

        if (MouseStartPositionX != std::numeric_limits<float>::min() && MouseStartPositionY != std::numeric_limits<float>::min())
        {
            PositionX = static_cast<int>(MouseStartPositionX - static_cast<float>(MousePositionX));
            PositionY = static_cast<int>(MouseStartPositionY - static_cast<float>(MousePositionY));
        }
    }

    void Camera::ToggleMobility(bool bMoveCamera)
    {
        this->bMoveCamera = bMoveCamera;
    }

    void Camera::Zoom(float ZoomValue)
    {
        int MousePositionX{ std::numeric_limits<int>::min() };
        int MousePositionY{ std::numeric_limits<int>::min() };

        SDL_GetMouseState(&MousePositionX, &MousePositionY);

        float GameMouseX = (MousePositionX / this->ZoomValue) + PositionX;
        float GameMouseY = (MousePositionY / this->ZoomValue) + PositionY;

        this->ZoomValue += ZoomValue;
        this->ZoomValue = std::max(0.1f, this->ZoomValue);

        float NewGameMouseX = (MousePositionX / this->ZoomValue) + PositionX;
        float NewGameMouseY = (MousePositionY / this->ZoomValue) + PositionY;

        const float DiffX = NewGameMouseX - GameMouseX;
        const float DiffY = NewGameMouseY - GameMouseY;

        PositionX -= DiffX;
        PositionY -= DiffY;
    }
}
