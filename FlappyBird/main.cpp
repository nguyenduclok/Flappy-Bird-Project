#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>
#include "RenderWindow.hpp"
#include "Entity.hpp"
#include "Utils.hpp"
#include <ctime>
const double SCREEN_WIDTH = 600;
const double SCREEN_HEIGHT = 800;
const double DISTANCE_BETWEEN_TWO_PIPE = 246;
const double PIPE_WIDTH = 108;
const double PIPE_HEIGHT = 400;
const double BASE_HEIGHT = 100;
const double BASE_Y = SCREEN_HEIGHT - BASE_HEIGHT;
const double BASE_REVERSE_Y = -BASE_HEIGHT;
const double DISTANCE = 180;
const int MIN = 300;
const int MAX = 580;
const double BIRD_VELOCITY = 1;
const double NUMBER_WIDTH = 36;
const double NUMBER_HEIGHT = 56;
const double NUMBER_GAP = 1;
const double NUMBER_Y = 100;
const double GRAVITY = 0.1;
const int NUMBER_OF_BACKGROUND = 2;
const int BIRD_ANIMATION_FRAME = 20;
const double BIRD_WIDTH = 68;
const double BIRD_HEIGHT = 48;
const double RESTART_WIDTH = 168;
const double RESTART_HEIGHT = 60;
const double RESTART_Y = 435;
const int NUMBER_OF_BIRD = 3;
const double SCORE_BOARD_WIDTH = 136;
const double SCORE_BOARD_Y = 235;
const double NUMBER_IN_SCORE_BOARD_Y = 85 + SCORE_BOARD_Y;
const double INIT_PIPE_VELOCITY = 1.7;
const double INIT_BASE_VELOCITY = 2;
const double INIT_BIRD_Y = SCREEN_HEIGHT/3;
const double INIT_PIPE1_X = 1000;
const double BIRD_X = SCREEN_WIDTH/3;
const double INIT_BASE1_X = 0;
const int INIT_SCORE = 0;

void set_rect(SDL_Rect &rect, double a, double b, double width, double height) {
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

    left1 = (double) (rect1.x);
    right1 = (double) (rect1.x + rect1.w);
    top1 = (double) (rect1.y);
    bot1 = (double) (rect1.y + rect1.h);

    left2 = (double) (rect2.x);
    right2 = (double) (rect2.x + rect2.w);
    top2 = (double) (rect2.y);
    bot2 = (double) (rect2.y + rect2.h);

    if (bot1 <= top2) {
        return false;
    }

    if (top1 >= bot2) {
        return false;
    }

    if (right1 <= left2) {
        return false;
    }

    if (left1 >= right2) {
        return false;
    }

    return true;
}

int main(int argc, char **argv) {
    if (SDL_Init(SDL_INIT_VIDEO) > 0) {
        std::cout << "HEY.. SDL_Init HAS FAILED. SDL_ERROR: " << SDL_GetError() << std::endl;
    }

    if (!(IMG_Init(IMG_INIT_PNG))) {
        std::cout << "IMG_Init has failed. Error: " << SDL_GetError() << std::endl;
    }

    RenderWindow window("Flappy Bird", SCREEN_WIDTH, SCREEN_HEIGHT);

    SDL_Event event;

    srand(time(NULL));

    SDL_Texture* bg1_texture = window.loadTexture("images/background-day.png");
    SDL_Texture* bg2_texture = window.loadTexture("images/background-night.png");
    SDL_Texture* base_texture = window.loadTexture("images/base.png");
    SDL_Texture* base_reverse_texture = window.loadTexture("images/base-reverse.png");
    SDL_Texture* yellow_bird_up_flap_texture = window.loadTexture("images/yellowbird-upflap.png");
    SDL_Texture* yellow_bird_mid_flap_texture = window.loadTexture("images/yellowbird-midflap.png");
    SDL_Texture* yellow_bird_down_flap_texture = window.loadTexture("images/yellowbird-downflap.png");
    SDL_Texture* red_bird_up_flap_texture = window.loadTexture("images/redbird-upflap.png");
    SDL_Texture* red_bird_mid_flap_texture = window.loadTexture("images/redbird-midflap.png");
    SDL_Texture* red_bird_down_flap_texture = window.loadTexture("images/redbird-downflap.png");
    SDL_Texture* blue_bird_up_flap_texture = window.loadTexture("images/bluebird-upflap.png");
    SDL_Texture* blue_bird_mid_flap_texture = window.loadTexture("images/bluebird-midflap.png");
    SDL_Texture* blue_bird_down_flap_texture = window.loadTexture("images/bluebird-downflap.png");
    SDL_Texture* pipe_texture = window.loadTexture("images/pipe.png");
    SDL_Texture* pipe_reverse_texture = window.loadTexture("images/pipe-reverse.png");
    SDL_Texture* restart_texture = window.loadTexture("images/restart.png");
    SDL_Texture* score_board_texture = window.loadTexture("images/score.png");

    int score = INIT_SCORE;
    int bird_frame = 0;
    bool pausing = false;
    // bool start_game = true;
    bool running = true;
    double pipe_velocity = INIT_PIPE_VELOCITY;
    double base_velocity = INIT_BASE_VELOCITY;
    double bird_drop_velocity = 0;
    bool pipe1_pass = false;
    bool pipe2_pass = false;
    double base1_x = INIT_BASE1_X;
    double base2_x = SCREEN_WIDTH;
    double pipe1_x = INIT_PIPE1_X;
    double pipe2_x = pipe1_x + DISTANCE_BETWEEN_TWO_PIPE + PIPE_WIDTH;
    double pipe1_y = (double) (rand() % (MAX - MIN + 1) + MIN);
    double pipe2_y = pipe1_y;
    double pipe1_reverse_y = pipe1_y - DISTANCE - PIPE_HEIGHT;
    double pipe2_reverse_y = pipe2_y - DISTANCE - PIPE_HEIGHT;
    double bird_y = INIT_BIRD_Y;
    int kind_of_bird = rand() % NUMBER_OF_BIRD;
    int random_bg = rand() % NUMBER_OF_BACKGROUND;

    std::vector<Entity> background;
    Entity background1 = Entity(Vector2f(0, 0), bg1_texture);
    Entity background2 = Entity(Vector2f(0, 0), bg2_texture);
    background.push_back(background1);
    background.push_back(background2);

    std::vector<std::vector<SDL_Texture*>> bird_texture;

    std::vector<SDL_Texture*> yellow_bird_texture;
    yellow_bird_texture.push_back(yellow_bird_up_flap_texture);
    yellow_bird_texture.push_back(yellow_bird_mid_flap_texture);
    yellow_bird_texture.push_back(yellow_bird_down_flap_texture);
    bird_texture.push_back(yellow_bird_texture);

    std::vector<SDL_Texture*> red_bird_texture;
    red_bird_texture.push_back(red_bird_up_flap_texture);
    red_bird_texture.push_back(red_bird_mid_flap_texture);
    red_bird_texture.push_back(red_bird_down_flap_texture);
    bird_texture.push_back(red_bird_texture);

    std::vector<SDL_Texture*> blue_bird_texture;
    blue_bird_texture.push_back(blue_bird_up_flap_texture);
    blue_bird_texture.push_back(blue_bird_mid_flap_texture);
    blue_bird_texture.push_back(blue_bird_down_flap_texture);
    bird_texture.push_back(blue_bird_texture);

    const double timeStep = 0.01f;
    double accumulator = 0.0f;
    double currentTime = utils::hireTimeInSeconds();

    Entity restart = Entity(Vector2f(SCREEN_WIDTH/2-RESTART_WIDTH/2, RESTART_Y), restart_texture);
    Entity score_board = Entity(Vector2f(SCREEN_WIDTH/2-SCORE_BOARD_WIDTH/2, SCORE_BOARD_Y), score_board_texture);
    Entity bg = background[random_bg];
    while(running) {
        int startTicks = SDL_GetTicks();
        double newTime = utils::hireTimeInSeconds();
        double frameTime = newTime - currentTime;
        currentTime = newTime;
        accumulator += frameTime;

        Entity bird = Entity(Vector2f(BIRD_X, bird_y), bird_texture[kind_of_bird][bird_frame/(BIRD_ANIMATION_FRAME-5)]);
        Entity base1 = Entity(Vector2f(base1_x, BASE_Y), base_texture);
        Entity base2 = Entity(Vector2f(base2_x, BASE_Y), base_texture);
        Entity base1_reverse = Entity(Vector2f(base1_x, BASE_REVERSE_Y), base_reverse_texture);
        Entity base2_reverse = Entity(Vector2f(base2_x, BASE_REVERSE_Y), base_reverse_texture);
        Entity pipe1 = Entity(Vector2f(pipe1_x, pipe1_y), pipe_texture);
        Entity pipe2 = Entity(Vector2f(pipe2_x, pipe2_y), pipe_texture);
        Entity pipe1_reverse = Entity(Vector2f(pipe1_x, pipe1_reverse_y), pipe_reverse_texture);
        Entity pipe2_reverse = Entity(Vector2f(pipe2_x, pipe2_reverse_y), pipe_reverse_texture);

        SDL_Rect bird_rect, pipe1_rect, pipe2_rect, pipe1_reverse_rect, pipe2_reverse_rect,
                            base1_rect, base2_rect, base1_reverse_rect, base2_reverse_rect;

        set_rect(bird_rect, BIRD_X, bird_y, BIRD_WIDTH, BIRD_HEIGHT);
        set_rect(pipe1_rect, pipe1_x, pipe1_y, PIPE_WIDTH, SCREEN_HEIGHT - BASE_HEIGHT - pipe1_y);
        set_rect(pipe2_rect, pipe2_x, pipe2_y, PIPE_WIDTH, SCREEN_HEIGHT - BASE_HEIGHT - pipe2_y);
        set_rect(pipe1_reverse_rect, pipe1_x, 0, PIPE_WIDTH, pipe1_y-DISTANCE);
        set_rect(pipe2_reverse_rect, pipe2_x, 0, PIPE_WIDTH, pipe2_y-DISTANCE);
        set_rect(base1_rect, base1_x, BASE_Y, SCREEN_WIDTH, BASE_HEIGHT);
        set_rect(base2_rect, base2_x, BASE_Y, SCREEN_WIDTH, BASE_HEIGHT);
        set_rect(base1_reverse_rect, base1_x, BASE_REVERSE_Y, SCREEN_WIDTH, BASE_HEIGHT);
        set_rect(base2_reverse_rect, base2_x, BASE_REVERSE_Y, SCREEN_WIDTH, BASE_HEIGHT);

        std::vector<SDL_Rect> all_of_rect;

        all_of_rect.push_back(pipe1_rect);
        all_of_rect.push_back(pipe1_reverse_rect);
        all_of_rect.push_back(pipe2_rect);
        all_of_rect.push_back(pipe2_reverse_rect);
        all_of_rect.push_back(base1_rect);
        all_of_rect.push_back(base2_rect);
        all_of_rect.push_back(base1_reverse_rect);
        all_of_rect.push_back(base2_reverse_rect);

        window.clear();

        pipe1_x -= pipe_velocity;
        pipe2_x -= pipe_velocity;
        base1_x -= base_velocity;
        base2_x -= base_velocity;

        if (pipe1_x < -PIPE_WIDTH) {
            pipe1_x = SCREEN_WIDTH;
            pipe1_pass = false;
            pipe1_y = (double) (rand() % (MAX - MIN + 1) + MIN);
            pipe1_reverse_y = pipe1_y - DISTANCE - PIPE_HEIGHT;
        }

        if (pipe2_x < -PIPE_WIDTH) {
            pipe2_x = SCREEN_WIDTH;
            pipe2_pass = false;
            pipe2_y = (double) (rand() % (MAX - MIN + 1) + MIN);
            pipe2_reverse_y = pipe2_y - DISTANCE - PIPE_HEIGHT;
        }

        if (base1_x <= -SCREEN_WIDTH) {
            base1_x = SCREEN_WIDTH;
        }

        if (base2_x <= -SCREEN_WIDTH) {
            base2_x = SCREEN_WIDTH;
        }

        if (pipe1_x <= BIRD_X && pipe1_pass == false) {
            score++;
            pipe1_pass = true;
        }

        if (pipe2_x <= BIRD_X && pipe2_pass == false) {
            score++;
            pipe2_pass = true;
        }

        std::string score_in_string = std::to_string(score);
        int len = score_in_string.length();
        std::string str;
        SDL_Texture* tex;
        std::vector<Entity> number;
        std::vector<Entity> number_in_score_board;

        if (len == 1) {
            str = "images/" + std::string(1,score_in_string[0]) + ".png";
            tex = window.loadTexture(str.c_str());
            number.push_back(Entity(Vector2f(SCREEN_WIDTH/2-NUMBER_WIDTH/2,NUMBER_Y), tex));
            number_in_score_board.push_back(Entity(Vector2f(SCREEN_WIDTH/2-NUMBER_WIDTH/2,NUMBER_IN_SCORE_BOARD_Y), tex));
        } else if (len == 2) {
            str = "images/" + std::string(1,score_in_string[0]) + ".png";
            tex = window.loadTexture(str.c_str());
            number.push_back(Entity(Vector2f(SCREEN_WIDTH/2-NUMBER_WIDTH-NUMBER_GAP,NUMBER_Y), tex));
            number_in_score_board.push_back(Entity(Vector2f(SCREEN_WIDTH/2-NUMBER_WIDTH-NUMBER_GAP,NUMBER_IN_SCORE_BOARD_Y), tex));

            str = "images/" + std::string(1,score_in_string[1]) + ".png";
            tex = window.loadTexture(str.c_str());
            number.push_back(Entity(Vector2f(SCREEN_WIDTH/2+NUMBER_GAP,NUMBER_Y), tex));
            number_in_score_board.push_back(Entity(Vector2f(SCREEN_WIDTH/2+NUMBER_GAP,NUMBER_IN_SCORE_BOARD_Y), tex));
        } else if (len == 3) {
            str = "images/" + std::string(1,score_in_string[0]) + ".png";
            tex = window.loadTexture(str.c_str());
            number.push_back(Entity(Vector2f(SCREEN_WIDTH/2-NUMBER_WIDTH/2*3-NUMBER_GAP,NUMBER_Y), tex));
            number_in_score_board.push_back(Entity(Vector2f(SCREEN_WIDTH/2-NUMBER_WIDTH/2*3-NUMBER_GAP,NUMBER_IN_SCORE_BOARD_Y), tex));

            str = "images/" + std::string(1,score_in_string[1]) + ".png";
            tex = window.loadTexture(str.c_str());
            number.push_back(Entity(Vector2f(SCREEN_WIDTH/2-NUMBER_WIDTH/2,NUMBER_Y), tex));
            number_in_score_board.push_back(Entity(Vector2f(SCREEN_WIDTH/2-NUMBER_WIDTH/2,NUMBER_IN_SCORE_BOARD_Y), tex));

            str = "images/" + std::string(1,score_in_string[2]) + ".png";
            tex = window.loadTexture(str.c_str());
            number.push_back(Entity(Vector2f(SCREEN_WIDTH/2+NUMBER_WIDTH/2+NUMBER_GAP,NUMBER_Y), tex));
            number_in_score_board.push_back(Entity(Vector2f(SCREEN_WIDTH/2+NUMBER_WIDTH/2+NUMBER_GAP,NUMBER_IN_SCORE_BOARD_Y), tex));
        }

        window.render(bg);
        window.render(pipe1);
        window.render(pipe2);
        window.render(pipe1_reverse);
        window.render(pipe2_reverse);
        window.render(base1);
        window.render(base2);
        window.render(base1_reverse);
        window.render(base2_reverse);
        window.render(bird);

        if (pausing && bird_y < SCREEN_HEIGHT - BASE_HEIGHT - BIRD_HEIGHT) {
            bird_y += 5;
        }

        if (pausing && bird_y >= SCREEN_HEIGHT - BASE_HEIGHT - BIRD_HEIGHT) {
            window.render(score_board);
            window.render(restart);
        }

        if (pausing == false) {
            for (Entity e : number) {
                window.render(e);
            }
            bird_frame++;
        } else {
            if (bird_y >= SCREEN_HEIGHT - BASE_HEIGHT - BIRD_HEIGHT) {
                for (Entity e : number_in_score_board) {
                    window.render(e);
                }
            }
        }

        if (bird_frame / (BIRD_ANIMATION_FRAME - 5) > (NUMBER_OF_BIRD - 1)) {
            bird_frame = 0;
        }

        bird_y += bird_drop_velocity;
        bird_drop_velocity += GRAVITY;

        for (SDL_Rect rect : all_of_rect) {
            if (collision(bird_rect, rect)) {
                pausing = true;
                pipe_velocity = 0;
                bird_drop_velocity = 0;
                base_velocity = 0;
                // SDL_Delay(1000);
            }
        }

        window.display();

        while(accumulator >= timeStep) {
            while(SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    running = false;
                }  else if (event.type == SDL_MOUSEBUTTONDOWN) {
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        if (pausing) {
                            if (event.button.x >= SCREEN_WIDTH / 2 - RESTART_WIDTH / 2
                                && event.button.x <= SCREEN_WIDTH / 2 + RESTART_WIDTH / 2
                                && event.button.y >= RESTART_Y
                                && event.button.y <= RESTART_Y + RESTART_HEIGHT) {
                                bird_y = INIT_BIRD_Y;
                                pipe_velocity = INIT_PIPE_VELOCITY;
                                base_velocity = INIT_BASE_VELOCITY;
                                pipe1_x = INIT_PIPE1_X;
                                pipe2_x = pipe1_x + DISTANCE_BETWEEN_TWO_PIPE + PIPE_WIDTH;
                                pipe1_y = (double) (rand() % (MAX - MIN + 1) + MIN);
                                pipe2_y = pipe1_y;
                                score = INIT_SCORE;
                                base1_x = INIT_BASE1_X;
                                base2_x = SCREEN_WIDTH;
                                pipe1_reverse_y = pipe1_y - DISTANCE - PIPE_HEIGHT;
                                pipe2_reverse_y = pipe2_y - DISTANCE - PIPE_HEIGHT;
                                pipe1_pass = false;
                                pipe2_pass = false;
                                bird_drop_velocity = 0;
                                bird_frame = 0;
                                random_bg = rand() % NUMBER_OF_BACKGROUND;
                                kind_of_bird = rand() % NUMBER_OF_BIRD;
                                pausing = false;
                                // start_game = true;
                                continue;
                            }
                        } else {
                            bird_drop_velocity = 0;
                            bird_drop_velocity -= 4;
                        }
                    }
                } else if (event.type == SDL_KEYDOWN) {
                    if (event.key.keysym.sym == SDLK_SPACE) {
                        if (!pausing) {
                            bird_drop_velocity = 0;
                            bird_drop_velocity -= 4;
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
    SDL_Quit();
    return 0;
}
