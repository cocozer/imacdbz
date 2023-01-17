#include "application_ui.h"
#include "SDL2_gfxPrimitives.h"
#include <iostream>
#include <ctime>
#include <cstdlib>
using namespace std;
// Fonctions

    int randomNumber(int min, int max)
    {
        return min + (rand() % max);
    }
// STRUCT VECTOR 2 FONCTIONS

int Vector2::returnx()
{
    return this->x;
}

int Vector2::returny()
{
    return this->y;
}

void Vector2::writeVector2(int x, int y)
{
    this->x = x;
    this->y = y;
}

// STRUCT VECTOR 3 FONCTIONS

int Vector3::returnr()
{
    return this->r;
}

int Vector3::returng()
{
    return this->g;
}

int Vector3::returnb()
{
    return this->b;
}

void Vector3::writeVector3(unsigned int r, unsigned int g, unsigned int b)
{
    this->r = r;
    this->g = g;
    this->b = b;
}

void Wall::writeWall(int x1, int y1, int x2, int y2)
{
    this->x1 = x1;
    this->x2 = x2;
    this->y1 = y1;
    this->y2 = y2;
}

void internWall::writeInternWall(bool horizontal, int taille, Vector2 position)
{
    this->horizontal = horizontal;
    this->taille = taille;
    this->position = position;
}
void internWall::draw(SDL_Renderer *renderer) {
    if (this->horizontal == true) {
        hlineRGBA(renderer, this->position.x, this->position.x+this->taille, this->position.y, 255, 255, 255, 255);
    } else {
        vlineRGBA(renderer, this->position.x, this->position.y, this->position.y+this->taille, 255, 255, 255, 255);
    }
}
// STRUCT BALL FONCTIONS
void Ball::writeBall(int x, int y, int vx, int vy, int rayon, unsigned int r, unsigned int g, unsigned int b, unsigned int br, unsigned int bg, unsigned int bb)
{
    this->position.writeVector2(x, y);
    this->velocity.writeVector2(vx, vy);
    this->radius.writeVector2(rayon, rayon);
    this->color.writeVector3(r, g, b);
    this->bordercolor.writeVector3(br, bg, bb);
}
void Ball::draw(SDL_Renderer *renderer)
{
    /* Remplissez cette fonction pour faire l'affichage du jeu */

    filledEllipseRGBA(renderer, this->position.returnx(), this->position.returny(), this->radius.returnx(), this->radius.returny(), this->color.returnr(), this->color.returng(), this->color.returnb(), 255);
    ellipseRGBA(renderer, this->position.returnx(), this->position.returny(), this->radius.returnx(), this->radius.returny(), this->bordercolor.returnr(), this->bordercolor.returng(), this->bordercolor.returnb(), 255);

    /*SDL2_GFXPRIMITIVES_SCOPE int filledEllipseRGBA(SDL_Renderer * renderer, Sint16 x, Sint16 y,
        Sint16 rx, Sint16 ry, Uint8 r, Uint8 g, Uint8 b, Uint8 a);*/
}
void Ball::updatePosition()
{
    this->position.x += this->velocity.x;
    this->position.y += this->velocity.y;
}
void Ball::checkWalls(Wall wall)
{

    // Si la balle touche un bord, sa vitesse est inversée
    if (this->position.x - this->radius.x <= wall.x1)
    {
        this->velocity.x = -this->velocity.x;
    }
    else if (this->position.y - this->radius.y <= wall.y1)
    {
        this->velocity.y = -this->velocity.y;
    }
    else if (this->position.x + this->radius.x >= wall.x2)
    {
        this->velocity.x = -this->velocity.x;
    }
    else if (this->position.y + this->radius.y >= wall.y2)
    {
        this->velocity.y = -this->velocity.y;
    }
}
void Ball::checkBalls(Ball *balls)
{
    // On parcourt toutes les balles
    Ball *otherball = balls;
    while (otherball != nullptr) {
        // On vérifie que la balle du parcourt n'est pas la balle du this
        if (otherball != this) {
            // On calcule la différence en x et en y entre la balle du this et la balle du parcourt
            int xDiff = otherball->position.x - this->position.x;
            int yDiff = otherball->position.y - this->position.y;
            // On calcule la distance entre les deux balles
            int distance = sqrt((xDiff * xDiff) + (yDiff * yDiff));
            // Si la distance entre les deux balles est plus grande que les deux rayons des balles additionnés (si elles se touchent)
            if (distance <= otherball->radius.x + this->radius.x) {
                // On inverse leurs vitesses 
                int otherballX = otherball->velocity.x;
                int otherballY = otherball->velocity.y;
                otherball->velocity.x = this->velocity.x;
                otherball->velocity.y = this->velocity.y;
                this->velocity.x = otherballX;
                this->velocity.y = otherballY;
            }
        }
        otherball = otherball->next;
    }
}
Ball *newBallWithPos(Ball *list, int x, int y) {
    Ball *new_ball = new Ball;
    if (new_ball == nullptr)
    {
        cout << "Memory run out." << endl;
        exit(1);
    }
    new_ball->position.x = x;
    new_ball->position.y = y;

    // Première génération d'un nombre aléatoire pour décider si la vélocité sera positive ou négative
    // Puis ensuite, si c'est négatif on génère une vélocité négative. sinon positive.
    if (randomNumber(0, 1) == 0) {
        new_ball->velocity.x = randomNumber(-7, 7);
    }   else {
        new_ball->velocity.x = randomNumber(7, 7);
    }
    if (randomNumber(0, 1) == 0) {
        new_ball->velocity.y = randomNumber(-7, 7);
    }   else {
        new_ball->velocity.y = randomNumber(7, 7);
    }
    int radius = randomNumber(10, 20);
    new_ball->radius.x = radius;
    new_ball->radius.y = radius;
    new_ball->color.r = 110;
    new_ball->color.g = 150;
    new_ball->color.b = 30;
    new_ball->bordercolor.r = 255;
    new_ball->bordercolor.g = 255;
    new_ball->bordercolor.b = 255;
    new_ball->next = nullptr;
    if (list != nullptr)
    {
        find_last(list)->next = new_ball;
        return list;
    }
    else
        return new_ball;
}
Ball *newBallWithColor(Ball *list, int r, int g, int b) {
    Ball *new_ball = new Ball;
    if (new_ball == nullptr)
    {
        cout << "Memory run out." << endl;
        exit(1);
    }
    new_ball->position.x = randomNumber(30, 690);
    new_ball->position.y = randomNumber(30, 410);

    // Première génération d'un nombre aléatoire pour décider si la vélocité sera positive ou négative
    // Puis ensuite, si c'est négatif on génère une vélocité négative. sinon positive.
    if (randomNumber(0, 1) == 0) {
        new_ball->velocity.x = randomNumber(-7, 7);
    }   else {
        new_ball->velocity.x = randomNumber(7, 7);
    }
    if (randomNumber(0, 1) == 0) {
        new_ball->velocity.y = randomNumber(-7, 7);
    }   else {
        new_ball->velocity.y = randomNumber(7, 7);
    }
    int radius = randomNumber(10, 20);
    new_ball->radius.x = radius;
    new_ball->radius.y = radius;
    new_ball->color.r = r;
    new_ball->color.g = g;
    new_ball->color.b = b;
    new_ball->bordercolor.r = 255;
    new_ball->bordercolor.g = 255;
    new_ball->bordercolor.b = 255;
    new_ball->next = nullptr;
    if (list != nullptr)
    {
        find_last(list)->next = new_ball;
        return list;
    }
    else
        return new_ball;
}
Ball *newBall(Ball *list)
{
    Ball *new_ball = new Ball;
    if (new_ball == nullptr)
    {
        cout << "Memory run out." << endl;
        exit(1);
    }
    new_ball->position.x = randomNumber(30, 690);
    new_ball->position.y = randomNumber(30, 410);
    // Première génération d'un nombre aléatoire pour décider si la vélocité sera positive ou négative
    // Puis ensuite, si c'est négatif on génère une vélocité négative. sinon positive.
    if (randomNumber(0, 1) == 0) {
        new_ball->velocity.x = -randomNumber(1, 7);
    }   else {
        new_ball->velocity.x = randomNumber(1, 7);
    }
    if (randomNumber(0, 1) == 0) {
        new_ball->velocity.y = -randomNumber(1, 7);
    }   else {
        new_ball->velocity.y = randomNumber(1, 7);
    }
    int radius = randomNumber(10, 20);
    new_ball->radius.x = radius;
    new_ball->radius.y = radius;
    new_ball->color.r = 110;
    new_ball->color.g = 150;
    new_ball->color.b = 30;
    new_ball->bordercolor.r = 255;
    new_ball->bordercolor.g = 255;
    new_ball->bordercolor.b = 255;
    new_ball->next = nullptr;
    if (list != nullptr)
    {
        find_last(list)->next = new_ball;
        return list;
    }
    else
        return new_ball;
}

Ball *find_last(Ball *list)
{
    while (list->next != nullptr)
    {
        list = list->next;
    }
    return list;
}

void handleEvent(Ball **balls)
{

    /* Définition des variables de position de curseur */
    int xMouse;
    int yMouse;
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        Ball *previous = nullptr;
        Ball *current = *balls;
        // Condition de clic
        if (e.type == SDL_MOUSEBUTTONDOWN)
        { // lors d'un clic
            // Capture les positions du curseur à l'instant du clic
            bool clickedonball = false;
            SDL_GetMouseState(&xMouse, &yMouse);
            while (current != nullptr)
            {
                
                // for (int i = 0; i<nbBalles;i++){

                // // Au lieu de prendre simplement la position, on divise les valeurs par le rayon pour pouvoir cliquer sur une balle en cliquant sur n'importe quel endroit de sa surface
                // int yMouseDividedByRadius = yMouse / balles[i].radius.y;
                // int xMouseDividedByRadius = xMouse / balles[i].radius.x;
                // int yBallPositionDividedByRadius = balles[i].position.y / balles[i].radius.y;
                // int xBallPositionDividedByRadius = balles[i].position.x / balles[i].radius.x;

                if (yMouse > current->position.y - current->radius.y && yMouse < current->position.y + current->radius.y && xMouse > current->position.x - current->radius.x && xMouse < current->position.x + current->radius.x)
                {
                //L'utilisateur a cliqué sur la balle
                    clickedonball = true;
                    if (previous == nullptr) {
                        *balls = current->next;
                    } else if (find_last(*balls) == current) {

                        previous->next = nullptr;
                    } else {
                        previous->next = current->next;
                    }
                    break;
                } 
                // }
                previous = current;
                current = current->next;
            }
            // Si l'utilisateur n'a pas cliqué sur la balle, on crée une nouvelle balle
            if (clickedonball == false) {
                *balls = newBallWithPos(*balls, xMouse, yMouse);
            }
        }
    }
    // Ball *previous = nullptr;
    // Ball *current = *balls;
    // // Condition de clic
    // if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(1))
    // { // lors d'un clic
    //     // Capture les positions du curseur à l'instant du clic
    //     bool clickedonball = false;
    //     SDL_GetMouseState(&xMouse, &yMouse);
    //     while (current != nullptr)
    //     {
            
    //         // for (int i = 0; i<nbBalles;i++){

    //         // // Au lieu de prendre simplement la position, on divise les valeurs par le rayon pour pouvoir cliquer sur une balle en cliquant sur n'importe quel endroit de sa surface
    //         // int yMouseDividedByRadius = yMouse / balles[i].radius.y;
    //         // int xMouseDividedByRadius = xMouse / balles[i].radius.x;
    //         // int yBallPositionDividedByRadius = balles[i].position.y / balles[i].radius.y;
    //         // int xBallPositionDividedByRadius = balles[i].position.x / balles[i].radius.x;

    //         if (yMouse > current->position.y - current->radius.y && yMouse < current->position.y + current->radius.y && xMouse > current->position.x - current->radius.x && xMouse < current->position.x + current->radius.x)
    //         {
    //            //L'utilisateur a cliqué sur la balle
    //             clickedonball = true;
    //             if (previous == nullptr) {
    //                 *balls = current->next;
    //             } else if (find_last(*balls) == current) {

    //                 previous->next = nullptr;
    //             } else {
    //                 previous->next = current->next;
    //             }
    //             break;
    //         } 
    //         // }
    //         previous = current;
    //         current = current->next;
    //     }
    //     // Si l'utilisateur n'a pas cliqué sur la balle, on crée une nouvelle balle
    //     bool ballspawned = false;
    //     if (clickedonball == false && ballspawned == false) {
    //         *balls = newBall(*balls);
    //     }
    // }
}

void moveBalls(Ball *balls, Wall wall)
{
    Ball *current = balls;
    while (current != nullptr)
    {
        current->checkBalls(balls);
        current->checkWalls(wall);
        current->updatePosition();
        current = current->next;
    }
}

void draw(Ball *balls, SDL_Renderer *renderer)
{
    while (balls->next != nullptr)
    {
        balls->draw(renderer);
        balls = balls->next;
    }
    balls->draw(renderer);
}
// bool handleEvent()
// {
//     /* Remplissez cette fonction pour gérer les inputs utilisateurs */
//     SDL_Event e;
//     while (SDL_PollEvent(&e)){
//         switch (e.type) {
//             case SDL_MouseButtonEvent:

//                 break;
//         }
//             return false;
//     }
//     return true;
// }

int main(int argc, char **argv)
{
    SDL_Window *gWindow;
    SDL_Renderer *renderer;
    bool is_running = true;

    // Creation de la fenetre
    gWindow = init("dbzimac (attrape toutes les boules de cristal)");

    if (!gWindow)
    {
        SDL_Log("Failed to initialize!\n");
        exit(1);
    }
    renderer = SDL_CreateRenderer(gWindow, -1, 0); // SDL_RENDERER_PRESENTVSYNC

    // Initialisation des acteurs

    // Initialisation des balles

    // Initialisation des balles
    Ball *balls = nullptr;
        int nbBalles = 5;
    if (argc == 2) {
        int arg = atoi(argv[1]);
        if (arg > 0) {
            nbBalles = arg;
        }
    } else if (argc > 2) {
        for (int i = 1; i+1 < argc; i++){
            int r;
            int g;
            int b;
            sscanf(argv[i], "%d,%d,%d", &r, &g, &b);
            balls = newBallWithColor(balls,r, g, b);
        }
    }

    for (int i = 0; i < nbBalles; i++)
    {
        balls = newBall(balls);
    }

    // Initialisations des murs
    Wall wall;
    wall.writeWall(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // Initialisation des murs internes
    internWall walls[3];
    int nbInternWalls = 3;
    
    for (int i = 0; i<nbInternWalls;i++) {
        internWall wall;
        Vector2 posmur;
        posmur.x = randomNumber(0, SCREEN_WIDTH);
        posmur.y = randomNumber(0, SCREEN_HEIGHT);
        int taille = randomNumber(50, 1000);
        bool horizontal;
        if (randomNumber(1, 2) == 1) {
            horizontal = true;
        } else {
            horizontal = false;
        }
        wall.writeInternWall(horizontal, taille, posmur);
        walls[i] = wall;
        wall.draw(renderer);
    }
    /*  GAME LOOP  */

    while (true)
    {
        // INPUTS
        // is_running = handleEvent(&balles[nbBalles], nbBalles);
        if (!is_running)
        {
            break;
        }
        handleEvent(&balls);
        // GESTION ACTEURS
        // for(int i=0; i<nbBalles; i++){
        //     balles[i].checkWalls(wall);
        //     balles[i].updatePosition();
        // }
        moveBalls(balls, wall);
        // ...

        // EFFACAGE FRAME
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // DESSIN
        // string var = "salut";
        // SDL_Log(var.c_str());
        draw(balls, renderer);

        // VALIDATION FRAME
        SDL_RenderPresent(renderer);

        // PAUSE en ms
        SDL_Delay(1000 / 30);
        // temporaire off de runing
    }

    // Free resources and close SDL
    close(gWindow, renderer);

    return 0;
}
