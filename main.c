#include "raylib.h"
#include "raymath.h"
#include <math.h>


typedef enum BALL_TYPE{
    BIT_ZERO ,
    BIT_ONE  ,
    RIGHT_SHIFT,
    LEFT_SHIFT,
}BALL_TYPE;



typedef struct Line {
    Vector2 startPos;
    Vector2 endPos;
}Line;
typedef struct Ball {
    BALL_TYPE type;
    Vector2 pos;
    float radius;
    Color color;
    Vector2 ballSpeed;
    bool active;
}Ball;




typedef struct Bar {
    Vector2 center;
    float length;
    float thickness;
}Bar;

typedef struct Bucket {
    int num;
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
void drawBall(Ball* b){
    DrawCircleV(b->pos,b->radius,b->color);
    const char *text = "";

    switch (b->type)
        {
            case BIT_ZERO:
                text = "0";
                break;
            case BIT_ONE:
                text = "1";
                break;
            case RIGHT_SHIFT:
                text = ">>";
                break;
            case LEFT_SHIFT:
                text ="<<";
                break;
            default:
                break;
            
        }
    DrawText(text,b->pos.x,b->pos.y,20.0f,BLACK);
}    


void createBar(Bar* bar, Vector2 center,float length,float thickness){
    bar->center = center;
    bar->thickness = thickness;
    bar->length = length;
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
    float barThickness = 20.0f;
    

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_HIGHDPI);
    InitWindow(screenWidth, screenHeight, "Bouncing Ball");
    Bar bar;
    Vector2 centre = {0.6f * screenWidth,0.6f*screenHeight};

    createBar(&bar,centre,barLength,barThickness);
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
    for (int i = 0; i < numOfBars; ++i) {
    Vector2 center = (Vector2){GetRandomValue(0,screenWidth),GetRandomValue(0,screenHeight)};
    createBar(&bars[i], center, barLength, barThickness);
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
        if(IsKeyDown(KEY_SPACE)) rotationAngle += 1.0f;

        
        // if(IsKeyDown(KEY_W)){
        //     bar->center.x -= 4.0f;
        //     bar->center.y -= 4.0f;
        // }
        // if(IsKeyDown(KEY_A)){
        //     bar->center.x -= 4.0f;
        //     bar->center.x -= 4.0f;
        // }
        // if(IsKeyDown(KEY_D)){
        //     bar->center.x += 4.0f;
        //     bar->center.x += 4.0f;
        // }
        // if(IsKeyDown(KEY_S)){
        //     bar->center.y += 4.0f;
        //     bar->center.y += 4.0f;
        // }
        if(IsKeyDown(KEY_W)) bar.center.y -= 4.0f;
        if(IsKeyDown(KEY_S)) bar.center.y += 4.0f;
        if(IsKeyDown(KEY_A)) bar.center.x -= 4.0f;
        if(IsKeyDown(KEY_D)) bar.center.x += 4.0f;

        
        float baseLength = bar.length;
        Vector2 barMid  = bar.center;
        

        
        Line line;
        line.startPos = (Vector2){
            barMid.x - cosf(degreeToRad(rotationAngle)) * baseLength / 2,
            barMid.y - sinf(degreeToRad(rotationAngle)) * baseLength / 2
        };

        line.endPos =   (Vector2){barMid.x + cosf(degreeToRad(rotationAngle)) * baseLength / 2,
                         barMid.y + sinf(degreeToRad(rotationAngle)) * baseLength / 2};
        // collsion logic                
        for(int i = 0;i < numOfBalls; ++i) 
        {
            // speed up the ball 
            balls[i].pos.x += balls[i].ballSpeed.x;
            balls[i].pos.y += balls[i].ballSpeed.y;

            // out of bounds check for the ball
            if ((balls[i].pos.x + balls[i].radius) >= screenWidth || (balls[i].pos.x - balls[i].radius) <= 0)
                balls[i].ballSpeed.x *= -1;

            if ((balls[i].pos.y+ balls[i].radius) >= screenHeight || (balls[i].pos.y - balls[i].radius) <= 0)
                balls[i].ballSpeed.y *= -1;
            

            //for our movable bar 
            Vector2 barDir = Vector2Normalize(Vector2Subtract(line.endPos,line.startPos));
            Vector2 normal = (Vector2) {-barDir.y,barDir.x};

            Vector2 offset = Vector2Scale(normal, bar.thickness / 2.0f);



            Line line1 = {Vector2Add(line.startPos,offset),Vector2Add(line.endPos,offset)};
            Line line2 = {Vector2Subtract(line.startPos,offset),Vector2Subtract(line.endPos,offset)};

            Line line3 = {line2.startPos,line1.startPos};

            Line line4 = {line2.endPos,line1.endPos};

            Vector2 dir1 = barDir;
            Vector2 dir2 = barDir;
            Vector2 dir3 = Vector2Normalize(Vector2Subtract(line3.endPos, line3.startPos));
            Vector2 dir4 = Vector2Normalize(Vector2Subtract(line4.endPos, line4.startPos)); 
            Vector2 normal1 = normal;             // points outward from line1
            Vector2 normal2 = (Vector2){ -normal.x, -normal.y }; // outward from line2
            Vector2 normal3 = (Vector2){ -barDir.y, barDir.x };  // outward from line3
            Vector2 normal4 = (Vector2){ barDir.y, -barDir.x };  // outward from line4




            Line edges[4] = {line1,line2,line3,line4};
            Vector2 normals[4] = {normal1,normal2,normal3,normal4};

            for (int e = 0 ; e < 4; e++)
            {
                Vector2 ab = Vector2Subtract(edges[e].endPos,edges[e].startPos);
                Vector2 ap = Vector2Subtract(balls[i].pos,edges[e].startPos);
                float t = Vector2DotProduct(ap, ab) / Vector2DotProduct(ab, ab);
                t = Clamp(t, 0.0f, 1.0f);

                Vector2 closestPoint = Vector2Add(edges[e].startPos, Vector2Scale(ab, t));
                float dist = Vector2Distance(closestPoint, balls[i].pos);

                if (dist <= balls[i].radius) {
                    Vector2 normal = normals[e];
                    float dot = Vector2DotProduct(balls[i].ballSpeed, normal);
                    if (dot > 0) normal = Vector2Negate(normal);

                    balls[i].ballSpeed = Vector2Reflect(balls[i].ballSpeed, normal);

                    Vector2 correction = Vector2Scale(normal, balls[i].radius - dist + 0.1f);
                    balls[i].pos = Vector2Add(balls[i].pos, correction);
                }

            }
            for(int j = 0; j < numOfBars; ++j) 
            {
                    Bar currBar = bars[j];
                    float baseLength = currBar.length;
                    Vector2 barMid  = currBar.center;
    
                    Line line;
                    line.startPos = (Vector2){
                        barMid.x - cosf(degreeToRad(rotationAngle)) * baseLength / 2,
                        barMid.y - sinf(degreeToRad(rotationAngle)) * baseLength / 2
                    };

                    line.endPos =   (Vector2){barMid.x + cosf(degreeToRad(rotationAngle)) * baseLength / 2,
                                    barMid.y + sinf(degreeToRad(rotationAngle)) * baseLength / 2};
                                


                    Vector2 barDir = Vector2Normalize(Vector2Subtract(line.endPos,line.startPos));
                    Vector2 normal = (Vector2) {-barDir.y,barDir.x};


                    Vector2 offset = Vector2Scale(normal,currBar.thickness/2.0f);

                    Line line1 = {Vector2Add(line.startPos,offset),Vector2Add(line.endPos,offset)};
                    Line line2 = {Vector2Subtract(line.startPos,offset),Vector2Subtract(line.endPos,offset)};

                    Line line3 = {line2.startPos,line1.startPos};

                    Line line4 = {line2.endPos,line1.endPos};

                    Vector2 dir1 = barDir;
                    Vector2 dir2 = barDir;
                    Vector2 dir3 = Vector2Normalize(Vector2Subtract(line3.endPos, line3.startPos));
                    Vector2 dir4 = Vector2Normalize(Vector2Subtract(line4.endPos, line4.startPos)); 
                    Vector2 normal1 = normal;             // points outward from line1
                    Vector2 normal2 = (Vector2){ -normal.x, -normal.y }; // outward from line2
                    Vector2 normal3 = (Vector2){ -barDir.y, barDir.x };  // outward from line3
                    Vector2 normal4 = (Vector2){ barDir.y, -barDir.x };  // outward from line4




                    Line edges[4] = {line1,line2,line3,line4};
                    Vector2 normals[4] = {normal1,normal2,normal3,normal4};

                    for (int e = 0 ; e < 4; e++)
                    {
                        Vector2 ab = Vector2Subtract(edges[e].endPos,edges[e].startPos);
                        Vector2 ap = Vector2Subtract(balls[i].pos,edges[e].startPos);
                        float t = Vector2DotProduct(ap, ab) / Vector2DotProduct(ab, ab);
                        t = Clamp(t, 0.0f, 1.0f);

                        Vector2 closestPoint = Vector2Add(edges[e].startPos, Vector2Scale(ab, t));
                        float dist = Vector2Distance(closestPoint, balls[i].pos);

                        if (dist <= balls[i].radius) 
                        {
                            Vector2 normal = normals[e];
                            float dot = Vector2DotProduct(balls[i].ballSpeed, normal);
                            if (dot > 0) normal = Vector2Negate(normal);

                            balls[i].ballSpeed = Vector2Reflect(balls[i].ballSpeed, normal);

                            Vector2 correction = Vector2Scale(normal, balls[i].radius - dist + 0.1f);
                            balls[i].pos = Vector2Add(balls[i].pos, correction);
                        }

                    }

                
            }

        }
        BeginDrawing();
            ClearBackground(RAYWHITE);  
            DrawLineEx(line.startPos,line.endPos,bar.thickness/2.0f,RED);
            for(int i = 0; i < numOfBuckets; ++i){
                DrawLineEx(srcBuckets[i].startPos,srcBuckets[i].endPos,srcBuckets[i].thickness,srcBuckets[i].color);
                DrawLineEx(destBuckets[i].startPos,destBuckets[i].endPos,destBuckets[i].thickness,destBuckets[i].color);
            }
            for(int i = 0; i < numOfBars; ++i) {
                Bar currBar = bars[i];
                float baseLength = currBar.length;
                Vector2 barMid  = currBar.center;
                Line line;
                line.startPos = (Vector2){
                    barMid.x - cosf(degreeToRad(rotationAngle)) * baseLength / 2,
                    barMid.y - sinf(degreeToRad(rotationAngle)) * baseLength / 2
                };

                line.endPos =   (Vector2){barMid.x + cosf(degreeToRad(rotationAngle)) * baseLength / 2,
                                barMid.y + sinf(degreeToRad(rotationAngle)) * baseLength / 2};
                DrawLineEx(line.startPos,line.endPos,currBar.thickness/2.0f,BLACK);
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