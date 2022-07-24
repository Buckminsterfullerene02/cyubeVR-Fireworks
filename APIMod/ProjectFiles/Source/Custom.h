#pragma once

#include "GameAPI.h"

#include <vector>
#include <fstream>

class Custom
{
public:
    static void LogCoordinate(const std::wstring& text, CoordinateInBlocks coords);
    static std::wstring LoadDelayConfig(const std::wstring& path);
    static void SaveDelayConfig(const UniqueID& CustomBlockID, const CoordinateInBlocks& coords);
    static std::vector<CoordinateInBlocks> CheckSurroundingBlocks(CoordinateInBlocks At, const std::vector<CoordinateInBlocks>
                                                                  & KnownNeighbours, const std::vector<UniqueID>&
                                                                  IDsToCheck);
    static CoordinateInBlocks GetHighestNeighbourDistance(const std::vector<CoordinateInBlocks>& RelativeNeighbours, const CoordinateInBlocks& Center);
};
