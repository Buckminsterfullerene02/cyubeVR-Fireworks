#pragma once

#include "GameAPI.h"

#include <vector>
#include <map>

class Custom
{
public:
    /*
     * Debug.
     * Logs the current coordinate to the output file.
     */
    static void LogCoordinate(const std::wstring& text, CoordinateInBlocks coords);

    /*
     * Check for a delay block in the delay config file.
     */
    static std::pair<int, bool> CheckDelayBlock(const CoordinateInBlocks& coords, const std::wstring& saveName);
 
    /*
     * Makes a new delay block entry in the delay config file, based off the save name.
     * Delay is in ms.
     */
    static void SaveDelayConfig(const CoordinateInBlocks& coords, const int& delay, const std::wstring& saveName);

    /*
     * Checks for neighboring blocks, and returns a vector of their coordinates.
     */
    static std::vector<CoordinateInBlocks> CheckSurroundingBlocks(CoordinateInBlocks At, const std::vector<CoordinateInBlocks>& KnownNeighbours, const std::vector<UniqueID>& IDsToCheck);
 
    /*
     * Returns the highest distance between each neighbour and the center block.
     */
    static CoordinateInBlocks GetHighestNeighbourDistance(const std::vector<CoordinateInBlocks>& RelativeNeighbours, const CoordinateInBlocks& Center);

private:
    /*
     * Returns the path to the delay config file.
     */
    static std::wstring GetFilePath(const std::wstring& saveName);

    /*
     * Delete the delay config file.
     */
    static void DeleteFile(const std::wstring& saveName);
};
