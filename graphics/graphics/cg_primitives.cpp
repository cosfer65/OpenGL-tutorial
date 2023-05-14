#include "pch.h"
#include "cg_primitives.h"

namespace atlas {
    void cg_prim::create_from_mesh(cg_mesh* mesh, GLenum drmode /*= GL_FILL*/, bool dr_el /*= true*/) {
        m_mesh_data = new cg_mesh_data;
        *m_mesh_data = mesh->m_data;

        draw_elements = dr_el;
        draw_mode = drmode;

        int idx = 0;

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        GLuint position_buffer;
        glGenBuffers(1, &position_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
        glBufferData(GL_ARRAY_BUFFER, m_mesh_data->vertices.size() * sizeof(float), &m_mesh_data->vertices[0], GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(idx);

        if (m_mesh_data->normals.size() > 0)
        {
            GLuint normals_buffer;
            glGenBuffers(1, &normals_buffer);
            glBindBuffer(GL_ARRAY_BUFFER, normals_buffer);
            glBufferData(GL_ARRAY_BUFFER, m_mesh_data->normals.size() * sizeof(float), &m_mesh_data->normals[0], GL_STATIC_DRAW);
            ++idx;
            glVertexAttribPointer(idx, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
            glEnableVertexAttribArray(idx);
        }

        if (m_mesh_data->texCoords.size() > 0)
        {
            GLuint texture_buffer;
            glGenBuffers(1, &texture_buffer);
            glBindBuffer(GL_ARRAY_BUFFER, texture_buffer);
            glBufferData(GL_ARRAY_BUFFER, m_mesh_data->texCoords.size() * sizeof(float), &m_mesh_data->texCoords[0], GL_STATIC_DRAW);
            ++idx;
            glVertexAttribPointer(idx, 2, GL_FLOAT, GL_FALSE, 0, NULL);
            glEnableVertexAttribArray(idx);
        }

        GLuint index_buffer;
        glGenBuffers(1, &index_buffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_mesh_data->indices.size() * sizeof(float), &m_mesh_data->indices[0], GL_STATIC_DRAW);
        glBindVertexArray(0);
    }

    void cg_gl_cube::create(GLenum drmode, bool dr_el /*= true*/)
    {
        cg_hexa_mesh ms;
        ms.create(1, 1, 1);
        create_from_mesh(&ms, drmode, dr_el);
    }

    void cg_gl_sphere::create(GLenum drmode /*= GL_FILL*/, bool dr_el /*= true*/) {
        cg_oblique_mesh om;
        om.create(radius, radius, radius);
        create_from_mesh(&om, drmode, dr_el);
    }

    void cg_gl_cylinder::create(GLenum drmode /*= GL_FILL*/, bool dr_el /*= true*/) {
        cg_cylindrical_mesh cm;
        cm.create(0.5f, 1, 0.5f);
        create_from_mesh(&cm, drmode, dr_el);
    }

    void cg_gl_mesh::create(GLenum drmode /*= GL_FILL*/, bool dr_el /*= true*/) {
        draw_elements = dr_el;
        draw_mode = drmode;

        if (vao == 0)
        {
            cg_plane_mesh* tmesh = new cg_plane_mesh;
            tmesh = new cg_plane_mesh;
            //tmesh->set_extent(-5, 5, -5, 5);
            tmesh->create(10, 0, 10);
            create_from_mesh(tmesh, drmode, dr_el);
            delete tmesh;
        }
    }
}