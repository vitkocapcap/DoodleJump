#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_audio.h>
#include <allegro5\allegro_acodec.h>
#include <list>
#include <cstdlib>
#include <string>
#include <ctime>

#include <iostream>

#include "Platform.h"
#include "Green_Platform.h"
#include "Blue_Platform.h"
#include "White_Platform.h"
#include "Brown_Platform.h"
#include "Yellow_Platform.h"
#include "Character.h"
#include "Player.h"
#include "Rocket.h"

#define FPS 75
#define WIDTH 399
#define HEIGHT 699
#define DISTANCE 40
#define LIMIT 300

#define BOUNCING_SPEED 15
#define GRAVITY 0.4

#define MAX_HEIGHT  (BOUNCING_SPEED * BOUNCING_SPEED) / 2 / GRAVITY - DISTANCE

#define LEFT_SPEED -10
#define RIGHT_SPEED 10

#define LEFT_COUNTER_SPEED 2
#define RIGHT_COUNTER_SPEED -2

#define BACKGROUND_COLOR 247, 239, 233
#define FRIST_OFFSET 50
#define DIFFICULTY_INCREASE 0.1

void init(std::list<Platform *> &platforms, Player *player);
void create_platform(std::list<Platform *> &platforms, bool guarantee = true);
void draw_background();
void draw_addition();
int death_addition();
unsigned to_unsigned(const char * str);
//create the needed function for the game 

enum State {pause, highscore, option, menu, game, game_over, none};
//creat the state for checking condition when playing game
int state = menu;
//the first state is to show the menu
int current_state = state;
int mutate = 20;
float tint_percent = 0;
float difficulty = 1;
float score = 0;
int my_highscore = 0;
bool need_restart = true;
float death_offset = 0;
float score_tint = 0;
bool inc = true;

ALLEGRO_BITMAP *menu_background, *background, *play_on, *score_on, *option_on, *score_bar, *open_spring, *closed_spring, *rocket_item, *rocket_attached, *pause_icon, *pause_icon_on, *dead, *death_sign, *flag, *flag_on;
Sprite *flag_sprite, *flag_on_sprite, *current_flag;
ALLEGRO_FONT *font;
ALLEGRO_CONFIG *cfg;
Timer *score_timer;
ALLEGRO_SAMPLE *sample, *jump, *spring;
//create the variable for sound, image,...
enum Platform_Type_Code { green, yellow, brown, dis_brown, blue, white, END };
const int Guarantee[] = { green, yellow, blue, white };
//Guarantee array to make sure doodle can jump on this platform to another platform which can be jumped on
//yellow can be jumped once but brown can not be jumped
ALLEGRO_BITMAP *imgs[END] = {NULL};
int main(int args, char **argv) {
	srand(time(NULL));
	al_init();

	al_init_image_addon();
	al_init_primitives_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_acodec_addon();
	al_install_mouse();
	al_install_keyboard();
	al_install_audio();
	al_reserve_samples(20);
	//addon needed program
	ALLEGRO_DISPLAY *display = al_create_display(WIDTH, HEIGHT);
	ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
	ALLEGRO_TIMER *delay = al_create_timer(float(1) / FPS);
	sample = al_load_sample("Audio/Jackpot.wav");
	al_play_sample(sample, 3.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
	//this command 3.0 means volume, 0.0 is to centred, 1.0 is the speed of sound
	// with this sample sound is the background music so it needs a loop
	jump = al_load_sample("Audio/Jump.wav");
	spring = al_load_sample("Audio/Boing.wav");
	//create display and load sound
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(delay));
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_mouse_event_source());
	//register for the event
	menu_background = al_load_bitmap("Data/background.png");
	background = al_load_bitmap("Data/background2.jpg");
	play_on = al_load_bitmap("Data/play_on.png");
	score_on = al_load_bitmap("Data/score_on.png");
	option_on = al_load_bitmap("Data/option_on.png");
	score_bar = al_load_bitmap("Data/score_bar.png");
	open_spring = al_load_bitmap("Data/open_spring.png");
	closed_spring = al_load_bitmap("Data/closed_spring.png");
	rocket_item = al_load_bitmap("Data/rocket_on_ground.png");
	rocket_attached = al_load_bitmap("Data/rocket_on_fire.png");
	pause_icon = al_load_bitmap("Data/pause_icon.png");
	pause_icon_on = al_load_bitmap("Data/pause_icon_on.png");
	dead = al_load_bitmap("Data/dead.png");
	death_sign = al_load_bitmap("Data/game_over.png");
	flag = al_load_bitmap("Data/restart_flag.png");
	flag_on = al_load_bitmap("Data/restart_flag_on.png");
	font = al_load_ttf_font("andy.ttf", 16, 0);

	flag_sprite = new Sprite(flag, 6, 0.2);
	flag_on_sprite = new Sprite(flag_on, 6, 0.2);

	imgs[green] = al_load_bitmap("Data/green_platform.png");
	imgs[yellow] = al_load_bitmap("Data/yellow_platform.png");
	imgs[brown] = al_load_bitmap("Data/brown_platform.png");
	imgs[dis_brown] = al_load_bitmap("Data/break_brown_platform.png");
	imgs[blue] = al_load_bitmap("Data/blue_platform.png");
	imgs[white] = al_load_bitmap("Data/white_platform.png");
	//loading images
	bool loop = true;
	bool redraw = false;
	bool left = false;
	bool right = false;
	
	std::list<Platform *> platforms;
	Platform * first_platform = new Green_Platform(imgs[green]);
	first_platform->set_location(FRIST_OFFSET, 500);
	platforms.push_back(first_platform);
	//create first platform for player can not die
	Player *player = NULL;
	cfg = al_load_config_file("Init/init.ini");
	player = new Player(cfg, 1);
	my_highscore = to_unsigned(al_get_config_value(cfg, "", "highscore"));

	player->set_location(FRIST_OFFSET + first_platform->get_width() / 2 - player->get_width() / 3, 800);
	player->set_dy(-22.5);
	//create player put him to the first platform
	//create highscore from ini file too
	score_timer = NULL;

	int press_at = none, release_at = none;

	float last_check = 0, highest = 0;

	al_start_timer(delay);
	bool check = false;
	while (loop) {
		ALLEGRO_EVENT event;
		al_wait_for_event(event_queue, &event);

		if (event.type == ALLEGRO_EVENT_TIMER) redraw = true;
		else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) break;
		else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
			if (event.keyboard.keycode == ALLEGRO_KEY_LEFT) left = true;
			else if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT) right = true;
		}
		else if (event.type == ALLEGRO_EVENT_KEY_UP) {
			if (event.keyboard.keycode == ALLEGRO_KEY_LEFT) left = false;
			else if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT) right = false;
		}
		//create input for the keyboard
		//up and down means bouncing of butten and when pressing down
		else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
			ALLEGRO_MOUSE_STATE mouse;
			al_get_mouse_state(&mouse);
			if (current_state == menu && state == menu) {
				if (mouse.x >= 89 * 3 && mouse.y >= 44 * 3 && mouse.x <= 128 * 3 && mouse.y <= 59 * 3) {
					press_at = game;
				}
			//check the position of button play with mouse
				else if (mouse.x >= 96 * 3 && mouse.y >= 71 * 3 && mouse.x <= 126 * 3 && mouse.y <= 87 * 3) {
					press_at = highscore;
				}
			}
			//check position of button highscore
			else if (current_state == game && state == game) {
				if (mouse.x >= 122 * 3 && mouse.y <= 8 * 3) {
					press_at = pause;
				}
			}
			else if (current_state == pause && state == pause) {
				int dx = al_get_bitmap_width(pause_icon), dy = al_get_bitmap_height(pause_icon);
				if (mouse.x >= WIDTH / 2 - dx / 2 && mouse.y >= HEIGHT / 2 - dy / 2 && mouse.x <= WIDTH / 2 + dx / 2 && mouse.y <= HEIGHT / 2 + dy / 2) {
					press_at = game;
				}
			}
			//pausing game state
			else if (current_state == game_over && state == game_over) {
				int add = death_addition();
				if (mouse.x >= WIDTH / 2 - flag_sprite->get_width() / 2
					&& mouse.y >= add + death_offset - flag_sprite->get_height() + 50
					&& mouse.x <= WIDTH / 2 + flag_sprite->get_width() / 2
					&& mouse.y <= add + death_offset - flag_sprite->get_height() + 50 + 19 * 3) {
					press_at = menu;
				}
			}
			//gameover but get the mouse postion for effect of graphics
			else press_at = none;
		}
		else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
			ALLEGRO_MOUSE_STATE mouse;
			al_get_mouse_state(&mouse);
			if (current_state == menu && state == menu) {
				if (mouse.x >= 89 * 3 && mouse.y >= 44 * 3 && mouse.x <= 128 * 3 && mouse.y <= 59 * 3) {
					release_at = game;
				}
				else if (mouse.x >= 96 * 3 && mouse.y >= 71 * 3 && mouse.x <= 126 * 3 && mouse.y <= 87 * 3) {
					release_at = highscore;
				}
			}
			else if (current_state == game && state == game) {
				if (mouse.x >= 122 * 3 && mouse.y <= 8 * 3) {
					release_at = pause;
				}
			}
			else if (current_state == pause && state == pause) {
				int dx = al_get_bitmap_width(pause_icon), dy = al_get_bitmap_height(pause_icon);
				if (mouse.x >= WIDTH / 2 - dx / 2 && mouse.y >= HEIGHT / 2 - dy / 2 && mouse.x <= WIDTH / 2 + dx / 2 && mouse.y <= HEIGHT / 2 + dy / 2) {
					release_at = game;
				}
			}
			else if (current_state == game_over && state == game_over) {
				int add = death_addition();
				if (mouse.x >= WIDTH / 2 - flag_sprite->get_width() / 2
					&& mouse.y >= add + death_offset - flag_sprite->get_height() + 50
					&& mouse.x <= WIDTH / 2 + flag_sprite->get_width() / 2
					&& mouse.y <= add + death_offset - flag_sprite->get_height() + 50 + 19 * 3) {
					release_at = menu;
				}
			}
			//corrolary after pressing down
			else release_at = none;
		}
		if (redraw == true) {
			redraw = false;
			if (score_timer != NULL && score_timer->passed()) {
				delete score_timer;
				score_timer = NULL;
			}
			if (press_at != none && press_at == release_at) {
				if (current_state == menu && press_at == game) {
					state = game;
					need_restart = true;
				}
				else if (current_state == menu && press_at == highscore) {
					if (score_timer == NULL) {
						score_timer = new Timer(5);
						score_timer->reset();
						inc = true;
					}
				}
				//check the state to release action
				else if (current_state == game && press_at == pause) state = pause;
				else if (current_state == pause && press_at == game) state = game;
				else if (current_state == game_over && press_at == menu) {
					state = menu;
					last_check = 0;
					player->set_flag(0);
					player->set_location(FRIST_OFFSET + first_platform->get_width() / 2 - player->get_width() / 3, 800);
					player->set_dy(-22.5);
					player->set_dx(0);
					platforms.clear();
					platforms.push_back(first_platform);
				}
				//A doodle jump at the first platform at menu screen
				press_at = release_at = none;
			}

			if (current_state != state) {
				tint_percent += 0.05;
			}
			if (tint_percent >= 1) {
				tint_percent = 0;
				current_state = state;
				if (state == game && need_restart == true) {
					init(platforms, player);
					need_restart = false;
				}
				if (state == pause) {

				}
				if (state == menu) {
					score = 0;
				}
			}
			//all things related to tint is just for visual effect

			draw_background();

			bool need_to_update = true;
			float offset = 0;

			if (current_state == game || current_state == menu) {
				if (current_state == game) {
					if (player->get_y() + player->get_height() < LIMIT) {
						offset = LIMIT - player->get_y() - player->get_height();
						player->set_y(LIMIT - player->get_height());
						score += offset;
					}
					//get the score by adding offset
					if (left == right) {
						if (player->get_dx() < 0) {
							float new_dx = player->get_dx() + LEFT_COUNTER_SPEED;
							if (new_dx > 0) new_dx = 0;
							player->set_dx(new_dx);
						}
						//move to the right but adding the opposite direction button give the slower speed
						else if (player->get_dx() > 0) {
							float new_dx = player->get_dx() + RIGHT_COUNTER_SPEED;
							if (new_dx < 0) new_dx = 0;
							player->set_dx(new_dx);
						}
						//same as above
					}
					else if (left == true) {
						player->set_dx(LEFT_SPEED);
						player->set_flag(ALLEGRO_FLIP_HORIZONTAL);
					}
					//move left
					else if (right == true) {
						player->set_dx(RIGHT_SPEED);
						player->set_flag(0);
					}
					//move right
					if (score >= DISTANCE * difficulty + last_check) {
						last_check = score;
						create_platform(platforms);
						if (DISTANCE * (difficulty + DIFFICULTY_INCREASE) < MAX_HEIGHT) difficulty += DIFFICULTY_INCREASE;
						if (mutate > 2 && rand() % 2 == 0) mutate -= 1;
						check = true;
					}
					//checking for the difficulty and increase when needing
					else if (score >= DISTANCE * difficulty / 2 + last_check && score < DISTANCE * difficulty  + last_check - DISTANCE && check == true && rand() % 2 == 0) {
						if (rand() % 2 == 0) {
							last_check = score;
							create_platform(platforms);
							if (DISTANCE * (difficulty + DIFFICULTY_INCREASE) < MAX_HEIGHT) difficulty += DIFFICULTY_INCREASE;
							if (mutate > 2 && rand() % 2 == 0) mutate -= 1;
							check = true;
						}
						//make sure there always has a chance for doodle can jump up, here is DISTANCE
						else {
							check = false;
							create_platform(platforms, false);
						}
					}
				}
				ALLEGRO_MOUSE_STATE mouse;
				al_get_mouse_state(&mouse);
				std::list<Platform *>::iterator plat = platforms.begin();
				while (plat != platforms.end()) {
					(*plat)->update(offset, WIDTH);
					if ((*plat)->collide_attach(player, HEIGHT)) {
						if ((*plat)->attach_allow(player)) {
							(*plat)->attach_affect(player);
							need_to_update = false;
						}						
					}
					//checking for attaching to platform
					if (player->is_falling() && (*plat)->collide(player, HEIGHT)) {
						if ((*plat)->avail) {
							(*plat)->fix_as(player);
							player->set_dy(-BOUNCING_SPEED);
							al_play_sample(jump, 0.1, (mouse.x - WIDTH / 2) / WIDTH, 2, ALLEGRO_PLAYMODE_ONCE, NULL);
							need_to_update = false;
						}
						//checking for falling and colliding to platform and then "DOODLE JUMP", sound on too
						(*plat)->affect(player, platforms, WIDTH);

						//give the effect if has
					}
					if ((*plat)->out_of_sight(HEIGHT) || (*plat)->to_be_destroyed() == true) {
						plat = platforms.erase(plat);
					}
					else {
						(*plat)->show(tint_percent);
						++plat;
					}
					//erase the platform out of sight
				}

				if (need_to_update) player->update(GRAVITY);
				player->show(tint_percent, WIDTH);
				if (!need_to_update) player->set_jumping();
				//check to update gravity and jumping
				if (current_state == game && player->die(HEIGHT)) {
					current_state = state = game_over;
					death_offset = HEIGHT;
					need_restart = true;
				}
			}//gameover when function die() is passed
			else if (current_state == game_over) {

			}
			else if (current_state == pause) {

			}
			
			draw_addition();
			//effect when game is being over
			al_flip_display();
		}

		
	}
	al_destroy_config(cfg);
	al_destroy_display(display);
	al_destroy_bitmap(menu_background);
	al_destroy_bitmap(play_on);
	al_destroy_bitmap(score_on);
	al_destroy_bitmap(option_on);
	al_destroy_bitmap(score_bar);
	al_destroy_bitmap(open_spring);
	al_destroy_bitmap(closed_spring);
	al_destroy_bitmap(rocket_item);
	al_destroy_bitmap(rocket_attached);
	al_destroy_bitmap(pause_icon);
	al_destroy_bitmap(pause_icon_on);
	al_destroy_bitmap(dead);
	al_destroy_bitmap(death_sign);
	al_destroy_bitmap(flag);
	al_destroy_bitmap(flag_on);
	al_destroy_font(font);
	al_destroy_sample(sample);
	al_destroy_sample(jump);
	al_destroy_sample(spring);
	return 1;
	//destroy all things and then replay
}

void init(std::list<Platform *> &platforms, Player *player) {
	Platform * new_platform;
	platforms.clear();
	new_platform = new Green_Platform(imgs[green]);
	new_platform->set_location(rand() % (WIDTH - new_platform->get_width()), HEIGHT - DISTANCE);
	player->set_location(new_platform->get_x() + new_platform->get_width() / 2 - player->get_width() / 3, 500);
	//checking for postion of player to give green platforms
	new_platform->set_code(green);
	platforms.push_back(new_platform);
	//generate green platform
	Object *obj = NULL;
	for (int h = HEIGHT - 2 * DISTANCE; h >= 0; h -= DISTANCE) {
		if (rand() % 10 == 0) obj = new Spring(closed_spring, open_spring);
		//randomly appear spring
		else obj = NULL;
		new_platform = new Green_Platform(imgs[green]);
		new_platform->set_location(rand() % (WIDTH - new_platform->get_width()), h);
		new_platform->set_code(green);
		if (obj != NULL) new_platform->set_attach(obj);
		platforms.push_back(new_platform);
		//attach spring to platform

	}
}

void draw_background() {
	al_draw_bitmap(background, 0, 0, 0);
	//main background
	ALLEGRO_MOUSE_STATE mouse;
	al_get_mouse_state(&mouse);
	//checking for mouse state
	if (current_state == menu) {
		al_draw_tinted_bitmap(menu_background, al_map_rgba_f(1, 1, 1, 1 - tint_percent), 0, 0, 0);
		//menu background
		if (mouse.x >= 89 * 3 && mouse.y >= 44 * 3 && mouse.x <= 128 * 3 && mouse.y <= 59 * 3) al_draw_tinted_bitmap(play_on, al_map_rgba_f(1, 1, 1, 1 - tint_percent), 89 * 3, 44 * 3, 0);
		if (mouse.x >= 96 * 3 && mouse.y >= 71 * 3 && mouse.x <= 126 * 3 && mouse.y <= 87 * 3) al_draw_tinted_bitmap(score_on, al_map_rgba_f(1, 1, 1, 1 - tint_percent), 96 * 3, 71 * 3, 0);
		if (mouse.x >= 89 * 3 && mouse.y >= 166 * 3 && mouse.x <= 124 * 3 && mouse.y <= 181 * 3) al_draw_tinted_bitmap(option_on, al_map_rgba_f(1, 1, 1, 1 - tint_percent), 89 * 3, 166 * 3, 0);
		if (score_timer != NULL) {
			al_draw_text(font, al_map_rgb(score_tint, score_tint, score_tint), 107 * 3, 116 * 3, 0, std::to_string(my_highscore).c_str());
			if (inc == true) {
				if (score_tint + 2 <= 255) score_tint += 2;
				else inc = false;
			}
			if (inc == false) {
				if (score_tint - 2 >= 0) score_tint -= 2;
				else {
					inc = true;
					delete score_timer;
					score_timer = NULL;
				}
			}
		}//show the highscore on menu screen with effect tint
	}
	else if (current_state == pause) {
		int dx = al_get_bitmap_width(pause_icon), dy = al_get_bitmap_height(pause_icon);
		if (   mouse.x >= WIDTH / 2 - dx / 2 
			&& mouse.y >= HEIGHT / 2 - dy / 2 
			&& mouse.x <= WIDTH / 2 + dx / 2 
			&& mouse.y <= HEIGHT / 2 + dy / 2) al_draw_tinted_bitmap(pause_icon_on, al_map_rgba_f(1, 1, 1, 1 - tint_percent), WIDTH / 2 - dx / 2, HEIGHT / 2 - dy / 2, 0);
		else al_draw_tinted_bitmap(pause_icon, al_map_rgba_f(1, 1, 1, 1 - tint_percent), WIDTH / 2 - dx / 2, HEIGHT / 2 - dy / 2, 0);
	}
	//position of pausing button 
	else if (current_state == game_over) {
		int add = death_addition();
		if (mouse.x >= WIDTH / 2 - flag_sprite->get_width() / 2
			&& mouse.y >= add + death_offset - flag_sprite->get_height() + 50
			&& mouse.x <= WIDTH / 2 + flag_sprite->get_width() / 2
			&& mouse.y <= add + death_offset - flag_sprite->get_height() + 50 + 19 * 3) {
			if (current_flag == NULL) current_flag = flag_on_sprite;
			else if (current_flag != flag_on_sprite) flag_on_sprite->get_current_of(current_flag);
			current_flag = flag_on_sprite;
		}
		else {
			if (current_flag == NULL) current_flag = flag_sprite;
			else if (current_flag != flag_sprite) flag_sprite->get_current_of(current_flag);
			current_flag = flag_sprite;
		}//the graphics when the game is over
		current_flag->draw(WIDTH / 2 - flag_sprite->get_width() / 2, add + death_offset - flag_sprite->get_height() + 50, 0, tint_percent);
		al_draw_tinted_bitmap(death_sign, al_map_rgba_f(1, 1, 1, 1 - tint_percent), WIDTH / 2 - al_get_bitmap_width(death_sign) / 2, 100, 0);
		if (death_offset >= HEIGHT - al_get_bitmap_height(dead) + 40) death_offset -= 0.1;
		al_draw_tinted_bitmap(dead, al_map_rgba_f(1, 1, 1, 1 - tint_percent), WIDTH / 2 - al_get_bitmap_width(dead) / 2, add + death_offset + 10, 0);
		//draw the graphics of death doodle
		if (int(score) > my_highscore) {
			my_highscore = int(score);
			al_set_config_value(cfg, "", "highscore", std::to_string(my_highscore).c_str());
			al_save_config_file("Init/init.ini", cfg);
		}
		//checking for highscore
		al_draw_text(font, al_map_rgb(37, 34, 99), WIDTH / 2 - al_get_bitmap_width(death_sign) / 2 + 55 * 3, 100 + 9 * 3, 0, std::to_string(my_highscore).c_str());
		al_draw_text(font, al_map_rgb(37, 34, 99), WIDTH / 2 - al_get_bitmap_width(death_sign) / 2 + 58 * 3, 100 + 18 * 3, 0, std::to_string(int(score)).c_str());
	}//show the score and highscore
}

void draw_addition() {
	if (current_state == game) {
		al_draw_tinted_bitmap(score_bar, al_map_rgba_f(1, 1, 1, 0.7), 0, 0, 0);
		al_draw_text(font, al_map_rgb(255, 255, 255), 4, 3, 0, std::to_string(unsigned(score)).c_str() );
	}
}//draw the score bar and add the score when playing game

void create_platform(std::list<Platform *> &platforms, bool guarantee) {
	Platform * new_platform;
	Object *obj = NULL;
	static bool got_white = false;
	static int cool_down = 5;
	int pick;
	if (got_white == true) {
		cool_down -= 1;
		if (cool_down > 0) {
			pick = white;
			goto skip;
		}
		else {
			got_white = false;
			cool_down = rand() % 5;
		}
	}//special case for white platform, never be alone
	if (guarantee == true) {
		if (rand() % mutate == 0) {
			if (mutate > 2 && rand() % 2 == 0) mutate -= 1;
			pick = Guarantee[rand() % (sizeof(Guarantee) / sizeof(Guarantee[0]))];
		}
		else pick = green;
	}//give chance for doodle, ignore the difficulty
	else {
		if (rand() % 1 == 0) pick = brown;
		else if (rand() % 3 == 0) pick = yellow;
		else if (rand() % mutate == 0) {
			if (mutate > 2 && rand() % 2 == 0) mutate -= 1;
			pick = rand() % END;
		}
		else pick = green;
	}
	if (pick == white) got_white = true;
	//randomly picking types of platforms
skip:
	switch (pick) {
	case green:
		new_platform = new Green_Platform(imgs[green]);
		if (rand() % 10 == 0) {
			obj = new Spring(closed_spring, open_spring);
			obj->set_audio(spring);
		}
		else if (rand() % 20 == 0) obj = new Rocket(rocket_item, rocket_attached);
		break;
		//adding rocket and spring for green
	case blue:
		new_platform = new Blue_Platform(imgs[blue]);
		if (rand() % 5 == 0) {
			obj = new Spring(closed_spring, open_spring);
			obj->set_audio(spring);
		}
		else if (rand() % 30 == 0) obj = new Rocket(rocket_item, rocket_attached);
		break;
		//adding rocket and spring for blue
	case white:
		new_platform = new White_Platform(imgs[white]);
		break;
	case brown:
		new_platform = new Brown_Platform(imgs[brown], imgs[dis_brown]);
		break;
	case yellow:
		new_platform = new Yellow_Platform(imgs[yellow]);
		break;
	default:
		new_platform = new Green_Platform(imgs[green]);
		break;
		//always has green platform
	}
	if (pick == blue) {
		new_platform->set_dx(5);
	}
	new_platform->set_location(rand() % (WIDTH - new_platform->get_width()), -new_platform->get_height());
	new_platform->set_code(pick);
	if (obj != NULL) new_platform->set_attach(obj);
	platforms.push_back(new_platform);
}//seting moving for blue platform
int death_addition() {
	ALLEGRO_MOUSE_STATE mouse;
	al_get_mouse_state(&mouse);
	if (mouse.y <= HEIGHT / 2) return 0;
	return - (mouse.y - HEIGHT / 2) * 45 / (HEIGHT / 2);
}

unsigned to_unsigned(const char * str) {
	unsigned ans = 0;
	for (int idx = 0; str[idx] != '\0'; idx++) ans = ans * 10 + (str[idx] - '0');
	return ans;
}//convert from ini file to number