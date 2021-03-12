// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Game.h"
#include <stdio.h>
#include <iostream>
#include <stdio.h>

const int thickness = 15;  // sera usado para setar a altura de alguns objetos
const float paddleH = 80.0f;  // tamanho da raquete


void Game::Text(SDL_Renderer* renderer) {

    TTF_Init();
    // load font.ttf at size 16 into font
    font = TTF_OpenFont("Vogue.ttf", 16);
    if (!font) {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        // handle error
    }
    printf("TTF_1\n");
    /////////////


// Render some UTF8 text in solid black to a new surface
// then blit to the upper left of the screen
// then free the text surface
    SDL_Color color = { 0,0,0 };
    SDL_Surface* text_surface;
    printf("TTF_2\n");
    text_surface = TTF_RenderUTF8_Solid(font, "Hello World!", color);
    text = SDL_CreateTextureFromSurface(mRenderer, text_surface);
    printf("TTF_3\n");
    textRect.x = 100;
    textRect.y = 100;
    SDL_QueryTexture(text, NULL, NULL, &textRect.w, &textRect.h);
    printf("TTF_4\n");
    SDL_FreeSurface(text_surface);
    // free the font
    TTF_CloseFont(font);
    printf("TTF_5\n");
    font = NULL; // to be safe...



    TTF_Quit();
}


Game::Game()
    : mWindow(nullptr)  // para criar uma janela
    ,
    mRenderer(nullptr)  // para fins de renderização na tela
    ,
    mTicksCount(0)  // para guardar o tempo decorrido no jogo
    ,
    mIsRunning(
        true)  // verificar se o jogo ainda deve continuar sendo executado
    ,
    mPaddleDir(0)  // direcao da bolinha
{}

bool Game::Initialize() {
    // Initialize SDL
    int sdlResult = SDL_Init(SDL_INIT_VIDEO);
    if (sdlResult != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }

    // Create an SDL Window
    mWindow = SDL_CreateWindow("Hello Pong!",  // Window title
        100,            // Top left x-coordinate of window
        100,            // Top left y-coordinate of window
        1024,           // Width of window
        768,            // Height of window
        0               // Flags (0 for no flags set)
    );

    if (!mWindow) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }

    //// Create SDL renderer
    mRenderer =
        SDL_CreateRenderer(mWindow,  // Window to create renderer for
            -1,       // Usually -1
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (!mRenderer) {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return false;
    }

    mPaddlePos.x = 15.0f;  // posição inicial da raquete eixo x
    mPaddlePos.y = 768.0f / 2.0f;  // posição inicial da raquee eixo y
    mBallPos.x = 1024.0f / 2.0f;  // posição da bola eixo x
    mBallPos.y = 768.0f / 2.0f;  // posição da bola eixo y
    mBallVel.x = -200.0f;  // velocidade de movimentação da bola no eixo x
    mBallVel.y = 200.0f;  // velocidade de movimentação da bola no eixo y

    // segunda bolinha
    mBallPos2.x = 1024.0f / 2.0f;  // posição da bola eixo x
    mBallPos2.y = 768.0f / 2.0f;  // posição da bola eixo y
    mBallVel2.x = -100.0f;  // velocidade de movimentação da bola no eixo x
    mBallVel2.y = 300.0f;  // velocidade de movimentação da bola no eixo y


    
    return true;
}

void Game::RunLoop() {
    while (mIsRunning) {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

void Game::ProcessInput() {
    SDL_Event event;  // evento, inputs do jogador são armazenados aqui
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            // If we get an SDL_QUIT event, end loop
        case SDL_QUIT:
            mIsRunning = false;
            break;
        }
    }

    // Get state of keyboard - podemos buscar por alguma tecla específica
    // pressionada no teclado, nesse caso, Escape
    const Uint8* state = SDL_GetKeyboardState(NULL);
    // If escape is pressed, also end loop
    if (state[SDL_SCANCODE_ESCAPE]) {
        mIsRunning = false;
    }

    // Update paddle direction based on W/S keys - atualize a direção da raquete
    // com base na entrada do jogador
    // W -> move a raquete para cima, S -> move a raquete para baixo
    mPaddleDir = 0;
    if (state[SDL_SCANCODE_W]) {
        mPaddleDir -= 1;
    }
    if (state[SDL_SCANCODE_S]) {
        mPaddleDir += 1;
    }
}

void Game::UpdateGame() {
    // Espere que 16ms tenham passado desde o último frame - limitando os frames
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));




    // Delta time é a diferença em ticks do último frame
    // (convertido pra segundos) - calcula o delta time para atualização do jogo
    float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;


    
    // "Clamp" (lima/limita) valor máximo de delta time
    if (deltaTime > 0.05f) {
        deltaTime = 0.05f;
    }

    // atualize a contagem de ticks par ao próximo frame
    mTicksCount = SDL_GetTicks();

    // atualiza a posição da raquete
    if (mPaddleDir != 0) {
        mPaddlePos.y += mPaddleDir * 300.0f * deltaTime;
        // verifique que a raquete não se move para fora da tela - usamos
        // "thickness", que definimos como a altura dos elementos
        if (mPaddlePos.y < (paddleH / 2.0f + thickness)) {
            mPaddlePos.y = paddleH / 2.0f + thickness;
        }
        else if (mPaddlePos.y > (768.0f - paddleH / 2.0f - thickness)) {
            mPaddlePos.y = 768.0f - paddleH / 2.0f - thickness;
        }
    }

    // atualiza a posição da bola com base na sua velocidade
    mBallPos.x += mBallVel.x * deltaTime;
    mBallPos.y += mBallVel.y * deltaTime;


    //atualiza texto

    SDL_RenderCopy(mRenderer, text, NULL, &textRect);


    // atualiza a posição da bola se ela colidiu com a raquete
    float diff = mPaddlePos.y - mBallPos.y;
    // pegue o valor absoluto de diferença entre o eixo y da bolinha e da raquete
    // isso é necessário para os casos em que no próximo frame a bolinha ainda não
    // esteja tão distante da raquete
    // verifica se a bola está na area da raquete e na mesma posição no eixo x
    diff = (diff > 0.0f) ? diff : -diff;
    if (
        // A diferença no eixo y y-difference is small enough
        diff <= paddleH / 2.0f &&
        // Estamos na posicao x correta
        mBallPos.x <= 25.0f && mBallPos.x >= 20.0f &&
        // A bolinha está se movendo para a esquerda
        mBallVel.x < 0.0f) {
        mBallVel.x *= -1.0f;
    }

    // Verifica se a bola saiu da tela (no lado esquerdo, onde é permitido)
    // Se sim, encerra o jogo
    else if (mBallPos.x <= 0.0f) {
        //mIsRunning = false;
        Text(mRenderer);
        mBallPos.x = (float)1024 / 2;
        mBallPos.y = (float)768 / 2;
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING,
            "Gol.", //titulo
            "Clique em 'ok' para continuar", //mensagem
            NULL);
    }

    // Atualize (negative) a velocidade da bola se ela colidir com a parede do
    // lado direito
    //
    else if (mBallPos.x >= (1024.0f - thickness) && mBallVel.x > 0.0f) {
        mBallVel.x *= -1.0f;
    }

    // Atualize (negative) a posição da bola se ela colidir com a parede de cima
    //
    if (mBallPos.y <= thickness && mBallVel.y < 0.0f) {
        mBallVel.y *= -1;
    }

    // Atualize (negative) a posição da bola se ela colidiu com a parede de baixo
    // Did the ball collide with the bottom wall?
    else if (mBallPos.y >= (768 - thickness) && mBallVel.y > 0.0f) {
        mBallVel.y *= -1;
    }

    // Ball 2
    // Update ball position based on ball velocity - atualiza a posição da bola
    // com base na sua velocidade
    mBallPos2.x += mBallVel2.x * deltaTime;
    mBallPos2.y += mBallVel2.y * deltaTime;

    // Atuliza a posição da bola se ela colidiu com a raquete
    // Bounce if needed
    // Did we intersect with the paddle?
    float diff2 = mPaddlePos.y - mBallPos2.y;
    // Take absolute value of difference
    // verifica se a bola está na area da raquete e na mesma posição no eixo x
    diff2 = (diff2 > 0.0f) ? diff2 : -diff2;
    if (
        // Our y-difference is small enough
        diff2 <= paddleH / 2.0f &&
        // We are in the correct x-position
        mBallPos2.x <= 25.0f && mBallPos2.x >= 20.0f &&
        // The ball is moving to the left
        mBallVel2.x < 0.0f) {  // if
        mBallVel2.x *= -1.0f;
    }

    // Verifica se a bola saiu da tela (no lado esquerdo, onde é permitido)
    // Se sim, encerra o jogo
    // Did the ball go off the screen? (if so, end game)
    else if (mBallPos2.x <= 0.0f) {
        //mIsRunning = false;
        mBallPos.x = (float)1024 / 2;
        mBallPos.y = (float)768 / 2;
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING,
            "Gol.", //titulo
            "Clique em 'ok' para continuar", //mensagem
            NULL);
    }

    // Atulize a velocidade da bola se ela colidir com a parede do lado direito
    // Did the ball collide with the right wall?
    else if (mBallPos2.x >= (1024.0f - thickness) && mBallVel2.x > 0.0f) {
        mBallVel2.x *= -1.0f;
    }

    // Atualize a posição da bola se ela colidir com a parede de cima
    // Did the ball collide with the top wall?
    if (mBallPos2.y <= thickness && mBallVel2.y < 0.0f) {
        mBallVel2.y *= -1;
    }

    // Atualize a posição da bola se ela colidiu com a parede de baixo
    // Did the ball collide with the bottom wall?
    else if (mBallPos2.y >= (768 - thickness) && mBallVel2.y > 0.0f) {
        mBallVel2.y *= -1;
    }
}

// Desenhando a tela do jogo
void Game::GenerateOutput() {
    // Setamos a cor de fundo par azul
    SDL_SetRenderDrawColor(mRenderer,
        32, 101, 99, 255   // A
    );

    // limpa o back buffer
    SDL_RenderClear(mRenderer);

    // Desenha as paredes - mudamos a cor de mRenderer para o desenho dos
    // retangulos que formaram as paredes
    SDL_SetRenderDrawColor(mRenderer, 150, 221, 200, 255);

    // Desenha a parede de cima - desenhando um retangulo no topo da tela,
    // coordenada x e y = 0, 0 representa o topo esquerdo
    // primeiro definimos as coordenadas e tamanhos do triangulo
    SDL_Rect wall{
        0,         // Top left x
        0,         // Top left y
        1024,      // Width
        thickness  // Height
    };
    SDL_RenderFillRect(mRenderer,
        &wall);  // finalmente, desenhamos um retangulo no topo

// desenhamos as outras paredes apenas mudando as coordenadas de wall

// mudando a cor da parede de baixo
    SDL_SetRenderDrawColor(mRenderer, 150, 221, 200, 255);

    // parede de baixo
    wall.y = 768 - thickness;
    SDL_RenderFillRect(mRenderer, &wall);

    // mudando a cor da parede da direita
    SDL_SetRenderDrawColor(mRenderer, 150, 221, 200, 255);

    // parede da direita
    wall.x = 1024 - thickness;
    wall.y = 0;
    wall.w = thickness;
    wall.h = 1024;
    SDL_RenderFillRect(mRenderer, &wall);


    // mudando a cor da parede da esquerda
    SDL_SetRenderDrawColor(mRenderer, 150, 221, 200, 255);

    // parede de esquerda
    wall.x = 0;
    wall.y = 0;
    wall.w = thickness;
    wall.h = 1024;
    SDL_RenderFillRect(mRenderer, &wall);



    // como as posições da raquete e da bola serão atualizadas a cada iteração do
    // game loop, criamos "membros" na classe
    // Game.h para tal

    // mudar a cor da raquete
    SDL_SetRenderDrawColor(mRenderer, 251, 196, 138, 255);

    // desenhando a raquete - usando mPaddlePos que é uma struct

    SDL_Rect paddle{ static_cast<int>(mPaddlePos.x),  // static_cast converte de
                                                     // float para inteiros, pois
                                                     // SDL_Rect trabalha com
                                                     // inteiros
                    static_cast<int>(mPaddlePos.y - paddleH / 2), thickness,
                    static_cast<int>(paddleH) };
    SDL_RenderFillRect(mRenderer, &paddle);

    // desenhando a bola - usando mBallPos que é uma struct de coordenadas
    // definida como membro em Game.h

    // mudar a cor do renderizador para a bola
    SDL_SetRenderDrawColor(mRenderer, 251, 233, 181, 255);
    // Draw ball
    SDL_Rect ball{ static_cast<int>(mBallPos.x + thickness / 2),
                  static_cast<int>(mBallPos.y + thickness / 2), thickness,
                  thickness };
    SDL_RenderFillRect(mRenderer, &ball);

    // Seta a cor para a segunda bolinha
    SDL_SetRenderDrawColor(mRenderer, 251, 196, 138, 255);

    // Crio o objeto SDL_Rect da segunda bolinha
    SDL_Rect ball2{ static_cast<int>(mBallPos2.x - thickness / 2),
                   static_cast<int>(mBallPos2.y - thickness / 2), thickness,
                   thickness };

    // por fim desenho a bolinha 2
    SDL_RenderFillRect(mRenderer, &ball2);
    

    // Swap front buffer and back buffer
    SDL_RenderPresent(mRenderer);
}

// Para encerrar o jogo
void Game::Shutdown() {
    SDL_DestroyRenderer(mRenderer);  // encerra o renderizador
    SDL_DestroyWindow(mWindow);  // encerra a janela aberta
    SDL_Quit();  // encerra o jogo
}
