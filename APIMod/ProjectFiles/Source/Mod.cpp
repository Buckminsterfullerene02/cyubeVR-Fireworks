#include <fstream>

#include "GameAPI.h"

/************************************************************
	Config Variables (Set these to whatever you need. They are automatically read by the game.)
*************************************************************/

UniqueID ThisModUniqueIDs[] = { 6942069, 6942070, 6942071, 6942072, 6942073 }; // RGB, Blue, Green, Red, White

float TickRate = 1;							 // Set how many times per second Event_Tick() is called. 0 means the Event_Tick() function is never called.

/************************************************************* 
//	Functions (Run automatically by the game, you can put any code you want into them)
*************************************************************/

// Run every time a block is placed
void Event_BlockPlaced(CoordinateInBlocks At, UniqueID CustomBlockID, bool Moved)
{
	SpawnHintText(At + CoordinateInBlocks(0, 0, 1), L"Hit me with a stick or arrow to spawn a firework!", 3);
}


// Run every time a block is destroyed
void Event_BlockDestroyed(CoordinateInBlocks At, UniqueID CustomBlockID, bool Moved)
{

}


// Run every time a block is hit by a tool
void Event_BlockHitByTool(CoordinateInBlocks At, UniqueID CustomBlockID, wString ToolName, CoordinateInCentimeters ExactHitLocation, bool ToolHeldByHandLeft)
{
	if (ToolName == L"T_Stick" || ToolName == L"T_Arrow") {
		std::wstring path = GetThisModInstallFolderPath();
		int count = 0;
		for (unsigned long long i = path.length() - 1; i > 0; i--) {
			if (path[i] == '\\') count++;
			if (count == 3) {
				path = path.substr(0, i);
				break;
			}
		}
		path.append(L"\\UE4Mods\\Fireworks__V1\\ID.txt");
		if (std::ofstream modInfo(path, std::ios::trunc); modInfo) {
			modInfo << CustomBlockID;
			modInfo.close();	
		}
		SpawnBPModActor(At + CoordinateInBlocks(0, 0, 1), L"Fireworks", L"Firework");
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


/*******************************************************

	For all the available game functions you can call, look at the GameAPI.h file

*******************************************************/