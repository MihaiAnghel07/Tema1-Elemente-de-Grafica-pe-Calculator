#include "object2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* object2D::CreateSquare(
    const std::string &name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    } else {
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* object2D::CreatePlayer(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    glm::vec3 color2,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner + glm::vec3(0.1f * length, 0.1f * length, 0), color),
        VertexFormat(corner + glm::vec3(0.1f * length, 0.5f * length, 0), color),
        VertexFormat(corner + glm::vec3((length / 2) + 0.1, 0.5f * length, 0), color),
        VertexFormat(corner + glm::vec3(length + 0.1, 0.1f * length, 0), color),
        VertexFormat(corner + glm::vec3(length + 0.1, 0.5f * length, 0), color),
        VertexFormat(corner + glm::vec3((length / 2) + 0.1, length + 0.1f, 0), color),
        
        VertexFormat(corner + glm::vec3(0.2f * length, 0, 0), color2),
        VertexFormat(corner + glm::vec3(0.2f * length, 0.4f * length, 0), color2),
        VertexFormat(corner + glm::vec3(0, 0.4f * length, 0), color2),
        VertexFormat(corner + glm::vec3(0, 0, 0), color2),

        VertexFormat(corner + glm::vec3(1.2f * length, 0, 0), color2),
        VertexFormat(corner + glm::vec3(1.2f * length, 0.4f * length, 0), color2),
        VertexFormat(corner + glm::vec3(length, 0.4f * length, 0), color2),
        VertexFormat(corner + glm::vec3(length, 0, 0), color2),
        
    };

    Mesh* player = new Mesh(name);
    std::vector<unsigned int> indices = 
        { 0, 1, 2,
          2, 3, 4,
          1, 5, 4,
          6, 7, 8,
          6, 8, 9,
          10, 11, 12,
          10, 12, 13};

        
    player->SetDrawMode(GL_TRIANGLES);
    player->InitFromData(vertices, indices);
    return player;
}

Mesh* object2D::CreateEnemy(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    glm::vec3 color2,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner + glm::vec3(0, 0.7f * length, 0), color2),
        VertexFormat(corner + glm::vec3(0.6f * length, 0.7f * length, 0), color2),
        VertexFormat(corner + glm::vec3(0.6f * length, 1.3f * length, 0), color2),
        VertexFormat(corner + glm::vec3(0, 1.3f * length, 0), color2),

        VertexFormat(corner + glm::vec3(length, 0.7f * length, 0), color2),
        VertexFormat(corner + glm::vec3(1.6f * length, 0.7f * length, 0), color2),
        VertexFormat(corner + glm::vec3(1.6f * length, 1.3f * length, 0), color2),
        VertexFormat(corner + glm::vec3(length, 1.3f * length, 0), color2),
        
        VertexFormat(corner + glm::vec3(0.3f * length, 0, 0), color),
        VertexFormat(corner + glm::vec3(1.3f * length, 0, 0), color),
        VertexFormat(corner + glm::vec3(1.3f * length, length, 0), color),
        VertexFormat(corner + glm::vec3(0.3f * length, length, 0), color),
       
    };

    Mesh* enemy = new Mesh(name);
    std::vector<unsigned int> indices = 
            { 0, 1, 2,
              0, 2, 3,
              4, 5, 6,
              4, 6, 7,
              8, 9, 10,
              8, 10, 11};

    
    enemy->SetDrawMode(GL_TRIANGLES);
    enemy->InitFromData(vertices, indices);
    return enemy;
}

Mesh* object2D::CreateHealthBarFrame(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner + glm::vec3(0, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color),

    };

    Mesh* frame = new Mesh(name);
    std::vector<unsigned int> indices =
        { 0, 1,
          1, 2,
          2, 3,
          3, 4};


    frame->SetDrawMode(GL_LINE_LOOP);
    frame->InitFromData(vertices, indices);
    return frame;
}
