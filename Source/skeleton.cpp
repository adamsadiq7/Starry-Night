#include <iostream>
#include <glm/glm.hpp>
#include <SDL.h>
#include "SDLauxiliary.h"
#include "TestModel.h"
#include <stdint.h>

using namespace std;
using glm::mat3;
using glm::vec3;

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 256
#define FULLSCREEN_MODE false

/* ----------------------------------------------------------------------------*/
/* GLOBAL VARIABLES                                                            */
int t;

/* ----------------------------------------------------------------------------*/
/* FUNCTIONS                                                                   */

void update(vector<vec3> &stars);
void draw(screen *screen);
void drawStarfield(screen *screen, vector<vec3> &stars);
void interpolate(vec3 a, vec3 b, vector<vec3> &result);

int main(int argc, char *argv[])
{

  screen *screen = InitializeSDL(SCREEN_WIDTH, SCREEN_HEIGHT, FULLSCREEN_MODE);
  t = SDL_GetTicks(); /*Set start value for timer.*/

  vector<vec3> stars(1000);

  for (int i = 0; i < stars.size(); i++)
  {
    stars[i].x = ((float(rand()) / float(RAND_MAX)) * 2) - 1;
    stars[i].y = ((float(rand()) / float(RAND_MAX)) * 2) - 1;
    stars[i].z = float(rand()) / float(RAND_MAX);
  }

  while (NoQuitMessageSDL())
  {
    update(stars);

    drawStarfield(screen, stars);
    // draw(screen);
    SDL_Renderframe(screen);
  }

  SDL_SaveImage(screen, "screenshot.bmp");

  KillSDL(screen);
  return 0;
}

/*Place your drawing here*/
void draw(screen *screen)
{
  /* Clear buffer */
  memset(screen->buffer, 0, screen->height * screen->width * sizeof(uint32_t));

  vec3 topLeft(1, 0, 0);     // red
  vec3 topRight(0, 0, 1);    // blue
  vec3 bottomRight(0, 1, 0); // green
  vec3 bottomLeft(1, 1, 0);  // yellow

  vector<vec3> leftSide(SCREEN_HEIGHT);
  vector<vec3> rightSide(SCREEN_HEIGHT);

  interpolate(topLeft, bottomLeft, leftSide);
  interpolate(topRight, bottomRight, rightSide);
  for (int i = 0; i < SCREEN_HEIGHT; i++)
  {
    vector<vec3> result(SCREEN_WIDTH);
    interpolate(leftSide[i], rightSide[i], result);
    for (int j = 0; j < SCREEN_WIDTH; j++)
    {
      PutPixelSDL(screen, j, i, result[j]);
    }
  }
}

void drawStarfield(screen *screen, vector<vec3> &stars)
{

  memset(screen->buffer, 0, screen->width * screen->height * sizeof(uint32_t));

  float u, v = 0;
  float f = 0;
  for (size_t s = 0; s < stars.size(); ++s)
  {
    vec3 colour = 0.2f * vec3(1, 1, 1) / (stars[s].z * stars[s].z);

    f = (screen->height) / 2;
    u = stars[s].x / stars[s].z + screen->width / 2;
    v = stars[s].y / stars[s].z + screen->height / 2;

    PutPixelSDL(screen, u, v, colour);
  }
}

/*Place updates of parameters here*/
void update(vector<vec3> &stars)
{

  /* Compute frame time */
  static int t = SDL_GetTicks();
  int t2 = SDL_GetTicks();
  float dt = float(t2 - t);
  t = t2;
  /* Update variables*/
  float v = 0.00001;

  for (int i = 0; i < stars.size(); i++)
  {
    stars[i].z = stars[i].z - v * dt;
    if (stars[i].z <= 0)
      stars[i].z += 1;
    if (stars[i].z > 1)
      stars[i].z -= 1;
  }
}

void interpolate(vec3 a, vec3 b, vector<vec3> &result)
{

  float size = result.size();
  float x = size - 1;

  float dx = (b.x - a.x) / (x);
  float dy = (b.y - a.y) / (x);
  float dz = (b.z - a.z) / (x);

  for (float i = 0; i < result.size(); ++i)
  {
    result[i].x = a.x + dx * i;
    result[i].y = a.y + dy * i;
    result[i].z = a.z + dz * i;
  }
}