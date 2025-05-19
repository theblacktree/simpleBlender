#ifndef UVSPHERE_H
#define UVSPHERE_H
#include <vector>
#include "object.h"
class UVSphere:public Object
{
public:
    UVSphere();

    void initialize()override;

    void draw(GLuint currentProgram)override;

    std::vector<Vertex>createSphereVertices(float radius, int nLongitude, int mLatitude);

private:

    // 球体参数
    float m_radius = 1.0f; // 球体半径
    int m_nLongitude = 50; // 经度分割数
    int m_mLatitude = 50; // 纬度分割数

    std::vector<Vertex>m_vertices;
    std::vector<unsigned int> m_indices;
};

#endif // UVSPHERE_H
