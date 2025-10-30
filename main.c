#include "raylib.h"
#include "raymath.h"
#include <math.h>
typedef struct Ball {
    Vector2 pos;
    float radius;
    Color color;
    Vector2 ballSpeed;
    bool active;
}Ball;

typedef struct Bar {
    Vector2 startPos;
    Vector2 endPos;
}Bar;

typedef struct Bucket {
    Vector2 startPos;
    Vector2 endPos;
    Color color;
    float thickness;
    int ballsConsumed;
    bool active;
}Bucket;


float degreeToRad(float degree) {
    float rad = degree * PI / 180.0f;
    return rad;
}


int main(void)
{
    int screenWidth = 800;
    int screenHeight = 600;
    float rotationAngle = 0.0f;
    int numOfBuckets = 3;
    int numOfBalls = 10;
    int numOfBars = 4;
    float barLength = 40.0f;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_HIGHDPI);
    InitWindow(screenWidth, screenHeight, "Bouncing Ball");
    Bar bar = {
        .startPos = {0.7f* screenWidth,0.7f*screenHeight},
        .endPos = {0.8f * screenWidth,0.6f*screenHeight},
    };


    SetTargetFPS(60);
    int bucketHeight = screenHeight / numOfBuckets;
    Color colors[] = {RED,GREEN,YELLOW};

    Color chosenColors[numOfBuckets];

    int count = sizeof(colors)/sizeof(colors[0]);
    Bucket srcBuckets[numOfBuckets] ;
    Bucket destBuckets[numOfBuckets];
    Bar bars[numOfBars];

    Ball balls[numOfBalls] ;
    for(int i = 0; i < numOfBuckets; ++i) {
        chosenColors[i] = colors[GetRandomValue(0,count-1)];
    }
    //each ball should generate from the bucket that matches their color 
    //we somehow have to tell the balls to start from the source with the same color 
    // how can i do that 
    for(int i = 0; i < numOfBars; ++i) {
    
        float x = GetRandomValue(0,screenWidth-10.0f);
        float y = GetRandomValue(0,screenHeight);

        Bar bar = {
            .startPos = {x,y},
            .endPos = {x + barLength, y + barLength},
        };
        bars[i] = bar;
    }
    for(int i = 0; i < numOfBuckets; i++){
        float yStart = i * bucketHeight;
        float yEnd = yStart + bucketHeight;
        Color _color = chosenColors[i];
        Bucket srcBucket = {
            .startPos = {2.0f,yStart},
            .endPos = {2.0f,yEnd},
            .color = _color,
            .thickness = 10.0f,
            .active = true,
            .ballsConsumed = 0,
             
        };
        Bucket destBucket = {
            .startPos = {screenWidth-10.0f,yStart},
            .endPos = {screenWidth-10.0f,yEnd},
            .color = _color,
            .thickness = 10.0f,
            .active = true,
            .ballsConsumed = 0,
        };

        
        srcBuckets[i] = srcBucket;
        destBuckets[i] = destBucket;
    }
    for (int i = 0; i < numOfBalls; ++i) {
    // Randomly pick a bucket color
    Color color = chosenColors[GetRandomValue(0, numOfBuckets - 1)];

    // Find the source bucket with that color
    Bucket correspondingBucket = {0};
    for (int j = 0; j < numOfBuckets; ++j) {
        if (ColorToInt(srcBuckets[j].color) == ColorToInt(color)) {
            correspondingBucket = srcBuckets[j];
            break;
        }
    }

    // Generate a random starting position inside that bucket
    float start_x = correspondingBucket.startPos.x + 20.0f;  // a little offset from wall
    float start_y = GetRandomValue(
        correspondingBucket.startPos.y + 10,
        correspondingBucket.endPos.y - 10
    );

    Ball ball = {
        .active = true,
        .ballSpeed = {1.5f,1.5f},
        .color = correspondingBucket.color,
        .pos = {start_x, start_y},
        .radius = 20.0f,
    };

    balls[i] = ball;
}

    while (!WindowShouldClose())
    {
        if (IsWindowResized()) {
            screenWidth = GetScreenWidth();
            screenHeight = GetScreenHeight();
        }
        
        if(IsKeyDown(KEY_W)){
            bar.startPos.y -= 4.0f;
            bar.endPos.y -= 4.0f;
        }
        if(IsKeyDown(KEY_A)){
            bar.startPos.x -= 4.0f;
            bar.endPos.x -= 4.0f;
        }
        if(IsKeyDown(KEY_D)){
            bar.startPos.x += 4.0f;
            bar.endPos.x += 4.0f;
        }
        if(IsKeyDown(KEY_S)){
            bar.startPos.y += 4.0f;
            bar.endPos.y += 4.0f;
        }
        float baseLength = Vector2Distance(bar.startPos,bar.endPos);
        Vector2 barMid  = Vector2Lerp(bar.startPos,bar.endPos,0.5f);
        

        if(IsKeyDown(KEY_SPACE)) rotationAngle += 1.0f;

        bar.startPos = (Vector2){
            barMid.x - cosf(degreeToRad(rotationAngle)) * baseLength / 2,
            barMid.y - sinf(degreeToRad(rotationAngle)) * baseLength / 2
        };

        bar.endPos =   (Vector2){barMid.x + cosf(degreeToRad(rotationAngle)) * baseLength / 2,
                         barMid.y + sinf(degreeToRad(rotationAngle)) * baseLength / 2};
        for(int i = 0;i < numOfBalls; ++i) 
       {
            balls[i].pos.x += balls[i].ballSpeed.x;
            balls[i].pos.y += balls[i].ballSpeed.y;


            if ((balls[i].pos.x + balls[i].radius) >= screenWidth || (balls[i].pos.x - balls[i].radius) <= 0)
                balls[i].ballSpeed.x *= -1;

            if ((balls[i].pos.y+ balls[i].radius) >= screenHeight || (balls[i].pos.y - balls[i].radius) <= 0)
                balls[i].ballSpeed.y *= -1;
            Vector2 barDir = Vector2Normalize(Vector2Subtract(bar.endPos, bar.startPos));
Vector2 normal = (Vector2){ -barDir.y, barDir.x };

            Vector2 ab = Vector2Subtract(bar.endPos, bar.startPos);
            Vector2 ap = Vector2Subtract(balls[i].pos, bar.startPos);
            float t = Vector2DotProduct(ap, ab) / Vector2DotProduct(ab, ab);
            t = Clamp(t, 0.0f, 1.0f); 
            Vector2 closestPoint = Vector2Add(bar.startPos, Vector2Scale(ab, t));
            float dist = Vector2Distance(closestPoint, balls[i].pos);

            if (dist <= balls[i].radius) {
                float dot = Vector2DotProduct(balls[i].ballSpeed, normal);
                if (dot > 0) normal = Vector2Negate(normal); 
                balls[i].ballSpeed = Vector2Reflect(balls[i].ballSpeed, normal);

                Vector2 correction = Vector2Scale(normal, balls[i].radius - dist + 0.1f);
                balls[i].pos = Vector2Add(balls[i].pos, correction);
            }
            for(int i = 0; i < numOfBars; ++i) {
                Bar currBar = bars[i];
                Vector2 barDir = Vector2Normalize(Vector2Subtract(currBar.endPos, currBar.startPos));
                Vector2 normal = (Vector2){ -barDir.y, barDir.x };

                Vector2 ab = Vector2Subtract(currBar.endPos, currBar.startPos);
                Vector2 ap = Vector2Subtract(balls[i].pos, currBar.startPos);
                float t = Vector2DotProduct(ap, ab) / Vector2DotProduct(ab, ab);
                t = Clamp(t, 0.0f, 1.0f); 
                Vector2 closestPoint = Vector2Add(currBar.startPos, Vector2Scale(ab, t));
                float dist = Vector2Distance(closestPoint, balls[i].pos);

                if (dist <= balls[i].radius) {
                    float dot = Vector2DotProduct(balls[i].ballSpeed, normal);
                    if (dot > 0) normal = Vector2Negate(normal); 
                    balls[i].ballSpeed = Vector2Reflect(balls[i].ballSpeed, normal);

                    Vector2 correction = Vector2Scale(normal, balls[i].radius - dist + 0.1f);
                    balls[i].pos = Vector2Add(balls[i].pos, correction);
                }
            }

        }
        BeginDrawing();
            ClearBackground(RAYWHITE);  
            DrawLineEx(bar.startPos,bar.endPos,5.0f,BLACK);
            for(int i = 0; i < numOfBuckets; ++i){
                DrawLineEx(srcBuckets[i].startPos,srcBuckets[i].endPos,srcBuckets[i].thickness,srcBuckets[i].color);
                DrawLineEx(destBuckets[i].startPos,destBuckets[i].endPos,destBuckets[i].thickness,destBuckets[i].color);
            }
            for(int i = 0; i < numOfBars; ++i) {
                DrawLineEx(bars[i].startPos,bars[i].endPos,5.0f,BLACK);
            }
            for(int i = 0; i < numOfBalls; ++i) {
                DrawCircleV(balls[i].pos,balls[i].radius,balls[i].color);
            }

            DrawText("Bouncing Ball", 10, 10, 20, DARKGRAY);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}