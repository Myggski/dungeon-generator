#pragma once
#include <array>
#include <optional>
#include <SDL_rect.h>

#include "MazeCell.h"
#include "utils/RandomGenerator.h"

namespace MazeGenerator
{
	template<size_t Width, size_t Height>
	class Maze
	{
	public:
		Maze() { }

		void Generate()
		{
            SDL_Point CurrentCellPosition = GetRandomStartPosition();
            std::vector<SDL_Point> Visited;
            Visited.push_back(CurrentCellPosition);

            while (Visited.size() < Width * Height) {
                std::vector<SDL_Point> UnvisitedNeighbors = GetUnvisitedNeighbors(CurrentCellPosition);

                if (!UnvisitedNeighbors.empty()) {
                    // Pick a random unvisited neighbor
                    SDL_Point NextCellPosition = UnvisitedNeighbors[Utils::RandomGenerator::GetRandom<int>(0, UnvisitedNeighbors.size() - 1)];
                }
            }
		}

	private:
        SDL_Point GetRandomStartPosition()
        {
            const int GridSide = Utils::RandomGenerator::GetRandom<int>(0, 3);

            switch (GridSide) {
            case 0: // Top side
                return { Utils::RandomGenerator::GetRandom<int>(0, Width - 1), 0 };
            case 1: // Right side
                return { Width - 1,Utils::RandomGenerator::GetRandom<int>(0, Height - 1) };
            case 2: // Bottom side
                return { Utils::RandomGenerator::GetRandom<int>(0, Width - 1), Height - 1 };
            case 3: // Left side
            default:
                return { 0,Utils::RandomGenerator::GetRandom<int>(0, Height - 1) };
            }
        }

        // Returns a list of unvisited neighbors for the given cell
        std::vector<SDL_Point> GetUnvisitedNeighbors(SDL_Point Cell) {
            std::vector<SDL_Point> UnvisitedNeighbors;
            UnvisitedNeighbors.reserve(4);

            const MazeCell Top = Grid[Cell.x][Cell.y - 1];
            const MazeCell Right = Grid[Cell.x + 1][Cell.y];
            const MazeCell Bottom = Grid[Cell.x][Cell.y + 1];
            const MazeCell Left = Grid[Cell.x - 1][Cell.y];

            // Check left
            if (Cell.x > 0 && !Left.IsRoom() && !Left.IsVisited()) {
                UnvisitedNeighbors.emplace_back(Left.GetPosition());
            }

            // Check right
            if (Cell.x < Width - 1 && !Right.IsRoom() && !Right.IsVisited()) {
                UnvisitedNeighbors.emplace_back(Right.GetPosition());
            }

            // Check up
            if (Cell.y > 0 && !Top.IsRoom() && !Top.IsVisited()) {
                UnvisitedNeighbors.emplace_back(Top.GetPosition());
            }

            // Check down
            if (Cell.y < Height - 1 && !Bottom.IsRoom() && !Bottom.IsVisited()) {
                UnvisitedNeighbors.emplace_back(Bottom.GetPosition());
            }

            return UnvisitedNeighbors;
        }

	private:
		std::array<std::optional<MazeCell>> Grid[Width][Height];

	};
}
