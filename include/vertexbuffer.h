#pragma once
#include <glm/glm.hpp>
#include "referencecounter.h"
#include "vertex.h"


class VertexBuffer : public ReferenceCounter {
public:
    VertexBuffer(const std::vector<Vertex> &vertices, const std::vector<uint32_t> indices) : vertices(vertices), indices(indices) {};
    ~VertexBuffer() {};

    inline const std::vector<Vertex>    &getVertices()        const { return vertices; };
    inline const std::vector<uint32_t>  &getVertexIndices()   const { return indices; };

private:
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
};