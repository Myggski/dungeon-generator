#pragma once
#include <optional>
#include <SDL_rect.h>

namespace Cyclic
{
	class CyclicNode
	{
	public:
		CyclicNode(SDL_Point Position);

	private:
		static int NextId;
		int Id;

		friend class CyclicGraph;
	};

	class CyclicGraph
	{
	

	public:
		CyclicGraph();

		int AddNode();
		void AddEdge();

	private:
		std::optional<CyclicNode> Nodes[5][4];
	};
}
