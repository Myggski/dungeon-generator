#pragma once

#include <array>
#include <queue>
#include <stack>
#include <vector>

#include "RuleLevelCell.h"

namespace LevelGeneration
{
    class Level;
}

namespace Command
{
    class AddSideRoomsCommand;
    class AddElementCommand;
    class DeadEndCommand;
    class BacktrackPassageCommand;
    class CarvePassageCommand;
    class CreateNewPathCommand;
    class CreateLowResLevelCommand;
}

namespace LevelGenerator
{

    // Todo: Fix better names
    enum class GeneratorActionType : uint16_t
    {
        None = 0,
        CarvingPath = 1 << 1,
        SavingPath = 1 << 2,
        CreatingPath = 1 << 3,
        ReachedDeadEnd = 1 << 4,
        AddMainRuleElements = 1 << 5,
        FillEmptySlots = 1 << 6,
        Failed = 1 << 7,
        Done = 1 << 8,
    };

    class RuleLevelGenerator;

    struct RuleLevelStateData final
    {
    public:
        RuleLevelStateData(int GridWidth, int GridHeight);

        std::vector<RuleLevelCell*>& GetCurrentPathway();
        std::vector<RuleLevelCell*>& GetPathway(int InsertionIndex);
        void AddCellToPathway(RuleLevelCell* Cell);
        void RemoveLastCellFromPathway();
        void ReversePathway(int InsertionIndex);
        bool IsPathwayEmpty() const;
        bool IsStartCell(RuleLevelCell* Cell) const;
        bool IsGoalCell(RuleLevelCell* Cell) const;
        void AddPreviousDirection(DirectionType Direction);
        int GetNumberOfEmptySlots();

        // TODO: Move GetAvailableDirections, IsOutOfBound and GetNeighborCell to a different file

        /**
         * \brief Checks the surroundings of the current cell and see if there are any unvisited cells
         * \return A list of available directions
         */
        std::vector<DirectionType> GetAvailableDirections(RuleLevelCell* Cell);

        /**
         * \brief Checks if the wanted position in the grid is out of bound
         * \param PositionX X-axis position in the 2D grid
         * \param PositionY Y-axis position in the 2D grid
         * \return Returns true if it's out of bound, else it returns false
         */
        bool IsOutOfBound(int PositionX, int PositionY) const;

        /**
         * \brief Returns the neighbor cell by direction
         * \param From Where to go from
         * \param Direction The direction to go to
         * \return Returns the neighboring cell, if it's not a valid direction it returns nullptr
         */
        RuleLevelCell* GetNeighborCell(RuleLevelCell* From, DirectionType Direction);

    private:
        int GridWidth;
        int GridHeight;
        int CurrentInsertionIndex;
        RuleLevelCell* CurrentCell;
        RuleLevelCell* StartCell;
        RuleLevelCell* GoalCell;
        std::vector<std::vector<RuleLevelCell>> LevelGrid;
        std::stack<RuleLevelCell*> VisitedCellStack;
        std::array<std::vector<RuleLevelCell*>, 2> InsertionPathways;
        std::queue<DirectionType> PreviousDirections;
        GeneratorActionType CurrentAction;

        friend class LevelGenerator;
        friend class LevelGeneration::Level;
        friend class Command::BacktrackPassageCommand;
        friend class Command::CarvePassageCommand;
        friend class Command::CreateNewPathCommand;
        friend class Command::DeadEndCommand;
        friend class Command::AddElementCommand;
        friend class Command::AddSideRoomsCommand;
        friend class Command::CreateLowResLevelCommand;
    };
}
