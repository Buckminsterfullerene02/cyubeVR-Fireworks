#include <thread>
#include <chrono>

#include "GameAPI.h"
#include "Custom.h"

/************************************************************
	Config Variables (Set these to whatever you need. They are automatically read by the game.)
*************************************************************/

UniqueID ThisModUniqueIDs[] = { 6942069, 6942070, 6942071, 6942072, 6942073, 6942074}; // RGB, Blue, Green, Red, White, Delay

float TickRate = 1;							 // Set how many times per second Event_Tick() is called. 0 means the Event_Tick() function is never called.

/************************************************************* 
//	Functions (Run automatically by the game, you can put any code you want into them)
*************************************************************/

// Run every time a block is placed
void Event_BlockPlaced(CoordinateInBlocks At, UniqueID CustomBlockID, bool Moved)
{
	std::wstring hintText;
	if (CustomBlockID == ThisModUniqueIDs[5]) hintText = L"Hit me with any pickaxe to edit the delay settings!";
	else hintText = L"Hit me with a stick or arrow to spawn a firework!";
	SpawnHintText(At + CoordinateInBlocks(0, 0, 1), hintText, 2);

	// If it is a delay block, add it to the list of delay blocks in the world
	if (CustomBlockID == ThisModUniqueIDs[5])
	{
		Custom::SaveDelayConfig(At, 1000, GetWorldName());
		SpawnBPModActor(At + CoordinateInBlocks(0, 0, 0), L"Fireworks", L"DelayMenu");
	}
}


// Run every time a block is destroyed
void Event_BlockDestroyed(CoordinateInBlocks At, UniqueID CustomBlockID, bool Moved)
{
	// If it is a delay block, delete it from the list of delay blocks in the world
	if (CustomBlockID == ThisModUniqueIDs[5]) Custom::CheckDelayBlock(At, GetWorldName());
}


// Run every time a block is hit by a tool
void Event_BlockHitByTool(CoordinateInBlocks At, UniqueID CustomBlockID, wString ToolName, CoordinateInCentimeters ExactHitLocation, bool ToolHeldByHandLeft)
{
	if (ToolName.starts_with(L"T_Pickaxe_") && CustomBlockID == ThisModUniqueIDs[5])
	{
		// Spawn the delay menu 
		SpawnBPModActor(At + CoordinateInBlocks(0, 0, 0), L"Fireworks", L"DelayMenu");
	}
	
	if (ToolName == L"T_Stick" || ToolName == L"T_Arrow")
	{
		std::wstring actorName;
		if (CustomBlockID == ThisModUniqueIDs[1]) actorName = L"BFirework";
		else if (CustomBlockID == ThisModUniqueIDs[2]) actorName = L"GFirework";
		else if (CustomBlockID == ThisModUniqueIDs[3]) actorName = L"RFirework";
		else if (CustomBlockID == ThisModUniqueIDs[4]) actorName = L"WFirework";
		else if (CustomBlockID == ThisModUniqueIDs[5]) actorName = L"DFirework";
		else actorName = L"RGBFirework";

		SpawnBPModActor(At + CoordinateInBlocks(0, 0, 1), L"Fireworks", actorName);

		// TODO: Do we not want to run the rest of the code for a delay block?
		if (actorName == L"DFirework") return;
		
		const std::vector<CoordinateInBlocks> surroundingBlocks = Custom::CheckSurroundingBlocks(At, {},
			std::vector(ThisModUniqueIDs, ThisModUniqueIDs + std::size(ThisModUniqueIDs)));
		const CoordinateInBlocks highestNeighbourDistance = Custom::GetHighestNeighbourDistance(surroundingBlocks, At);
		Custom::LogCoordinate(L"[Fireworks]: Highest neighbour distance is ", highestNeighbourDistance);

		// Get the highest of X, Y, Z
		const int highest = max(std::abs(highestNeighbourDistance.X),
			max(std::abs(highestNeighbourDistance.Y), std::abs(highestNeighbourDistance.Z)));
		
		// Loop up to the highest X, Y, Z coordinate in highestNeighbourDistance
		// Each loop, spawn the BPModActor for each surroundingBlock in that range
		for (int a = 0; a < highest; a++)
		{
			for (auto surroundingBlock : surroundingBlocks)
			{
				if (surroundingBlock.X == a || surroundingBlock.Y == a || surroundingBlock.Z == a)
				{
					std::pair<int, bool> out = Custom::CheckDelayBlock(surroundingBlock, GetWorldName());
					if (out.second) std::this_thread::sleep_for(std::chrono::milliseconds(out.first));
					SpawnBPModActor(surroundingBlock + CoordinateInBlocks(0, 0, 1), L"Fireworks", actorName);
				}
			}
		}
	}
}


// Run X times per second, as specified in the TickRate variable at the top
void Event_Tick()
{

}


// Run once when the world is loaded
void Event_OnLoad(bool CreatedNewWorld)
{

}

// Run once when the world is exited
void Event_OnExit()
{
	
}

/*******************************************************

	Advanced functions

*******************************************************/


// Run every time any block is placed by the player
void Event_AnyBlockPlaced(CoordinateInBlocks At, BlockInfo Type, bool Moved)
{

}

// Run every time any block is destroyed by the player
void Event_AnyBlockDestroyed(CoordinateInBlocks At, BlockInfo Type, bool Moved)
{

}

// Run every time any block is hit by a tool
void Event_AnyBlockHitByTool(CoordinateInBlocks At, BlockInfo Type, wString ToolName, CoordinateInCentimeters ExactHitLocation, bool ToolHeldByHandLeft)
{

}