#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <math.h>
#include <Windows.h>

// If defined, logs will be shown on console and written to file.
// If commented out, logs will not be shown nor be saved.
#define LOG_ENABLED
//math
#define PI 3.14159265

/* Constant*/

// Frame rate (frame per second)
const int FPS = 60;
// Display (screen) width.
const int SCREEN_W = 800;
const int SCREEN_H = 600;
// At most 10 audios can be played at a time.
const int RESERVE_SAMPLES = 10;

enum {
    SCENE_MENU = 1,
    SCENE_START = 2,
    SCENE_SETTINGS = 3,
    SCENE_CHECKIN = 4,
    SCENE_ENDING = 5,
    SCENE_TEMP = 6,
    SCENE_CONTINUE = 7,
    SCENE_STORE=8,
};
ALLEGRO_EVENT event;
/* Input states */
// The active scene id.
int active_scene;
// Keyboard state, whether the key is down or not.
bool key_state[ALLEGRO_KEY_MAX];
// Mouse state, whether the key is down or not.
// 1 is for left, 2 is for right, 3 is for middle.
bool* mouse_state;
// Mouse position.
int mouse_x, mouse_y;

/* Variables for allegro basic routines. */
ALLEGRO_DISPLAY* game_display;
ALLEGRO_EVENT_QUEUE* game_event_queue;
ALLEGRO_TIMER* game_update_timer;

/* Shared resources*/
ALLEGRO_FONT* font_pirulen_32;
ALLEGRO_FONT* font_pirulen_24;
ALLEGRO_FONT* font_pirulen_16;

/* Menu Scene resources*/
ALLEGRO_BITMAP* main_img_background;
ALLEGRO_BITMAP* img_settings;
ALLEGRO_BITMAP* img_settings2;
ALLEGRO_SAMPLE* main_bgm;
ALLEGRO_SAMPLE_ID main_bgm_id;

/* Start Scene resources*/
ALLEGRO_BITMAP* start_img_background;
ALLEGRO_BITMAP* start_img_plane;
ALLEGRO_BITMAP* start_img_enemy;
ALLEGRO_BITMAP* start_img_enemy_2;
ALLEGRO_SAMPLE* start_bgm;
ALLEGRO_SAMPLE_ID start_bgm_id;
//characters
ALLEGRO_BITMAP* ghost1;
ALLEGRO_BITMAP* ghost2;
ALLEGRO_BITMAP* ghost3;

// other background
ALLEGRO_BITMAP* night_img_background;
ALLEGRO_SAMPLE* night_bgm;
ALLEGRO_SAMPLE_ID night_bgm_id;
ALLEGRO_SAMPLE* ending_bgm;
ALLEGRO_SAMPLE_ID ending_bgm_id;
ALLEGRO_SAMPLE* breakrecord_bgm;
ALLEGRO_SAMPLE_ID breakrecord_bgm_id;
ALLEGRO_SAMPLE* over_bgm;
ALLEGRO_SAMPLE_ID over_bgm_id;
ALLEGRO_SAMPLE* select_bgm;
ALLEGRO_SAMPLE_ID select_bgm_id;
ALLEGRO_BITMAP* music_1;
ALLEGRO_BITMAP* music_2;
ALLEGRO_SAMPLE* song;
ALLEGRO_SAMPLE_ID song_id;

//weapon and shooting sound effect
ALLEGRO_BITMAP* img_bullet;
ALLEGRO_BITMAP* img_bullet_enemy_2;
ALLEGRO_BITMAP* img_rocket;
ALLEGRO_BITMAP* img_rocket_2;
ALLEGRO_SAMPLE* shoot_bgm;
ALLEGRO_SAMPLE_ID shoot_bgm_id;
ALLEGRO_SAMPLE* shoot_bgm_enemy_2;
ALLEGRO_SAMPLE_ID shoot_bgm_id_enemy_2;


/* Store resources*/
ALLEGRO_BITMAP* store_img_background;
ALLEGRO_BITMAP* img_bullet_bag;
ALLEGRO_BITMAP* img_bullet_0;
ALLEGRO_BITMAP* img_bullet_1;
ALLEGRO_BITMAP* img_bullet_con_1;
ALLEGRO_BITMAP* img_bullet_2;
ALLEGRO_BITMAP* img_bullet_con_2;
ALLEGRO_BITMAP* img_bullet_3;
ALLEGRO_BITMAP* img_bullet_con_3;
ALLEGRO_SAMPLE* buy;
ALLEGRO_SAMPLE_ID buy_id;

//file
FILE* pfile;
int hav[4];
int money;

typedef struct {
    // The center coordinate of the image.
    float x, y;
    // The width and height of the object.
    float w, h;
    // The velocity in x, y axes.
    float vx, vy;
    // Should we draw this object on the screen.
    bool hidden;
    // The pointer to the object¡¦s image.
    ALLEGRO_BITMAP* img;
    //the lives you have
    int live;
    // the blood you have
    float blood;
} MovableObject;

void draw_movable_object(MovableObject obj);

#define MAX_ENEMY 5
#define MAX_BULLET 6
#define MAX_BULLET_ENEMY_2 15
#define MAX_ROCKET 4

MovableObject plane;
MovableObject enemies[MAX_ENEMY];
MovableObject enemy_2;
MovableObject bullets[MAX_BULLET];
MovableObject rockets[MAX_ROCKET];
MovableObject enemy_2_bullets[MAX_BULLET_ENEMY_2];


//the character's ability
float MAX_COOLDOWN=0.2;
int kill=1;
double last_shoot_timestamp;
double last_shoot_timestamp_2; 

int score = 0;
int highscore=0;
double ENDING_time;
//volume size
double sound = 1.0;
//enemy_2 movement direction
int dir = 1;
int dir1 = 1;

/* Declare function prototypes. */

// Initialize allegro5 library
void allegro5_init(void);
// Initialize variables and resources.
// Allows the game to perform any initialization it needs before
// starting to run.
void game_init(void);
// Process events inside the event queue using an infinity loop.
void game_start_event_loop(void);
// Run game logic such as updating the world, checking for collision,
// switching scenes and so on.
// This is called when the game should update its logic.
void game_update(void);
// Draw to display.
// This is called when the game should draw itself.
void game_draw(void);
// Release resources.
// Free the pointers we allocated.
void game_destroy(void);
// Function to change from one scene to another.
void game_change_scene(int next_scene);
// Load resized bitmap and check if failed.
ALLEGRO_BITMAP* load_bitmap_resized(const char* filename, int w, int h);
// [HACKATHON 3-2]
// TODO: Declare a function.
// Determines whether the point (px, py) is in rect (x, y, w, h).
// Uncomment the code below.
bool pnt_in_rect(int px, int py, int x, int y, int w, int h);

/* Event callbacks. */
void on_key_down(int keycode);
void on_mouse_down(int btn, int x, int y);

/* Declare function prototypes for debugging. */

// Display error message and exit the program, used like 'printf'.
// Write formatted output to stdout and file from the format string.
// If the program crashes unexpectedly, you can inspect "log.txt" for
// further information.
void game_abort(const char* format, ...);
// Log events for later debugging, used like 'printf'.
// Write formatted output to stdout and file from the format string.
// You can inspect "log.txt" for logs in the last run.
void game_log(const char* format, ...);
// Log using va_list.
void game_vlog(const char* format, va_list arg);
//read the file 
void take();