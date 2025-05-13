#include "pch.h"
#include "ColorMap.h"
#include "resource.h"
#include <wtypes.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

ImColor ColorMap::InterpolateColor(const ImColor& c1, const ImColor& c2, float t) {
    float c1r = c1.Value.x;
    float c1g = c1.Value.y;
    float c1b = c1.Value.z;

    float c2r = c2.Value.x;
    float c2g = c2.Value.y;
    float c2b = c2.Value.z;

    float r = c1r + (c2r - c1r) * t;
    float g = c1g + (c2g - c1g) * t;
    float b = c1b + (c2b - c1b) * t;
    return ImColor(r, g, b);
}

ImColor ColorMap::SampleColorMap(float t, std::vector<ImColor>& colorMap) {
    // Clamp t
    t = std::clamp(t, 0.0f, 1.0f);

    float scaledT = t * (colorMap.size() - 1);
    int index = static_cast<int>(std::floor(scaledT));
    float localT = scaledT - index;

    if (index >= static_cast<int>(colorMap.size()) - 1)
    {
        return colorMap.back();
    }

    return InterpolateColor(colorMap[index], colorMap[index + 1], localT);
}

std::vector<ImColor> ColorMap::LoadColorMap(ColorMapName colormapName)
{
    std::vector<ImColor> grayColorMap;
    grayColorMap.push_back(ImColor(0, 0, 0, 1));
    grayColorMap.push_back(ImColor(1, 1, 1, 1));

    // Load content from dll
    HMODULE hModule = NULL;
    bool isModuleFound = GetModuleHandleEx(
        GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
        GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        (LPCTSTR)&ColorMap::LoadColorMap, &hModule);

    HRSRC myResource = ::FindResource(hModule, MAKEINTRESOURCE(colormapName), RT_RCDATA);
    if (myResource == nullptr)
    {
        return grayColorMap;
    }

    unsigned int myResourceSize = ::SizeofResource(hModule, myResource);
    HGLOBAL myResourceData = ::LoadResource(hModule, myResource);
    if (myResourceData == nullptr)
    {
        return grayColorMap;
    }
    void* pMyBinaryData = ::LockResource(myResourceData);

    // parse into color map
    std::string line;
    std::vector<ImColor> returned;
    std::string csvContent(static_cast<char*>(pMyBinaryData), myResourceSize);

    // Utiliser un stringstream pour lire ligne par ligne
    std::stringstream ss(csvContent);
    while (std::getline(ss, line)) {
        std::stringstream ss(line);
        std::string item;
        std::vector<float> numbers;

        while (std::getline(ss, item, ',')) {
            try {
                numbers.push_back((float)std::stod(item));
            }
            catch (const std::invalid_argument& e) {
                return grayColorMap;
            }
        }

        if (numbers.size() == 3) {
            returned.push_back(ImColor(numbers[0], numbers[1], numbers[2], 1.0));
        }
        else {
            return grayColorMap;
        }
    }
   
    return returned;
}