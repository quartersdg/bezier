#include <pebble.h>
#include "bezier.h"

BEZIER_DIGIT_T bezier_digits[] = {
    //case 0: 
    {
        { 254, 47 },
        {
            {{{159, 84}, {123, 158}, {131, 258}}},
            {{{139, 358}, {167, 445}, {256, 446}}},
            {{{345, 447}, {369, 349}, {369, 275}}},
            {{{369, 201}, {365, 81}, {231, 75}}}
        },
    },
    //      case 1:
    {
        { 138, 180 },
        {
            {{{226, 99}, {230, 58}, {243, 43}}},
            {{{256, 28}, {252, 100}, {253, 167}}},
            {{{254, 234}, {254, 194}, {255, 303}}},
            {{{256, 412}, {254, 361}, {255, 424}}}
        },
    },
    //      case 2:
    {
        { 104, 111 },
        {
            {{{152, 55}, {208, 26}, {271, 50}}},
            {{{334, 74}, {360, 159}, {336, 241}}},
            {{{312, 323}, {136, 454}, {120, 405}}},
            {{{104, 356}, {327, 393}, {373, 414}}}
        },
    },
    //      case 3:
    {
        { 96, 132 },
        {
            {{{113, 14}, {267, 17}, {311, 107}}},
            {{{355, 197}, {190, 285}, {182, 250}}},
            {{{174, 215}, {396, 273}, {338, 388}}},
            {{{280, 503}, {110, 445}, {93, 391}}}
        },
    },
    //      case 4:
    {
        { 374, 244 },
        {
            {{{249, 230}, {192, 234}, {131, 239}}},
            {{{70, 244}, {142, 138}, {192, 84}}},
            {{{242, 30}, {283, 30}, {260, 108}}},
            {{{237, 246}, {246, 435}, {247, 438}}}
        },
    },
    //      case 5:
    {
        { 340, 52 },
        {
            {{{226, 42}, {153, 44}, {144, 61}}},
            {{{135, 78}, {145, 203}, {152, 223}}},
            {{{159, 243}, {351, 165}, {361, 302}}},
            {{{371, 439}, {262, 452}, {147, 409}}}
        },
    },
    //      case 6:
    {
        { 301, 26 },
        {
            {{{191, 104}, {160, 224}, {149, 296}}},
            {{{138, 368}, {163, 451}, {242, 458}}},
            {{{321, 465}, {367, 402}, {348, 321}}},
            {{{329, 240}, {220, 243}, {168, 285}}}
        },
    },
    //      case 7:
    {
        { 108, 52 },
        {
            {{{168, 34}, {245, 42}, {312, 38}}},
            {{{379, 34}, {305, 145}, {294, 166}}},
            {{{283, 187}, {243, 267}, {231, 295}}},
            {{{219, 323}, {200, 388}, {198, 452}}}
        },
    },
    //      case 8:
    {
        { 243, 242 },
        {
            {{{336, 184}, {353, 52}, {240, 43}}},
            {{{127, 34}, {143, 215}, {225, 247}}},
            {{{307, 279}, {403, 427}, {248, 432}}},
            {{{93, 437}, {124, 304}, {217, 255}}}
        },
    },
    //      case 9:
    {
        { 322, 105 },
        {
            {{{323, 6}, {171, 33}, {151, 85}}},
            {{{131, 137}, {161, 184}, {219, 190}}},
            {{{277, 196}, {346, 149}, {322, 122}}},
            {{{298, 95}, {297, 365}, {297, 448}}}
        },
    },
};

POINT_T interp(POINT_T s, POINT_T d, int i) {
    POINT_T r;

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
            r.controls[i].P[j] = interp(s.controls[i].P[j],d.controls[i].P[j],percent);
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

#define SCALE_FACTOR 8

void draw_cubic_bezier(GContext* ctx, int xoffset, int yoffset, POINT_T p0, POINT_T p1, POINT_T p2, POINT_T p3, int depth) {
    if (depth > 0) {
        POINT_T m1 = { (p0.x + p1.x) / 2, (p0.y + p1.y) / 2 };
        POINT_T m2 = { (p2.x + p3.x) / 2, (p2.y + p3.y) / 2 };
        POINT_T q1 = { (p0.x + p1.x * 2 + p2.x) / 4, (p0.y + p1.y * 2 + p3.y) / 4 };
        POINT_T q2 = { (p1.x + p2.x * 2 + p3.x) / 4, (p1.y + p2.y * 2 + p3.y) / 4 };
        POINT_T q3 = { (p0.x + p1.x * 3 + p2.x * 3 + p3.x) / 8, (p0.y + p1.y * 3 + p2.y * 3 + p3.y) / 8 };
        draw_cubic_bezier(ctx, xoffset, yoffset, p0, m1, q1, q3,depth-1);    // Draw "left" part
        draw_cubic_bezier(ctx, xoffset, yoffset, q3, q2, m2, p3,depth-1);    // Draw "right" part
    }
    else {
        //app_log(APP_LOG_LEVEL_INFO,__FILE__,__LINE__,"DRAW_LINE: <%d,%d> - <%d,%d>\n", p0.x, p0.y, p3.x, p3.y);
        //APP_LOG(APP_LOG_LEVEL_DEBUG, "DRAW_LINE: <%d,%d> - <%d,%d>\n", p0.x, p0.y, p3.x, p3.y);
        draw_fat_line(ctx,
                (GPoint){xoffset + p0.x/SCALE_FACTOR, yoffset + p0.y/SCALE_FACTOR},
                (GPoint){xoffset + p3.x/SCALE_FACTOR,yoffset + p3.y/SCALE_FACTOR});
    }
}

void draw_bezier_digit(GContext* ctx, int xoffset, int yoffset, BEZIER_DIGIT_T d) {
    int i;
    POINT_T s;
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

static void draw_digit(GContext* ctx, int xoffset, int yoffset, int prev, int cur) {
    BEZIER_DIGIT_T d,t;
    d = get_bezier_digit(cur);
    if (prev != cur) {
        t = d;
        d = get_bezier_digit(prev);
        d = interp_digit(d,t,step);
    }
    draw_bezier_digit(ctx,xoffset,yoffset,d);
}

static void draw_time(GContext* ctx, int hour, int min) {

    draw_digit(ctx,0,0,prev_hour_tens,hour/10);
    if (step == 100) prev_hour_tens = hour/10;
    draw_digit(ctx,display_bounds.size.w/2,0,prev_hour_ones,hour%10);
    if (step == 100) prev_hour_ones = hour%10;
    draw_digit(ctx,0,display_bounds.size.h/2,prev_min_tens,min/10);
    if (step == 100) prev_min_tens = min/10;
    draw_digit(ctx,display_bounds.size.w/2,display_bounds.size.h/2,prev_min_ones,min%10);
    if (step == 100) prev_min_ones = min%10;
}

unsigned short get_display_hour(unsigned short hour) {

    if (clock_is_24h_style()) {
        return hour;
    }

    unsigned short display_hour = hour % 12;

    // Converts "0" to "12"
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

    //GBitmap* bmp = graphics_capture_frame_buffer(ctx);
    graphics_context_set_stroke_color(ctx,GColorBlack);
    draw_time(ctx,display_hour,t->tm_min);

    //graphics_release_frame_buffer(ctx,bmp); 
}

void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed) {
    draw_timer = app_timer_register(0,draw_time_timer,NULL);
}

static void window_load(Window *window) {
    window_set_background_color(window, GColorWhite);

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

    APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

    app_event_loop();
    deinit();
}
