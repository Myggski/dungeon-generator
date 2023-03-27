#pragma once
#include <memory>

#include "Events.h"
#include "Renderer.h"

namespace Application
{
	class Camera final
	{
	public:
		static void CreateInstance();
		static Camera& GetInstance();
		
		void Update();
		void ToggleMobility(bool bMoveCamera);
		EventCallback UpdateMouseStatus(SDL_Event Event);

	private:
		Camera();

	private:
		static std::unique_ptr<Camera> Instance;

		bool bMoveCamera;

		float MouseStartPositionX;
		float MouseStartPositionY;

		float PositionX;
		float PositionY;

		friend class Renderer;
	};
}
