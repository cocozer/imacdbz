#include "application_ui.h"
#include "SDL2_gfxPrimitives.h"

// STRUCT VECTOR 2 FONCTIONS

int Vector2::returnx()
{
    return this->x;
}

int Vector2::returny()
{
    return this->y;
}

void Vector2::writeVector2(int x, int y) {
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

void Vector3::writeVector3(int r, int g, int b){
    this->r = r;
    this->g = g;
    this->b = b;
}

void Wall::writeWall(int x1, int y1, int x2, int y2){
    this->x1 = x1;
    this->x2 = x2;
    this->y1 = y1;
    this->y2 = y2;
}
// STRUCT BALL FONCTIONS
void Ball::writeBall(int x, int y, int vx, int vy, int rx, int ry, int r, int g, int b, int br, int bg, int bb){
    this->position.writeVector2(x, y);
    this->velocity.writeVector2(vx, vy);
    this->radius.writeVector2(rx, ry);
    this->color.writeVector3(r, g, b);
    this->bordercolor.writeVector3(br, bg, bb);
}
void Ball::draw(SDL_Renderer* renderer)
{
    /* Remplissez cette fonction pour faire l'affichage du jeu */
    
    filledEllipseRGBA(renderer, this->position.returnx(), this->position.returny(), this->radius.returnx(), this->radius.returny(), this->color.returnr(), this->color.returng(), this->color.returnb(), 255); 
    ellipseRGBA(renderer, this->position.returnx(), this->position.returny(), this->radius.returnx(), this->radius.returny(), this->bordercolor.returnr(), this->bordercolor.returng(), this->bordercolor.returnb(), 255);

    /*SDL2_GFXPRIMITIVES_SCOPE int filledEllipseRGBA(SDL_Renderer * renderer, Sint16 x, Sint16 y,
		Sint16 rx, Sint16 ry, Uint8 r, Uint8 g, Uint8 b, Uint8 a);*/
   
}
void Ball::updatePosition(){
    this->position.x+=this->velocity.x;
    this->position.y+=this->velocity.y;
}
void Ball::checkWalls(Wall wall){

    // Si la balle touche un bord, sa vitesse est inversée
    if (this->position.x - this->radius.x <= wall.x1) {
        this->velocity.x = - this->velocity.x;
    } else if (this->position.y - this->radius.y <= wall.y1) {
        this->velocity.y = -this->velocity.y;
    } else if (this->position.x + this->radius.x >= wall.x2) {
        this->velocity.x = - this->velocity.x;
    }   else if (this->position.y + this->radius.y >= wall.y2) {
        this->velocity.y = - this->velocity.y;
    }
}
bool handleEvent()
{
    /* Remplissez cette fonction pour gérer les inputs utilisateurs */
    SDL_Event e; 
    while (SDL_PollEvent(&e)){ 
        if (e.type == SDL_QUIT) 
            return false; 
    }
    return true;
}


int main(int argc, char** argv) {
    SDL_Window* gWindow;
    SDL_Renderer* renderer;
    bool is_running = true; 

    // Creation de la fenetre
    gWindow = init("Awesome Game");

    if (!gWindow)
    {
        SDL_Log("Failed to initialize!\n");
        exit(1);
    }

    renderer = SDL_CreateRenderer(gWindow, -1, 0); // SDL_RENDERER_PRESENTVSYNC

        // Initialisation des acteurs
        Ball balle1;
        balle1.writeBall(50, 50, 3, 1, 15, 15, 255, 0, 100, 255, 255, 255);
        Wall wall;
        wall.writeWall(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    /*  GAME LOOP  */
    
    while(true)
    {
        // INPUTS
        is_running = handleEvent();
        if (!is_running)
            break;

        // GESTION ACTEURS
        balle1.checkWalls(wall);
        balle1.updatePosition();
        // ...
        
        // EFFACAGE FRAME
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        
        // DESSIN
        balle1.draw(renderer);

        // VALIDATION FRAME
        SDL_RenderPresent(renderer);

        // PAUSE en ms
        SDL_Delay(1000/30); 
    }

    //Free resources and close SDL
    close(gWindow, renderer);

    return 0;
}
