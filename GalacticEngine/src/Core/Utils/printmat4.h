#pragma once
#include "PrintMat4.h"
#include <iostream>
#include <string>

inline void PrintMat4(const GalacticEngine::Matrix4& m, const std::string& name = "")
{
    if (!name.empty())
        std::cout << name << ":\n";

    for (int row = 0; row < 4; ++row)
    {
        std::cout << "[ ";
        for (int col = 0; col < 4; ++col)
        {
            // Your Matrix4 is column-major: m[col * 4 + row]
            std::cout << m(row, col) << " ";
        }
        std::cout << "]\n";
    }

    std::cout << std::endl;
}