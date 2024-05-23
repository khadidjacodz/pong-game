#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <SDL2/SDL_ttf.h>



   /* declaration constantes */

    const int hauteur = 800;// la hauteur de la fenetre
    const int largeur = 1280;// la largeur de la fenetre
    const int ball_size = 15;// taille de la balle
    const int Player_largeur = 20;// largeur de la raquette
    const int Player_hauteur = 75;// hauteur de la raquette
    const int Player_marge = 10;// la marge entre le bord de la fenetre et la raquette
    const int Player_vitesse_mouvement = 200.0f;// la vitesse du mouvement de la raquette
    const float speed = 250; // on initialise la vitesse de la balle

     /* enregistrement pour les donnees de la balle */
    typedef struct Ball {
        float x; // l'abscisse de la balle
        float y;// l'ordonn�e de la balle
        int size; // la taille de la balle
        float xspeed;// la vitesse de la balle sur l'axe x
        float yspeed;// la vitesse de la balle sur l'axe y

    } Ball;

    /* enregistrement pour les donnees des joueurs */
    typedef struct Player{
        int score; // score du joueur
        float yPosition; // position de la raquette du joueur
    }Player;


    bool served = false; // bool�en qu'on va utiliser pour faire bouger la balle

    Ball ball; // faire appel a l'enregistrement Ball pour ball

    Player player1;// faire appel a l'enregistrement player pour player1
    Player player2;// faire appel a l'enregistrement player pour player2

    SDL_Window *Window = NULL;     // pointeur de la fenetre de la sdl
    SDL_Renderer *renderer = NULL; // structure pour travailler sur le rendu de la fenetre

   /* declaration des fonctions que nous allonts utiliser */
    bool Initialiser_Pong (void);
    void update(float);
    void shutdown(void);
    Ball makeball(int size);
    void updateball(Ball *ball, float elapsed);
    void renderball(const Ball*);

    Player makePlayer(void);
    void updatePlayers(float elapsed);
    void renderPlayers(void);

    int updatescore(int player ,int points);
    bool coinflip (void);


    /* initialitaion du jeu pong */

    bool nitialiser_Pong (void)
     {
        if (SDL_Init(SDL_INIT_VIDEO) != 0 )
        {
        fprintf(stdout,"�chec de l'initialisation de la SDL (%s)\n",SDL_GetError()); // afficher le message en cas d'echec de l'initialisation
        return false;
        }

        Window = SDL_CreateWindow("jeu de pong s&k",SDL_WINDOWPOS_UNDEFINED,//creation de
        SDL_WINDOWPOS_UNDEFINED,largeur,hauteur ,SDL_WINDOW_SHOWN);         //la fenetre

        if(!Window) // gestion des erreurs
        {
            return false;
        }

         renderer = SDL_CreateRenderer(Window, -1,                 //creation
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);//du renderer

        if(!Window) // gestion des erreurs
        {
            return false;
        }
        ball = makeball(ball_size); // on appel la fonction pour repr�senter la ball

        return true;
    }



 /* fonction update qui sert a metre a jour tout ce qu'il faut */
    void update(float elapsed)
    {
 SDL_SetRenderDrawColor(renderer,53,114,142,199); // g�rer la couleur de la fenetre
 SDL_RenderClear(renderer);

 updateball(&ball, elapsed);// on appelle la fonction updateball
 renderball(&ball);// on appelle la fonction renderball

 updatePlayers(elapsed);// on appelle la fonction updateplayers
 renderPlayers(); // on appelle la fonction renderplayers

 SDL_RenderPresent(renderer);// faire aparaitre les modifications

    }




/* la fonction shutdown */
    void shutdown(void){
       if  (renderer){
        SDL_DestroyRenderer(renderer); // d�truire le rendere
       }

       if(Window){
        SDL_DestroyWindow(Window); // d�truire la fenetre
       }


        SDL_Quit(); // quiter la sdl
    }


        /* sous-programme pour g�rer le rebondissement */

    bool coinflip(void){
    return rand() % 2 == 1 ? true : false; // bool�en pour g�rer le rebond sur les coins
    }
        /* fonction pour dessiner notre balle */
        Ball makeball(int size){
        Ball ball = {
            .x = largeur / 2 - size / 2,              //
            .y = hauteur / 2 - size / 2,              //  on remplie les
            .size = size,                             //      donnees
            .xspeed = speed * (coinflip() ? 1 : -1),  //  de notre balle
            .yspeed = speed * (coinflip() ? 1 : -1),  //
         };
            return ball;

        }


                /* fonction pour remplire la balle */
        void renderball(const Ball *ball){ // cr�er� la balle
        int size = ball ->size;
        int halfsize = size/2;
        SDL_Rect rect = {    // dessiner notre balle
            .x = ball->x - halfsize, //  repr�sente la position
            .y = ball->y - halfsize, //     dans la fenetre
            .w = size,  // largeur
            .h = size, // hauteur
        };
        SDL_SetRenderDrawColor(renderer,0,0,0,225); // g�rer la couleur de la balle
        SDL_RenderFillRect(renderer,&rect);
        }



           /* fonction faire bouger la balle */
    void updateball(Ball *ball, float elapsed){

       if(!served){
        ball->x = largeur / 2; //servire la balle
        ball->y = hauteur / 2; //au milieu
        return;
       }
        ball->x += ball->xspeed * elapsed;//faire bouger la
        ball->y += ball->yspeed * elapsed;//balle

     if(ball->x <ball_size / 2 )
     {
        updatescore(2,1); // on incr�mente le score
     }
     if(ball->x > largeur - ball_size / 2)
     {
       updatescore(1,1); // on incr�mente le score
     }
     if(ball->y < ball_size / 2)
     {
        ball->yspeed = fabs(ball->yspeed);
     }
     if(ball->y > hauteur - ball_size / 2)
     {
        ball->yspeed = -fabs(ball->yspeed);
     }
    }


        Player makePlayer(void){
           Player player ={
                .yPosition = hauteur / 2,
           };
            return player;
        }

/* sous-programme pour faire bouger les raquetttes */
    void updatePlayers(float elapsed)
    {
       const Uint8 *KeyboardState = SDL_GetKeyboardState(NULL);

       if(KeyboardState[SDL_SCANCODE_SPACE]) { //commencer � jouer
                                               // quand l'utilisateur
            served = true;                     // appuie sur espace
       }

       // faire bouger la raquette gauche automatiquement
       if(player1.yPosition+Player_hauteur/2<ball.y){

        player1.yPosition += Player_vitesse_mouvement*1.3 * elapsed;

       }

        if(player1.yPosition+Player_hauteur/2>ball.y){

        player1.yPosition-=Player_vitesse_mouvement*1.3 * elapsed;

       }
            // faire bouger la balle avec les fleches
         if(KeyboardState[SDL_SCANCODE_UP]){

        player2.yPosition -= Player_vitesse_mouvement * elapsed;

       }

        if(KeyboardState[SDL_SCANCODE_DOWN]){

        player2.yPosition += Player_vitesse_mouvement * elapsed;

       }

        if(player1.yPosition < Player_hauteur / 2){
            player1.yPosition = Player_hauteur - Player_hauteur/2;
        }
         if(player1.yPosition > hauteur - Player_hauteur / 2){
            player1.yPosition = hauteur - Player_hauteur / 2 ;
        }

         if(player2.yPosition < Player_hauteur / 2){
            player2.yPosition = Player_hauteur - Player_hauteur/2;
        }
         if(player2.yPosition > hauteur - Player_hauteur / 2){
            player2.yPosition = hauteur - Player_hauteur / 2 ;
        }

        // dessiner la balle
        SDL_Rect ballRect = {
            .x = ball.x - ball.size / 2,
            .y = ball.y - ball.size / 2,
            .w = ball.size,
            .h = ball.size,
        };

        // dessiner la raquette 1
        SDL_Rect player1rect = {
            .x = Player_marge,
            .y = (int)(player1.yPosition) - Player_hauteur / 2 ,
            .w = Player_largeur,
            .h = Player_hauteur,
        };

        if(SDL_HasIntersection(&ballRect,&player1rect)) {  //faire rebondire la balle
            ball.xspeed = fabs(ball.xspeed);               //  sur la raquette
        }


        // dessiner la raquette 2
        SDL_Rect player2rect = {
            .x =  largeur - Player_largeur - Player_marge,
            .y = (int)(player2.yPosition) - Player_hauteur / 2 ,
            .w = Player_largeur,
            .h = Player_hauteur,
        };

        if(SDL_HasIntersection(&ballRect,&player2rect)) {   //faire rebondire la balle
            ball.xspeed = -fabs(ball.xspeed);               //  sur la raquette
        }

    }

    // remplire les raquettes
    void renderPlayers(void){

    // render player 1
    SDL_SetRenderDrawColor(renderer,192, 71, 117,1); // gerer la couleur de la raquette
    SDL_Rect player1rect = { // dessiner la raquette
        .x = Player_marge,                                     // la position de la raquette par
        .y = (int)(player1.yPosition) - Player_hauteur / 2 ,   //     rapport a la fenetre
        .w = Player_largeur,   // largeur
        .h = Player_hauteur,   // hauteur
    };
    SDL_RenderFillRect(renderer, &player1rect);


      // render player 2
    SDL_SetRenderDrawColor(renderer,192, 71, 117 ,1);
    SDL_Rect player2rect = {
        .x =  largeur - Player_largeur - Player_marge,
        .y = (int)(player2.yPosition) - Player_hauteur / 2 ,
        .w = Player_largeur,
        .h = Player_hauteur,
    };
    SDL_RenderFillRect(renderer, &player2rect);

    }

 // metre a jour le score

    int updatescore(int player ,int points){
      served=false;

     if(player == 1){
        player1.score += points;
     }

    if(player == 2){
        player2.score += points;
     }


          char *fmt = " ordinateur : %d    |    joueur : %d ";
     int len = snprintf(NULL,0,fmt,player1.score,player2.score);
     char buf[len + 1];
    snprintf(buf , len+1,fmt,player1.score,player2.score);
    SDL_SetWindowTitle(Window, buf);

    }




 /* programme principale */

int main(int argc, char** argv)
{
    srand((unsigned int)time_t(NULL)); //initialiser le generateur de nombre al�atoires
    atexit(shutdown); //inscrire la fonction de nettoyage

    if(!nitialiser_Pong()){
        exit(1);
    }

bool quit = false;

SDL_Event event;// supprimer l'�venemment suivant de la file d'attente des �venements
   Uint32 lastTick = SDL_GetTicks();
while(!quit)
{
  while( SDL_PollEvent(&event)){ //traiter chaque �venement � tour de role
       if(event.type == SDL_QUIT)
   quit =true;

  }
                                    // obtenir le nombre de milisecondes d'epuis
    Uint32 curTick = SDL_GetTicks(); // linitialisation de la sdl
    Uint32 diff = curTick - lastTick;
    float elapsed = diff / 1000.0f;
    update(elapsed); // appel a la fonction update
    lastTick= curTick;

   /* d'eterminer le gagnant */
    if(player1.score == 7){
        quit = true;
        printf("lordinateur a ganger");
    }
    if(player2.score == 7){
        quit = true;
        printf("vous avez ganger ");
    }

}

 SDL_Quit();

 }








