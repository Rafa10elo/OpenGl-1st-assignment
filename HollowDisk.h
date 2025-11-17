#pragma once
#include "Polygon.h"
#include <vector>
#include <cmath>

class HollowDisk : public Polygon {
public:
    HollowDisk(const glm::vec3& pos, float outerR, float innerR, int segments, const glm::vec3& color = {1,1,1})
    {
        m_vertices = generateVertices(outerR, innerR, segments, color);
        m_indices  = generateIndices(segments);
        setupBuffers();
        setPosition(pos);
        updateModelMatrix();
    }

private:
    static std::vector<Vertex> generateVertices(float outerR, float innerR, int seg, const glm::vec3& color) {
        std::vector<Vertex> verts;
        for(int i=0;i<=seg;i++){
            float angle = 2.0f * M_PI * i / seg;
            float xOut = cos(angle)*outerR, yOut = sin(angle)*outerR;
            float xIn  = cos(angle)*innerR, yIn  = sin(angle)*innerR;

            verts.push_back({{xOut, yOut, 0.2}, {0,0}, color});
            verts.push_back({{xIn,  yIn,  0.3}, {0,0}, color});
        }
        return verts;
    }

    static std::vector<unsigned int> generateIndices(int seg){
        std::vector<unsigned int> ind;
        for(int i=0;i<seg;i++){
            int o0 = i*2;
            int i0 = i*2+1;
            int o1 = (i+1)*2;
            int i1 = (i+1)*2+1;

            ind.push_back(o0);
            ind.push_back(i0);
            ind.push_back(o1);


            ind.push_back(i1);
            ind.push_back(o1);
            ind.push_back(i0);

        }
        return ind;
    }
};
