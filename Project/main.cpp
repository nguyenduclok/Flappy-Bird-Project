#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "RenderWindow.hpp"
#include "Entity.hpp"
#include "Utils.hpp"
#include <ctime>
#include "Constants.hpp"

void set_rect(SDL_Rect & rect, double a, double b, double width, double height) {
    rect.x = int(a);
    rect.y = int(b);
    rect.w = int(width);
    rect.h = int(height);
}

bool collision(SDL_Rect rect1, SDL_Rect rect2) {
    double left1, left2;
    double right1, right2;
    double top1, top2;
    double bot1, bot2;

    left1 = (double)(rect1.x);
    right1 = (double)(rect1.x + rect1.w);
    top1 = (double)(rect1.y);
    bot1 = (double)(rect1.y + rect1.h);

    left2 = (double)(rect2.x);
    right2 = (double)(rect2.x + rect2.w);
    top2 = (double)(rect2.y);
    bot2 = (double)(rect2.y + rect2.h);

    if (bot1 <= top2) return false;
    if (top1 >= bot2) return false;
    if (right1 <= left2) return false;
    if (left1 >= right2) return false;
    return true;
}

int main(int argc, char ** argv) {
    std::ios::sync_with_stdio();
    std::cin.tie(0);
    srand(time(NULL));

    if (SDL_Init(SDL_INIT_VIDEO) > 0) {
        std::cout << "HEY... SDL_Init HAS FAILED. SDL_ERROR: " << SDL_GetError() << std::endl;
    }
    if (!(IMG_Init(IMG_INIT_PNG))) {
        std::cout << "IMG_Init has failed. Error: " << SDL_GetError() << std::endl;
    }

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    double accumulator = 0.0f;
    double currentTime = utils::hireTimeInSeconds();

    RenderWindow window("Flappy Bird", SCREEN_WIDTH, SCREEN_HEIGHT);

    SDL_Event event;

    SDL_Texture * backgroundTexture1 = window.loadTexture("images/background-day.png");
    SDL_Texture * backgroundTexture2 = window.loadTexture("images/background-night.png");
    SDL_Texture * baseTexture = window.loadTexture("images/base.png");
    SDL_Texture * baseReverseTexture = window.loadTexture("images/base-reverse.png");
    SDL_Texture * yellowBirdUpFlapTexture = window.loadTexture("images/yellowbird-upflap.png");
    SDL_Texture * yellowBirdMidFlapTexture = window.loadTexture("images/yellowbird-midflap.png");
    SDL_Texture * yellowBirdDownFlapTexture = window.loadTexture("images/yellowbird-downflap.png");
    SDL_Texture * redBirdUpFlapTexture = window.loadTexture("images/redbird-upflap.png");
    SDL_Texture * redBirdMidFlapTexture = window.loadTexture("images/redbird-midflap.png");
    SDL_Texture * redBirdDownFlapTexture = window.loadTexture("images/redbird-downflap.png");
    SDL_Texture * blueBirdUpFlapTexture = window.loadTexture("images/bluebird-upflap.png");
    SDL_Texture * blueBirdMidFlapTexture = window.loadTexture("images/bluebird-midflap.png");
    SDL_Texture * blueBirdDownFlapTexture = window.loadTexture("images/bluebird-downflap.png");
    SDL_Texture * pipeTexture = window.loadTexture("images/pipe.png");
    SDL_Texture * pipeReverseTexture = window.loadTexture("images/pipe-reverse.png");
    SDL_Texture * restartButtonTexture = window.loadTexture("images/restart.png");
    SDL_Texture * scoreBoardTexture = window.loadTexture("images/score.png");
    SDL_Texture * playButtonTexture = window.loadTexture("images/play.png");
    SDL_Texture * easyButtonTexture = window.loadTexture("images/easy.png");
    SDL_Texture * harderButtonTexture = window.loadTexture("images/harder.png");
    SDL_Texture * impossibleButtonTexture = window.loadTexture("images/impossible.png");
    SDL_Texture * startTexture = window.loadTexture("images/start.png");
    SDL_Texture * mainMenuTexture = window.loadTexture("images/main-menu.png");
    SDL_Texture * gameOverTexture = window.loadTexture("images/game-over.png");
    SDL_Texture * greyBirdUpFlapTexture = window.loadTexture("images/greybird-upflap.png");
    SDL_Texture * greyBirdMidFlapTexture = window.loadTexture("images/greybird-midflap.png");
    SDL_Texture * greyBirdDownFlapTexture = window.loadTexture("images/greybird-downflap.png");

    Mix_Chunk * hit = Mix_LoadWAV("audio/hit.wav");
    Mix_Chunk * point = Mix_LoadWAV("audio/point.wav");
    Mix_Chunk * wing = Mix_LoadWAV("audio/wing.wav");

    int bestScore;
    int bestEasyScore, bestHarderScore, bestImpossibleScore;
    std::ifstream inputFile("bestscore.txt");
    inputFile >> bestEasyScore >> bestHarderScore >> bestImpossibleScore;
    inputFile.close();
    double temp1;
    double temp2;
    double temp3;
    int score = INIT_SCORE;
    int birdFrame = 0;
    int greyBirdFrame = 0;
    bool pausing = false;
    bool startGame = false;
    bool running = true;
    double pipeVelocity = INIT_EASY_PIPE_VELOCITY;
    double baseVelocity = INIT_EASY_BASE_VELOCITY;
    double birdDropVelocity = 0;
    bool pipe1Pass = false;
    bool pipe2Pass = false;
    double base1X = INIT_BASE_X;
    double base2X = BASE_WIDTH;
    double pipe1X = INIT_EASY_PIPE_X;
    double pipe2X = pipe1X + DISTANCE_BETWEEN_TWO_PIPE + PIPE_WIDTH;
    double pipe1Y = (double)(rand() % (MAX - MIN + 1) + MIN);
    double pipe2Y = pipe1Y;
    double pipe1ReverseY = pipe1Y - DISTANCE - PIPE_HEIGHT;
    double pipe2ReverseY = pipe2Y - DISTANCE - PIPE_HEIGHT;
    double birdY = INIT_BIRD_Y;
    int kindOfBird = rand() % NUMBER_OF_BIRD;
    int kindOfBackGround = rand() % NUMBER_OF_BACKGROUND;
    bool waitingMode = true;
    bool chooseMode = false;
    bool mixCollisionPlayed = false;

    std::vector <Entity> background;
    Entity background1 = Entity(Vector2f(0, 0), backgroundTexture1);
    Entity background2 = Entity(Vector2f(0, 0), backgroundTexture2);
    background.push_back(background1);
    background.push_back(background2);

    std::vector <std::vector< SDL_Texture*>> birdTexture;
    std::vector <SDL_Texture*> yellowBirdTexture = {yellowBirdUpFlapTexture, yellowBirdMidFlapTexture, yellowBirdDownFlapTexture};
    birdTexture.push_back(yellowBirdTexture);
    std::vector <SDL_Texture*> redBirdTexture = {redBirdUpFlapTexture, redBirdMidFlapTexture, redBirdDownFlapTexture};
    birdTexture.push_back(redBirdTexture);
    std::vector <SDL_Texture*> blueBirdTexture = {blueBirdUpFlapTexture, blueBirdMidFlapTexture, blueBirdDownFlapTexture};
    birdTexture.push_back(blueBirdTexture);
    std::vector<SDL_Texture*> greyBirdTexture = {greyBirdUpFlapTexture, greyBirdMidFlapTexture, greyBirdDownFlapTexture};

    Entity restartButton = Entity(Vector2f(SCREEN_WIDTH / 2 - RESTART_WIDTH - DISTANCE_BETWEEN_TWO_BUTTON / 2, RESTART_Y), restartButtonTexture);
    Entity scoreBoard = Entity(Vector2f(SCREEN_WIDTH / 2 - SCORE_BOARD_WIDTH / 2, SCORE_BOARD_Y), scoreBoardTexture);
    Entity playButton = Entity(Vector2f(SCREEN_WIDTH / 2 - PLAY_WIDTH / 2, PLAY_Y), playButtonTexture);
    Entity easyButton = Entity(Vector2f(SCREEN_WIDTH / 2 - EASY_WIDTH / 2, LEVEL_Y - BUTTON_DISTANCE - BUTTON_HEIGHT), easyButtonTexture);
    Entity harderButton = Entity(Vector2f(SCREEN_WIDTH / 2 - HARDER_WIDTH / 2, LEVEL_Y), harderButtonTexture);
    Entity impossibleButton = Entity(Vector2f(SCREEN_WIDTH / 2 - IMPOSSIBLE_WIDTH / 2, LEVEL_Y + BUTTON_DISTANCE + BUTTON_HEIGHT), impossibleButtonTexture);
    Entity start = Entity(Vector2f(SCREEN_WIDTH / 2 - START_WIDTH / 2, START_Y), startTexture);
    Entity mainMenuButton = Entity(Vector2f(SCREEN_WIDTH / 2 + DISTANCE_BETWEEN_TWO_BUTTON / 2, MAIN_MENU_Y), mainMenuTexture);
    Entity gameOver = Entity(Vector2f(SCREEN_WIDTH / 2 - GAME_OVER_WIDTH / 2, GAME_OVER_Y), gameOverTexture);

    while (running) {
        int startTicks = SDL_GetTicks();
        double newTime = utils::hireTimeInSeconds();
        double frameTime = newTime - currentTime;
        currentTime = newTime;
        accumulator += frameTime;

        Entity bg = background[kindOfBackGround];
        Entity bird = Entity(Vector2f(BIRD_X, birdY), birdTexture[kindOfBird][birdFrame / BIRD_ANIMATION_FRAME]);
        Entity base1 = Entity(Vector2f(base1X, BASE_Y), baseTexture);
        Entity base2 = Entity(Vector2f(base2X, BASE_Y), baseTexture);
        Entity baseReverse1 = Entity(Vector2f(base1X, BASE_REVERSE_Y), baseReverseTexture);
        Entity baseReverse2 = Entity(Vector2f(base2X, BASE_REVERSE_Y), baseReverseTexture);
        Entity pipe1 = Entity(Vector2f(pipe1X, pipe1Y), pipeTexture);
        Entity pipe2 = Entity(Vector2f(pipe2X, pipe2Y), pipeTexture);
        Entity pipeReverse1 = Entity(Vector2f(pipe1X, pipe1ReverseY), pipeReverseTexture);
        Entity pipeReverse2 = Entity(Vector2f(pipe2X, pipe2ReverseY), pipeReverseTexture);

        SDL_Rect rectOfBird, rectOfPipe1, rectOfPipe2, rectOfPipe1Reverse, rectOfPipe2Reverse,
                            rectOfBase1, rectOfBase2, rectOfbaseReverse1, rectOfbaseReverse2;

        set_rect(rectOfBird, BIRD_X, birdY, BIRD_WIDTH, BIRD_HEIGHT);
        set_rect(rectOfPipe1, pipe1X, pipe1Y, PIPE_WIDTH, SCREEN_HEIGHT - BASE_HEIGHT - pipe1Y);
        set_rect(rectOfPipe2, pipe2X, pipe2Y, PIPE_WIDTH, SCREEN_HEIGHT - BASE_HEIGHT - pipe2Y);
        set_rect(rectOfPipe1Reverse, pipe1X, 0, PIPE_WIDTH, pipe1Y - DISTANCE);
        set_rect(rectOfPipe2Reverse, pipe2X, 0, PIPE_WIDTH, pipe2Y - DISTANCE);
        set_rect(rectOfBase1, base1X, BASE_Y, BASE_WIDTH, BASE_HEIGHT);
        set_rect(rectOfBase2, base2X, BASE_Y, BASE_WIDTH, BASE_HEIGHT);
        set_rect(rectOfbaseReverse1, base1X, BASE_REVERSE_Y, BASE_WIDTH, BASE_HEIGHT);
        set_rect(rectOfbaseReverse2, base2X, BASE_REVERSE_Y, BASE_WIDTH, BASE_HEIGHT);

        std::vector <SDL_Rect> rectOfObjects = {rectOfPipe1, rectOfPipe1Reverse, rectOfPipe2, rectOfPipe2Reverse, rectOfBase1, rectOfBase2, rectOfbaseReverse1, rectOfbaseReverse2};

        window.clear();

        if (!startGame && !waitingMode && !chooseMode) {
            pipe1X -= pipeVelocity;
            pipe2X -= pipeVelocity;
        }

        if (!pausing) {
            base1X -= baseVelocity;
            base2X -= baseVelocity;
        }

        if (pipe1X <= -PIPE_WIDTH) {
            pipe1X = SCREEN_WIDTH;
            pipe1Pass = false;
            pipe1Y = (double)(rand() % (MAX - MIN + 1) + MIN);
            pipe1ReverseY = pipe1Y - DISTANCE - PIPE_HEIGHT;
        }

        if (pipe2X <= -PIPE_WIDTH) {
            pipe2X = SCREEN_WIDTH;
            pipe2Pass = false;
            pipe2Y = (double)(rand() % (MAX - MIN + 1) + MIN);
            pipe2ReverseY = pipe2Y - DISTANCE - PIPE_HEIGHT;
        }

        if (base1X <= -(BASE_WIDTH)) {
            base1X = BASE_WIDTH;
        }

        if (base2X <= -(BASE_WIDTH)) {
            base2X = BASE_WIDTH;
        }

        if (pipe1X <= BIRD_X && pipe1Pass == false) {
            score++;
            Mix_PlayChannel(-1, point, 0);
            pipe1Pass = true;
        }

        if (pipe2X <= BIRD_X && pipe2Pass == false) {
            score++;
            Mix_PlayChannel(-1, point, 0);
            pipe2Pass = true;
        }

        if (pipeVelocity == INIT_EASY_PIPE_VELOCITY) {
            if (score > bestEasyScore) {
                bestEasyScore = score;
            }
        } else if (pipeVelocity == INIT_HARDER_PIPE_VELOCITY) {
            if (score > bestHarderScore) {
                bestHarderScore = score;
            }
        } else if (pipeVelocity == INIT_IMPOSSIBLE_PIPE_VELOCITY) {
            if (score > bestImpossibleScore) {
                bestImpossibleScore = score;
            }
        }

        std::string stringOfScore = std::to_string(score);
        int len = stringOfScore.length();
        std::string str1;
        SDL_Texture * texture1;
        SDL_Texture * texture2;
        std::vector <Entity> number;
        std::vector <Entity> numberInScoreBoard;

        if (len % 2) {
            for (int i = 0; i < len; i++) {
                str1 = "images/" + std::string(1, stringOfScore[i]) + ".png";
                texture1 = window.loadTexture(str1.c_str());
                number.push_back(Entity(Vector2f(SCREEN_WIDTH / 2 - (len - 2 * i) * NUMBER_WIDTH / 2 - (len / 2 - i) * NUMBER_GAP, NUMBER_Y), texture1));
                str1 = "images/" + std::string(1, stringOfScore[i]) + "-small.png";
                texture2 = window.loadTexture(str1.c_str());
                numberInScoreBoard.push_back(Entity(Vector2f(SCREEN_WIDTH / 2 - (len - 2 * i) * NUMBER_SMALL_WIDTH / 2 - (len / 2 - i) * NUMBER_GAP, NUMBER_IN_SCORE_BOARD_Y), texture2));
            }
        } else {
            for (int i = 0; i < len; i++) {
                str1 = "images/" + std::string(1, stringOfScore[i]) + ".png";
                texture1 = window.loadTexture(str1.c_str());
                number.push_back(Entity(Vector2f(SCREEN_WIDTH / 2 - (len / 2 - i) * NUMBER_WIDTH - (len - 2 * i - 1) * NUMBER_GAP / 2, NUMBER_Y), texture1));
                str1 = "images/" + std::string(1, stringOfScore[i]) + "-small.png";
                texture2 = window.loadTexture(str1.c_str());
                numberInScoreBoard.push_back(Entity(Vector2f(SCREEN_WIDTH / 2 - (len / 2 - i) * NUMBER_SMALL_WIDTH - (len - 2 * i - 1) * NUMBER_GAP / 2, NUMBER_IN_SCORE_BOARD_Y), texture2));
            }
        }

        if (pipeVelocity == INIT_EASY_PIPE_VELOCITY) bestScore = bestEasyScore;
        else if (pipeVelocity == INIT_HARDER_PIPE_VELOCITY) bestScore = bestHarderScore;
        else if (pipeVelocity == INIT_IMPOSSIBLE_PIPE_VELOCITY) bestScore = bestImpossibleScore;
        std::string stringOfBestScore = std::to_string(bestScore);
        int len_best = stringOfBestScore.length();
        std::string str2;
        SDL_Texture * texture;
        std::vector <Entity> bestScoreInScoreBoard;

        if (len_best % 2) {
            for (int i = 0; i < len_best; i++) {
                str2 = "images/" + std::string(1, stringOfBestScore[i]) + "-small.png";
                texture = window.loadTexture(str2.c_str());
                bestScoreInScoreBoard.push_back(Entity(Vector2f(SCREEN_WIDTH / 2 - (len_best - 2 * i) * NUMBER_SMALL_WIDTH / 2 - (len_best / 2 - i) * NUMBER_GAP, BEST_IN_SCORE_BOARD_Y), texture));
            }
        } else {
            for (int i = 0; i < len_best; i++) {
                str2 = "images/" + std::string(1, stringOfBestScore[i]) + "-small.png";
                texture = window.loadTexture(str2.c_str());
                bestScoreInScoreBoard.push_back(Entity(Vector2f(SCREEN_WIDTH / 2 - (len_best / 2 - i) * NUMBER_SMALL_WIDTH - (len_best - 2 * i - 1) * NUMBER_GAP / 2, BEST_IN_SCORE_BOARD_Y), texture));
            }
        }

        window.render(bg);
        window.render(pipe1);
        window.render(pipe2);
        window.render(pipeReverse1);
        window.render(pipeReverse2);
        window.render(base1);
        window.render(base2);
        window.render(baseReverse1);
        window.render(baseReverse2);
        if (!waitingMode && !chooseMode) window.render(bird);
        if (waitingMode) {
            window.render(playButton);
            window.render(start);
            Entity greyBird = Entity(Vector2f(GREY_BIRD_X, GREY_BIRD_Y), greyBirdTexture[greyBirdFrame / BIRD_ANIMATION_FRAME]);
            window.render(greyBird);
            greyBirdFrame++;
            if (greyBirdFrame / BIRD_ANIMATION_FRAME >= NUMBER_OF_BIRD) greyBirdFrame = 0;
        }
        if (chooseMode) {
            window.render(easyButton);
            window.render(harderButton);
            window.render(impossibleButton);
        }

        if (pausing && birdY < SCREEN_HEIGHT - BASE_HEIGHT - BIRD_HEIGHT) {
            birdY += BIRD_DIE_VELOCITY;
        }

        if (pausing && birdY >= SCREEN_HEIGHT - BASE_HEIGHT - BIRD_HEIGHT) {
            window.render(gameOver);
            window.render(scoreBoard);
            window.render(restartButton);
            window.render(mainMenuButton);
        }

        if (!pausing && !waitingMode && !chooseMode) {
            for (Entity entity: number) {
                window.render(entity);
            }
            birdFrame++;
        } else if (pausing){
            if (birdY >= SCREEN_HEIGHT - BASE_HEIGHT - BIRD_HEIGHT) {
                for (Entity entity: numberInScoreBoard) {
                    window.render(entity);
                }
                for (Entity entity : bestScoreInScoreBoard) {
                    window.render(entity);
                }
            }
        }

        if (birdFrame / BIRD_ANIMATION_FRAME > (NUMBER_OF_BIRD - 1)) {
            birdFrame = 0;
        }

        if (!startGame && !chooseMode && !waitingMode) {
            birdY += birdDropVelocity;
            birdDropVelocity += GRAVITY;
        }

        for (SDL_Rect rect: rectOfObjects) {
            if (collision(rectOfBird, rect)) {
                if (!mixCollisionPlayed) {
                    Mix_PlayChannel(-1, hit, 0);
                    mixCollisionPlayed = true;
                }
                pausing = true;
                pipeVelocity = 0;
                birdDropVelocity = 0;
            }
        }

        window.display();

        while (accumulator >= timeStep) {
            while (SDL_PollEvent( & event)) {
                if (event.type == SDL_QUIT) {
                    std::ofstream outputFile("bestscore.txt");
                    outputFile << bestEasyScore << " " << bestHarderScore << " " << bestImpossibleScore;
                    outputFile.close();
                    running = false;
                } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        if (pausing) {
                            if (event.button.x >= SCREEN_WIDTH / 2 - DISTANCE_BETWEEN_TWO_BUTTON / 2 - RESTART_WIDTH  &&
                                event.button.x <= SCREEN_WIDTH / 2 - DISTANCE_BETWEEN_TWO_BUTTON / 2 &&
                                event.button.y >= RESTART_Y &&
                                event.button.y <= RESTART_Y + BUTTON_HEIGHT) {
                                    birdY = INIT_BIRD_Y;
                                    pipeVelocity = temp1;
                                    baseVelocity = temp2;
                                    pipe1X = temp3;
                                    pipe2X = pipe1X + DISTANCE_BETWEEN_TWO_PIPE + PIPE_WIDTH;
                                    pipe1Y = (double)(rand() % (MAX - MIN + 1) + MIN);
                                    pipe2Y = pipe1Y;
                                    score = INIT_SCORE;
                                    base1X = INIT_BASE_X;
                                    base2X = BASE_WIDTH;
                                    pipe1ReverseY = pipe1Y - DISTANCE - PIPE_HEIGHT;
                                    pipe2ReverseY = pipe2Y - DISTANCE - PIPE_HEIGHT;
                                    pipe1Pass = false;
                                    pipe2Pass = false;
                                    birdDropVelocity = 0;
                                    birdFrame = 0;
                                    kindOfBackGround = rand() % NUMBER_OF_BACKGROUND;
                                    kindOfBird = rand() % NUMBER_OF_BIRD;
                                    pausing = false;
                                    startGame = true;
                                    mixCollisionPlayed = false;
                                    continue;
                            } else if (event.button.x >= SCREEN_WIDTH / 2 + DISTANCE_BETWEEN_TWO_BUTTON / 2 &&
                                        event.button.x <= SCREEN_WIDTH / 2 + DISTANCE_BETWEEN_TWO_BUTTON / 2 + MAIN_MENU_WIDTH &&
                                        event.button.y >= MAIN_MENU_Y &&
                                        event.button.y <= MAIN_MENU_Y + BUTTON_HEIGHT) {
                                            birdY = INIT_BIRD_Y;
                                            pipeVelocity = INIT_EASY_PIPE_VELOCITY;
                                            baseVelocity = INIT_EASY_BASE_VELOCITY;
                                            pipe1X = INIT_EASY_PIPE_X;
                                            pipe2X = pipe1X + DISTANCE_BETWEEN_TWO_PIPE + PIPE_WIDTH;
                                            pipe1Y = (double)(rand() % (MAX - MIN + 1) + MIN);
                                            pipe2Y = pipe1Y;
                                            score = INIT_SCORE;
                                            base1X = INIT_BASE_X;
                                            base2X = BASE_WIDTH;
                                            pipe1ReverseY = pipe1Y - DISTANCE - PIPE_HEIGHT;
                                            pipe2ReverseY = pipe2Y - DISTANCE - PIPE_HEIGHT;
                                            pipe1Pass = false;
                                            pipe2Pass = false;
                                            birdDropVelocity = 0;
                                            birdFrame = 0;
                                            kindOfBackGround = rand() % NUMBER_OF_BACKGROUND;
                                            kindOfBird = rand() % NUMBER_OF_BIRD;
                                            pausing = false;
                                            waitingMode = true;
                                            mixCollisionPlayed = false;
                                            continue;
                                        }
                        } else if (waitingMode) {
                            if (event.button.x >= SCREEN_WIDTH / 2 - PLAY_WIDTH / 2 &&
                                event.button.x <= SCREEN_WIDTH / 2 + PLAY_WIDTH / 2 &&
                                event.button.y >= PLAY_Y &&
                                event.button.y <= PLAY_Y + BUTTON_HEIGHT) {
                                waitingMode = false;
                                chooseMode = true;
                                }
                        } else if (chooseMode) {
                            if (event.button.x >= SCREEN_WIDTH / 2 - EASY_WIDTH / 2 &&
                                event.button.x <= SCREEN_WIDTH / 2 + EASY_WIDTH / 2 &&
                                event.button.y >= LEVEL_Y - BUTTON_DISTANCE - BUTTON_HEIGHT &&
                                event.button.y <= LEVEL_Y - BUTTON_DISTANCE) {
                                temp1 = INIT_EASY_PIPE_VELOCITY;
                                temp2 = INIT_EASY_BASE_VELOCITY;
                                pipeVelocity = temp1;
                                baseVelocity = temp2;
                                temp3 = INIT_EASY_PIPE_X;
                                pipe1X = temp3;
                                base1X = INIT_BASE_X;
                                base2X = BASE_WIDTH;
                                chooseMode = false;
                                startGame = true;
                            } else if (event.button.x >= SCREEN_WIDTH / 2 - HARDER_WIDTH / 2 &&
                                        event.button.x <= SCREEN_WIDTH / 2 + HARDER_WIDTH / 2 &&
                                        event.button.y >= LEVEL_Y &&
                                        event.button.y <= LEVEL_Y + BUTTON_HEIGHT) {
                                            temp1 = INIT_HARDER_PIPE_VELOCITY;
                                            temp2 = INIT_HARDER_BASE_VELOCITY;
                                            pipeVelocity = temp1;
                                            baseVelocity = temp2;
                                            temp3 = INIT_HARDER_PIPE_X;
                                            pipe1X = temp3;
                                            pipe2X = pipe1X + DISTANCE_BETWEEN_TWO_PIPE + PIPE_WIDTH;
                                            base1X = INIT_BASE_X;
                                            base2X = BASE_WIDTH;
                                            chooseMode = false;
                                            startGame = true;
                            } else if (event.button.x >= SCREEN_WIDTH / 2 - IMPOSSIBLE_WIDTH / 2 &&
                                        event.button.x <= SCREEN_WIDTH / 2 + IMPOSSIBLE_WIDTH / 2 &&
                                        event.button.y >= LEVEL_Y + BUTTON_DISTANCE + BUTTON_HEIGHT &&
                                        event.button.y <= LEVEL_Y + BUTTON_DISTANCE + 2*BUTTON_HEIGHT) {
                                            temp1 = INIT_IMPOSSIBLE_PIPE_VELOCITY;
                                            temp2 = INIT_IMPOSSIBLE_BASE_VELOCITY;
                                            pipeVelocity = temp1;
                                            baseVelocity = temp2;
                                            temp3 = INIT_IMPOSSIBLE_PIPE_X;
                                            pipe1X = temp3;
                                            pipe2X = pipe1X + DISTANCE_BETWEEN_TWO_PIPE + PIPE_WIDTH;;
                                            base1X = INIT_BASE_X;
                                            base2X = BASE_WIDTH;
                                            chooseMode = false;
                                            startGame = true;
                            }
                        }
                    }
                } else if (event.type == SDL_KEYDOWN) {
                    if (event.key.keysym.sym == SDLK_SPACE) {
                        if (!pausing && !startGame && !waitingMode && !chooseMode) {
                            birdDropVelocity = 0;
                            birdDropVelocity -= DROP_VALUE;
                            Mix_PlayChannel(-1, wing, 0);
                        } else if (startGame) {
                            birdDropVelocity = 0;
                            birdDropVelocity -= DROP_VALUE;
                            Mix_PlayChannel(-1, wing, 0);
                            startGame = false;
                        }
                    }
                }
            }
            accumulator -= timeStep;
        }

        int frameTicks = SDL_GetTicks() - startTicks;
        if (frameTicks < 1000 / window.getRefreshRate()) {
            SDL_Delay(1000 / window.getRefreshRate() - frameTicks);
        }
    }

    window.cleanUp();
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
    return 0;
}
