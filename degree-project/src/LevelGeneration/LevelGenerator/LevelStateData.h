#pragma once

#include <array>
#include <queue>
#include <stack>
#include <vector>

#include "DirectionType.h"
#include "LevelCell.h"

namespace LevelGeneration
{
    class Level;
}

namespace Command
{
	class DeadEndCommand;
	class BacktrackPassageCommand;
    class CarvePassageCommand;
    class CreateNewPathCommand;
}

namespace LevelGenerator
{
    // Todo: Fix better names
    enum class GeneratorActionType : uint8_t
    {
		None = 0,
        CarvingPath = 1 << 1,
        SavingPath = 1 << 2,
        CreatingPath = 1 << 3,
        ReachedDeadEnd = 1 << 4,
        Failed = 1 << 5,
        Done = 1 << 6,
    };

	class LevelGenerator;

	struct LevelStateData final
	{
	public:
        LevelStateData(int GridWidth, int GridHeight);

        std::vector<LevelCell*>& GetCurrentPathway();
        std::vector<LevelCell*>& GetPathway(int InsertionIndex);
        void AddCellToPathway(LevelCell* Cell);
        void RemoveLastCellFromPathway();
        void ReversePathway(int InsertionIndex);
        bool IsPathwayEmpty() const;
        void AddPreviousDirection(DirectionType Direction);

	private:
        int GridWidth;
        int GridHeight;
        int CurrentInsertionIndex;
        LevelCell* CurrentCell;
        LevelCell* StartCell;
        LevelCell* GoalCell;
        std::vector<std::vector<LevelCell>> MazeGrid;
        std::stack<LevelCell*> VisitedCellStack;
        std::array<std::vector<LevelCell*>, 2> InsertionPathways;
        std::queue<DirectionType> PreviousDirections;
        GeneratorActionType CurrentAction;

		friend class LevelGenerator;
        friend class LevelGeneration::Level;
        friend class Command::BacktrackPassageCommand;
        friend class Command::CarvePassageCommand;
        friend class Command::CreateNewPathCommand;
        friend class Command::DeadEndCommand;
	};
}
