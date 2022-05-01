#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <iostream>
#include <vector>
#include "RenderWindow.hpp"
#include "Entity.hpp"
#include "Utils.hpp"
#include <ctime>

// SCREEN'S DATA
const double SCREEN_WIDTH = 600;
const double SCREEN_HEIGHT = 800;
const int INIT_LIVE = 1;
//PIPE'S DATA
const double DISTANCE_BETWEEN_TWO_PIPE = 246;
const double PIPE_WIDTH = 108;
const double PIPE_HEIGHT = 400;
const double INIT_EASY_PIPE1_X = 1000;
const double INIT_HARDER_PIPE1_X = 1300;
const double INIT_IMPOSSIBLE_PIPE1_X = 1600;
const double INIT_EASY_PIPE_VELOCITY = 1.8;
const double INIT_HARDER_PIPE_VELOCITY = 2.4;
const double INIT_IMPOSSIBLE_PIPE_VELOCITY = 3;
const int MIN = 300;
const int MAX = 580;
const double DISTANCE = 180;
//BASE'S DATA
const double BASE_WIDTH = 616;
const double BASE_HEIGHT = 100;
const double INIT_BASE1_X = 0;
const double BASE_Y = SCREEN_HEIGHT - BASE_HEIGHT;
const double BASE_REVERSE_Y = -BASE_HEIGHT;
const double INIT_EASY_BASE_VELOCITY = 2;
const double INIT_HARDER_BASE_VELOCITY = 2.2;
const double INIT_IMPOSSIBLE_BASE_VELOCITY = 3.5;
//BIRD'S DATA
const int NUMBER_OF_BIRD = 3;
const int BIRD_ANIMATION_FRAME = 20;
const double BIRD_WIDTH = 68;
const double BIRD_HEIGHT = 48;
const double BIRD_X = 230;
const double INIT_BIRD_Y = 380;
//NUMBER'S DATA
const double NUMBER_WIDTH = 36;
const double NUMBER_SMALL_WIDTH = 26;
const double NUMBER_HEIGHT = 56;
const double NUMBER_GAP = 2;
const double NUMBER_Y = 100;
//RESTART BUTTON'S DATA
const double RESTART_WIDTH = 168;
const double BUTTON_HEIGHT = 60;
const double RESTART_Y = 435;
//SCORE BOARD'S DATA
const double SCORE_BOARD_WIDTH = 136;
const double SCORE_BOARD_Y = 235;
const double NUMBER_IN_SCORE_BOARD_Y = 50 + SCORE_BOARD_Y;
const double BEST_IN_SCORE_BOARD_Y = 72 + NUMBER_IN_SCORE_BOARD_Y;
//GRATIVITY
const double GRAVITY = 0.1;
//BACKGROUND
const int NUMBER_OF_BACKGROUND = 2;
//SCORE
const int INIT_SCORE = 0;
//BIRD DROP VALUE
const double DROP_VALUE = 4;
//BIRD DIE VELOCITY
const double BIRD_DIE_VELOCITY = 5;
//INIT BEST SCORE
const int INIT_BEST_SCORE = 0;
const double PLAY_WIDTH = 168;
const double EASY_WIDTH = 102;
const double HARDER_WIDTH = 142;
const double IMPOSSIBLE_WIDTH = 200;
const double BUTTON_DISTANCE = 20;
const double PLAY_Y = 525;
const double LEVEL_Y = SCREEN_HEIGHT / 2 - BUTTON_HEIGHT / 2;
const double START_WIDTH = 276;
const double START_Y = 100;
const double HEART_WIDTH = 26;
const double HEART_GAP = 2;
const double DISTANCE_BETWEEN_TWO_BUTTON = 100;
const double MAIN_MENU_WIDTH = 200;
const double MAIN_MENU_Y = RESTART_Y;
const double BACK_WIDTH = 168;
const double BACK_Y = 435;
const double NEXT_WIDTH = 168;
const double NEXT_Y = BACK_Y;
const double GAME_OVER_WIDTH = 384;
const double GAME_OVER_Y = 131;
const double GREY_BIRD_X = SCREEN_WIDTH / 2 - START_WIDTH / 2 + 112;
const double GREY_BIRD_Y = START_Y + 254;

// Set rectangle from a texture
void set_rect(SDL_Rect & rect, double a, double b, double width, double height) {
    rect.x = int(a);
    rect.y = int(b);
    rect.w = int(width);
    rect.h = int(height);
}

// Check collision
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

int main(int argc, char ** argv) {
    std::ios::sync_with_stdio();
    std::cin.tie(0);

    // Init screen
    if (SDL_Init(SDL_INIT_VIDEO) > 0) {
        std::cout << "HEY... SDL_Init HAS FAILED. SDL_ERROR: " << SDL_GetError() << std::endl;
    }

    // Init PNG image
    if (!(IMG_Init(IMG_INIT_PNG))) {
        std::cout << "IMG_Init has failed. Error: " << SDL_GetError() << std::endl;
    }
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);


    // Set window
    RenderWindow window("Flappy Bird", SCREEN_WIDTH, SCREEN_HEIGHT);

    SDL_Event event;

    srand(time(NULL));

    int best_score;
    int best_easy_score = INIT_BEST_SCORE;
    int best_harder_score = INIT_BEST_SCORE;
    int best_impossible_score = INIT_BEST_SCORE;
    double temp1;
    double temp2;
    double temp3;

    // Init texture
    SDL_Texture * bg1_texture = window.loadTexture("images/background-day.png");
    SDL_Texture * bg2_texture = window.loadTexture("images/background-night.png");
    SDL_Texture * base_texture = window.loadTexture("images/base.png");
    SDL_Texture * base_reverse_texture = window.loadTexture("images/base-reverse.png");
    SDL_Texture * yellow_bird_up_flap_texture = window.loadTexture("images/yellowbird-upflap.png");
    SDL_Texture * yellow_bird_mid_flap_texture = window.loadTexture("images/yellowbird-midflap.png");
    SDL_Texture * yellow_bird_down_flap_texture = window.loadTexture("images/yellowbird-downflap.png");
    SDL_Texture * red_bird_up_flap_texture = window.loadTexture("images/redbird-upflap.png");
    SDL_Texture * red_bird_mid_flap_texture = window.loadTexture("images/redbird-midflap.png");
    SDL_Texture * red_bird_down_flap_texture = window.loadTexture("images/redbird-downflap.png");
    SDL_Texture * blue_bird_up_flap_texture = window.loadTexture("images/bluebird-upflap.png");
    SDL_Texture * blue_bird_mid_flap_texture = window.loadTexture("images/bluebird-midflap.png");
    SDL_Texture * blue_bird_down_flap_texture = window.loadTexture("images/bluebird-downflap.png");
    SDL_Texture * pipe_texture = window.loadTexture("images/pipe.png");
    SDL_Texture * pipe_reverse_texture = window.loadTexture("images/pipe-reverse.png");
    SDL_Texture * restart_texture = window.loadTexture("images/restart.png");
    SDL_Texture * score_board_texture = window.loadTexture("images/score.png");
    SDL_Texture * play_texture = window.loadTexture("images/play.png");
    SDL_Texture * easy_texture = window.loadTexture("images/easy.png");
    SDL_Texture * harder_texture = window.loadTexture("images/harder.png");
    SDL_Texture * impossible_texture = window.loadTexture("images/impossible.png");
    SDL_Texture * start_texture = window.loadTexture("images/start.png");
    SDL_Texture * main_menu_texture = window.loadTexture("images/main-menu.png");
    SDL_Texture * game_over_texture = window.loadTexture("images/game-over.png");
    SDL_Texture * grey_bird_up_flap_texture = window.loadTexture("images/greybird-upflap.png");
    SDL_Texture * grey_bird_mid_flap_texture = window.loadTexture("images/greybird-midflap.png");
    SDL_Texture * grey_bird_down_flap_texture = window.loadTexture("images/greybird-downflap.png");

    Mix_Chunk * hit = Mix_LoadWAV("audio/hit.wav");
    Mix_Chunk * point = Mix_LoadWAV("audio/point.wav");
    Mix_Chunk * wing = Mix_LoadWAV("audio/wing.wav");


    // Init game assets
    int score = INIT_SCORE;
    int bird_frame = 0;
    int grey_frame = 0;
    bool pausing = false;
    bool start_game = false;
    bool running = true;
    double pipe_velocity = INIT_EASY_PIPE_VELOCITY;
    double base_velocity = INIT_EASY_BASE_VELOCITY;
    double bird_drop_velocity = 0;
    bool pipe1_pass = false;
    bool pipe2_pass = false;
    double base1_x = INIT_BASE1_X;
    double base2_x = BASE_WIDTH;
    double pipe1_x = INIT_EASY_PIPE1_X;
    double pipe2_x = pipe1_x + DISTANCE_BETWEEN_TWO_PIPE + PIPE_WIDTH;
    double pipe1_y = (double)(rand() % (MAX - MIN + 1) + MIN);
    double pipe2_y = pipe1_y;
    double pipe1_reverse_y = pipe1_y - DISTANCE - PIPE_HEIGHT;
    double pipe2_reverse_y = pipe2_y - DISTANCE - PIPE_HEIGHT;
    double bird_y = INIT_BIRD_Y;
    int kind_of_bird = rand() % NUMBER_OF_BIRD;
    int random_bg = rand() % NUMBER_OF_BACKGROUND;

    // BACKGROUND ENTITY
    std::vector <Entity> background;
    Entity background1 = Entity(Vector2f(0, 0), bg1_texture);
    Entity background2 = Entity(Vector2f(0, 0), bg2_texture);
    background.push_back(background1);
    background.push_back(background2);

    // BIRD TEXTURE
    std::vector <std::vector< SDL_Texture*>> bird_texture;

    std::vector <SDL_Texture*> yellow_bird_texture = {yellow_bird_up_flap_texture, yellow_bird_mid_flap_texture, yellow_bird_down_flap_texture};
    bird_texture.push_back(yellow_bird_texture);

    std::vector <SDL_Texture*> red_bird_texture = {red_bird_up_flap_texture, red_bird_mid_flap_texture, red_bird_down_flap_texture};
    bird_texture.push_back(red_bird_texture);

    std::vector <SDL_Texture*> blue_bird_texture = {blue_bird_up_flap_texture, blue_bird_mid_flap_texture, blue_bird_down_flap_texture};
    bird_texture.push_back(blue_bird_texture);

    std::vector<SDL_Texture*> grey_bird_texture = {grey_bird_up_flap_texture, grey_bird_mid_flap_texture, grey_bird_down_flap_texture};

    // Set FPS
    const double timeStep = 0.01f;
    double accumulator = 0.0f;
    double currentTime = utils::hireTimeInSeconds();

    // Set end game
    Entity restart = Entity(Vector2f(SCREEN_WIDTH / 2 - RESTART_WIDTH - DISTANCE_BETWEEN_TWO_BUTTON / 2, RESTART_Y), restart_texture);
    Entity score_board = Entity(Vector2f(SCREEN_WIDTH / 2 - SCORE_BOARD_WIDTH / 2, SCORE_BOARD_Y), score_board_texture);
    Entity play = Entity(Vector2f(SCREEN_WIDTH / 2 - PLAY_WIDTH / 2, PLAY_Y), play_texture);
    Entity easy = Entity(Vector2f(SCREEN_WIDTH / 2 - EASY_WIDTH / 2, LEVEL_Y - BUTTON_DISTANCE - BUTTON_HEIGHT), easy_texture);
    Entity harder = Entity(Vector2f(SCREEN_WIDTH / 2 - HARDER_WIDTH / 2, LEVEL_Y), harder_texture);
    Entity impossible = Entity(Vector2f(SCREEN_WIDTH / 2 - IMPOSSIBLE_WIDTH / 2, LEVEL_Y + BUTTON_DISTANCE + BUTTON_HEIGHT), impossible_texture);
    Entity start = Entity(Vector2f(SCREEN_WIDTH / 2 - START_WIDTH / 2, START_Y), start_texture);
    Entity main_menu = Entity(Vector2f(SCREEN_WIDTH / 2 + DISTANCE_BETWEEN_TWO_BUTTON / 2, MAIN_MENU_Y), main_menu_texture);
    Entity game_over = Entity(Vector2f(SCREEN_WIDTH / 2 - GAME_OVER_WIDTH / 2, GAME_OVER_Y), game_over_texture);

    bool play_click = true;
    bool choose_mode = false;
    bool mix_collision_play = false;

    while (running) {
        // FPS
        int startTicks = SDL_GetTicks();
        double newTime = utils::hireTimeInSeconds();
        double frameTime = newTime - currentTime;
        currentTime = newTime;
        accumulator += frameTime;

        // Init entity
        Entity bg = background[random_bg];
        Entity bird = Entity(Vector2f(BIRD_X, bird_y), bird_texture[kind_of_bird][bird_frame / BIRD_ANIMATION_FRAME]);
        Entity base1 = Entity(Vector2f(base1_x, BASE_Y), base_texture);
        Entity base2 = Entity(Vector2f(base2_x, BASE_Y), base_texture);
        Entity base1_reverse = Entity(Vector2f(base1_x, BASE_REVERSE_Y), base_reverse_texture);
        Entity base2_reverse = Entity(Vector2f(base2_x, BASE_REVERSE_Y), base_reverse_texture);
        Entity pipe1 = Entity(Vector2f(pipe1_x, pipe1_y), pipe_texture);
        Entity pipe2 = Entity(Vector2f(pipe2_x, pipe2_y), pipe_texture);
        Entity pipe1_reverse = Entity(Vector2f(pipe1_x, pipe1_reverse_y), pipe_reverse_texture);
        Entity pipe2_reverse = Entity(Vector2f(pipe2_x, pipe2_reverse_y), pipe_reverse_texture);

        // Declare rectangle
        SDL_Rect bird_rect, pipe1_rect, pipe2_rect, pipe1_reverse_rect, pipe2_reverse_rect,
                            base1_rect, base2_rect, base1_reverse_rect, base2_reverse_rect;

        // Set rectangle
        set_rect(bird_rect, BIRD_X, bird_y, BIRD_WIDTH, BIRD_HEIGHT);
        set_rect(pipe1_rect, pipe1_x, pipe1_y, PIPE_WIDTH, SCREEN_HEIGHT - BASE_HEIGHT - pipe1_y);
        set_rect(pipe2_rect, pipe2_x, pipe2_y, PIPE_WIDTH, SCREEN_HEIGHT - BASE_HEIGHT - pipe2_y);
        set_rect(pipe1_reverse_rect, pipe1_x, 0, PIPE_WIDTH, pipe1_y - DISTANCE);
        set_rect(pipe2_reverse_rect, pipe2_x, 0, PIPE_WIDTH, pipe2_y - DISTANCE);
        set_rect(base1_rect, base1_x, BASE_Y, BASE_WIDTH, BASE_HEIGHT);
        set_rect(base2_rect, base2_x, BASE_Y, BASE_WIDTH, BASE_HEIGHT);
        set_rect(base1_reverse_rect, base1_x, BASE_REVERSE_Y, BASE_WIDTH, BASE_HEIGHT);
        set_rect(base2_reverse_rect, base2_x, BASE_REVERSE_Y, BASE_WIDTH, BASE_HEIGHT);

        // Vector contains all rectangle
        std::vector <SDL_Rect> all_of_rect = {pipe1_rect, pipe1_reverse_rect, pipe2_rect, pipe2_reverse_rect, base1_rect, base2_rect, base1_reverse_rect, base2_reverse_rect};

        // Clear window
        window.clear();

        // Move pipe
        if (!start_game && !play_click && !choose_mode) {
            pipe1_x -= pipe_velocity;
            pipe2_x -= pipe_velocity;
        }

        // Move base
        if (!pausing) {
            base1_x -= base_velocity;
            base2_x -= base_velocity;
        }

        // Set new pipe
        if (pipe1_x <= -PIPE_WIDTH) {
            pipe1_x = SCREEN_WIDTH;
            pipe1_pass = false;
            pipe1_y = (double)(rand() % (MAX - MIN + 1) + MIN);
            pipe1_reverse_y = pipe1_y - DISTANCE - PIPE_HEIGHT;
        }

        if (pipe2_x <= -PIPE_WIDTH) {
            pipe2_x = SCREEN_WIDTH;
            pipe2_pass = false;
            pipe2_y = (double)(rand() % (MAX - MIN + 1) + MIN);
            pipe2_reverse_y = pipe2_y - DISTANCE - PIPE_HEIGHT;
        }

        // Set new base
        if (base1_x <= -(BASE_WIDTH)) {
            base1_x = BASE_WIDTH;
        }

        if (base2_x <= -(BASE_WIDTH)) {
            base2_x = BASE_WIDTH;
        }

        // Increase score
        if (pipe1_x <= BIRD_X && pipe1_pass == false) {
            score++;
            Mix_PlayChannel(-1, point, 0);
            pipe1_pass = true;
        }

        if (pipe2_x <= BIRD_X && pipe2_pass == false) {
            score++;
            Mix_PlayChannel(-1, point, 0);
            pipe2_pass = true;
        }

        // Update best score
        if (pipe_velocity == INIT_EASY_PIPE_VELOCITY) {
            if (score > best_easy_score) {
                best_easy_score = score;
            }
        } else if (pipe_velocity == INIT_HARDER_PIPE_VELOCITY) {
            if (score > best_harder_score) {
                best_harder_score = score;
            }
        } else if (pipe_velocity == INIT_IMPOSSIBLE_PIPE_VELOCITY) {
            if (score > best_impossible_score) {
                best_impossible_score = score;
            }
        }

        // Convert score to texture
        std::string score_in_string = std::to_string(score);
        int len = score_in_string.length();
        std::string str;
        SDL_Texture * tex1;
        SDL_Texture * tex2;
        std::vector <Entity> number;
        std::vector <Entity> number_in_score_board;

        if (len % 2) {
            for (int i = 0; i < len; i++) {
                str = "images/" + std::string(1, score_in_string[i]) + ".png";
                tex1 = window.loadTexture(str.c_str());
                number.push_back(Entity(Vector2f(SCREEN_WIDTH / 2 - (len - 2 * i) * NUMBER_WIDTH / 2 - (len / 2 - i) * NUMBER_GAP, NUMBER_Y), tex1));
                str = "images/" + std::string(1, score_in_string[i]) + "-small.png";
                tex2 = window.loadTexture(str.c_str());
                number_in_score_board.push_back(Entity(Vector2f(SCREEN_WIDTH / 2 - (len - 2 * i) * NUMBER_SMALL_WIDTH / 2 - (len / 2 - i) * NUMBER_GAP, NUMBER_IN_SCORE_BOARD_Y), tex2));
            }
        } else {
            for (int i = 0; i < len; i++) {
                str = "images/" + std::string(1, score_in_string[i]) + ".png";
                tex1 = window.loadTexture(str.c_str());
                number.push_back(Entity(Vector2f(SCREEN_WIDTH / 2 - (len / 2 - i) * NUMBER_WIDTH - (len - 2 * i - 1) * NUMBER_GAP / 2, NUMBER_Y), tex1));
                str = "images/" + std::string(1, score_in_string[i]) + "-small.png";
                tex2 = window.loadTexture(str.c_str());
                number_in_score_board.push_back(Entity(Vector2f(SCREEN_WIDTH / 2 - (len / 2 - i) * NUMBER_SMALL_WIDTH - (len - 2 * i - 1) * NUMBER_GAP / 2, NUMBER_IN_SCORE_BOARD_Y), tex2));
            }
        }

        // Convert best score to texture
        if (pipe_velocity == INIT_EASY_PIPE_VELOCITY) best_score = best_easy_score;
        else if (pipe_velocity == INIT_HARDER_PIPE_VELOCITY) best_score = best_harder_score;
        else if (pipe_velocity == INIT_IMPOSSIBLE_PIPE_VELOCITY) best_score = best_impossible_score;
        std::string best_in_string = std::to_string(best_score);
        int len_best = best_in_string.length();
        std::string s;
        SDL_Texture * tex;
        std::vector <Entity> best_in_score_board;

        if (len_best % 2) {
            for (int i = 0; i < len_best; i++) {
                s = "images/" + std::string(1, best_in_string[i]) + "-small.png";
                tex = window.loadTexture(s.c_str());
                best_in_score_board.push_back(Entity(Vector2f(SCREEN_WIDTH / 2 - (len_best - 2 * i) * NUMBER_SMALL_WIDTH / 2 - (len_best / 2 - i) * NUMBER_GAP, BEST_IN_SCORE_BOARD_Y), tex));
            }
        } else {
            for (int i = 0; i < len_best; i++) {
                s = "images/" + std::string(1, best_in_string[i]) + "-small.png";
                tex = window.loadTexture(s.c_str());
                best_in_score_board.push_back(Entity(Vector2f(SCREEN_WIDTH / 2 - (len_best / 2 - i) * NUMBER_SMALL_WIDTH - (len_best - 2 * i - 1) * NUMBER_GAP / 2, BEST_IN_SCORE_BOARD_Y), tex));
            }
        }

        // Window render
        window.render(bg);
        window.render(pipe1);
        window.render(pipe2);
        window.render(pipe1_reverse);
        window.render(pipe2_reverse);
        window.render(base1);
        window.render(base2);
        window.render(base1_reverse);
        window.render(base2_reverse);
        if (!play_click && !choose_mode) window.render(bird);
        if (play_click) {
            window.render(play);
            window.render(start);
            Entity grey_bird = Entity(Vector2f(GREY_BIRD_X, GREY_BIRD_Y), grey_bird_texture[grey_frame / BIRD_ANIMATION_FRAME]);
            window.render(grey_bird);
            grey_frame++;
            if (grey_frame / BIRD_ANIMATION_FRAME >= NUMBER_OF_BIRD) grey_frame = 0;
        }
        if (choose_mode) {
            window.render(easy);
            window.render(harder);
            window.render(impossible);
        }

        // Bird dies
        if (pausing && bird_y < SCREEN_HEIGHT - BASE_HEIGHT - BIRD_HEIGHT) {
            bird_y += BIRD_DIE_VELOCITY;
        }

        // Render score board and restart button when bird dies
        if (pausing && bird_y >= SCREEN_HEIGHT - BASE_HEIGHT - BIRD_HEIGHT) {
            window.render(game_over);
            window.render(score_board);
            window.render(restart);
            window.render(main_menu);
        }

        // Render final score and best score when pausing
        if (!pausing && !play_click && !choose_mode) {
            for (Entity e: number) {
                window.render(e);
            }
            bird_frame++;
        } else if (pausing){
            if (bird_y >= SCREEN_HEIGHT - BASE_HEIGHT - BIRD_HEIGHT) {
                for (Entity e: number_in_score_board) {
                    window.render(e);
                }
                for (Entity e : best_in_score_board) {
                    window.render(e);
                }
            }
        }

        // Reset bird frame
        if (bird_frame / BIRD_ANIMATION_FRAME > (NUMBER_OF_BIRD - 1)) {
            bird_frame = 0;
        }

        // Bird drop
        if (!start_game && !choose_mode && !play_click) {
            bird_y += bird_drop_velocity;
            bird_drop_velocity += GRAVITY;
        }

        // When collision
        for (SDL_Rect rect: all_of_rect) {
            if (collision(bird_rect, rect)) {
                if (!mix_collision_play) {
                    Mix_PlayChannel(-1, hit, 0);
                    mix_collision_play = true;
                }
                pausing = true;
                pipe_velocity = 0;
                bird_drop_velocity = 0;
            }
        }

        window.display();

        // Handle event
        while (accumulator >= timeStep) {
            while (SDL_PollEvent( & event)) {
                if (event.type == SDL_QUIT) {
                    running = false;
                } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        if (pausing) {
                            if (event.button.x >= SCREEN_WIDTH / 2 - DISTANCE_BETWEEN_TWO_BUTTON / 2 - RESTART_WIDTH  &&
                                event.button.x <= SCREEN_WIDTH / 2 - DISTANCE_BETWEEN_TWO_BUTTON / 2 &&
                                event.button.y >= RESTART_Y &&
                                event.button.y <= RESTART_Y + BUTTON_HEIGHT) {
                                    bird_y = INIT_BIRD_Y;
                                    pipe_velocity = temp1;
                                    base_velocity = temp2;
                                    pipe1_x = temp3;
                                    pipe2_x = pipe1_x + DISTANCE_BETWEEN_TWO_PIPE + PIPE_WIDTH;
                                    pipe1_y = (double)(rand() % (MAX - MIN + 1) + MIN);
                                    pipe2_y = pipe1_y;
                                    score = INIT_SCORE;
                                    base1_x = INIT_BASE1_X;
                                    base2_x = BASE_WIDTH;
                                    pipe1_reverse_y = pipe1_y - DISTANCE - PIPE_HEIGHT;
                                    pipe2_reverse_y = pipe2_y - DISTANCE - PIPE_HEIGHT;
                                    pipe1_pass = false;
                                    pipe2_pass = false;
                                    bird_drop_velocity = 0;
                                    bird_frame = 0;
                                    random_bg = rand() % NUMBER_OF_BACKGROUND;
                                    kind_of_bird = rand() % NUMBER_OF_BIRD;
                                    pausing = false;
                                    start_game = true;
                                    mix_collision_play = false;
                                    continue;
                            } else if (event.button.x >= SCREEN_WIDTH / 2 + DISTANCE_BETWEEN_TWO_BUTTON / 2 &&
                                        event.button.x <= SCREEN_WIDTH / 2 + DISTANCE_BETWEEN_TWO_BUTTON / 2 + MAIN_MENU_WIDTH &&
                                        event.button.y >= MAIN_MENU_Y &&
                                        event.button.y <= MAIN_MENU_Y + BUTTON_HEIGHT) {
                                            bird_y = INIT_BIRD_Y;
                                            pipe_velocity = INIT_EASY_PIPE_VELOCITY;
                                            base_velocity = INIT_EASY_BASE_VELOCITY;
                                            pipe1_x = INIT_EASY_PIPE1_X;
                                            pipe2_x = pipe1_x + DISTANCE_BETWEEN_TWO_PIPE + PIPE_WIDTH;
                                            pipe1_y = (double)(rand() % (MAX - MIN + 1) + MIN);
                                            pipe2_y = pipe1_y;
                                            score = INIT_SCORE;
                                            base1_x = INIT_BASE1_X;
                                            base2_x = BASE_WIDTH;
                                            pipe1_reverse_y = pipe1_y - DISTANCE - PIPE_HEIGHT;
                                            pipe2_reverse_y = pipe2_y - DISTANCE - PIPE_HEIGHT;
                                            pipe1_pass = false;
                                            pipe2_pass = false;
                                            bird_drop_velocity = 0;
                                            bird_frame = 0;
                                            random_bg = rand() % NUMBER_OF_BACKGROUND;
                                            kind_of_bird = rand() % NUMBER_OF_BIRD;
                                            pausing = false;
                                            play_click = true;
                                            mix_collision_play = false;
                                            continue;
                                        }
                        } else if (play_click) {
                            if (event.button.x >= SCREEN_WIDTH / 2 - PLAY_WIDTH / 2 &&
                                event.button.x <= SCREEN_WIDTH / 2 + PLAY_WIDTH / 2 &&
                                event.button.y >= PLAY_Y &&
                                event.button.y <= PLAY_Y + BUTTON_HEIGHT) {
                                play_click = false;
                                choose_mode = true;
                                }
                        } else if (choose_mode) {
                            if (event.button.x >= SCREEN_WIDTH / 2 - EASY_WIDTH / 2 &&
                                event.button.x <= SCREEN_WIDTH / 2 + EASY_WIDTH / 2 &&
                                event.button.y >= LEVEL_Y - BUTTON_DISTANCE - BUTTON_HEIGHT &&
                                event.button.y <= LEVEL_Y - BUTTON_DISTANCE) {
                                temp1 = INIT_EASY_PIPE_VELOCITY;
                                temp2 = INIT_EASY_BASE_VELOCITY;
                                pipe_velocity = temp1;
                                base_velocity = temp2;
                                temp3 = INIT_EASY_PIPE1_X;
                                pipe1_x = temp3;
                                base1_x = INIT_BASE1_X;
                                base2_x = BASE_WIDTH;
                                choose_mode = false;
                                start_game = true;
                            } else if (event.button.x >= SCREEN_WIDTH / 2 - HARDER_WIDTH / 2 &&
                                        event.button.x <= SCREEN_WIDTH / 2 + HARDER_WIDTH / 2 &&
                                        event.button.y >= LEVEL_Y &&
                                        event.button.y <= LEVEL_Y + BUTTON_HEIGHT) {
                                            temp1 = INIT_HARDER_PIPE_VELOCITY;
                                            temp2 = INIT_HARDER_BASE_VELOCITY;
                                            pipe_velocity = temp1;
                                            base_velocity = temp2;
                                            temp3 = INIT_HARDER_PIPE1_X;
                                            pipe1_x = temp3;
                                            pipe2_x = pipe1_x + DISTANCE_BETWEEN_TWO_PIPE + PIPE_WIDTH;
                                            base1_x = INIT_BASE1_X;
                                            base2_x = BASE_WIDTH;
                                            choose_mode = false;
                                            start_game = true;
                            } else if (event.button.x >= SCREEN_WIDTH / 2 - IMPOSSIBLE_WIDTH / 2 &&
                                        event.button.x <= SCREEN_WIDTH / 2 + IMPOSSIBLE_WIDTH / 2 &&
                                        event.button.y >= LEVEL_Y + BUTTON_DISTANCE + BUTTON_HEIGHT &&
                                        event.button.y <= LEVEL_Y + BUTTON_DISTANCE + 2*BUTTON_HEIGHT) {
                                            temp1 = INIT_IMPOSSIBLE_PIPE_VELOCITY;
                                            temp2 = INIT_IMPOSSIBLE_BASE_VELOCITY;
                                            pipe_velocity = temp1;
                                            base_velocity = temp2;
                                            temp3 = INIT_IMPOSSIBLE_PIPE1_X;
                                            pipe1_x = temp3;
                                            pipe2_x = pipe1_x + DISTANCE_BETWEEN_TWO_PIPE + PIPE_WIDTH;;
                                            base1_x = INIT_BASE1_X;
                                            base2_x = BASE_WIDTH;
                                            choose_mode = false;
                                            start_game = true;
                            }
                        }
                    }
                } else if (event.type == SDL_KEYDOWN) {
                    if (event.key.keysym.sym == SDLK_SPACE) {
                        if (!pausing && !start_game && !play_click && !choose_mode) {
                            bird_drop_velocity = 0;
                            bird_drop_velocity -= DROP_VALUE;
                            Mix_PlayChannel(-1, wing, 0);
                        } else if (start_game) {
                            bird_drop_velocity = 0;
                            bird_drop_velocity -= DROP_VALUE;
                            Mix_PlayChannel(-1, wing, 0);
                            start_game = false;
                        }
                    }
                }
            }
            accumulator -= timeStep;
        }

        // Get ticks
        int frameTicks = SDL_GetTicks() - startTicks;
        if (frameTicks < 1000 / window.getRefreshRate()) {
            SDL_Delay(1000 / window.getRefreshRate() - frameTicks);
        }
    }

    // Quit
    window.cleanUp();
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
    return 0;
}
