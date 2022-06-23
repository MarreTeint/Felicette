// TP Corrige avec ajout de la gestion des textures
// For Code::Blocks 20 or higher (gcc/g++ x64)
// Date: 2022
// Using SDL, SDL OpenGL and standard IO
#include <iostream>
#include <cmath>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_opengl.h>
//#include <SDL/SDL_ttf.h>
#include <GL/GLU.h>

// Module for space geometry
#include "geometry.h"
// Module for generating and rendering forms
#include "forms.h"
#include <vector>


/***************************************************************************/
/* Constants and functions declarations                                    */
/***************************************************************************/
// Screen dimension constants
const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;
//const int SCREEN_WIDTH = 640;
//const int SCREEN_HEIGHT = 480;

// Max number of forms : static allocation
const int MAX_FORMS_NUMBER = 10;

// Animation actualization delay (in ms) => 100 updates per second
const Uint32 ANIM_DELAY = 40;

// Render actualization delay 40 (in ms) => 25 updates per second
const Uint32 FRAME_DELAY = 40;


// Starts up SDL, creates window, and initializes OpenGL
bool init(SDL_Window** window, SDL_GLContext* context);

// Initializes matrices and clear color
bool initGL();

// Updating forms for animation
void update(Form* formlist[MAX_FORMS_NUMBER], double delta_t);

// Renders scene to the screen
void render(std::vector<Body> bodies, const Point &cam_pos, double angle);

// Frees media and shuts down SDL
void close(SDL_Window** window);

// Creates a texture into graphic memory from an image file and assign it a
// unique ID, inside textureID
// returns 0 if all went fine, a negative value otherwise
int createTextureFromImage (const char* filename, GLuint* textureID);


/***************************************************************************/
/* Functions implementations                                               */
/***************************************************************************/
bool init(SDL_Window** window, SDL_GLContext* context)
{
    // Initialization flag
    bool success = true;

    // Initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
        success = false;
    }
    else
    {
        // Use OpenGL 2.1
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

        // Create window
        *window = SDL_CreateWindow( "FELICETTE - Simulation de systeme solaire", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
        if( *window == NULL )
        {
            std::cerr << "Window could not be created! SDL Error: " << SDL_GetError() << std::endl;
            success = false;
        }
        else
        {
            // Create context
            *context = SDL_GL_CreateContext(*window);
            if( *context == NULL )
            {
                std::cerr << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << std::endl;
                success = false;
            }
            else
            {
                // Use Vsync
                if(SDL_GL_SetSwapInterval(1) < 0)
                {
                    std::cerr << "Warning: Unable to set VSync! SDL Error: " << SDL_GetError() << std::endl;
                }

                // Initialize OpenGL
                if( !initGL() )
                {
                    std::cerr << "Unable to initialize OpenGL!"  << std::endl;
                    success = false;
                }
            }
        }
    }

    return success;
}


bool initGL()
{
    bool success = true;
    GLenum error = GL_NO_ERROR;

    // Initialize Projection Matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Set the viewport : use all the window to display the rendered scene
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // Fix aspect ratio and depth clipping planes
    gluPerspective(40.0, (GLdouble)SCREEN_WIDTH/SCREEN_HEIGHT, 100, 1.0e15);

    // Initialize Modelview Matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Initialize clear color : black with no transparency
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f );

    // Activate Z-Buffer
    glEnable(GL_DEPTH_TEST);


    // Lighting basic configuration and activation
    const GLfloat light_ambient[]  = { 0.3f, 0.3f, 0.3f, 1.0f };
    const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
    const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

    const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
    const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
    const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
    const GLfloat high_shininess[] = { 100.0f };

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);


    // Check for error
    error = glGetError();
    if( error != GL_NO_ERROR )
    {
        std::cerr << "Error initializing OpenGL!  " << gluErrorString( error ) << std::endl;
        success = false;
    }

    return success;
}

void update(Form* formlist[MAX_FORMS_NUMBER], double delta_t)
{
    // Update the list of forms
    unsigned short i = 0;
    while(formlist[i] != NULL)
    {
        formlist[i]->update(delta_t);
        i++;
    }
}

void render(std::vector<Body> bodies, Camera &cam)

{
	// Clear color buffer and Z-Buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Initialize Modelview Matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Set the camera position and parameters
	gluLookAt(cam.getAnim().getPos().x, cam.getAnim().getPos().y, cam.getAnim().getPos().z, 0, 0, 0, 0.0, 1.0, 0.0);

	Vector camVec = Vector(cam.getLookTarget().x - cam.getAnim().getPos().x, cam.getLookTarget().y - cam.getAnim().getPos().y, cam.getLookTarget().z - cam.getAnim().getPos().z);
	Vector horizCam = camVec ^ Vector(0, 1, 0);
	horizCam = 1 / horizCam.norm() * horizCam;
	glRotated(cam.getAnim().getPhi(), 0, 1, 0);
	glRotated(cam.getAnim().getTheta(), horizCam.x, horizCam.y, horizCam.z);
	glScaled(cam.getZoom(), cam.getZoom(), cam.getZoom());
    glTranslated(-cam.getLookTarget().x, -cam.getLookTarget().y, -cam.getLookTarget().z);
    // Render the list of forms
    unsigned short i = 0;
    for(int i=0;i<bodies.size(); i++)
    {
        glPushMatrix(); // Preserve the camera viewing point for further forms
        bodies.at(i).render();
        glPopMatrix(); // Restore the camera viewing point for next object
    }



	// X, Y and Z axis
	glPushMatrix(); // Preserve the camera viewing point for further forms
	// Render the coordinates system
	glBegin(GL_LINES);
	{
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3i(0, 0, 0);
		glVertex3i(1, 0, 0);
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3i(0, 0, 0);
		glVertex3i(0, 1, 0);
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3i(0, 0, 0);
		glVertex3i(0, 0, 1);
	}
	glEnd();
	glPopMatrix(); // Restore the camera viewing point for next object
}

void close(SDL_Window** window)
{
    //Destroy window
    SDL_DestroyWindow(*window);
    *window = NULL;

    //Quit SDL subsystems
    SDL_Quit();
}


int createTextureFromImage (const char* filename, GLuint* textureID)
{
    SDL_Surface *imgSurface = IMG_Load(filename);
    if (imgSurface == NULL)
    {
        std::cerr << "Failed to load texture image: " << filename << std::endl;
        return -1;
    }
    else
    {
        // Work out what format to tell glTexImage2D to use...
        int mode;
        if (imgSurface->format->BytesPerPixel == 3)   // RGB 24bit
        {
            mode = GL_RGB;
        }
        else if (imgSurface->format->BytesPerPixel == 4)     // RGBA 32bit
        {
            mode = GL_RGBA;
        }
        else
        {
            SDL_FreeSurface(imgSurface);
            std::cerr << "Unable to detect the image color format of: " << filename << std::endl;
            return -2;
        }
        // create one texture name
        glGenTextures(1, textureID);

        // tell opengl to use the generated texture name
        glBindTexture(GL_TEXTURE_2D, *textureID);

        // this reads from the sdl imgSurface and puts it into an openGL texture
        glTexImage2D(GL_TEXTURE_2D, 0, mode, imgSurface->w, imgSurface->h, 0, mode, GL_UNSIGNED_BYTE, imgSurface->pixels);

        // these affect how this texture is drawn later on...
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

        // clean up
        SDL_FreeSurface(imgSurface);
        return 0;
    }
}




/***************************************************************************/
/* MAIN Function                                                           */
/***************************************************************************/
int main(int argc, char* args[])
{
    // The window we'll be rendering to
    SDL_Window* gWindow = NULL;

    // OpenGL context
    SDL_GLContext gContext;


    // Start up SDL and create window
    if( !init(&gWindow, &gContext))
    {
        std::cerr << "Failed to initialize!" << std::endl;
    }
    else
    {
        // Main loop flag
        bool quit = false;
        Uint32 current_time, previous_time_anim, previous_time_render, elapsed_time_anim, elapsed_time_render;

        // Event handler
        SDL_Event event;

        // Textures creation //////////////////////////////////////////////////////////
        GLuint soleil, mercure, venus, terre, mars, jupiter, saturne, uranus, neptune,univers;
        createTextureFromImage("resources/images/earth.jpg", &terre);
        createTextureFromImage("resources/images/sun.jpg", &soleil);
        createTextureFromImage("resources/images/mars.jpg", &mars);
        createTextureFromImage("resources/images/mercure.jpg", &mercure);
        createTextureFromImage("resources/images/neptune.jpg", &neptune);
        createTextureFromImage("resources/images/saturne.jpg", &saturne);
        createTextureFromImage("resources/images/uranus.jpg", &uranus);
        createTextureFromImage("resources/images/venus.jpg", &venus);
        createTextureFromImage("resources/images/jupiter.jpg", &jupiter);
        createTextureFromImage("resources/images/universe.jpg", &univers);
        // Textures ready to be enabled (with private member " texture_id" of each form)


        // The forms to render
        Form* forms_list[MAX_FORMS_NUMBER];
        unsigned short number_of_forms = 0, i;
        for (i=0; i<MAX_FORMS_NUMBER; i++)
        {
            forms_list[i] = NULL;
        }



        // Spheres
        std::vector<Body> bodies;

        Body* pSphere2 = NULL;
        pSphere2 = new Body("Soleil",1900,20,Vector(0,0,0),Vector(0,0,0), Point(0,0,0));
        pSphere2->setTexture(soleil);
        forms_list[number_of_forms] = pSphere2;
        bodies.push_back(*pSphere2);
        number_of_forms++;

       Body* pSphere3 = NULL;
        pSphere3 = new Body("Mercure",3.3,2.5,Vector(0,0,0),Vector(0,0,6.5), Point(30,0,0));
        pSphere3->setTexture(mercure);
        forms_list[number_of_forms] = pSphere3;
        bodies.push_back(*pSphere3);
        number_of_forms++;

   /*      Body* pSphere4 = NULL;
        pSphere4 = new Body("Venus",4.8,6,Vector(0,0,0),Vector(0,0,2), Point(40,0,0));
        pSphere4->setTexture(venus);
        forms_list[number_of_forms] = pSphere4;
        bodies.push_back(*pSphere4);
        number_of_forms++;

       Body* pSphere = NULL;
        pSphere = new Body("Terre",3.2,2.5,Vector(0,0,0),Vector(0,0,0), Point(9,0,0));
        pSphere->setTexture(terre);
       forms_list[number_of_forms] = pSphere;
        bodies.push_back(*pSphere);
        number_of_forms++;

        Body* pSphere5 = NULL;
        pSphere5 = new Body("Mars",3.2,2.5,Vector(0,0,0),Vector(0,0,0), Point(12,0,0));
        pSphere5->setTexture(mars);
        forms_list[number_of_forms] = pSphere5;
        bodies.push_back(*pSphere5);
        number_of_forms++;

        Body* pSphere6 = NULL;
        pSphere6 = new Body("Jupiter",3.2,2.5,Vector(0,0,0),Vector(0,0,0), Point(15,0,0));
        pSphere6->setTexture(jupiter);
        forms_list[number_of_forms] = pSphere6;
        bodies.push_back(*pSphere6);
        number_of_forms++;

        Body* pSphere7 = NULL;
        pSphere7 = new Body("Saturne",3.2,2.5,Vector(0,0,0),Vector(0,0,0), Point(18,0,0));
        pSphere7->setTexture(saturne);
        forms_list[number_of_forms] = pSphere7;
        bodies.push_back(*pSphere7);
        number_of_forms++;

        Body* pSphere8 = NULL;
        pSphere8 = new Body("Uranus",3.2,2.5,Vector(0,0,0),Vector(0,0,0), Point(21,0,0));
        pSphere8->setTexture(uranus);
        forms_list[number_of_forms] = pSphere8;
        bodies.push_back(*pSphere8);
        number_of_forms++;

        Body* pSphere9 = NULL;
        pSphere9 = new Body("Neptune",3.2,2.5,Vector(0,0,0),Vector(0,0,0), Point(24,0,0));
        pSphere9->setTexture(neptune);
        forms_list[number_of_forms] = pSphere9;
        bodies.push_back(*pSphere9);
        number_of_forms++;*/

        Body* pSphere10 = NULL;
        pSphere10 = new Body("Univers",0,100500,Vector(0,0,0),Vector(0,0,0), Point(0,0,0));
        pSphere10->setTexture(univers);
        bodies.push_back(*pSphere10);
   //     forms_list[number_of_forms] = pSphere9;


        Camera camera = Camera(Point(0, 0, 0), Point(2, 2, 2));
		camera.setZoom(1);
		int click = 0;
		int cameraTargetId = 0;
        // Get first "current time"
        previous_time_anim = previous_time_render = SDL_GetTicks();
        // While application is running
        while(!quit)
        {
            // Handle events on queue
            while(SDL_PollEvent(&event) != 0)
            {
                int x = 0, y = 0;
                SDL_Keycode key_pressed = event.key.keysym.sym;
                SDL_GetRelativeMouseState(&x, &y);

                switch(event.type)
                {
                // User requests quit
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_KEYDOWN:
                    switch(key_pressed)
                    {
                    // Quit the program when 'q' or Escape keys are pressed
                    case SDLK_q:
                    case SDLK_ESCAPE:
                        quit = true;
                        break;
                    // Camera movement
					case SDLK_UP:
						camera.getAnim().incrTheta(-1);
						break;
					case SDLK_DOWN:
						camera.getAnim().incrTheta(1);
						break;
					case SDLK_LEFT:
						camera.getAnim().incrPhi(1);
						break;
					case SDLK_RIGHT:
						camera.getAnim().incrPhi(-1);
						break;
					// Camera target cycle
					case SDLK_TAB:
						cameraTargetId = (cameraTargetId + 1) % bodies.size();
                    default:
                        break;
                    }
                    break;
                //MOUSECONTROL
				case SDL_MOUSEMOTION:
					if (click == 1)
					{
						camera.getAnim().incrPhi(x);
						camera.getAnim().incrTheta(y);
					}
					break;
				case SDL_MOUSEBUTTONDOWN:
					click = 1;
					break;
				case SDL_MOUSEBUTTONUP:
					click = 0;
					break;
				case SDL_MOUSEWHEEL:
					if (event.wheel.y > 0) // scroll up
					{
						camera.incrZoom(10);
					}
					if (event.wheel.y < 0)
					{
						camera.incrZoom(-10);
					}

				default:
					break;
				}
            }

            // Update the scene
            current_time = SDL_GetTicks(); // get the elapsed time from SDL initialization (ms)
            elapsed_time_anim = current_time - previous_time_anim;
            elapsed_time_render = current_time - previous_time_render;

            if (elapsed_time_anim > ANIM_DELAY)
            {
                previous_time_anim = current_time;

                for(int i=0; i<bodies.size()-1;i++){
                    bodies.at(i).update(1e3 * elapsed_time_anim,bodies);

                }

            }
            // Camera update
				Point newTargetPos = bodies.at(cameraTargetId).getAnim().getPos();
				camera.setLookTarget(newTargetPos);
				camera.getAnim().setPos(Point(newTargetPos.x + 1, newTargetPos.y + 1000, newTargetPos.z + 1));


				/* TEST */
				//this opens a font style and sets a size
/*TTF_Font* Sans = TTF_OpenFont("Sans.ttf", 24);

// this is the color in rgb format,
// maxing out all would give you the color white,
// and it will be your text's color
SDL_Color White = {255, 255, 255};

// as TTF_RenderText_Solid could only be used on
// SDL_Surface then you have to create the surface first
SDL_Surface* surfaceMessage =
    TTF_RenderText_Solid(Sans, bodies.at(cameraTargetId).getName(), White);

// now you can convert it into a texture
SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

SDL_Rect Message_rect; //create a rect
Message_rect.x = 0;  //controls the rect's x coordinate
Message_rect.y = 0; // controls the rect's y coordinte
Message_rect.w = 100; // controls the width of the rect
Message_rect.h = 100; // controls the height of the rect

// (0,0) is on the top left of the window/screen,
// think a rect as the text's box,
// that way it would be very simple to understand

// Now since it's a texture, you have to put RenderCopy
// in your game loop area, the area where the whole code executes

// you put the renderer's name first, the Message,
// the crop size (you can ignore this if you don't want
// to dabble with cropping), and the rect which is the size
// and coordinate of your texture
SDL_RenderCopy(renderer, Message, NULL, &Message_rect);

// Don't forget to free your surface and texture
SDL_FreeSurface(surfaceMessage);
SDL_DestroyTexture(Message);

/* *********************************** */

            // Render the scene
            if (elapsed_time_render > FRAME_DELAY)
            {
                previous_time_render = current_time;

                render(bodies, camera);


                // Update window screen
                SDL_GL_SwapWindow(gWindow);
            }
        }
    }

    // Free resources and close SDL
    close(&gWindow);

    return 0;
}
