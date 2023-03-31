
#include "Application.h"

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer.h"
#include <cassert>
#include <SDL.h>

#include "Camera.h"
#include "CommandStack/CommandStack.h"
#include "utils/RandomGenerator.h"
#include "LevelGeneration/Level.h"

#include "Application/Font.h"
#include "Application/Configuration.h"
#include "Application/Keyboard.h"
#include "LevelGeneration/Cyclic/CyclicRuleRepository.h"
#include "LevelGeneration/LevelElement/LockKeyProperty.h"
#include "LevelGeneration/LevelElement/MiniBossProperty.h"
#include "LevelGeneration/LevelElement/RewardProperty.h"

namespace Application
{
	Application::Application()
		: Events({}),
		Time({}),
		Renderer({}),
		ExitApplication([&](SDL_Event)
		{
			bIsRunning = false;
		}),
		OnKeyDown([&](SDL_Event Event)
		{
			Keyboard::GetInstance().OnKeyPressed(Event);
		}),
		OnKeyReleased([&](SDL_Event Event)
		{
			Keyboard::GetInstance().OnKeyReleased(Event);
		}),
		OnMouseDown([&](SDL_Event Event)
		{
			if (Event.button.button == SDL_BUTTON_RIGHT)
			{
				Camera::GetInstance().ToggleMobility(true);
			}
		}),
		OnMouseUp([&](SDL_Event Event)
		{
			if (Event.button.button == SDL_BUTTON_RIGHT)
			{
				Camera::GetInstance().ToggleMobility(false);
			}
		}) { }

	void Application::Init()
	{
		assert(SDL_Init(SDL_INIT_EVERYTHING) == 0);

		Window = SDL_CreateWindow(
			"Degree Project",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			Configuration::WindowWidth,
			Configuration::WindowHeight,
			0
		);

		SDL_RaiseWindow(Window);
		Renderer.Init(Window);

		Utils::RandomGenerator::CreateInstance();
		Command::CommandStack::CreateInstance(10000);
		Keyboard::CreateInstance();
		Camera::CreateInstance();
		Font::CreateInstance(Renderer);
		
		Events.AddListener(SDL_QUIT, &ExitApplication);
		Events.AddListener(SDL_KEYDOWN, &OnKeyDown);
		Events.AddListener(SDL_KEYUP, &OnKeyReleased);
		Events.AddListener(SDL_MOUSEBUTTONDOWN, &OnMouseDown);
		Events.AddListener(SDL_MOUSEBUTTONUP, &OnMouseUp);

		Font::GetInstance().Load("Default", "resources/fonts/Silver.ttf", 32);
	}

	void Application::Run()
	{
		bIsRunning = true;
		Time.Init();

		Cyclic::CyclicInsertionPoint LongLockKey
		{
			{ std::make_unique<LevelElement::LockKeyProperty>(LevelElement::LockKeyType::LockKey) },
			Cyclic::ArcType::Long
		};

		Cyclic::CyclicInsertionPoint ShortLockKey
		{
			{ std::make_unique<LevelElement::LockKeyProperty>(LevelElement::LockKeyType::LockKey) },
			Cyclic::ArcType::Short
		};

		Cyclic::CyclicInsertionPoint ShortMiniBoss
		{
			{ std::make_unique<LevelElement::MiniBossProperty>(LevelElement::DifficultyType::Easy) },
			Cyclic::ArcType::Short
		};

		Cyclic::CyclicInsertionPoint ShortTraps
		{
			{ std::make_unique<LevelElement::RewardProperty>(LevelElement::RewardType::Tech, LevelElement::RarityType::Epic)},
			Cyclic::ArcType::Short
		};

		Cyclic::CyclicRuleRepository RuleRepository = { 4 };

		RuleRepository.Add({
			"Two Locks and Keys - Short",
			{  std::make_unique<Cyclic::CyclicInsertionPoint>(ShortLockKey), std::make_unique<Cyclic::CyclicInsertionPoint>(ShortLockKey) },
			Cyclic::GoalType::KillTarget
			});

		RuleRepository.Add({
			"Two Locks and Keys - Long",
			{ std::make_unique<Cyclic::CyclicInsertionPoint>(LongLockKey), std::make_unique<Cyclic::CyclicInsertionPoint>(LongLockKey) },
			Cyclic::GoalType::KillTarget | Cyclic::GoalType::SecretDocuments | Cyclic::GoalType::Treasure
			});

		RuleRepository.Add({
			"Two Locks and Keys - ShortLong",
			{ std::make_unique<Cyclic::CyclicInsertionPoint>(LongLockKey), std::make_unique<Cyclic::CyclicInsertionPoint>(ShortLockKey) },
			Cyclic::GoalType::SecretDocuments
			});

		RuleRepository.Add({
			"MiniBossTrap - ShortShort",
			{ std::make_unique<Cyclic::CyclicInsertionPoint>(ShortTraps), std::make_unique<Cyclic::CyclicInsertionPoint>(ShortMiniBoss) },
			Cyclic::GoalType::Treasure | Cyclic::GoalType::SecretDocuments,
			});

		LevelGeneration::Level Level{
			RuleRepository,
			static_cast<int>(static_cast<float>(Configuration::GridSize) * Configuration::GridAspectRatio),
			Configuration::GridSize
		};

		while (bIsRunning)
		{
			Time.RefreshDT();
			Events.Pull();
			Renderer.ClearCanvas();

			Camera::GetInstance().Update();
			Level.Update(Time.DeltaTime);
			Level.Draw(Renderer);

			Renderer.DrawCanvas();
		}

		Exit();
	}

	void Application::Exit()
	{
		Events.Clear();
		Renderer.Clear();

		SDL_DestroyWindow(Window);
		Window = nullptr;

		ImGui_ImplSDLRenderer_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();

		SDL_Quit();
	}
}
