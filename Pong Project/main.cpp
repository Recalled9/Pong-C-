#include <iostream>
#include <raylib.h>

using namespace std;
//To Start press Fn+F5
int Player_Score = 0;
int AI_Score = 0;
class Ball{
public:
    float x,y;
    int speed_x, speed_y;
    int radius;
    void Draw(){
        DrawCircle(x,y,radius,WHITE);
    }
    void Reset_ball(){
        x = GetScreenWidth()/2;
        y = GetScreenHeight()/2;
        int Choice[2] = {-1,1};
        speed_x *= Choice[GetRandomValue(0,1)];
        speed_y *= Choice[GetRandomValue(0,1)];
    }
    void Update(){
        x += speed_x;
        y += speed_y;
        if(y + radius >= GetScreenHeight() || y - radius <= 0) speed_y*= -1;
        if(x + radius >= GetScreenWidth()){
            speed_x*= -1;
            Player_Score++;
            Reset_ball();
        }
        if(x - radius <= 0){
            speed_x*= -1;
            AI_Score++;
            Reset_ball();
        }
    }


};
class Paddle{
public:
    int height,width;
    int x,y;
    float speed;
    Paddle(int Paddle_height, int Paddle_width, int pos_x, int pos_y,float Paddle_speed){
        height = Paddle_height;
        width = Paddle_width;
        x = pos_x;
        y = pos_y;
        speed = Paddle_speed;
        
    }
    void draw(){
        DrawRectangle(x,y,width,height,WHITE);
    }

    void Update(){
        if(IsKeyDown(KEY_W) && y > 0){
            y -= speed;
        }
        if(IsKeyDown(KEY_S) && y + height <GetScreenHeight()){
            y += speed; 
        }
    }
};
class AiPaddle : public Paddle{
public:
    float Diffculty;
    AiPaddle(int Paddle_height, int Paddle_width, int pos_x, int pos_y,float Paddle_speed,float Diff)
        :Paddle(Paddle_height,Paddle_width,pos_x,pos_y,Paddle_speed)
    {
        Diffculty = Diff;
    }
    void draw(){
        DrawRectangle(x,y,width,height,WHITE);
    }
    void Update(int ball_y, int ball_x){
        if(y + height/2 > ball_y && y > 0 && ball_x > GetScreenWidth()/Diffculty) y -= speed;
        if(y + height/2 < ball_y && y + height <GetScreenHeight() && ball_x > GetScreenWidth()/Diffculty) y += speed;
    }
};

int main () {
    const int Window_width = 1280;
    const int Window_height = 800;
    const int Paddle_height = 150;
    const float Diffculty = 5;//Higher Value Higher the Difficulty Level Value Should be greater than 1
    SetTargetFPS(60);
    InitWindow(Window_width,Window_height, "Pong!");
    Ball ball;
    Paddle Player1(Paddle_height,25,10,GetScreenHeight()/2 - Paddle_height/2,6);
    AiPaddle Player2(Paddle_height,25,GetScreenWidth()-35,GetScreenHeight()/2 - Paddle_height/2,6,Diffculty);
    ball.radius = 25;
    ball.speed_x = 7;
    ball.speed_y = 7;
    ball.x = Window_width / 2;
    ball.y = Window_height / 2;
    while(WindowShouldClose() == false)
    {
        BeginDrawing();
        //Updating 
        ball.Update();
        Player1.Update();
        Player2.Update(ball.y,ball.x);

        //Check for Collision
        if(CheckCollisionCircleRec(Vector2 {ball.x,ball.y}, ball.radius,Rectangle{(float)Player1.x,(float)Player1.y,(float)Player1.width,(float)Player1.height})){
            ball.speed_x *= -1;
        }
        if(CheckCollisionCircleRec(Vector2 {ball.x,ball.y}, ball.radius,Rectangle{(float)Player2.x,(float)Player2.y,(float)Player2.width,(float)Player2.height})){
            ball.speed_x *= -1;
        }
        //Drawing
        ClearBackground(BLACK);
        ball.Draw();
        Player2.draw();
        Player1.draw();
        DrawLine(Window_width/2,0,Window_width/2,Window_height,WHITE);
        DrawText(TextFormat("%i",Player_Score),Window_width/4-20,20,80,WHITE);
        DrawText(TextFormat("%i",AI_Score),3 * Window_width/4-20,20,80,WHITE);
        EndDrawing();

    }
    CloseWindow();
    return 0;
}