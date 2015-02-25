#include <pebble.h>

typedef struct BEZIER_CONTROLS_T {
    GPoint P[3];
} BEZIER_CONTROLS_T;
typedef struct BEZIER_DIGIT_T {
    GPoint             start;
    BEZIER_CONTROLS_T   controls[4];
} BEZIER_DIGIT_T;

GPoint interp(GPoint s, GPoint d, int i);
BEZIER_DIGIT_T get_digit(int i);

void draw_bezier_digit(GContext* ctx, int xoffset, int yoffset, BEZIER_DIGIT_T d);

static int g_invert = 0;

BEZIER_DIGIT_T bezier_digits[] = {
    //case 0:
    {
        { 131, 0 },
        {
            { { { 36, 37 }, { 0, 111 }, { 8, 211 }, } },
            { { { 16, 311 }, { 44, 398 }, { 133, 399 }, } },
            { { { 222, 400 }, { 246, 302 }, { 246, 228 }, } },
            { { { 246, 154 }, { 242, 34 }, { 108, 28 }, } },
        }
    },
    //case 1:
    {
        { 0, 152 },
        {
            { { { 88, 71 }, { 92, 30 }, { 105, 15 }, } },
            { { { 118, 0 }, { 114, 72 }, { 115, 139 }, } },
            { { { 116, 206 }, { 116, 166 }, { 117, 275 }, } },
            { { { 118, 384 }, { 116, 333 }, { 117, 396 }, } },
        }
    },
    //case 2:
    {
        { 0, 85 },
            {
                { { { 48, 29 }, { 104, 0 }, { 167, 24 }, } },
                { { { 230, 48 }, { 256, 133 }, { 232, 215 }, } },
                { { { 208, 297 }, { 32, 428 }, { 16, 379 }, } },
                { { { 0, 330 }, { 223, 367 }, { 269, 388 }, } },
            }
    },
    //case 3:
    {
        { 3, 118 },
            {
                { { { 20, 0 }, { 174, 3 }, { 218, 93 }, } },
                { { { 262, 183 }, { 97, 271 }, { 89, 236 }, } },
                { { { 81, 201 }, { 303, 259 }, { 245, 374 }, } },
                { { { 187, 489 }, { 17, 431 }, { 0, 377 }, } },
            }
    },
    //case 4:
    {
        { 304, 214 },
            {
                { { { 179, 200 }, { 122, 204 }, { 61, 209 }, } },
                { { { 0, 214 }, { 72, 108 }, { 122, 54 }, } },
                { { { 172, 0 }, { 213, 0 }, { 190, 78 }, } },
                { { { 167, 216 }, { 176, 405 }, { 177, 408 }, } },
            }
    },
    //case 5:
    {
        { 205, 10 },
            {
                { { { 91, 0 }, { 18, 2 }, { 9, 19 }, } },
                { { { 0, 36 }, { 10, 161 }, { 17, 181 }, } },
                { { { 24, 201 }, { 216, 123 }, { 226, 260 }, } },
                { { { 236, 397 }, { 127, 410 }, { 12, 367 }, } },
            }
    },
    //case 6:
    {
        { 163, 0 },
            {
                { { { 53, 78 }, { 22, 198 }, { 11, 270 }, } },
                { { { 0, 342 }, { 25, 425 }, { 104, 432 }, } },
                { { { 183, 439 }, { 229, 376 }, { 210, 295 }, } },
                { { { 191, 214 }, { 82, 217 }, { 30, 259 }, } },
            }
    },
    //case 7:
    {
        { 0, 18 },
            {
                { { { 60, 0 }, { 137, 8 }, { 204, 4 }, } },
                { { { 271, 0 }, { 197, 111 }, { 186, 132 }, } },
                { { { 175, 153 }, { 135, 233 }, { 123, 261 }, } },
                { { { 111, 289 }, { 92, 354 }, { 90, 418 }, } },
            }
    },
    //case 8:
    {
        { 150, 208 },
            {
                { { { 243, 150 }, { 260, 18 }, { 147, 9 }, } },
                { { { 34, 0 }, { 50, 181 }, { 132, 213 }, } },
                { { { 214, 245 }, { 310, 393 }, { 155, 398 }, } },
                { { { 0, 403 }, { 31, 270 }, { 124, 221 }, } },
            }
    },
    //case 9:
    {
        { 191, 99 },
            {
                { { { 192, 0 }, { 40, 27 }, { 20, 79 }, } },
                { { { 0, 131 }, { 30, 178 }, { 88, 184 }, } },
                { { { 146, 190 }, { 215, 143 }, { 191, 116 }, } },
                { { { 167, 89 }, { 166, 359 }, { 166, 442 }, } },
            }
    },
};

GPoint interp(GPoint s, GPoint d, int i) {
    GPoint r;

    r.x = ((d.x*i)+(s.x*(100-i))) / 100;
    r.y = ((d.y*i)+(s.y*(100-i))) / 100;

    return r;
}

BEZIER_DIGIT_T interp_digit(BEZIER_DIGIT_T s, BEZIER_DIGIT_T d, int percent) {
    BEZIER_DIGIT_T r;
    int i,j;

    r.start = interp(s.start,d.start,percent);
    for (i=0;i<4;i++) {
        for (j=0;j<3;j++) {
            r.controls[i].P[j] = interp(
                                    s.controls[i].P[j],
                                    d.controls[i].P[j],
                                    percent);
        }
    }
    return r;
}

BEZIER_DIGIT_T get_bezier_digit(int i) {
    return bezier_digits[i];
}

static void draw_fat_line(GContext* ctx, GPoint p0, GPoint p1) {
    graphics_draw_line(ctx,p0,p1);
    graphics_draw_line(ctx,(GPoint){p0.x-1,p0.y},(GPoint){p1.x-1,p1.y});
    graphics_draw_line(ctx,(GPoint){p0.x+1,p0.y},(GPoint){p1.x+1,p1.y});
}

static int SCALE_FACTOR = 6;

void draw_cubic_bezier(GContext* ctx, int xoffset, int yoffset, GPoint p0, GPoint p1, GPoint p2, GPoint p3, int depth) {
    if (depth > 0) {
        GPoint m1 = { (p0.x + p1.x) / 2, (p0.y + p1.y) / 2 };
        GPoint m2 = { (p2.x + p3.x) / 2, (p2.y + p3.y) / 2 };
        GPoint q1 = { (p0.x + p1.x * 2 + p2.x) / 4,
                       (p0.y + p1.y * 2 + p3.y) / 4 };
        GPoint q2 = { (p1.x + p2.x * 2 + p3.x) / 4,
                       (p1.y + p2.y * 2 + p3.y) / 4 };
        GPoint q3 = { (p0.x + p1.x * 3 + p2.x * 3 + p3.x) / 8,
                       (p0.y + p1.y * 3 + p2.y * 3 + p3.y) / 8 };
        // Draw "left" part
        draw_cubic_bezier(ctx, xoffset, yoffset, p0, m1, q1, q3,depth-1);
        // Draw "right" part
        draw_cubic_bezier(ctx, xoffset, yoffset, q3, q2, m2, p3,depth-1);
    }
    else {
        draw_fat_line(ctx,
                (GPoint){xoffset + p0.x/SCALE_FACTOR,
                         yoffset + p0.y/SCALE_FACTOR},
                (GPoint){xoffset + p3.x/SCALE_FACTOR,
                         yoffset + p3.y/SCALE_FACTOR});
    }
}

void draw_bezier_digit(
        GContext* ctx,
        int xoffset,
        int yoffset,
        BEZIER_DIGIT_T d
        ) {
    int i;
    GPoint s;
    s = d.start;
    for (i=0;i<4;i++) {
        BEZIER_CONTROLS_T* c;
        c = &d.controls[i];
        draw_cubic_bezier(ctx,xoffset,yoffset,s,c->P[0],c->P[1],c->P[2],4);
        s = c->P[2];
    }
};


static Window *window;
static Layer *display_layer;
static GRect display_bounds;
static int step = 0;
static int prev_hour_tens = 0;
static int prev_hour_ones = 0;
static int prev_min_tens = 0;
static int prev_min_ones = 0;

static void draw_digit(
        GContext* ctx,
        int xoffset,
        int yoffset,
        int prev,
        int cur
        ) {
    BEZIER_DIGIT_T d,t;
    d = get_bezier_digit(cur);
    if (prev != cur) {
        t = d;
        d = get_bezier_digit(prev);
        d = interp_digit(d,t,step);
    }
    draw_bezier_digit(ctx,xoffset,yoffset,d);
}

static const int xoffset = 20;
static const int xwidth = 60;
static const int xsecwidth = 30;
static const int yoffset = 5;
static const int yheight = 70;

static void draw_time(GContext* ctx, int hour, int min) {

    draw_digit(ctx,xoffset,yoffset,prev_hour_tens,hour/10);
    if (step == 100) prev_hour_tens = hour/10;
    draw_digit(ctx,xoffset+xwidth,yoffset,prev_hour_ones,hour%10);
    if (step == 100) prev_hour_ones = hour%10;
    draw_digit(ctx,xoffset,yoffset+yheight,prev_min_tens,min/10);
    if (step == 100) prev_min_tens = min/10;
    draw_digit(ctx,
        xoffset+xwidth,
        yoffset+yheight,
        prev_min_ones,min%10);
    if (step == 100) prev_min_ones = min%10;
}

unsigned short get_display_hour(unsigned short hour) {

    if (clock_is_24h_style()) {
        return hour;
    }

    unsigned short display_hour = hour % 12;

    return display_hour ? display_hour : 12;

}

static AppTimer* draw_timer;

static void draw_time_timer(void* context) {
    step += 10;
    if (step <= 100)
    {
        draw_timer = app_timer_register(100,draw_time_timer,NULL);
        layer_mark_dirty(display_layer);
    }
    else
    {
        step = 0;
    }
}

void display_layer_update_callback(Layer *me, GContext* ctx) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    unsigned short display_hour = get_display_hour(t->tm_hour);

    if (!g_invert) {
        window_set_background_color(window, GColorWhite);
        graphics_context_set_stroke_color(ctx,GColorBlack);
    } else {
        graphics_context_set_stroke_color(ctx,GColorWhite);
        window_set_background_color(window, GColorBlack);
    }
    draw_time(ctx,display_hour,t->tm_min);
}

void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed) {
    draw_timer = app_timer_register(0,draw_time_timer,NULL);
}

static void window_load(Window *window) {
    if (!g_invert) {
        window_set_background_color(window, GColorWhite);
    } else {
        window_set_background_color(window, GColorBlack);
    }

    Layer *root_layer = window_get_root_layer(window);
    GRect frame = layer_get_frame(root_layer);

    // Init the layer for the display
    display_layer = layer_create(frame);
    display_bounds = layer_get_bounds(display_layer);

    layer_set_update_proc(display_layer, &display_layer_update_callback);
    layer_add_child(root_layer, display_layer);

    tick_timer_service_subscribe(MINUTE_UNIT , &handle_minute_tick);
}

static void window_unload(Window *window) {
    layer_destroy(display_layer);
}

static void init(void) {
    window = window_create();
    window_set_window_handlers(window, (WindowHandlers) {
            .load = window_load,
            .unload = window_unload,
            });
    const bool animated = true;
    window_stack_push(window, animated);
}

static void deinit(void) {
    window_destroy(window);
}

int main(void) {

    init();

    APP_LOG(APP_LOG_LEVEL_DEBUG,
        "Done initializing, pushed window: %p", window);

    app_event_loop();
    deinit();
}
