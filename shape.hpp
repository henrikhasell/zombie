#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <vector>
#include <iostream>
#include <glm/vec2.hpp>

struct Shape
{
    Shape()
    {
        std::cout << "Creating shape." << std::endl;
    }

    ~Shape()
    {
        std::cout << "Destroying shape." << std::endl;
    }

    std::vector<glm::vec2> points;
};

#endif