#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "glsl.h"
#include "objloader.h"
#include "texture.h"

#include "cube.h"

using namespace std;
#define Array_Length 4
#define NUMBER_OF_PROGRAM_IDS 2

//--------------------------------------------------------------------------------
// Consts
//--------------------------------------------------------------------------------

const int WIDTH = 800, HEIGHT = 600;

const char* fragshader_name = "fragmentshader.fsh";
const char* vertexshader_name = "vertexshader.vsh";

unsigned const int DELTA_TIME = 10;


//--------------------------------------------------------------------------------
// Variables
//--------------------------------------------------------------------------------

// ID's
GLuint program_id[NUMBER_OF_PROGRAM_IDS];
GLuint vao[Array_Length];
GLuint texture_id[Array_Length];

// Uniform ID's
GLuint uniform_mv;

// Matrices
glm::mat4 model[Array_Length], view, projection;
glm::mat4 mv[Array_Length];

// eye, where is the camera?
double LookAt_Eye_X = 2.0;
double LookAt_Eye_Y = 2.0;
double LookAt_Eye_Z = 8.0;

// center, waar kijkt de camera naar toe?
double LookAt_Center_X = 0.0;
double LookAt_Center_Y = 2.0;
double LookAt_Center_Z = 0.0;



//--------------------------------------------------------------------------------
// Mesh variables
//--------------------------------------------------------------------------------

// For some reason I cannot move this to Cube.h.
// I get the error that it already is defined.
GLushort cube_elements[] = {
    0,1,2,0,2,3, // front
    1,5,6,1,6,2, // right side
    4,0,3,4,3,7, // left side
    5,4,7,5,7,6, // rear side
    3,2,6,3,6,7, // top
    4,5,1,4,1,0, // bottom
};


vector<glm::vec3> normals[Array_Length];
vector<glm::vec3> vertices[Array_Length];
vector<glm::vec2> uvs[Array_Length];

bool res = loadOBJ("teapot.obj", vertices[0], uvs[0], normals[0]);
bool res1 = loadOBJ("torus.obj", vertices[1], uvs[1], normals[1]);


//--------------------------------------------------------------------------------
// Keyboard handling
//--------------------------------------------------------------------------------

void keyboardHandler(unsigned char key, int a, int b)
{
    if (key == 27)
        glutExit();

    if (key == 'w' || key == 'W') {
        std::cout << "w was pressed" << "\n\r";
        LookAt_Eye_Z -= 0.1;
        LookAt_Center_Z -= 0.1;
    }
    if (key == 'a' || key == 'A') {
        std::cout << "a was pressed" << "\n\r";
        LookAt_Eye_X -= 0.1;
        LookAt_Center_X -= 0.1;
    }
    if (key == 's' || key == 'S') {
        std::cout << "s was pressed" << "\n\r";
        LookAt_Eye_Z += 0.1;
        LookAt_Center_Z += 0.1;
    }
    if (key == 'd' || key == 'D') {
        std::cout << "d was pressed" << "\n\r";
        LookAt_Eye_X += 0.1;
        LookAt_Center_X += 0.1;
    }
    if (key == 'i' || key == 'I') {
        // look up?
        std::cout << "i was pressed" << "\n\r";
        LookAt_Center_Y += 0.1;
    }
    if (key == 'j' || key == 'J') {
        // look down?
        std::cout << "j was pressed" << "\n\r";
        LookAt_Center_Y -= 0.1;
    }
    if (key == 'k' || key == 'K') {
        // look more to the left.
        std::cout << "k was pressed" << "\n\r";
        LookAt_Center_X -= 0.1;
    }
    if (key == 'l' || key == 'L') {
        // look more to the right.
        std::cout << "L was pressed" << "\n\r";
        LookAt_Center_X += 0.1;
    }

    if (key == 'c' || key == 'C') {
        LookAt_Eye_X = 0.0;
        LookAt_Eye_Z = 1.0;
        LookAt_Eye_Y = 25.0;

        LookAt_Center_X = 0.0;
        LookAt_Center_Y = 0.0;
        LookAt_Center_Z = 0.0;
    }
}

//------------------------------------------------------------
// void InitMatrices()
//------------------------------------------------------------

void InitMatrices()
{
    
    for (int i = 0; i < Array_Length; i++) {
        model[i] = glm::mat4();
        view = glm::lookAt(
            glm::vec3(LookAt_Eye_X, LookAt_Eye_Y, LookAt_Eye_Z),  // eye, waar staat de camera
            glm::vec3(LookAt_Center_X, LookAt_Center_Y, LookAt_Center_Z),  // center, waar kijkt ie naar toe?
            glm::vec3(0.0, 1.0, 0.0));  // // up, Head is up (set to 0,-1,0 to look upside-down)
        projection = glm::perspective(
            glm::radians(45.0f),
            1.0f * WIDTH / HEIGHT, 0.1f,
            30.0f);
         mv[i] = view * model[i];
    }
}


//--------------------------------------------------------------------------------
// Rendering
//--------------------------------------------------------------------------------

void Render()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    // refresh view;
    view = glm::lookAt(
        glm::vec3(LookAt_Eye_X, LookAt_Eye_Y, LookAt_Eye_Z),  // eye, waar staat de camera
        glm::vec3(LookAt_Center_X, LookAt_Center_Y, LookAt_Center_Z),  // center, waar kijkt ie naar toe?
        glm::vec3(0.0, 1.0, 0.0));  // // up, Head is up (set to 0,-1,0 to look upside-down)

 

    // Do transformation
    for (int i = 0; i < Array_Length; i++) {

        // Attach to program_id
        if (i == 0 || i == 1) {
            glUseProgram(program_id[0]);
        }
        else if (i == 2 || i == 3) {
            glUseProgram(program_id[1]);
        }

        model[0] = glm::rotate(model[0], 0.01f, glm::vec3(0.0f, 1.0f, 0.0f));
        model[1] = glm::translate(glm::mat4(), glm::vec3(3.0, 0.5, 0.0));
        model[2] = glm::translate(glm::mat4(), glm::vec3(0.0, -2.0, 0.0));
        model[3] = glm::translate(glm::mat4(), glm::vec3(7.0, -2.0, 0.0));
        mv[i] = view * model[i];


        // Send mv
        glUniformMatrix4fv(uniform_mv, 1, GL_FALSE, glm::value_ptr(mv[i]));

        glBindVertexArray(vao[i]);

        if (i == 0 || i == 1) {
            glDrawArrays(GL_TRIANGLES, 0, vertices[i].size());
        } else if(i == 2 || i == 3) {
            glDrawElements(GL_TRIANGLES, sizeof(cube_elements) / sizeof(GLushort),
                GL_UNSIGNED_SHORT, 0);
        }
        glBindVertexArray(0);
    }

    glutSwapBuffers();
}


//------------------------------------------------------------
// void Render(int n)
// Render method that is called by the timer function
//------------------------------------------------------------

void Render(int n)
{
    Render();
    glutTimerFunc(DELTA_TIME, Render, 0);
}


//------------------------------------------------------------
// void InitGlutGlew(int argc, char **argv)
// Initializes Glut and Glew
//------------------------------------------------------------

void InitGlutGlew(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Hello OpenGL");
    glutDisplayFunc(Render);
    glutKeyboardFunc(keyboardHandler);
    glutTimerFunc(DELTA_TIME, Render, 0);

    glewInit();
}


//------------------------------------------------------------
// void InitShaders()
// Initializes the fragmentshader and vertexshader
//------------------------------------------------------------

void InitShaders()
{
    GLuint fsh_id[2] = {};
    char* vertexshader = glsl::readFile(vertexshader_name);
    GLuint vsh_id = glsl::makeVertexShader(vertexshader);

    char* fragshader = glsl::readFile(fragshader_name);
    fsh_id[0] = glsl::makeFragmentShader(fragshader);

    char* fragshaderAsphalt = glsl::readFile("fragmentshaderAsphalt.fsh");
    fsh_id[1] = glsl::makeFragmentShader(fragshaderAsphalt);

    program_id[0] = glsl::makeShaderProgram(vsh_id, fsh_id[0]);
    program_id[1] = glsl::makeShaderProgram(vsh_id, fsh_id[1]);
}


//------------------------------------------------------------
// void InitBuffers()
// Allocates and fills buffers
//------------------------------------------------------------

void InitBuffers()
{
    GLuint position_id, color_id, uv_id, normal_id;
    GLuint vbo_vertices, vbo_uvs, vbo_normals;
    GLuint ibo_elements, vbo_colors;

    glm::vec3 light_position = glm::vec3{4,4,4};
    glm::vec3 ambient_color = glm::vec3{0.2,0.2,0};
    glm::vec3 diffuse_color = glm::vec3{0.5,0.5,0};

    for (int i = 0; i < Array_Length; i++) {
        if (i == 0 || i == 1) {
            // vbo for uvs
            glGenBuffers(1, &vbo_uvs);
            glBindBuffer(GL_ARRAY_BUFFER, vbo_uvs);
            glBufferData(GL_ARRAY_BUFFER, uvs[i].size() * sizeof(glm::vec2),
                &uvs[i][0], GL_STATIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            uv_id = glGetAttribLocation(program_id[0], "uv");
        }
 
       
        // vbo for vertices
        glGenBuffers(1, &vbo_vertices);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
        glBufferData(GL_ARRAY_BUFFER,
            vertices[i].size() * sizeof(glm::vec3), &(vertices[i][0]),
            GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);



        if (i == 0 || i == 1) {
            // vbo for normals
            glGenBuffers(1, &vbo_normals);
            glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
            glBufferData(GL_ARRAY_BUFFER,
                normals[i].size() * sizeof(glm::vec3),
                &normals[i][0], GL_STATIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            normal_id = glGetAttribLocation(program_id[0], "normal");

        }
        else if (i == 2 || i == 3) {
            //vbo for colors
            glGenBuffers(1, &vbo_colors);
            glBindBuffer(GL_ARRAY_BUFFER, vbo_colors);
            glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            // vbo for elements
            glGenBuffers(1, &ibo_elements);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_elements);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements),
                cube_elements, GL_STATIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        }


        if(i == 0 || i == 1){
            position_id = glGetAttribLocation(program_id[0], "position");
            color_id = glGetAttribLocation(program_id[0], "color");
        }
        else if (i == 2 || i == 3) {
            position_id = glGetAttribLocation(program_id[1], "position");
            color_id = glGetAttribLocation(program_id[1], "color");
        }

        // Allocate memory for vao
        glGenVertexArrays(1, &vao[i]);

        // Bind to vao
        glBindVertexArray(vao[i]);

        //mv[i] = view * model[i];


        // Bind vertices to vao
        glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
        glVertexAttribPointer(position_id, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(position_id);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        if (i == 0 || i == 1) {
            // Bind normals to vao
            glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
            glVertexAttribPointer(normal_id, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(normal_id);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            // Bind uvs to vao
            glBindBuffer(GL_ARRAY_BUFFER, vbo_uvs);
            glVertexAttribPointer(uv_id, 2, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(uv_id);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
        else if (i == 2 || i == 3) {
            // bind colors to vao
            glBindBuffer(GL_ARRAY_BUFFER, vbo_colors);
            glVertexAttribPointer(color_id, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(color_id);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            // bind elements to VAO
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_elements);

        }

        // Stop bind to vao
        glBindVertexArray(0);

        // Make uniform vars
        if(i == 0 || i == 1){
            uniform_mv = glGetUniformLocation(program_id[0], "mv");
            GLuint uniform_proj = glGetUniformLocation(program_id[0], "projection");
            GLuint uniform_light_pos = glGetUniformLocation(program_id[0], "light_pos");
            GLuint uniform_material_ambient = glGetUniformLocation(program_id[0],
                "mat_ambient");
            GLuint uniform_material_diffuse = glGetUniformLocation(program_id[0],
                "mat_diffuse");

            mv[i] = view * model[i];



            glUseProgram(program_id[0]);
            // Fill uniform vars
            glUniformMatrix4fv(uniform_mv, 1, GL_FALSE, glm::value_ptr(mv[i]));
            glUniformMatrix4fv(uniform_proj, 1, GL_FALSE, glm::value_ptr(projection));
            glUniform3fv(uniform_light_pos, 1, glm::value_ptr(light_position));
            glUniform3fv(uniform_material_ambient, 1, glm::value_ptr(ambient_color));
            glUniform3fv(uniform_material_diffuse, 1, glm::value_ptr(diffuse_color));
        }
        else if (i == 2 || i == 3) {
            uniform_mv = glGetUniformLocation(program_id[1], "mv");
            GLuint uniform_proj = glGetUniformLocation(program_id[1], "projection");
            GLuint uniform_light_pos = glGetUniformLocation(program_id[1], "light_pos");
            GLuint uniform_material_ambient = glGetUniformLocation(program_id[1],
                "mat_ambient");
            GLuint uniform_material_diffuse = glGetUniformLocation(program_id[1],
                "mat_diffuse");

            mv[i] = view * model[i];


            glUseProgram(program_id[1]);
            // Fill uniform vars
            glUniformMatrix4fv(uniform_mv, 1, GL_FALSE, glm::value_ptr(mv[i]));
            glUniformMatrix4fv(uniform_proj, 1, GL_FALSE, glm::value_ptr(projection));
            glUniform3fv(uniform_light_pos, 1, glm::value_ptr(light_position));
            glUniform3fv(uniform_material_ambient, 1, glm::value_ptr(ambient_color));
            glUniform3fv(uniform_material_diffuse, 1, glm::value_ptr(diffuse_color));
        }
    }




}

void InitObjects() {
    texture_id[0] = loadBMP("Yellobrk.bmp");

    vertices[2] = Create_Cube_Vertices(10, 1, 4);
    vertices[3] = Create_Cube_Vertices(4, 1, 15);
}


int main(int argc, char** argv)
{
    InitGlutGlew(argc, argv);
    InitObjects();
    InitShaders();
    InitMatrices();
    InitBuffers();

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    // SHOW console window
    HWND hWnd = GetConsoleWindow();
    ShowWindow(hWnd, SW_SHOW);

    // Main loop
    glutMainLoop();

    return 0;
}