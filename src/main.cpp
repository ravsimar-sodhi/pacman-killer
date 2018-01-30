#include "main.h"
#include "timer.h"
#include "ball.h"
#include "floor.h"
#include "obstacle.h"
#include "pond.h"
#include "trampoline.h"
#include "magnet.h"

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

Floor floor1;
Floor grass;
Ball ball1;
vector<Obstacle> fballs(20);
Pond pond1;
Trampoline tramp1;
Magnet mag1,mag2;
color_t fballCol[] = {COLOR_PINK, COLOR_PURPLE, COLOR_YELLOW};
float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;

Timer t60(1.0 / 60);
Timer t2(2);

/* Render the scene with openGL */
/* Edit this function according to your assignment */
double getRandDouble(double l, double r)
{
    return l + (((double)rand())/RAND_MAX)*(r - l);
}
void draw() {
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram (programID);

    // Eye - Location of camera. Don't change unless you are sure!!
    // glm::vec3 eye ( 5*cos(camera_rotation_angle*M_PI/180.0f), 0, 5*sin(camera_rotation_angle*M_PI/180.0f) );
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    // glm::vec3 target (0, 0, 0);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    // glm::vec3 up (0, 1, 0);

    // Compute Camera matrix (view)
    // Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    // Don't change unless you are sure!!
    Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP;  // MVP = Projection * View * Model

    // Scene render
    floor1.draw(VP);
    tramp1.draw(VP);
    grass.draw(VP);
    pond1.draw(VP);
    mag1.draw(VP);
    mag2.draw(VP);
    ball1.draw(VP);
    for(int i=0;i<fballs.size();i++)
    {
        fballs[i].draw(VP);
    }
}

void tick_input(GLFWwindow *window) {
    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int up = glfwGetKey(window, GLFW_KEY_UP);
    if (left) {
        ball1.speed.x = 0.04;
        ball1.position.x -= (ball1.speed.x);
        ball1.speed.x = 0;
        // Do something
    }
    if (right) {

        ball1.speed.x = 0.04;
        ball1.position.x += (ball1.speed.x);
        ball1.speed.x = 0;
    }
    if (up) {
//        keyboard(window,GLFW_KEY_UP,0,GLFW_RELEASE,0);

    }
}

void tick_elements() {
    ball1.tick();
    mag1.tick();
    mag2.tick();
    for(int i=0;i<fballs.size();i++)
    {
        fballs[i].tick();
    }
    for(int i=0;i<fballs.size();i++)
    {
        if(ball1.speed.y < 0 && detectCollision(ball1.bounding_box(),fballs[i].bounding_box()))
        {
            double angle,anglei,angler,speed;
            angle = fballs[i].slope;
            angle = ( (fballs[i].slope > 0) ? 90 - fballs[i].slope :90 + abs(fballs[i].slope) );
            if (ball1.speed.x == 0)
                anglei = 90;
            else
            {
                anglei = atan(ball1.speed.y / ball1.speed.x);
                if (anglei < 0)
                {
                    anglei = abs(anglei) + 90;
                }
            }
            if((angle >70 && angle < 110)||abs(angle-anglei) > 90)
            {
                ball1.speed.y = 0.1;
            }
            else
            {
                speed = sqrt(pow(ball1.speed.x,2) + pow(ball1.speed.y,2));
                speed = 0.1;
                angler = 2*angle - anglei;
                ball1.speed.x = -speed*cos(angler*M_PI/180.0);
                ball1.speed.y = speed*sin(angler*M_PI/180.0);
            }
            fballs.erase(fballs.begin()+i);
            break;
        }
    }
    if(detectWater(ball1.bounding_box(), pond1.bounding_box()))
    {
        if (ball1.speed.y > 0.06)
            ball1.speed.y = 0.06;
        else if (ball1.speed.y < -0.06)
            ball1.speed.y = -0.06;
        if (ball1.position.y > pond1.corrsY(ball1.position.x) + 0.3)
        {
            ball1.accel.y = -0.001;
        }
        else
        {
            ball1.accel.y = 0;
            ball1.speed.x = -0.005 * (ball1.position.x - pond1.position.x) / abs((ball1.position.x - pond1.position.x));
        }
        if (ball1.position.y < pond1.corrsY(ball1.position.x) + 0.3)
        {
            ball1.position.y = pond1.corrsY(ball1.position.x) + 0.3;
        }
    }
    else
    {
        if (ball1.position.y <= -2)
        {
            ball1.accel.y = 0;
            ball1.speed.y = 0;
            ball1.speed.x = 0;
        }
        else
        {
            ball1.accel.y = -0.004;
        }
    }
    if (ball1.speed.y <= 0 && detectTrampoline(ball1.bounding_box(), tramp1.bounding_box()))
    {
        ball1.speed.y = 0.18;
    }

    if(mag1.active == 1)
    {
        if(mag1.position.x <= -4)
        {
            mag1.speed.x = 0.05;
        }
        else
        {
            mag1.speed.x = 0;
        }
        ball1.accel.x = -0.001;
    }
    else
    {
        if(mag1.position.x >= -5)
        {
            mag1.speed.x  = -0.05;
            ball1.accel.x = 0;
        }
    }
    if (mag2.active == 1)
    {
        if (mag2.position.x >= 4)
        {
            mag2.speed.x = -0.05;
        }
        else
        {
            mag2.speed.x = 0;
        }
        ball1.accel.x = 0.001;
    }
    else
    {
        if(mag2.position.x <= 4)
        {
            mag2.speed.x = 0.05;
            ball1.accel.x = 0;
        }
    }
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models
    floor1       = Floor(-6, -4, 6, -2.7, COLOR_BLACK);
    grass       = Floor(-6,-2.7, 6, -2.3, COLOR_GREEN);
    ball1       = Ball(1, -2, COLOR_RED);
    for(int i=0;i<fballs.size();i++)
    {
        fballs[i] = Obstacle(getRandDouble(-5,3),
                                getRandDouble(-1.6,3),
                                getRandDouble(0.1,0.25),
                                getRandDouble(0.01,0.04),
                                getRandDouble(-45,45),
                                fballCol[(int)(getRandDouble(0,3))]);
    }
    tramp1      = Trampoline(3, -1.8, COLOR_ORANGE);

    ball1.speed.x = 0;

    pond1 = Pond(-2,-2.3,1);
    
    mag1 = Magnet(-6.0, 1.0, COLOR_RED_BR);
    mag2 = Magnet(6, 1.0, COLOR_RED_BR);
    mag2.rotation = 180;

    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");

    reshapeWindow (window, width, height);

    // Background color of the scene
    glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

    // cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    // cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    // cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    // cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}

int main(int argc, char **argv) 
{
    srand(time(0));
    int width  = 800;
    int height = 600;

    window = initGLFW(width, height);

    initGL (window, width, height);
            // mag2.active = 1;

    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) 
    {
        // Process timers
        if (t60.processTick()) 
        {
            // 60 fps
            // OpenGL Draw commands
            draw();
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);

            tick_elements();
            tick_input(window);
        }
        if(t2.processTick()) 
        {
            for(int i=0;i<fballs.size();i++)
            {
                if(fballs[i].position.x > 4)
                {
                    fballs.erase(fballs.begin()+i);
                }
            }
            mag2.active = 0;
            // mag1.active = 1;
            while(fballs.size() < 20)
            {
                fballs.insert(fballs.end(),Obstacle(getRandDouble(-7,-6),
                                                    getRandDouble(-1.6,3),
                                                    getRandDouble(0.1,0.25),
                                                    getRandDouble(0.01,0.04),
                                                    getRandDouble(-45,45),
                                                    fballCol[(int)(getRandDouble(0,3))]));
            }

        }

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

    quit(window);
}


bool detectCollision(bounding_box_t player, bounding_box_t obs)
{
    return ( (player.y > obs.y) && 
            (2*(player.y - obs.y )<= (0.6+obs.width)) && 
            (abs(player.x - obs.x)*2<=(0.6 + obs.width)));
}

bool detectWater(bounding_box_t player, bounding_box_t pond)
{
    return ( (abs(player.x - pond.x) <= pond.width) && 
            (player.y <= pond.y + 0.3) );
}

bool detectTrampoline(bounding_box_t player, bounding_box_t tramp)
{
    return( (player.x >= (tramp.x - (tramp.width/2))) && 
            ((player.x <= tramp.x + (tramp.width/2))) && 
            ((player.y - tramp.y)*2 <= player.width) && 
            (player.y >= tramp.y) );
}

void reset_screen() {
    float top    = screen_center_y + 4 / screen_zoom;
    float bottom = screen_center_y - 4 / screen_zoom;
    float left   = screen_center_x - 5.5 / screen_zoom;
    float right  = screen_center_x + 5.5 / screen_zoom;
    Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}

void jump()
{
    if(ball1.speed.y == 0 || ball1.position.y < -2.6)
    ball1.speed.y = 0.12;
}
