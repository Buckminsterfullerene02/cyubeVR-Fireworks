#include "Custom.h"

void Custom::LogCoordinate(const std::wstring& text, CoordinateInBlocks coords)
{
	Log(text + L"X="
		+ std::to_wstring(coords.X) + L", Y="
		+ std::to_wstring(coords.Y) + L", Z="
		+ std::to_wstring(coords.Z));
}

void Custom::SaveDelayConfig(const UniqueID& CustomBlockID, const CoordinateInBlocks& coords)
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
	path.append(L"\\UE4Mods\\Fireworks__V1\\IDs.txt");
	std::ofstream file(path);
	if (file.is_open()) {
		file << std::to_string(CustomBlockID) << " " << coords.X << " " << coords.Y << " " << coords.Z << std::endl;
		file.close();
	}
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

