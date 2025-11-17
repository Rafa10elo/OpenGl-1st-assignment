#pragma once
#include "Polygon.h"
#include <cmath>

class Disk : public Polygon {
public:
    Disk(const glm::vec3& position, float radius, int segments = 32, float thickness = 0.0f, const glm::vec3& color = glm::vec3(1.0f)) {
        m_vertices = generateVerts(radius, segments, thickness, color);
        m_indices  = generateIndices(segments, thickness);
        setupBuffers();
        setPosition(position);
        updateModelMatrix();
    }

private:
    static std::vector<Vertex> generateVerts(float r, int seg, float thickness, const glm::vec3& color) {
        std::vector<Vertex> verts;
        if (thickness <= 0.0f) {
            verts.reserve(seg + 2);
            verts.push_back({ glm::vec3(0,0,0), glm::vec2(0.5f,0.5f), color });
            for (int i=0;i<=seg;i++){
                float a = (float)i/seg * 2.0f * M_PI;
                float x = cos(a)*r, y = sin(a)*r;
                verts.push_back({ glm::vec3(x,y,0.0f), glm::vec2((x/r)*0.5f+0.5f,(y/r)*0.5f+0.5f), color });
            }
        } else {
            float zf = thickness * 0.5f;
            float zb = -thickness * 0.5f;
            verts.push_back({ glm::vec3(0,0,zf), glm::vec2(0.5f,0.5f), color });
            for (int i=0;i<=seg;i++){
                float a = (float)i/seg * 2.0f * M_PI;
                float x = cos(a)*r,
                y = sin(a)*r;
                verts.push_back({ glm::vec3(x,y,zf), glm::vec2((x/r)*0.5f+0.5f,(y/r)*0.5f+0.5f), color });
            }
            verts.push_back({ glm::vec3(0,0,zb), glm::vec2(0.5f,0.5f), color });
            for (int i=0;i<=seg;i++){
                float a = (float)i/seg * 2.0f * M_PI;
                float x = cos(a)*r, y = sin(a)*r;
                verts.push_back({ glm::vec3(x,y,zb), glm::vec2((x/r)*0.5f+0.5f,(y/r)*0.5f+0.5f), color });
            }
        }
        return verts;
    }
    static std::vector<unsigned int> generateIndices(int seg, float thickness) {
        std::vector<unsigned int> ind;
        if (thickness <= 0.0f) {
            for (int i=1;i<=seg;i++){
                ind.push_back(0);
                ind.push_back(i);
                ind.push_back(i+1);
            }
        } else {

            int frontCenter = 0;
            int frontStart  = 1;
            int backCenter  = frontStart + (seg+1);
            int backStart   = backCenter + 1;

            //front face
            for (int i=0;i<seg;i++){
                ind.push_back(frontCenter);
                ind.push_back(frontStart + i);
                ind.push_back(frontStart + i + 1);
            }
            // back face
            for (int i=0;i<seg;i++){
                ind.push_back(backCenter);
                ind.push_back(backStart + i + 1);
                ind.push_back(backStart + i);
            }
            //bunch of triangles i want to put on the side
            for (int i=0;i<seg;i++){
                int f0 = frontStart + i;
                int f1 = frontStart + i + 1;
                int b0 = backStart + i;
                int b1 = backStart + i + 1;
                ind.push_back(f0); ind.push_back(b0); ind.push_back(b1);
                ind.push_back(f0); ind.push_back(b1); ind.push_back(f1);
            }
        }
        return ind;
    }
};
