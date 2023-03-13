#define SDL_MAIN_HANDLED

#include "Application/Application.h"
#include "LevelGeneration/Cyclic/CyclicInsertionPoint.h"
#include "LevelGeneration/Cyclic/CyclicRuleRepository.h"

int main(int argc, char* args[])
{
	Application::Application App {  };

	App.Init();
	App.Run();

	Cyclic::CyclicInsertionPoint LongLockKey { Cyclic::ArcType::Long, Cyclic::InsertionType::LockKey };
	Cyclic::CyclicInsertionPoint ShortLockKey { Cyclic::ArcType::Short, Cyclic::InsertionType::LockKey };
	Cyclic::CyclicInsertionPoint ShortMiniBoss { Cyclic::ArcType::Short, Cyclic::InsertionType::MiniBoss };
	Cyclic::CyclicInsertionPoint ShortTraps { Cyclic::ArcType::Short, Cyclic::InsertionType::Traps };

	Cyclic::CyclicRuleRepository RuleRepository{ 4 };

	RuleRepository.Add({
		"Two Locks and Keys - Short",
		{  ShortLockKey, ShortLockKey },
		Cyclic::GoalType::KillTarget
	});

	/*RuleRepository.Add({
		"Two Locks and Keys - Long",
		{ LongLockKey, LongLockKey },
		Cyclic::GoalType::KillTarget | Cyclic::GoalType::SecretDocuments | Cyclic::GoalType::Treasure
	});

	RuleRepository.Add({
		"Two Locks and Keys - Short and Long",
		{ LongLockKey, ShortLockKey },
		Cyclic::GoalType::KillTarget
	});

	RuleRepository.Add({
		"MiniBossOrTrap - Short and Short",
		{ ShortTraps, ShortMiniBoss },
		Cyclic::GoalType::Treasure | Cyclic::GoalType::SecretDocuments,
	});*/

	return 0;
}
