#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace object2D
{

    Mesh* CreateSquare(const std::string &name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
    Mesh* CreatePlayer(const std::string& name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, glm::vec3 color2, bool fill = false);
    Mesh* CreateEnemy(const std::string& name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, glm::vec3 color2, bool fill = false);
    Mesh* CreateHealthBarFrame(const std::string& name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);

}
