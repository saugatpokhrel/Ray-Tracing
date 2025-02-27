#include <stdio.h>
#include <SDL3/SDL.h>
#include <math.h>
#define WIDTH 1200
#define HEIGHT 600
#define COLOR_WHITE 0xFFFFFFFF
#define COLOR_BLACK 0x00000000
#define COLOR_GREY 0x80808080
#define COLOR_RAY 0xffd43b
#define RAYS_NUMBER 500
#define RAY_THICKNESS 5
#define M_PI 3.14159265358979323846

struct Circle {
    double x;
    double y;
    double radius;
};

struct Ray {
    double x_start;
    double y_start;
    double angle;
};

int is_point_inside_circle(double x, double y, struct Circle circle) {
    return ((x - circle.x) * (x - circle.x) + (y - circle.y) * (y - circle.y)) <= (circle.radius * circle.radius);
}

void Fillcircle(SDL_Surface *surface, struct Circle circle, Uint32 color) {
    for (double x = circle.x - circle.radius; x < circle.x + circle.radius; x++) {
        for (double y = circle.y - circle.radius; y < circle.y + circle.radius; y++) {
            double dx = x - circle.x;
            double dy = y - circle.y;
            double distance_squared = dx * dx + dy * dy;
            if (distance_squared < circle.radius * circle.radius) {
                SDL_Rect ray_point = {(int)x, (int)y, RAY_THICKNESS, RAY_THICKNESS};
                int blur_size = (int)(1.5 * RAY_THICKNESS);
                SDL_Rect ray_blur = {(int)x, (int)y, blur_size, blur_size};
                SDL_FillSurfaceRect(surface, &ray_blur, color);
                SDL_FillSurfaceRect(surface, &ray_point, color);
            }
        }
    }
}

// Draws the shadow circle with a radial gradient to give it a softer look.
void DrawShadow(SDL_Surface *surface, struct Circle circle) {
    int steps = 30; // More steps means a smoother gradient.
    for (int i = 0; i < steps; i++) {
        double t = (double)i / steps;  // t ranges from 0 (edge) to 1 (center)
        // Interpolate between a lighter grey (edge) and a dark grey (center)
        Uint8 outer_val = 0xAA;
        Uint8 inner_val = 0x33;
        Uint8 value = (Uint8)((1 - t) * outer_val + t * inner_val);
        Uint32 shadow_color = (0xFF << 24) | (value << 16) | (value << 8) | value;
        struct Circle temp;
        temp.x = circle.x;
        temp.y = circle.y;
        temp.radius = circle.radius * (1 - (double)i / steps);
        Fillcircle(surface, temp, shadow_color);
    }
}

void generate_rays(struct Circle circle, struct Ray rays[RAYS_NUMBER]) {
    for (int i = 0; i < RAYS_NUMBER; i++) {
        double angle = ((double)i / RAYS_NUMBER) * 2 * M_PI;
        struct Ray ray = {circle.x, circle.y, angle};
        rays[i] = ray;
    }
}

void FillRays(SDL_Surface *surface, struct Ray rays[RAYS_NUMBER], Uint32 color, struct Circle obstacle) {
    for (int i = 0; i < RAYS_NUMBER; i++) {
        struct Ray ray = rays[i];
        double step = 1;
        double x_draw = ray.x_start;
        double y_draw = ray.y_start;

        int end_of_screen = 0;
        int object_hit = 0;

        while (!end_of_screen && !object_hit) {
            x_draw += step * cos(ray.angle);
            y_draw += step * sin(ray.angle);

            if (x_draw < 0 || x_draw >= WIDTH || y_draw < 0 || y_draw >= HEIGHT) {
                end_of_screen = 1;
                break;
            }

            SDL_Rect pixel = {(int)x_draw, (int)y_draw, 1, 1};
            SDL_FillSurfaceRect(surface, &pixel, color);

            if (is_point_inside_circle(x_draw, y_draw, obstacle)) {
                object_hit = 1;
            }
        }
    }
}

// Draws an outline around a circle for an extra finishing touch.
void DrawCircleOutline(SDL_Surface *surface, struct Circle circle, Uint32 color) {
    for (int i = 0; i < 360; i++) {
         double angle = i * M_PI / 180.0;
         int x = (int)(circle.x + circle.radius * cos(angle));
         int y = (int)(circle.y + circle.radius * sin(angle));
         SDL_Rect pixel = {x, y, 1, 1};
         SDL_FillSurfaceRect(surface, &pixel, color);
    }
}

// Moves the shadow circle horizontally and adds a sine-based vertical movement.
void move_shadow_circle(struct Circle *shadow_circle) {
    shadow_circle->x += 1;
    shadow_circle->y = 300 + 50 * sin(shadow_circle->x * 0.05);
    if (shadow_circle->x - shadow_circle->radius > WIDTH) {
        shadow_circle->x = -shadow_circle->radius;
    }
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Ray Tracing By Saugat Pokhrel", WIDTH, HEIGHT, 0);
    SDL_Surface *surface = SDL_GetWindowSurface(window);
    
    struct Circle circle = {200, 200, 80};
    struct Circle shadow_circle = {650, 300, 140};
    SDL_Rect erase_rect = {0, 0, WIDTH, HEIGHT};
    
    int simulation_running = 1;
    SDL_Event event;
    struct Ray rays[RAYS_NUMBER];
    generate_rays(circle, rays);

    while (simulation_running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                simulation_running = 0;
            }
            if (event.type == SDL_EVENT_MOUSE_MOTION && event.motion.state != 0) {
                circle.x = event.motion.x;
                circle.y = event.motion.y;
                generate_rays(circle, rays);
            }
        }
        
        SDL_FillSurfaceRect(surface, &erase_rect, COLOR_BLACK);
        // Draw the light source with an outline.
        Fillcircle(surface, circle, COLOR_WHITE);
        DrawCircleOutline(surface, circle, COLOR_GREY);
        // Draw the improved shadow with a radial gradient.
        DrawShadow(surface, shadow_circle);
        // Render the rays, which stop when they hit the shadow.
        FillRays(surface, rays, COLOR_RAY, shadow_circle);
        move_shadow_circle(&shadow_circle);
        
        SDL_UpdateWindowSurface(window);
        SDL_Delay(10);
    }
    
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
