#pragma once

#include <array>
#include <stack>
#include <vector>

#include "DirectionType.h"
#include "MazeCell.h"

namespace LevelGeneration
{
    class Level;
}

namespace Command
{
    class BacktrackPassageCommand;
    class CarvePassageCommand;
    class CreateNewPathCommand;
}

namespace MazeGenerator
{
    enum class MazeActionType : uint8_t
    {
		None = 0,
        CarvePassage = 1 << 1,
        BacktrackPassage = 1 << 2,
        CreateNewPath = 1 << 3,
        CarvePassageFailed = 1 << 4,
        Done = 1 << 5,
    };

	class Maze;

	struct MazeStateData final
	{
	public:
        MazeStateData(int GridWidth, int GridHeight);

        std::vector<MazeCell* >& GetCurrentPathway();
        void AddCellToPathway(MazeCell* Cell);
        void RemoveLastCellFromPathway();
        bool IsPathwayEmpty() const;

	private:
        int GridWidth;
        int GridHeight;
        int CurrentPathwayIndex;
        MazeCell* CurrentCell;
        MazeCell* StartCell;
        MazeCell* GoalCell;
        std::vector<std::vector<MazeCell>> MazeGrid;
        std::stack<MazeCell*> VisitedCellStack;
        std::array<std::vector<MazeCell*>, 2> Pathways;
        DirectionType PreviousDirection;
        MazeActionType CurrentAction;

		friend class Maze;
        friend class LevelGeneration::Level;
        friend class Command::BacktrackPassageCommand;
        friend class Command::CarvePassageCommand;
        friend class Command::CreateNewPathCommand;
	};
}
