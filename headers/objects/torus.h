#ifndef TORUS_H
#define TORUS_H
#include <vector>
#include "object.h"
class Torus: public Object
{
public:
    Torus();
    std::vector<Vertex> createTorusVertices(float R, float r, int radialSegments, int tubularSegments);

    std::vector<unsigned int> createTorusIndices(int radialSegments, int tubularSegments);

    void initialize();

    void draw(GLuint currentProgram);
private:
    float m_R = 0.5f; // 主半径
    float m_r = 0.2f; // 次半径
    int m_radialSegments = 32; // 环向分割数
    int m_tubularSegments = 24; // 管状分割数

    std::vector<Vertex>m_vertices;
    std::vector<unsigned int> m_indices;
};

#endif // TORUS_H
