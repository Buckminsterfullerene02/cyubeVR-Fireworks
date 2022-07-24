#include "Custom.h"

#include <fstream>
#include <sstream>

void Custom::LogCoordinate(const std::wstring& text, CoordinateInBlocks coords)
{
	Log(text + L"X="
		+ std::to_wstring(coords.X) + L", Y="
		+ std::to_wstring(coords.Y) + L", Z="
		+ std::to_wstring(coords.Z));
}

void Custom::CheckDelayBlock(const CoordinateInBlocks& coords, const std::wstring& saveName, bool isDelete, bool isActive = false)
{
	/* File is in the following format:
	 * x coord, y coord, z coord, delay, is active\n
	 * e.g.
	 * 1,1,1,1000,true\n3,2,2,1000,false\n4,3,3,1000,false\n
	*/
	std::wstring path = GetFilePath(saveName);
	if (std::ifstream file(path); file.is_open())
	{
		std::string line;
		while (std::getline(file, line))
		{
			// Split string into tokens
			std::vector<std::string> tokens;
			std::stringstream ss(line);
			std::string token;
			while (std::getline(ss, token, ','))
			{
				tokens.push_back(token);
			}
			// Check if the block is in the file (it should always be)
			if (tokens[0] == std::to_string(coords.X)
				&& tokens[1] == std::to_string(coords.Y)
				&& tokens[2] == std::to_string(coords.Z))
			{
				// Block is in the file, so update the block line
				file.close();
				std::ofstream out(path, std::ios::out | std::ios::trunc);
				if (isDelete) out << line << std::endl;
				else
				{
					out << std::to_string(coords.X) << ","
					<< std::to_string(coords.Y) << ","
					<< std::to_string(coords.Z) << ","
					<< tokens[3] << ","
					<< std::to_string(isActive) << std::endl;
				}
				out.close();
				return;
			}
		}
		file.close();
	}
	else Log(L"Could not open file " + path);
}

void Custom::SaveDelayConfig(const CoordinateInBlocks& coords, const int& delay, const std::wstring& saveName)
{
	/* File is in the following format:
	 * x coord, y coord, z coord, delay, is active\n
	 * e.g.
	 * 1,1,1,1000,true\n3,2,2,1000,false\n4,3,3,1000,false\n
	*/
	if (std::ofstream file(GetFilePath(saveName)); file.is_open())
	{
		// Add block to file - if it already exists (it shouldn't), it will be overwritten
		// The isHit is always going to be false when created
		file << coords.X << "," << coords.Y << "," << coords.Z << "," << delay << "," << false << std::endl;
		file.close();
	}
	else Log(L"Failed to open file " + GetFilePath(saveName));
}

std::vector<CoordinateInBlocks> Custom::CheckSurroundingBlocks(const CoordinateInBlocks At,
                                                               const std::vector<CoordinateInBlocks>& KnownNeighbours,
                                                               const std::vector<UniqueID>& IDsToCheck)
{
	std::vector<CoordinateInBlocks> surroundings;
	int neighbours = 0;
	for (int x = -1; x < 2; x++)
	{
		for (int y = -1; y < 2; y++)
		{
			for (int z = -1; z < 2; z++)
			{
				// If block is in the list of known neighbours, skip it
				bool isNeighbour = false;
				for (const auto known : KnownNeighbours)
				{
					if (At + CoordinateInBlocks(x, y, z) == known)
					{
						LogCoordinate(L"Skipping known neighbour", At + CoordinateInBlocks(x, y, z));
						isNeighbour = true;
					}
				}

				if (isNeighbour) continue;
				if (x == 0 && y == 0 && z == 0) continue;

				// If block is in the list of IDs to check, add it to the list of surroundings
				if (std::count(IDsToCheck.begin(), IDsToCheck.end(),
					GetBlock(At + CoordinateInBlocks(x, y, z)).CustomBlockID))
				{
					neighbours++;
					surroundings.push_back(At + CoordinateInBlocks(x, y, z));
					LogCoordinate(L"[CSB]: Added neighbour num=" + std::to_wstring(neighbours)
						+ L" at ", At + CoordinateInBlocks(x, y, z));
				}
			}
		}
	}
	
	if (neighbours > 0)
	{
		Log(L"[CSB]: Neighbour count at check: " + std::to_wstring(neighbours));
		for (const auto surrounding : surroundings)
		{
			std::vector<CoordinateInBlocks> moreSurroundings = CheckSurroundingBlocks(surrounding, surroundings, IDsToCheck);
			surroundings.insert(surroundings.end(), moreSurroundings.begin(), moreSurroundings.end());
		}
	}
	return surroundings;
}

CoordinateInBlocks Custom::GetHighestNeighbourDistance(const std::vector<CoordinateInBlocks>& RelativeNeighbours, const CoordinateInBlocks& Center)
{
	CoordinateInBlocks furthest = CoordinateInBlocks(0, 0, 0);
	for (auto neighbour : RelativeNeighbours)
	{
		LogCoordinate(L"Specific neighbour is ", neighbour);
		neighbour = Center - neighbour;
		LogCoordinate(L"Relative neighbour is ", neighbour);
		if (std::abs(neighbour.X) > std::abs(furthest.X)) furthest.X = neighbour.X;
		if (std::abs(neighbour.Y) > std::abs(furthest.Y)) furthest.Y = neighbour.Y;
		if (std::abs(neighbour.Z) > std::abs(furthest.Z)) furthest.Z = neighbour.Z;
	}
	return furthest;
}

std::wstring Custom::GetFilePath(const std::wstring& saveName)
{
	std::wstring path = GetThisModInstallFolderPath();
	int count = 0;
	for (unsigned long long i = path.length() - 1; i > 0; i--) {
		if (path[i] == '\\') count++;
		if (count == 3) {
			path = path.substr(0, i);
			break;
		}
	}
	path.append(L"\\UE4Mods\\Fireworks__V1\\");
	path.append(saveName + L"\\DelayConfig.json");
	return path;
}

void Custom::DeleteFile(const std::wstring& saveName)
{
	std::wstring path = GetFilePath(saveName);
	const std::string path_s(path.begin(), path.end());
	if (std::remove(path_s.c_str()) != 0) Log(L"Error deleting file " + path);
}