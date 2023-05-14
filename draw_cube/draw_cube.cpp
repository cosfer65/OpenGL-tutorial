#include "draw_cube.h"
#include "cg_camera.h"
#include "cg_util.h"
#include "cg_primitives.h"

// #define __WIREFRAME_DRAW__

using namespace atlas;

class atlas_app :public cg_app {
    cg_viewport* m_view;
    cg_camera* m_cam;
    cg_shader* m_shader;
    cg_shader* mt_shader;

    cg_gl_cube* simple_cube;
    cg_gl_sphere* simple_globe;
    GLuint   texture;
    GLuint   texture2;

public:
    atlas_app() {
        m_view = new cg_viewport();
        m_window.szTitle = "Altair (draw cube)";
    }
    virtual int init_game() {
        // zoom is the angle of the field of view
        m_view->set_fov(dtr(15));
        // camera position, look at point, and up vector orientation
        m_cam = new cg_camera(vec3(0, 0, -20), vec3(0, 0, 0), vec3(0, 1, 0));

        // create the basic shader
        m_shader = new cg_shader;
        m_shader->add_file(GL_VERTEX_SHADER, "resources/draw_cube_vs.hlsl");
        m_shader->add_file(GL_FRAGMENT_SHADER, "resources/draw_cube_fs.hlsl");
        m_shader->load();

        // create the texture shader
        mt_shader = new cg_shader;
        mt_shader->add_file(GL_VERTEX_SHADER, "resources/draw_cubet_vs.hlsl");
        mt_shader->add_file(GL_FRAGMENT_SHADER, "resources/draw_cubet_fs.hlsl");
        mt_shader->load();

        texture = load_texture("resources/earth2.tga");
        texture2 = load_texture("resources/planks.tga");

        // create a simple cube
        simple_cube = new cg_gl_cube;
        simple_cube->set_scale(atlas::vec3(0.5, 0.5, 0.5));
        simple_globe = new cg_gl_sphere(0.5);

        


#ifdef __WIREFRAME_DRAW__
        simple_cube->create(GL_LINE);
        simple_globe->create(GL_LINE);
#else
        simple_cube->create(GL_FILL);
        simple_globe->create(GL_FILL);
#endif
        simple_cube->move_to(vec3(0, 0, -6.f));
        simple_globe->move_to(vec3(0, 0, -6.f));

        // OpenGL initialization we want for this sample
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glEnable(GL_MULTISAMPLE);

        return 1;
    }

    virtual void frame_move(float fElapsed) {
        // rotate the cube
        simple_cube->rotate_by(vec3(dtr(fElapsed * 10), dtr(fElapsed * 20), dtr(fElapsed * 30)));
        simple_globe->rotate_by(vec3(0, dtr(fElapsed * 20), 0));

        // move the cube around
        float angular_velocity = 1.25;
        static float angle = 0;
        // current angle
        angle += fElapsed * angular_velocity;
        float y1 = 1.f * (float)sin(angle);
        float x1 = 1.5f * (float)cos(angle);
        simple_cube->move_to(vec3(x1, y1, -6));
    }

    virtual void frame_render() {
        // set the viewport to the whole window
        m_view->set_viewport();

        // clear screen
        glClearColor(.0f, .0f, .0f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // combine the view and camera matrices into one
        mat4 cam_matrix = m_view->perspective() * m_cam->perspective();

#ifdef __WIREFRAME_DRAW__
        cg_shader* the_shader = m_shader;
#else
        cg_shader* the_shader = mt_shader;
#endif

        // enable the shader
        the_shader->use();
        // // set the combined view matrix
        the_shader->set_mat4("camera", cam_matrix);
        // texture has no effect on wireframe drawing

        glActiveTexture(GL_TEXTURE0);

        glBindTexture(GL_TEXTURE_2D, texture2);
        simple_cube->render(the_shader);

        glBindTexture(GL_TEXTURE_2D, texture);
        simple_globe->render(the_shader);
    }

    virtual void resize_window(int width, int height) {
        m_view->set_window_aspect(width, height);
    }
};

atlas_app my_app;       // default