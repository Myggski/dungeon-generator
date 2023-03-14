#define SDL_MAIN_HANDLED

#include "Application/Application.h"
#include "LevelGeneration/Cyclic/CyclicInsertionPoint.h"
#include "LevelGeneration/Cyclic/CyclicRule.h"
#include "LevelGeneration/Cyclic/CyclicRuleRepository.h"

int main(int argc, char* args[])
{
	Application::Application App {  };
	App.Init();

	Cyclic::CyclicInsertionPoint LongLockKey{ Cyclic::ArcType::Long, Cyclic::InsertionType::LockKey };
	Cyclic::CyclicInsertionPoint ShortLockKey{ Cyclic::ArcType::Short, Cyclic::InsertionType::LockKey };
	Cyclic::CyclicInsertionPoint ShortMiniBoss{ Cyclic::ArcType::Short, Cyclic::InsertionType::MiniBoss };
	Cyclic::CyclicInsertionPoint ShortTraps{ Cyclic::ArcType::Short, Cyclic::InsertionType::Traps };

	Cyclic::CyclicRuleRepository RuleRepository{ 4 };

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

	auto SelectedRule = RuleRepository.GetRandomRule();

	App.Run();

	return 0;
}
