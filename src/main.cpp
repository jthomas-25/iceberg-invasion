#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_native_dialog.h>
#include "level.h"


const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 680;
const int FPS = 60;

// initializes Allegro addons and installs the keyboard
static bool allegro_init();

// creates all required Allegro objects
static bool setup(ALLEGRO_DISPLAY*& display, ALLEGRO_EVENT_QUEUE*& event_queue, ALLEGRO_TIMER*& clock);

// loads all sprite images exactly once from disk and stores them
static int load_all_sprite_graphics();

// callback function passed to al_for_each_fs_entry
static int load_image(ALLEGRO_FS_ENTRY* filename, void* extra);

// destroys all Allegro objects
static void clean_up(ALLEGRO_DISPLAY* display, ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_TIMER* clock);


int main(int argc, char** argv)
{
    bool done = false;
    bool redraw = false;
    
    bool keys[ALLEGRO_KEY_MAX]; // key states
    // keyboard controls
    keys[ALLEGRO_KEY_LEFT] = false;
    keys[ALLEGRO_KEY_RIGHT] = false;
    keys[ALLEGRO_KEY_SPACE] = false;

    ALLEGRO_DISPLAY* display = nullptr;
    ALLEGRO_EVENT_QUEUE* event_queue = nullptr;
    ALLEGRO_TIMER* clock = nullptr; // runs the game at the given framerate

    if (!setup(display, event_queue, clock)) {
        clean_up(display, event_queue, clock);
        return -1;
    }

    Level level(SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!level.load()) {
        al_show_native_message_box(display, "Error!", "Failed to load.", 0, 0, ALLEGRO_MESSAGEBOX_ERROR);
        clean_up(display, event_queue, clock);
        return -1;
    }

    srand(time(nullptr)); // init RNG

    al_start_timer(clock);
    while (!done) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        level.getEvent(ev);
        
        // check if the close button of the window was clicked
        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            done = true;
        }
        // check if a key was pressed
        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (ev.keyboard.keycode) {
            case ALLEGRO_KEY_ESCAPE:
                done = true;
                break;
            case ALLEGRO_KEY_LEFT:
                keys[ALLEGRO_KEY_LEFT] = true;
                break;
            case ALLEGRO_KEY_RIGHT:
                keys[ALLEGRO_KEY_RIGHT] = true;
                break;
            case ALLEGRO_KEY_SPACE:
                keys[ALLEGRO_KEY_SPACE] = true;
                break;
            }
        }
        // check if a key was released
        else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
            switch (ev.keyboard.keycode) {
            case ALLEGRO_KEY_ESCAPE:
                done = true;
                break;
            case ALLEGRO_KEY_LEFT:
                keys[ALLEGRO_KEY_LEFT] = false;
                break;
            case ALLEGRO_KEY_RIGHT:
                keys[ALLEGRO_KEY_RIGHT] = false;
                break;
            case ALLEGRO_KEY_SPACE:
                keys[ALLEGRO_KEY_SPACE] = false;
                break;
            }
        }
        else if (ev.type == ALLEGRO_EVENT_TIMER) {
            redraw = true;
            level.update(keys, done);
        }

        if (redraw && al_is_event_queue_empty(event_queue)) {
            redraw = false;
            level.draw();
            al_flip_display();
            al_clear_to_color(colors::BLACK);
        }
    }
    clean_up(display, event_queue, clock);

    return 0;
}

static bool allegro_init() {
    if (!al_init()) {
        al_show_native_message_box(nullptr, "Error!", "Failed to initialize Allegro.", 0, 0, ALLEGRO_MESSAGEBOX_ERROR);
        return false;
    }
    
    if (!al_install_keyboard()) {
        al_show_native_message_box(nullptr, "Error!", "Failed to install the keyboard.", 0, 0, ALLEGRO_MESSAGEBOX_ERROR);
        return false;
    }
    
    if (!al_init_image_addon()) {
        al_show_native_message_box(nullptr, "Error!", "Failed to initialize the image addon.", 0, 0, ALLEGRO_MESSAGEBOX_ERROR);
        return false;
    }

    al_init_font_addon();
    al_init_ttf_addon();

    al_init_primitives_addon();

    return true;
}

static bool setup(ALLEGRO_DISPLAY*& display, ALLEGRO_EVENT_QUEUE*& event_queue, ALLEGRO_TIMER*& clock) {
    if (!allegro_init()) {
        return false;
    }

    display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!display) {
        al_show_native_message_box(nullptr, "Error!", "Failed to create display.", 0, 0, ALLEGRO_MESSAGEBOX_ERROR);
        return false;
    }

    al_set_window_title(display, "Iceberg Invasion");

    event_queue = al_create_event_queue();
    if (!event_queue) {
        al_show_native_message_box(display, "Error!", "Failed to create event queue.", 0, 0, ALLEGRO_MESSAGEBOX_ERROR);
        return false;
    }

    clock = al_create_timer(1.0 / FPS);
    if (!clock) {
        al_show_native_message_box(display, "Error!", "Failed to create FPS timer.", 0, 0, ALLEGRO_MESSAGEBOX_ERROR);
        return false;
    }

    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(clock));

    int result = load_all_sprite_graphics();
    if (result == ALLEGRO_FOR_EACH_FS_ENTRY_ERROR) {
        al_show_native_message_box(display, "Error!", "Failed to load a sprite image.", 0, 0, ALLEGRO_MESSAGEBOX_ERROR);
        return false;
    }

    return true;
}

static int load_image(ALLEGRO_FS_ENTRY* entry, void* extra) {
    (void)extra;
    // absolute path
    ALLEGRO_PATH* filePath = al_create_path(al_get_fs_entry_name(entry));
    ALLEGRO_BITMAP* image = al_load_bitmap(al_path_cstr(filePath, ALLEGRO_NATIVE_PATH_SEP));
    if (!image) {
        al_destroy_path(filePath);
        return ALLEGRO_FOR_EACH_FS_ENTRY_ERROR;
    }
    al_convert_mask_to_alpha(image, colors::MAGENTA); // set transparent color
    std::string filename(al_get_path_filename(filePath));
    sprite::Sprite::graphics.insert(std::make_pair(filename, image));
    image = nullptr; // no dangling pointers...
    al_destroy_path(filePath);
    return ALLEGRO_FOR_EACH_FS_ENTRY_OK;
}

static int load_all_sprite_graphics() {
    //https://liballeg.org/a5docs/trunk/fshook.html#al_for_each_fs_entry
    ALLEGRO_FS_ENTRY* directory = al_create_fs_entry("assets/sprites");
    int result = al_for_each_fs_entry(directory, load_image, (void*)al_get_fs_entry_name(directory));
    al_destroy_fs_entry(directory);
    return result;
}

static void clean_up(ALLEGRO_DISPLAY* display, ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_TIMER* clock) {
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    al_destroy_timer(clock);
    for (auto& pair : sprite::Sprite::graphics) {
        al_destroy_bitmap(pair.second);
    }
}
