#if !defined(__BEZIER_H__)
#define __BEZIER_H__

typedef struct POINT_T {
    int x,y;
} POINT_T;
typedef struct BEZIER_CONTROLS_T {
    POINT_T P[3];
} BEZIER_CONTROLS_T;
typedef struct BEZIER_DIGIT_T {
    POINT_T             start;
    BEZIER_CONTROLS_T   controls[4];
} BEZIER_DIGIT_T;

POINT_T interp(POINT_T s, POINT_T d, int i);
BEZIER_DIGIT_T get_digit(int i);

void draw_bezier_digit(GContext* ctx, int xoffset, int yoffset, BEZIER_DIGIT_T d);
#endif // __BEZIER_H__
