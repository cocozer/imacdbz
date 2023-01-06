#ifndef APPLICATION_UI_H
#define APPLICATION_UI_H
using namespace std;
#include <iostream>
#include <SDL2/SDL.h>
#include <string>

extern const int SCREEN_WIDTH, SCREEN_HEIGHT;

SDL_Window* init(std::string windowTitle);
SDL_Surface* loadMedia(std::string path);
void close(SDL_Window* gWindow, SDL_Renderer* renderer);

// Création de vecteurs pour pouvoir stocker les informations doubles et triples (par exemple, la position, la vitesse, le rayon, la couleur en RGB)
struct Vector2 {
    int x;
    int y;

    int returnx();
    int returny();
    // Assignation des valeurs dans l'objet crée
    void writeVector2(int x, int y);
};
struct Vector3 {
    int r;
    int g;
    int b;

    int returnr();
    int returng();
    int returnb();
    // Assignation des valeurs dans l'objet crée
    void writeVector3(int r, int g, int b);
};

struct Ball {
    Vector2 position;
    Vector2 velocity;
    Vector2 radius;
    Vector3 color;
    Vector3 bordercolor;
    // Assignation des valeurs dans l'objet crée
    void writeBall(int x, int y, int vx, int vy, int rx, int ry, int r, int g, int b, int br, int bg, int bb);
    // On dessine les balles à l'endroit où elles sont à l'instant T
    void draw(SDL_Renderer* renderer);
    // On update la position des balles en fonction de la vélocité
    void updatePosition();

};
#endif

