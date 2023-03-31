#include "AddElementsCommand.h"

namespace Command
{
	AddElementCommand::AddElementCommand(const Cyclic::CyclicRule& MainRule, LevelGenerator::LevelStateData& StateData)
	: MainRule(MainRule),
	StateData(StateData) { }

	void AddElementCommand::Execute()
	{
		StateData.GetPathway(0)[static_cast<int>(StateData.GetPathway(0).size()) / 2]->AddElement(MainRule.GetElement(0));
		StateData.GetPathway(1)[static_cast<int>(StateData.GetPathway(1).size()) / 2]->AddElement(MainRule.GetElement(1));
	}

	void AddElementCommand::Undo()
	{

	}
}