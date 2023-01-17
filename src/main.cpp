#include "application_ui.h"
#include "SDL2_gfxPrimitives.h"
#include <iostream>
#include <ctime>
#include <cstdlib>
using namespace std;
// Fonctions
// Fonction qui à partir d'un minimum et d'un maximum, renvoie un nombre aléatoire
int randomNumber(int min, int max){
        return min + (rand() % max);
}
// STRUCT VECTOR 2 FONCTIONS
// Fonctions qui retournent les valeurs du Vecteur2
int Vector2::returnx()
{
    return this->x;
}

int Vector2::returny()
{
    return this->y;
}

//Fonction d'écriture d'un Vecteur2
void Vector2::writeVector2(int x, int y)
{
    this->x = x;
    this->y = y;
}

// STRUCT VECTOR 3 FONCTIONS

// Fonctions qui retournent les valeurs du Vecteur3
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
// Fonction qui écrit et assigne les valeurs du Vecteur 3
void Vector3::writeVector3(unsigned int r, unsigned int g, unsigned int b)
{
    this->r = r;
    this->g = g;
    this->b = b;
}

// Fonctions de la structure WALL
// Fonction qui assigne et écrit les valeurs d'un mur rectanglaire (mur extérieur)
void Wall::writeWall(int x1, int y1, int x2, int y2)
{
    this->x1 = x1;
    this->x2 = x2;
    this->y1 = y1;
    this->y2 = y2;
}

//Fonction qui écrit et assigne les valeurs d'un mur interne
void internWall::writeInternWall(bool horizontal, int taille, Vector2 position)
{
    this->horizontal = horizontal;
    this->taille = taille;
    this->position = position;
}
// Fonction qui dessine un mur interne
void internWall::draw(SDL_Renderer *renderer) {

    //Si le mur est horizontal, on appelle la fonction qui dessine une ligne horizontale
    if (this->horizontal == true) {
        hlineRGBA(renderer, this->position.x, this->position.x+this->taille, this->position.y, 255, 255, 255, 255);
    } else { // Sinon, on appelle celle de la ligne verticale
        vlineRGBA(renderer, this->position.x, this->position.y, this->position.y+this->taille, 255, 255, 255, 255);
    }
}
// STRUCT BALL FONCTIONS
// Fonction qui assigne et écrit les valeurs d'une balle
void Ball::writeBall(int x, int y, int vx, int vy, int rayon, unsigned int r, unsigned int g, unsigned int b, unsigned int br, unsigned int bg, unsigned int bb)
{
    this->position.writeVector2(x, y);
    this->velocity.writeVector2(vx, vy);
    this->radius.writeVector2(rayon, rayon);
    this->color.writeVector3(r, g, b);
    this->bordercolor.writeVector3(br, bg, bb);
}
// Fonction qui dessine une balle
void Ball::draw(SDL_Renderer *renderer)
{
    /* Remplissez cette fonction pour faire l'affichage du jeu */
    // on dessine la balle 
    filledEllipseRGBA(renderer, this->position.returnx(), this->position.returny(), this->radius.returnx(), this->radius.returny(), this->color.returnr(), this->color.returng(), this->color.returnb(), 255);
    // on dessine l'anneau de la balle pour que ça ressemble à une planète comme saturne lol !!!
    ellipseRGBA(renderer, this->position.returnx(), this->position.returny(), this->radius.returnx()+30, this->radius.returny()-10, this->bordercolor.returnr(), this->bordercolor.returng(), this->bordercolor.returnb(), 255);

}
// Fonction qui met à jour la position de la balle en additionant la velocité (qui comprend la direction et la vitesse) à la position pour définir la nouvelle position
void Ball::updatePosition()
{
    this->position.x += this->velocity.x;
    this->position.y += this->velocity.y;
}
// Fonction qui vérifie si une balle heurte un mur extérieur. Si c'est le cas, on inverse la vélocité de la balle sur l'axe qui a touché le mur pour la faire rebondir
void Ball::checkWalls(Wall wall)
{

    // Si la balle touche un bord, sa vitesse dans l'axe concerné est inversée
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
// Fonction qui vérifie si une balle heurte une autre balle. Si c'est le cas, on échange les vitesses des deux balles pour les faire rebondir entre elles
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
//On crée une nouvelle balle aux coordonnées données
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
    if (randomNumber(1, 2) == 1) {
        new_ball->velocity.x = randomNumber(-7, 7);
    }   else {
        new_ball->velocity.x = randomNumber(7, 7);
    }
    if (randomNumber(1, 2) == 1) {
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
// Création d'une balle avec une couleur donnée
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
    if (randomNumber(1, 2) == 1) {
        new_ball->velocity.x = randomNumber(-7, 7);
    }   else {
        new_ball->velocity.x = randomNumber(7, 7);
    }
    if (randomNumber(1, 2) == 1) {
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
// Création d'une balle totalement aléatoire
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
    if (randomNumber(1, 2) == 1) {
        new_ball->velocity.x = -randomNumber(1, 7);
    }   else {
        new_ball->velocity.x = randomNumber(1, 7);
    }
    if (randomNumber(1, 2) == 1) {
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
// On trouve la dernière balle de la liste chainée et on la renvoie
Ball *find_last(Ball *list)
{
    while (list->next != nullptr)
    {
        list = list->next;
    }
    return list;
}
// Fonction qui gère les clics de l'utilisateur
void handleEvent(Ball **balls)
{
    /* Définition des variables de position de curseur */
    int xMouse;
    int yMouse;
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        // On définit une balle actuelle et la balle qui la précède (ça sert à pouvoir supprimer la balle actuelle)
        Ball *previous = nullptr;
        Ball *current = *balls;
        // Si on presse le clic (renvoie une seule information et n'est pas un levier)
        if (e.type == SDL_MOUSEBUTTONDOWN)
        { // lors d'un clic
            // On crée une variable qui indique si le clic est sur la balle ou pas. On met cette variable de base sur faux
            bool clickedonball = false;
            // Capture les positions du curseur à l'instant du clic
            SDL_GetMouseState(&xMouse, &yMouse);
            // On parcourt toutes les balles
            while (current != nullptr)
            {
                //Si l'utilisateur a cliqué sur la balle
                if (yMouse > current->position.y - current->radius.y && yMouse < current->position.y + current->radius.y && xMouse > current->position.x - current->radius.x && xMouse < current->position.x + current->radius.x)
                {

                    // On indique que la balle a été cliquée à la variable de tout à l'heure
                    clickedonball = true;
                    // On cherche maintenant à supprimer la balle
                    // On traite chaque cas : si c'est la première balle de la liste chainée, alors la liste commencera à partir de la deuxième balle
                    if (previous == nullptr) {
                        *balls = current->next;
                    } else if (find_last(*balls) == current) { // on traite chaque cas : Si c'est la dernière balle, alors la balle qui la précède n'aura plus de balle suivante
                        previous->next = nullptr;
                    } else { // Si aucun des cas spéciaux n'est effectif, on assigne la prochaine balle de la précédente à la prochaine (ce qui supprime celle actuelle)
                        previous->next = current->next;
                    }
                    break;
                } 
                // }
                // On assigne la balle précédente à l'actuelle
                previous = current;
                // On assigne la balle actuelle à la prochaine 
                current = current->next; // Ce qui fait que la balle previous reste celle d'avant
            }
            // Si l'utilisateur n'a pas cliqué sur la balle, on crée une nouvelle balle à l'endroit cliqué
            if (clickedonball == false) {
                *balls = newBallWithPos(*balls, xMouse, yMouse);
            }
        }
    }
}
// Fonction qui fait bouger les balles
void moveBalls(Ball *balls, Wall wall)
{
    // On parcourt toutes les balles.
    Ball *current = balls;
    while (current != nullptr)
    {
        current->checkBalls(balls); // On vérifie si la balle touche une balle, si oui la fonction fait rebondir les balles entre elles
        current->checkWalls(wall); // On vérifie si la balle touche un mur extérieur, si oui la fonction fait rebondir la balle sur l'axe touché
        current->updatePosition(); // On fait bouger la balle vers l'endroit où elle est dirigée à la vitesse qui lui est assignée
        current = current->next;
    }
}
// Fonction qui dessine toutes les balles
void draw(Ball *balls, SDL_Renderer *renderer)
{
    // On parcourt toutes les balles et on les dessine toutes
    while (balls->next != nullptr)
    {
        balls->draw(renderer);
        balls = balls->next;
    }
    balls->draw(renderer);
}
// Je vous présente la fonction main Ô_Ô
int main(int argc, char **argv) {
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

    // Initialisation des balles
    Ball *balls = nullptr;
    int nbBalles = 5; // A la base, le nombre de balles est de 5
    if (argc == 2) { // Si on lance le programme avec un argument
        int arg = atoi(argv[1]); // on récupère l'argument
        if (arg > 0) { // Si cet argument n'est pas égal à 0
            nbBalles = arg; // le nombre de balles change et devient l'argument
        }
    } else if (argc > 2) { // Si il y a plusieurs arguments 
        for (int i = 1; i+1 < argc; i++){ // On part de l'argument 1 et on parcourt tous les arguments
            int r;
            int g;
            int b;
            sscanf(argv[i], "%d,%d,%d", &r, &g, &b); // on récupère les nombres des couleurs et on les assigne dans les variables r, g et b
            balls = newBallWithColor(balls,r, g, b); // on crée une balle à partir des couleurs et on l'assigne à la liste chainée balls
        }
    }

    for (int i = 0; i < nbBalles; i++) // on crée autant de balles qu'il y a dans nbBalles
    {
        balls = newBall(balls);
    }

    // Initialisations des murs
    Wall wall; // On crée un mur
    wall.writeWall(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT); // on met le mur autour de la fenêtre

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
        moveBalls(balls, wall);
        // ...

        // EFFACAGE FRAME
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // DESSIN
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
