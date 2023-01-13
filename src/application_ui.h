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

// Fonctions
int randomNumber(int min, int max);
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
    unsigned int r;
    unsigned int g;
    unsigned int b;

    int returnr();
    int returng();
    int returnb();
    // Assignation des valeurs dans l'objet crée
    void writeVector3(unsigned int r, unsigned int g, unsigned int b);
};
struct Wall {
    int x1;
    int x2;
    int y1;
    int y2;

    void writeWall(int x1, int y1, int x2, int y2);
};
typedef struct Ball {
    Vector2 position;
    Vector2 velocity;
    Vector2 radius;
    Vector3 color;
    Vector3 bordercolor;
    Ball *next;

    bool active;
    // Assignation des valeurs dans l'objet crée
    void writeBall(int x, int y, int vx, int vy, int rayon, unsigned int r, unsigned int g, unsigned int b, unsigned int br, unsigned int bg, unsigned int bb);
    // On ajoute une nouvelle balle à la liste chainée
    // On dessine les balles à l'endroit où elles sont à l'instant T
    void draw(SDL_Renderer* renderer);
    // On update la position des balles en fonction de la vélocité
    void updatePosition();
    // On vérifie si la balle touche les murs, si oui, on la fait changer de sens
    void checkWalls(Wall wall);

} Ball;
//Création d'une nouvelle balle et ajout dans la liste chainée
Ball *newBall(Ball *list);
Ball *newBallWithPos(Ball *list, int x, int y);
void handleEvent(Ball *balls);
Ball *find_last(Ball *list);

void moveBalls(Ball *Balls, Wall wall);
void draw(Ball *Balls, SDL_Renderer* renderer);
#endif

