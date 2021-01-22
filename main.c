#include "../VS2019/Final Template/initial.h"

int main(int argc, char** argv) {
    //read the file
    pfile = fopen("file.txt","r");
    take();
    fclose(pfile);

    // Set random seed for better random outcome.
    srand(time(NULL));
    allegro5_init();
    game_log("Allegro5 initialized");
    game_log("Game begin");

    // Initialize game variables.
    game_init();
    game_log("Game initialized");

    // Draw the first frame.
    game_draw();
    game_log("Game start event loop");

    // This call blocks until the game is finished.
    game_start_event_loop();
    game_log("Game end");

    //write into the file
    pfile = fopen("file.txt", "w");
    fprintf(pfile, "%d,%d,%d,%d,%d,%d", highscore, money, hav[0], hav[1], hav[2], hav[3]);
    fclose(pfile);

    game_destroy();
    return 0;
}

void allegro5_init(void) {
    if (!al_init())
        game_abort("failed to initialize allegro");

    // Initialize add-ons.
    if (!al_init_primitives_addon())
        game_abort("failed to initialize primitives add-on");
    if (!al_init_font_addon())
        game_abort("failed to initialize font add-on");
    if (!al_init_ttf_addon())
        game_abort("failed to initialize ttf add-on");
    if (!al_init_image_addon())
        game_abort("failed to initialize image add-on");
    if (!al_install_audio())
        game_abort("failed to initialize audio add-on");
    if (!al_init_acodec_addon())
        game_abort("failed to initialize audio codec add-on");
    if (!al_reserve_samples(RESERVE_SAMPLES))
        game_abort("failed to reserve samples");
    if (!al_install_keyboard())
        game_abort("failed to install keyboard");
    if (!al_install_mouse())
        game_abort("failed to install mouse");

    // Setup game display.
    game_display = al_create_display(SCREEN_W, SCREEN_H);
    if (!game_display)
        game_abort("failed to create display");
    al_set_window_title(game_display, "I2P(I)_2020 Final Project <109062101>");

    // Setup update timer.
    game_update_timer = al_create_timer(1.0f / FPS);
    if (!game_update_timer)
        game_abort("failed to create timer");

    // Setup event queue.
    game_event_queue = al_create_event_queue();
    if (!game_event_queue)
        game_abort("failed to create event queue");

    // Malloc mouse buttons state according to button counts.
    const unsigned m_buttons = al_get_mouse_num_buttons();
    game_log("There are total %u supported mouse buttons", m_buttons);
    // mouse_state[0] will not be used.
    mouse_state = malloc((m_buttons + 1) * sizeof(bool));
    memset(mouse_state, false, (m_buttons + 1) * sizeof(bool));

    // Register display, timer, keyboard, mouse events to the event queue.
    al_register_event_source(game_event_queue, al_get_display_event_source(game_display));
    al_register_event_source(game_event_queue, al_get_timer_event_source(game_update_timer));
    al_register_event_source(game_event_queue, al_get_keyboard_event_source());
    al_register_event_source(game_event_queue, al_get_mouse_event_source());
   
    // Start the timer to update and draw the game.
    al_start_timer(game_update_timer);
}

void game_init(void) {
    /* Shared resources*/
    font_pirulen_32 = al_load_font("pirulen.ttf", 32, 0);
    if (!font_pirulen_32)
        game_abort("failed to load font: pirulen.ttf with size 32");

    font_pirulen_24 = al_load_font("pirulen.ttf", 24, 0);
    if (!font_pirulen_24)
        game_abort("failed to load font: pirulen.ttf with size 24");

    font_pirulen_16 = al_load_font("pirulen.ttf", 16, 0);
    if (!font_pirulen_16)
        game_abort("failed to load font: pirulen.ttf with size 16");

    /* Menu Scene resources*/
    main_img_background = load_bitmap_resized("main-bg.jpg", SCREEN_W, SCREEN_H);

    main_bgm = al_load_sample("music_1.mp3");
    if (!main_bgm)
        game_abort("failed to load audio: music_1.mp3");
    song = main_bgm;
    if (!al_play_sample(song, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &song_id))
        game_abort("??failed to play audio (bgm)???"), game_log("0099failed to play audio (bgm)");
    
    img_settings = al_load_bitmap("settings.png");
    if (!img_settings)
        game_abort("failed to load image: settings.png");
    img_settings2 = al_load_bitmap("settings2.png");
    if (!img_settings2)
        game_abort("failed to load image: settings2.png");

    /* Start Scene resources*/
    start_img_background = load_bitmap_resized("start-bg.jpg", SCREEN_W, SCREEN_H);

    ghost1 = al_load_bitmap("ghost_1.png");
    if (!ghost1)
        game_abort("failed to load image: ghost_1.png");
    ghost2 = al_load_bitmap("ghost_2.png");
    if (!ghost2)
        game_abort("failed to load image: ghost_2.png");
    ghost3 = al_load_bitmap("ghost_3.png");
    if (!ghost3)
        game_abort("failed to load image: ghost_3.png");

    start_img_plane = ghost1;

    start_img_enemy = al_load_bitmap("smallfighter0006.png");
    if (!start_img_enemy)
        game_abort("failed to load image: smallfighter0006.png");

    start_img_enemy_2 = al_load_bitmap("bat.png");
    if (!start_img_enemy_2)
        game_abort("failed to load image: bat.png");

    start_bgm = al_load_sample("music_2.mp3");
    if (!start_bgm)
        game_abort("failed to load audio: music_2.mp3");

    /*Night Scene resources*/
    night_img_background = load_bitmap_resized("night_bg.png", SCREEN_W, SCREEN_H);
    night_bgm = al_load_sample("music_3.mp3");
    if (!night_bgm)
        game_abort("failed to load audio: music_3.mp3");
    //Ending sound
    over_bgm = al_load_sample("ending.mp3");
    if (!over_bgm)
        game_abort("failed to load audio: ending.mp3");

    ending_bgm = over_bgm;
    //select(click) sound
    select_bgm = al_load_sample("select01.mp3");
    if (!select_bgm)
        game_abort("failed to load audio: select01.mp3");
    //music picture
    music_1 = al_load_bitmap("music_1.png");
    if (!music_1)
        game_abort("failed to load image: music_1.png");

    music_2 = al_load_bitmap("music_2.png");
    if (!music_2)
        game_abort("failed to load image: music_2.png");
    
    img_bullet_0 = al_load_bitmap("image12.png");
    if (!img_bullet_0)
        game_abort("failed to load image: image12.png");
    img_bullet = img_bullet_0;

    img_rocket = al_load_bitmap("fire5.png");
    if (!img_rocket)
        game_abort("fire5.png");
    img_rocket_2 = al_load_bitmap("fire3.png");
    if (!img_rocket_2)
        game_abort("failed to load image: fire5.png");

    img_bullet_enemy_2 = al_load_bitmap("image_enemy.png");
    if (!img_bullet_enemy_2)
        game_abort("failed to load image: image_enemy.png");

    shoot_bgm = al_load_sample("laser.mp3");
    if (!shoot_bgm)
        game_abort("failed to load audio: laser.mp3");

    shoot_bgm_enemy_2 = al_load_sample("beam.mp3");
    if (!shoot_bgm_enemy_2)
        game_abort("failed to load audio: beam.mp3");
    breakrecord_bgm = al_load_sample("breakrecord.mp3");
    if (!breakrecord_bgm)
        game_abort("failed to load audio: breakrecord.mp3");
    //store
    store_img_background = al_load_bitmap("set-bg.jpg");
    if (!store_img_background)
        game_abort("failed to load image: set-bg.jpg");
    img_bullet_1 = al_load_bitmap("fire6.png");
    if (!img_bullet_1)
        game_abort("failed to load image: fire6.png");
    img_bullet_con_1 = al_load_bitmap("pic1.png");
    if (!img_bullet_con_1)
        game_abort("failed to load image: pic1.png");
    img_bullet_con_2 = al_load_bitmap("pic2.png");
    if (!img_bullet_con_2)
        game_abort("failed to load image: pic2.png");
    img_bullet_con_3 = al_load_bitmap("pic3.png");
    if (!img_bullet_con_3)
        game_abort("failed to load image: pic3.png");

    img_bullet_2 = al_load_bitmap("fire2.png");
    if (!img_bullet_2)
        game_abort("failed to load image: fire2.png");
    img_bullet_3 = al_load_bitmap("fire7.png");
    if (!img_bullet_3)
        game_abort("failed to load image: fire7.png");

    buy = al_load_sample("money.mp3");
    if (!buy)
        game_abort("failed to load audio: money.mp3");
    
    // Change to first scene.
    game_change_scene(SCENE_MENU);
}

void game_start_event_loop(void) {
    bool done = false;
    //ALLEGRO_EVENT event;
    int redraws = 0;
    while (!done) {
        al_wait_for_event(game_event_queue, &event);
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            // Event for clicking the window close button.
            game_log("Window close button clicked");
            done = true;
        } else if (event.type == ALLEGRO_EVENT_TIMER) {
            // Event for redrawing the display.
            if (event.timer.source == game_update_timer)
                // The redraw timer has ticked.
                redraws++;
        } else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            // Event for keyboard key down.
            game_log("Key with keycode %d down", event.keyboard.keycode);
            key_state[event.keyboard.keycode] = true;
            on_key_down(event.keyboard.keycode);
        } else if (event.type == ALLEGRO_EVENT_KEY_UP) {
            // Event for keyboard key up.
            game_log("Key with keycode %d up", event.keyboard.keycode);
            key_state[event.keyboard.keycode] = false;
        } else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            // Event for mouse key down.
            game_log("Mouse button %d down at (%d, %d)", event.mouse.button, event.mouse.x, event.mouse.y);
            mouse_state[event.mouse.button] = true;
            on_mouse_down(event.mouse.button, event.mouse.x, event.mouse.y);
        } else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
            // Event for mouse key up.
            game_log("Mouse button %d up at (%d, %d)", event.mouse.button, event.mouse.x, event.mouse.y);
            mouse_state[event.mouse.button] = false;
        } else if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
            if (event.mouse.dx != 0 || event.mouse.dy != 0) {
                // Event for mouse move.
                // game_log("Mouse move to (%d, %d)", event.mouse.x, event.mouse.y);
                mouse_x = event.mouse.x;
                mouse_y = event.mouse.y;
            } else if (event.mouse.dz != 0) {
                // Event for mouse scroll.
                game_log("Mouse scroll at (%d, %d) with delta %d", event.mouse.x, event.mouse.y, event.mouse.dz);
            }
        }

        // Redraw
        if (redraws > 0 && al_is_event_queue_empty(game_event_queue)) {
            // if (redraws > 1)
            //     game_log("%d frame(s) dropped", redraws - 1);
            // Update and draw the next frame.
            game_update();
            game_draw();
            redraws = 0;
        }
    }
}
//keyboard update
void game_update(void) {
    if (active_scene == SCENE_START || active_scene == SCENE_CONTINUE) {
        plane.vx = plane.vy = 0;
        if (key_state[ALLEGRO_KEY_UP] || key_state[ALLEGRO_KEY_W])
            plane.vy -= 1;
        if (key_state[ALLEGRO_KEY_DOWN] || key_state[ALLEGRO_KEY_S])
            plane.vy += 1;
        if (key_state[ALLEGRO_KEY_LEFT] || key_state[ALLEGRO_KEY_A])
            plane.vx -= 1;
        if (key_state[ALLEGRO_KEY_RIGHT] || key_state[ALLEGRO_KEY_D])
            plane.vx += 1;
        // 0.71 is (1/sqrt(2)).
        plane.y += plane.vy * 4 * (plane.vx ? 0.71f : 1);
        plane.x += plane.vx * 4 * (plane.vy ? 0.71f : 1);
        // Limit the plane's collision box inside the frame.
        if (plane.x-plane.w/2 < 0)
            plane.x = plane.w/2;
        else if (plane.x+plane.w/2 > SCREEN_W)
            plane.x = SCREEN_W-plane.w/2;
        if (plane.y-plane.h/2 < 0)
            plane.y = plane.h/2;
        else if (plane.y+plane.h/2 > SCREEN_H)
            plane.y = SCREEN_H-plane.h/2;

        // Update bullet coordinates.
        // 1) For each bullets, if it's not hidden, update x, y
        // according to vx, vy.
        // 2) If the bullet is out of the screen, hide it.
        // Uncomment and fill in the code below.
        int i;
        for (int i = 0; i < MAX_BULLET;i++) {
            if (bullets[i].hidden)
                continue;
            bullets[i].x += bullets[i].vx;
            bullets[i].y += bullets[i].vy;
            if ( bullets[i].y-bullets[i].h/2 < 0)
                bullets[i].hidden = true;
        }

        //Update enemy coordinate
        for (int j = 0; j < MAX_ENEMY; j++) {
            if (enemies[j].hidden) continue;
            enemies[j].vx = cos(enemies[j].y/10)*5;
            enemies[j].vy = 1.5;
            enemies[j].x += enemies[j].vx;
            enemies[j].y += enemies[j].vy;
            if (enemies[j].y - enemies[j].h / 2 > SCREEN_H) {
                game_log("enemy flies out of the screen");
                enemies[j].blood = 3, enemies[j].hidden = true;
            }
        }
        //Update enemy_2 coordinate
        if (enemy_2.hidden == false) {
            enemy_2.vx = 1*dir;
            enemy_2.vy = cos(enemy_2.x / 20) * 3 *dir1;
            enemy_2.x += enemy_2.vx;
            enemy_2.y += enemy_2.vy;

            if (enemy_2.x + enemy_2.w / 2 > SCREEN_W || enemy_2.x - enemy_2.w / 2 <0) {
                dir = 0-dir;
            }
            if (enemy_2.y + enemy_2.h / 2 > SCREEN_H || enemy_2.y - enemy_2.h / 2 < 0) {
                dir1 = 0 - dir1;
            }
        }
        //Update enemy_2_bullet coordinates.
        for (int i = 0; i < MAX_BULLET_ENEMY_2; i++) {
            if (enemy_2_bullets[i].hidden)
                continue;
            enemy_2_bullets[i].x += enemy_2_bullets[i].vx;
            enemy_2_bullets[i].y += enemy_2_bullets[i].vy;
            float xplace = enemy_2_bullets[i].x - enemy_2_bullets[i].w / 2;
            float yplace = enemy_2_bullets[i].y - enemy_2_bullets[i].h / 2;
            if (xplace > SCREEN_W || xplace < 0 || yplace > SCREEN_H)
                enemy_2_bullets[i].hidden = true;
        }

        //kill enemies
        for (int i = 0; i < MAX_ENEMY; i++) {
            if (enemies[i].hidden == false) {
                for (int j = 0; j < MAX_BULLET; j++) {
                    if (abs(bullets[j].x-enemies[i].x) <=(bullets[j].w/2 + enemies[i].w/2) && abs(bullets[j].y - enemies[i].y) <= (bullets[j].h/2 + enemies[i].h/2)) {
                        if(bullets[j].hidden==false) enemies[i].blood-=kill;
                        bullets[j].hidden = true;
                    }
                    if (enemies[i].blood <= 0) 
                        score += 5, enemies[i].blood=3 ,enemies[i].hidden = true;
                }
                for (int j = 0; j < MAX_ROCKET; j++) {
                    if (abs(rockets[j].x - enemies[i].x) <= (rockets[j].w / 2 + enemies[i].w / 2) && abs(rockets[j].y - enemies[i].y) <= (rockets[j].h / 2 + enemies[i].h / 2)) {
                        if (rockets[j].hidden == false) enemies[i].blood -= 2;
                        rockets[j].hidden = true;
                    }
                    if (enemies[i].blood <= 0) {
                        score += 5, enemies[i].blood = 3, enemies[i].hidden = true;
                    }
                }
            }
        }
        
        //kill enemy_2
        if (enemy_2.hidden == false) {
            for (int j = 0; j < MAX_BULLET; j++) {
                if (abs(bullets[j].x - enemy_2.x) <= (bullets[j].w / 2 + enemy_2.w / 2) && abs(bullets[j].y - enemy_2.y) <= (bullets[j].h / 2 + enemy_2.h / 2)) {
                    if (bullets[j].hidden == false) enemy_2.blood-=kill;
                    bullets[j].hidden = true;
                }
                if (enemy_2.blood <= 0)
                    score += 10, enemy_2.blood = 10, enemy_2.hidden = true;
            }
            for (int j = 0; j < MAX_ROCKET; j++) {
                if (abs(rockets[j].x - enemy_2.x) <= (rockets[j].w / 2 + enemy_2.w / 2) && abs(rockets[j].y - enemy_2.y) <= (rockets[j].h / 2 + enemy_2.h / 2)) {
                    if (rockets[j].hidden == false) enemy_2.blood -= 2;
                    rockets[j].hidden = true;
                }
                if (enemy_2.blood <= 0) {
                    score += 10, enemy_2.blood = 10, enemy_2.hidden = true;
                }
            }
        }
        //Update rocket coordinate
        for (int j = 0; j < MAX_ROCKET; j++) {
            if (rockets[j].hidden)
                continue;
            rockets[j].vx = cos(rockets[j].y / 10) * 5 * (3 - j);
            //rockets[j].vx = cos(rockets[j].y / 10) * 10 * ((-1) ^ j);
            rockets[j].vy = -2.5;
            rockets[j].x += rockets[j].vx;
            rockets[j].y += rockets[j].vy;
            if (rockets[j].y - rockets[j].h / 2 < 0) {
                game_log("enemy flies out of the screen");
                rockets[j].hidden = true;
            }
        }
           
        //  Shoot if key is down and cool-down is over.
        // 1) Get the time now using 'al_get_time'.
        // 2) If Space key is down in 'key_state' and the time
        //    between now and last shoot is not less that cool
        //    down time.
        // 3) Loop through the bullet array and find one that is hidden.
        //    (This part can be optimized.)
        // 4) The bullet will be found if your array is large enough.
        // 5) Set the last shoot time to now.
        // 6) Set hidden to false (recycle the bullet) and set its x, y to the
        //    front part of your plane.

        double now = al_get_time();
        if (key_state[ALLEGRO_KEY_SPACE] && now - last_shoot_timestamp >= MAX_COOLDOWN) {
            for (i = 0; i < MAX_BULLET;i++) {
                if (bullets[i].hidden== true) {
                    last_shoot_timestamp = now;
                    bullets[i].hidden = false;
                    bullets[i].x = plane.x;
                    bullets[i].y = plane.y-plane.h/2;
                    if (!al_play_sample(shoot_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &shoot_bgm_id))
                        game_abort("failed to play audio (bgm)111111");
                    break;
                }
            }
        }

        //reset the enemies
        float enemies_xplace = (float)rand() / RAND_MAX * (SCREEN_W - al_get_bitmap_width(start_img_enemy));
        for (i = 0; i < MAX_ENEMY; i++) {
            if (enemies[i].hidden == true) {
                enemies[i].hidden = false;
                enemies[i].x = enemies[i].w / 2 + enemies_xplace;// +sin(2 * PI * i / MAX_ENEMY) * enemies[i].w;
                enemies[i].y = 80;//250 - i * enemies[i].h;
                break;
            }
        }
        //enemy_2 appears & setup enemy_2_bullets
        if (score % 40 == 0 && score!=0) {
            float enemy_2_vy = (float)rand() / RAND_MAX * (SCREEN_W - al_get_bitmap_width(start_img_enemy_2));
            if (enemy_2.hidden == true) {
                if (enemy_2.hidden == true) {
                    enemy_2.hidden = false;
                    enemy_2.x = enemy_2.w / 2 + enemies_xplace;
                    enemy_2.y = 90;
                }
                for (int i = 0; i < MAX_BULLET_ENEMY_2; i++) {
                    if (enemy_2_bullets[i].hidden == false) continue;
                    enemy_2_bullets[i].hidden = false;
                    enemy_2_bullets[i].x = enemy_2.x;
                    enemy_2_bullets[i].y = enemy_2.y;
                    enemy_2_bullets[i].vy = 2;
                    enemy_2_bullets[i].vx = 7 - i;
                    if (i == 0)
                        if (!al_play_sample(shoot_bgm_enemy_2, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &shoot_bgm_id_enemy_2))
                            game_abort("failed to play audio (bgm)2222");
                }
            }
        }
        //set up rocket
        double now2 = al_get_time();
        if(key_state[ALLEGRO_KEY_LCTRL] && now2 - last_shoot_timestamp_2 > 12){
            for (i = 0; i < MAX_ROCKET; i++) {
                if (!rockets[i].hidden)continue;
                last_shoot_timestamp_2 = now;
                rockets[i].hidden = false;
                rockets[i].x = plane.x;
                rockets[i].y = plane.y - plane.h / 2;
                if (!al_play_sample(shoot_bgm_enemy_2, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &shoot_bgm_id_enemy_2))
                    game_abort("failed to play audio (bgm)331111");
            }
        }
        
        //if my plane is attacked
        for (int j = 0; j < MAX_BULLET_ENEMY_2; j++) {
            if (enemy_2_bullets[j].hidden == false) {
                if (abs(enemy_2_bullets[j].x - plane.x) <= (enemy_2_bullets[j].w / 2 + plane.w / 2) &&
                        abs(enemy_2_bullets[j].y - plane.y) <= (enemy_2_bullets[j].h / 2 + plane.h / 2)) {
                        plane.blood-=2;
                        if (plane.blood == 0)plane.blood = 4, plane.live--;
                        game_log("blood:%d", plane.blood);
                        enemy_2_bullets[j].hidden = true;
                }
                if (plane.live == 0) {
                    game_change_scene(SCENE_ENDING);
                }
            }
        }
        for (i = 0; i < MAX_ENEMY; i++) {
            if (enemies[i].hidden == false) {
                if (abs(enemies[i].x - plane.x) <= (enemies[i].w / 2 + plane.w / 2) && abs(enemies[i].y - plane.y) <= (enemies[i].h / 2 + plane.h / 2))
                    plane.blood-=2,enemies[i].hidden=true;
                if (plane.blood == 0)plane.blood = 4, plane.live--;
                if (plane.live == 0) {
                    game_change_scene(SCENE_ENDING);
                    break;
                }
            }
        }
        if (abs(enemy_2.x - plane.x) <= (enemy_2.w / 2 + plane.w / 2) &&
            abs(enemy_2.y - plane.y) <= (enemy_2.h / 2 + plane.h / 2)) {
            plane.blood -= 4;
            if (plane.blood == 0)plane.blood = 4, plane.live--;
            else if (plane.blood < 0)plane.blood = -plane.blood, plane.live--;
            game_log("blood:%d", plane.blood);
            if (plane.live == 0) {
                game_change_scene(SCENE_ENDING);
            }
        }
    }
 }

void game_draw(void) {
    if (active_scene == SCENE_MENU) {
        al_draw_bitmap(main_img_background, 0, 0, 0);
        al_draw_text(font_pirulen_32, al_map_rgb(108, 166, 205), SCREEN_W / 2, 100, ALLEGRO_ALIGN_CENTER, "Ghost Shooter");
        al_draw_text(font_pirulen_24, al_map_rgb(159 ,182, 205), SCREEN_W / 2, SCREEN_H - 250, ALLEGRO_ALIGN_CENTER, "Play");
        al_draw_text(font_pirulen_24, al_map_rgb(104, 131, 139), SCREEN_W / 2, SCREEN_H - 200, ALLEGRO_ALIGN_CENTER, "Press enter key to start! ");
        al_draw_text(font_pirulen_24, al_map_rgb(159, 182, 205), SCREEN_W / 2, SCREEN_H - 150, ALLEGRO_ALIGN_CENTER, "Store");
        al_draw_text(font_pirulen_24, al_map_rgb(104, 131, 139), SCREEN_W / 2, SCREEN_H - 100, ALLEGRO_ALIGN_CENTER, "Press shift key to enter the store!");

        // Draw settings images.
        // The settings icon should be located at (x, y, w, h) =
        // (SCREEN_W - 48, 10, 38, 38).
        // Change its image according to your mouse position.
        if (pnt_in_rect(mouse_x, mouse_y, SCREEN_W - 48, 10, 38, 38))
            al_draw_bitmap(img_settings2, SCREEN_W - 48, 10, 0);
        else
            al_draw_bitmap(img_settings, SCREEN_W - 48, 10, 0);
    } 
    else if (active_scene == SCENE_START ||  active_scene == SCENE_TEMP || active_scene == SCENE_CONTINUE) {
        int i;
        if(score < 50 || (score > 100 && score < 150) || score >200 ) al_draw_bitmap(start_img_background, 0, 0, 0);
        else al_draw_bitmap(night_img_background, 0, 0, 0);

        // TODO: Draw all bullets in your bullet array.
        for (int i=0;i<MAX_BULLET;i++)
            draw_movable_object(bullets[i]);
        draw_movable_object(plane);

        al_draw_filled_rectangle(plane.x - plane.w/2, plane.y - plane.h/2 - 1, plane.x -plane.w/2 + plane.w *plane.blood/4, plane.y - plane.h/2 - 3, al_map_rgb(155, 255, 155));
        al_draw_filled_rectangle(plane.x - plane.w / 2 + plane.w * plane.blood / 4, plane.y - plane.h / 2 - 1, plane.x + plane.w / 2, plane.y - plane.h / 2 - 3, al_map_rgb(255, 0, 0));
        
        for (i = 0; i < MAX_ENEMY; i++) {
            draw_movable_object(enemies[i]);
            if (enemies[i].blood > 0) {
                al_draw_filled_rectangle(enemies[i].x - enemies[i].w / 2, enemies[i].y - enemies[i].h / 2 - 1, enemies[i].x - enemies[i].w / 2 + enemies[i].w * enemies[i].blood / 3, enemies[i].y - enemies[i].h / 2 - 3, al_map_rgb(155, 255, 155));
                al_draw_filled_rectangle(enemies[i].x - enemies[i].w / 2 + enemies[i].w * enemies[i].blood / 3, enemies[i].y - enemies[i].h / 2 - 1, enemies[i].x + enemies[i].w / 2, enemies[i].y - enemies[i].h / 2 - 3, al_map_rgb(255, 0, 0));
            }
        }
            
         draw_movable_object(enemy_2);
         if (!enemy_2.hidden && enemy_2.blood > 0) {
             al_draw_filled_rectangle(enemy_2.x - enemy_2.w / 2, enemy_2.y - enemy_2.h / 2 - 1, enemy_2.x - enemy_2.w / 2 + enemy_2.w * enemy_2.blood / 10, enemy_2.y - enemy_2.h / 2 - 3, al_map_rgb(155, 255, 155));
             al_draw_filled_rectangle(enemy_2.x - enemy_2.w / 2 + enemy_2.w * enemy_2.blood / 10, enemy_2.y - enemy_2.h / 2 - 1, enemy_2.x + enemy_2.w / 2, enemy_2.y - enemy_2.h / 2 - 3, al_map_rgb(255, 0, 0));
         }
         
        for (i = 0; i < MAX_BULLET_ENEMY_2; i++)
             draw_movable_object(enemy_2_bullets[i]);

        for (i = 0; i < MAX_ROCKET; i++)
            draw_movable_object(rockets[i]);
       
        al_draw_textf(font_pirulen_16, al_map_rgb(60, 58, 154), 20, 20, 0, "Score:%d", score);
        al_draw_textf(font_pirulen_16, al_map_rgb(60, 58, 154), 20, 40, 0, "Lives: %d", plane.live);

        if (active_scene == SCENE_TEMP) {
            al_draw_filled_rectangle(0, 0, SCREEN_W, SCREEN_H, al_map_rgba(0, 0, 0, 100));
            al_draw_text(font_pirulen_24, al_map_rgb(158, 207, 216), 400, 250, ALLEGRO_ALIGN_CENTER, "Press ENTER to continue the game");
            al_draw_text(font_pirulen_24, al_map_rgb(158, 207, 216), 400, 300, ALLEGRO_ALIGN_CENTER, "Press BACKSPACE to exist the game");
        }
    }
    else if (active_scene == SCENE_ENDING) {
        al_draw_bitmap(main_img_background, 0, 0, 0);
        al_draw_filled_rectangle(0, 0, SCREEN_W, SCREEN_H, al_map_rgba(0, 0, 0, 150));
        al_draw_text(font_pirulen_32, al_map_rgb(158, 207, 216), 280, 180, 0, "Game Over");
        al_draw_textf(font_pirulen_32, al_map_rgb(158, 207, 216), 280, 220, 0, "Score:%4d", score);
        al_draw_textf(font_pirulen_24, al_map_rgb(158, 207, 216), 250, 290, 0, "Money You Earn:%d", score/20);
        al_draw_textf(font_pirulen_24, al_map_rgb(158, 207, 216), 250, 320, 0, "Highestscore:%4d", highscore);
        al_draw_text(font_pirulen_24, al_map_rgb(158, 207, 216), 170, 400, 0, "Press ENTER to play again!");
        al_draw_text(font_pirulen_24, al_map_rgb(158, 207, 216), 170, 450, 0, "Press BACKSPACE to exist!");
    }
    else if (active_scene == SCENE_SETTINGS) {
        al_clear_to_color(al_map_rgb(0, 100, 100));
        //music change
        al_draw_text(font_pirulen_32, al_map_rgb(55, 164, 185), 50, 50, 0, "Music");
        if (pnt_in_rect(mouse_x, mouse_y, 100, 150, 40, 40)) 
            al_draw_bitmap(music_2, 100, 150, 0);
        else
            al_draw_bitmap(music_1, 100, 150, 0);

        if (pnt_in_rect(mouse_x, mouse_y, 300, 150, 40, 40)) 
            al_draw_bitmap(music_2, 300, 150, 0);
        else
            al_draw_bitmap(music_1, 300, 150, 0);

        if (pnt_in_rect(mouse_x, mouse_y, 500, 150, 40, 40)) 
            al_draw_bitmap(music_2, 500, 150, 0);
        else
            al_draw_bitmap(music_1, 500, 150, 0);

        al_draw_text(font_pirulen_24, al_map_rgb(69, 157, 156), 150, 150, 0, "Music1");
        al_draw_text(font_pirulen_24, al_map_rgb(69, 157, 156), 350, 150, 0, "Music2");
        al_draw_text(font_pirulen_24, al_map_rgb(69, 157, 156), 550, 150, 0, "Music3");

        //character
        al_draw_text(font_pirulen_32, al_map_rgb(55, 164, 185), 50, 250, 0, "Character");
        if (pnt_in_rect(mouse_x, mouse_y, 100, 350, 50, 50)) {
            al_draw_bitmap(ghost1, 100, 350, 0);
            start_img_plane = ghost1;
        }
        else
            al_draw_bitmap(ghost1, 100, 380, 0);

        if (pnt_in_rect(mouse_x, mouse_y, 300, 350, 40, 40)) {
            al_draw_bitmap(ghost2, 300, 350, 0);
            start_img_plane = ghost2;
        }
        else
            al_draw_bitmap(ghost2, 300, 380, 0);

        if (pnt_in_rect(mouse_x, mouse_y, 500, 350, 40, 40)) {
            al_draw_bitmap(ghost3, 500, 350, 0);
            start_img_plane = ghost3;
        }
        else
            al_draw_bitmap(ghost3, 500, 380, 0);

        al_draw_text(font_pirulen_24, al_map_rgb(69, 157, 156), 150, 420, 0, "Ghost1");
        al_draw_text(font_pirulen_24, al_map_rgb(69, 157, 156), 350, 420, 0, "Ghost2");
        al_draw_text(font_pirulen_24, al_map_rgb(69, 157, 156), 550, 420, 0, "Ghost3");

        al_draw_text(font_pirulen_16, al_map_rgb(250, 25, 0), 50, 480, 0, "Click The Music and The Character You Want!");
        al_draw_text(font_pirulen_16, al_map_rgb(250, 25, 0), 50, 520, 0, "Press Backspace To Back To MENU!");
    }
    else if (active_scene == SCENE_STORE) {
        al_draw_bitmap(store_img_background, 0, 0, 0);
        al_draw_filled_rectangle(0, 0, SCREEN_W, SCREEN_H, al_map_rgba(0, 0, 0, 150));
        al_draw_text(font_pirulen_32, al_map_rgb(224 ,255, 255), 50, 20, 0, "STORE");
        //buy bullet 
        al_draw_text(font_pirulen_32, al_map_rgb(224, 255, 255), 50, 80, 0, "Fire");

        if (pnt_in_rect(mouse_x, mouse_y, 100, 150, 30, 30)) {
            al_draw_filled_rectangle(100, 150, 130, 180, al_map_rgba(240, 255, 255, 50));
            al_draw_bitmap(img_bullet_con_2, 100, 180, 0);
        }
        al_draw_bitmap(img_bullet_1, 100, 150, 0);

        if (pnt_in_rect(mouse_x, mouse_y, 320, 150, 20, 30)) {
            al_draw_filled_rectangle(320, 150, 340, 180, al_map_rgba(240, 255, 255, 50));
            al_draw_bitmap(img_bullet_con_1, 320, 180, 0);
        }
        al_draw_bitmap(img_bullet_2, 320, 150, 0);
       
        if (pnt_in_rect(mouse_x, mouse_y, 550, 150, 30, 30)) {
            al_draw_filled_rectangle(550, 150, 580, 180, al_map_rgba(240, 255, 255, 50));
            al_draw_bitmap(img_bullet_con_3, 550, 180, 0);
        }
        al_draw_bitmap(img_bullet_3, 550, 150, 0);

        al_draw_text(font_pirulen_24, al_map_rgb(69, 157, 156), 150, 150, 0, "Fire1: $30");
        al_draw_text(font_pirulen_24, al_map_rgb(69, 157, 156), 360, 150, 0, "Fire2: $20");
        al_draw_text(font_pirulen_24, al_map_rgb(69, 157, 156), 600, 150, 0, "Fire3: $20");

        //choose the fire now to use
        al_draw_text(font_pirulen_32, al_map_rgb(224, 255, 255), 50, 250, 0, "My bag:");
        al_draw_textf(font_pirulen_24, al_map_rgb(224, 255, 255), 500, 250, 0, "My money:%d",money);
        int j = 0;
        for(int i = 0; i < 4; i++) {
            if(i==0)img_bullet_bag= img_bullet_0;
            else if (i == 1)img_bullet_bag = img_bullet_1;
            else if (i == 2)img_bullet_bag = img_bullet_2;
            else if (i == 3)img_bullet_bag = img_bullet_3;
            if (hav[i] == 1) {
                if (pnt_in_rect(mouse_x, mouse_y, 100+200*j, 350, 50, 50)) {
                    al_draw_bitmap(img_bullet_bag, 100 + 200 * j, 350, 0);
                }
                else
                    al_draw_bitmap(img_bullet_bag, 100 + 200 * j, 380, 0);
                if (img_bullet == img_bullet_bag) {
                    al_draw_filled_rectangle(100 + 200 * j, 420, 100 + 200 * j + 100, 450, al_map_rgba(240, 255, 0, 50));
                }
                al_draw_textf(font_pirulen_24, al_map_rgb(69, 157, 156), 100 + 200 * j, 420, 0, "fire%d",i);
                j++;
            }
        }
        al_draw_text(font_pirulen_16, al_map_rgb(250, 25, 0), 50, 480, 0, "Press Enter key to start the game!");
        al_draw_text(font_pirulen_16, al_map_rgb(250, 25, 0), 50, 520, 0, "Press Backspace key to back to MENU!");
    }
    al_flip_display();
}

void game_destroy(void) {
    // Destroy your bullet image.
    //al_destroy_bitmap(img_bullet_0);> <
    al_destroy_bitmap(img_bullet_1);
    al_destroy_bitmap(img_bullet_2);
    al_destroy_bitmap(img_bullet_bag);
    al_destroy_bitmap(img_bullet_con_1);
    al_destroy_bitmap(img_bullet_con_2);
    al_destroy_bitmap(img_bullet_con_3);
    al_destroy_bitmap(img_rocket);
    al_destroy_bitmap(img_rocket_2);

    // Destroy shared resources.
    al_destroy_font(font_pirulen_32);
    al_destroy_font(font_pirulen_24);
    al_destroy_font(font_pirulen_16);

    /* Menu Scene resources*/
    al_destroy_bitmap(main_img_background);
    al_destroy_sample(main_bgm);
    al_destroy_bitmap(img_settings);
    al_destroy_bitmap(img_settings2);

    /* Start Scene resources*/
    al_destroy_bitmap(start_img_background);
    al_destroy_bitmap(start_img_plane);
    al_destroy_bitmap(start_img_enemy);
    al_destroy_bitmap(start_img_enemy_2);
    al_destroy_sample(start_bgm);
    /* Night Scene resources*/
    al_destroy_bitmap(night_img_background);
    al_destroy_sample(night_bgm);
    al_destroy_bitmap(store_img_background);
    al_destroy_sample(ending_bgm);
    al_destroy_sample(breakrecord_bgm);
    //al_destroy_sample(over_bgm);> <
    //al_destroy_sample(song);> <
    //music
    //al_destroy_bitmap(music_1);
    //al_destroy_bitmap(music_2);
    // [HACKATHON 2-10]
    // TODO: Destroy your bullet image.
    // Uncomment and fill in the code below.
    //al_destroy_bitmap(img_bullet);
    //al_destroy_bitmap(img_bullet_enemy_2);> <
    //al_destroy_sample(shoot_bgm);
    //al_destroy_sample(shoot_bgm_enemy_2);
    //al_destroy_sample(buy);

    //al_destroy_timer(game_update_timer);
    //al_destroy_event_queue(game_event_queue);
    //al_destroy_display(game_display);
    free(mouse_state);
}

void game_change_scene(int next_scene) {
    game_log("Change scene from %d to %d", active_scene, next_scene);
    if (active_scene == SCENE_ENDING) {
        al_stop_sample(&ending_bgm_id);
        game_log("stop audio (bgm)");
        if (!al_play_sample(song, sound, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &song_id))
            game_abort("failed to play audio (bgm)"), game_log("failed to play audio (bgm)");
        ending_bgm = over_bgm;
        money += score / 20;
    }

    active_scene = next_scene;
    if (active_scene == SCENE_START) {
        int i;
        plane.img = start_img_plane;
        plane.x = 400;
        plane.y = 500;
        plane.w = al_get_bitmap_width(plane.img);
        plane.h = al_get_bitmap_height(plane.img);
        plane.blood = 4;
        plane.live = 3;
        //Initialize enemies.
        float enemies_xplace = (float)rand() / RAND_MAX * (SCREEN_W - al_get_bitmap_width(start_img_enemy));
        float enemies_yplace = (float)rand() / RAND_MAX * (SCREEN_H - al_get_bitmap_height(start_img_enemy));
        for (i = 0; i < MAX_ENEMY; i++) {
            enemies[i].img = start_img_enemy;
            enemies[i].w = al_get_bitmap_width(start_img_enemy);
            enemies[i].h = al_get_bitmap_height(start_img_enemy);
            enemies[i].x = enemies[i].w / 2 + (float)rand() / RAND_MAX * (SCREEN_W - enemies[i].w);
            enemies[i].y = enemies[i].h / 2 + (float)rand() / RAND_MAX * (SCREEN_H - enemies[i].h) - 200;
            enemies[i].vx = 3 * ((-1) ^ i);
            enemies[i].vy = 3;
            enemies[i].blood = 3;
            enemies[i].hidden = false;
        }
        //Initialize bullets.
        for (int i = 0; i < MAX_BULLET; i++) {
            bullets[i].w = al_get_bitmap_width(img_bullet);
            bullets[i].h = al_get_bitmap_height(img_bullet);
            bullets[i].img = img_bullet;
            bullets[i].vx = 0;
            bullets[i].vy = -3;
            bullets[i].hidden = true;
        }
        //Initialize enemy_2
        enemy_2.w = al_get_bitmap_width(start_img_enemy_2);
        enemy_2.h = al_get_bitmap_height(start_img_enemy_2);
        enemy_2.img = start_img_enemy_2;
        enemy_2.vx = 0;
        enemy_2.vy = 0;
        enemy_2.blood = 10;
        enemy_2.hidden = true;
        //Initialize enemy_2_bullets
        for (int i = 0; i < MAX_BULLET_ENEMY_2; i++) {
            enemy_2_bullets[i].w = al_get_bitmap_width(img_bullet_enemy_2);
            enemy_2_bullets[i].h = al_get_bitmap_height(img_bullet_enemy_2);
            enemy_2_bullets[i].img = img_bullet_enemy_2;
            enemy_2_bullets[i].hidden = true;
        }
        //Initialize rocket
        for (int i = 0; i < MAX_ROCKET; i++) {
            rockets[i].w = al_get_bitmap_width(img_rocket);
            rockets[i].h = al_get_bitmap_height(img_rocket);
            if(i%2)rockets[i].img = img_rocket;
            else rockets[i].img = img_rocket_2;
            rockets[i].x = plane.x;
            rockets[i].y = plane.y;
            rockets[i].hidden = true;
        }
        score = 0;
    }
    else if (active_scene == SCENE_ENDING) {
        al_stop_sample(&song_id);
        if (highscore < score) {
            highscore = score;
            ending_bgm = breakrecord_bgm;
            if (!al_play_sample(ending_bgm, sound, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &ending_bgm_id))
                game_abort("??failed to play audio (bgm)???"), game_log("0099failed to play audio (bgm)");
        }
        else {
            ending_bgm = over_bgm;
            if (!al_play_sample(ending_bgm, sound, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &ending_bgm_id))
                game_abort("??failed to play audio (bgm)???"), game_log("0099failed to play audio (bgm)");
        }
    }
}
//keyboard change in different scene
void on_key_down(int keycode) {
    if (active_scene == SCENE_MENU) {
        if (keycode == ALLEGRO_KEY_ENTER)
            game_change_scene(SCENE_START);
        else if (keycode == ALLEGRO_KEY_LSHIFT)
            game_change_scene(SCENE_STORE);
    }
    else if (active_scene == SCENE_SETTINGS) {
        if (keycode == ALLEGRO_KEY_BACKSPACE)
            game_change_scene(SCENE_MENU);
    }
    else if (active_scene == SCENE_START) {
        if (keycode == ALLEGRO_KEY_BACKSPACE)
            game_change_scene(SCENE_TEMP);
    }
    else if (active_scene == SCENE_TEMP) {
        if (keycode == ALLEGRO_KEY_BACKSPACE)
            game_change_scene(SCENE_MENU);
        else if (keycode == ALLEGRO_KEY_ENTER) {
            game_change_scene(SCENE_CONTINUE);
        }
    }
    else if (active_scene == SCENE_ENDING) {
        if (keycode == ALLEGRO_KEY_BACKSPACE)
            game_change_scene(SCENE_MENU);
        else if (keycode == ALLEGRO_KEY_ENTER) {
            game_change_scene(SCENE_START);
        }
    }
    else if (active_scene == SCENE_CONTINUE) {
        if (keycode == ALLEGRO_KEY_BACKSPACE)
            game_change_scene(SCENE_TEMP);
    }
    else if (active_scene == SCENE_STORE) {
        if (keycode == ALLEGRO_KEY_BACKSPACE)
            game_change_scene(SCENE_MENU);
        if (keycode == ALLEGRO_KEY_ENTER)
            game_change_scene(SCENE_START);
    }
}
//mouse change in different scene
void on_mouse_down(int btn, int x, int y) {
    // When settings clicked, switch to settings scene.
    if (active_scene == SCENE_MENU) {
        if (btn == true) {
            if (pnt_in_rect(x, y, SCREEN_W - 48, 10, 38, 38))
                game_change_scene(SCENE_SETTINGS);
        }
    }
    else if (active_scene == SCENE_SETTINGS) {
        if (btn == true) {
            if (pnt_in_rect(x, y, 100, 150, 40, 40)) {
                al_stop_sample(&song_id);
                song = main_bgm;
                song_id = main_bgm_id;
                if (!al_play_sample(song, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &song_id))
                    game_abort("??failed to play audio (bgm)"), game_log("0099failed to play audio (bgm)");
            }
            else if (pnt_in_rect(x, y, 300, 150, 40, 40)) {
                al_stop_sample(&song_id);
                song = start_bgm;
                song_id = start_bgm_id;
                if (!al_play_sample(song, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &song_id))
                    game_abort("??failed to play audio (bgm)???"), game_log("0099failed to play audio (bgm)");
            }
            else if (pnt_in_rect(x, y, 500, 150, 40, 40)) {
                al_stop_sample(&song_id);
                song = night_bgm;
                song_id = night_bgm_id;
                if (!al_play_sample(song, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &song_id))
                    game_abort("??failed to play audio (bgm)???"), game_log("0099failed to play audio (bgm)");
            }

            if (pnt_in_rect(mouse_x, mouse_y, 100, 350, 50, 50)) {
                start_img_plane = ghost1;
                if (!al_play_sample(select_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &select_bgm_id))
                    game_abort("??failed to play audio (bgm)???"), game_log("0099failed to play audio (bgm)");
            }
            else if (pnt_in_rect(mouse_x, mouse_y, 300, 350, 40, 40)) {
                start_img_plane = ghost2;
                if (!al_play_sample(select_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &select_bgm_id))
                    game_abort("??failed to play audio (bgm)???"), game_log("0099failed to play audio (bgm)");
            }
            else if (pnt_in_rect(mouse_x, mouse_y, 500, 350, 40, 40)) {
                start_img_plane = ghost3;
                if (!al_play_sample(select_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &select_bgm_id))
                    game_abort("??failed to play audio (bgm)???"), game_log("0099failed to play audio (bgm)");
            }
        }
    }
    else if (active_scene == SCENE_STORE) {
        if (btn == true) {
            //buy
            if (pnt_in_rect(x, y, 100, 150, 30, 30) && hav[1]==0 && money-30 >=0) {
                if (!al_play_sample(buy, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &buy_id))
                    game_abort("??failed to play audio (bgm)"), game_log("0099failed to play audio (bgm)");
                hav[1] = true;
                money -= 30;
            }
            else if (pnt_in_rect(x, y, 320, 150, 20, 30) && hav[2] == 0 && money - 20 >= 0) {
                if (!al_play_sample(buy, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &buy_id))
                    game_abort("??failed to play audio (bgm)???"), game_log("0099failed to play audio (bgm)");
                hav[2] = true;
                money -= 20;
            }
            else if (pnt_in_rect(x, y, 550, 150, 30, 30) && hav[3] == 0  && money - 20 >= 0) {
                if (!al_play_sample(buy, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &buy_id))
                    game_abort("??failed to play audio (bgm)???"), game_log("0099failed to play audio (bgm)");
                hav[3] = true;
                money -= 20;
            }
            //choose the bullet
            int j = 0;
            int temp1 = 0;
            float temp2=0;
            for (int i = 0; i < 4; i++) {
                if (i == 0)img_bullet_bag = img_bullet_0 , temp1=1 ,temp2=0.2;
                else if (i == 1)img_bullet_bag = img_bullet_1, temp1=2.5, temp2 = 0.2;
                else if (i == 2)img_bullet_bag = img_bullet_2, temp1=2, temp2 = 0.2;
                else if (i == 3)img_bullet_bag = img_bullet_3, temp1=1, temp2 = 0.1;
                if (hav[i] == 1) {
                    if (pnt_in_rect(mouse_x, mouse_y, 100 + 200 * j, 350, 50, 50)) {
                        kill = temp1;
                        MAX_COOLDOWN = temp2;
                        img_bullet = img_bullet_bag;
                        if (!al_play_sample(select_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &select_bgm_id))
                            game_abort("??failed to play audio (bgm)???"), game_log("0099failed to play audio (bgm)");
                    }
                    j++;
                }
            }
        }
    }
}

void draw_movable_object(MovableObject obj) {
    if (obj.hidden)
        return;
    al_draw_bitmap(obj.img, round(obj.x - obj.w / 2), round(obj.y - obj.h / 2), 0);
}

ALLEGRO_BITMAP *load_bitmap_resized(const char *filename, int w, int h) {
    ALLEGRO_BITMAP* loaded_bmp = al_load_bitmap(filename);
    if (!loaded_bmp)
        game_abort("failed to load image: %s", filename);
    ALLEGRO_BITMAP *resized_bmp = al_create_bitmap(w, h);
    ALLEGRO_BITMAP *prev_target = al_get_target_bitmap();

    if (!resized_bmp)
        game_abort("failed to create bitmap when creating resized image: %s", filename);
    al_set_target_bitmap(resized_bmp);
    al_draw_scaled_bitmap(loaded_bmp, 0, 0,
        al_get_bitmap_width(loaded_bmp),
        al_get_bitmap_height(loaded_bmp),
        0, 0, w, h, 0);
    al_set_target_bitmap(prev_target);
    al_destroy_bitmap(loaded_bmp);

    game_log("resized image: %s", filename);

    return resized_bmp;
}

// Define bool pnt_in_rect(int px, int py, int x, int y, int w, int h)
bool pnt_in_rect(int px, int py, int x, int y, int w, int h) {
    if (px <= (x + w) && px >=(x - w) && py <= (y + h) && py >=(y - h))
        return true;
    else return false;
}

void take() {
    while (!feof(pfile)) {	//whether it is the end of the file
        fscanf(pfile, "%d,%d,%d,%d,%d,%d", &highscore,&money, &hav[0], &hav[1], &hav[2], &hav[3]);
    }
}


// +=================================================================+
// | Code below is for debugging purpose, it's fine to remove it.    |
// | Deleting the code below and removing all calls to the functions |
// | doesn't affect the game.                                        |
// +=================================================================+

void game_abort(const char* format, ...) {
    va_list arg;
    va_start(arg, format);
    game_vlog(format, arg);
    va_end(arg);
    fprintf(stderr, "error occured, exiting after 2 secs");
    // Wait 2 secs before exiting.
    al_rest(2);
    // Force exit program.
    exit(1);
}

void game_log(const char* format, ...) {
#ifdef LOG_ENABLED
    va_list arg;
    va_start(arg, format);
    game_vlog(format, arg);
    va_end(arg);
#endif
}

void game_vlog(const char* format, va_list arg) {
#ifdef LOG_ENABLED
    static bool clear_file = true;
    vprintf(format, arg);
    printf("\n");
    // Write log to file for later debugging.
    FILE* pFile = fopen("log.txt", clear_file ? "w" : "a");
    if (pFile) {
        vfprintf(pFile, format, arg);
        fprintf(pFile, "\n");
        fclose(pFile);
    }
    clear_file = false;
#endif
}
