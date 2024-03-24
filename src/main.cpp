#include <raylib.h>
#include <string>
const Color DARK_BLUE = {153, 204, 255, 255};
const Color LIGHT_BLUE = {153, 179, 255, 255};
const Color WHITE_ALPHA = {255, 255, 255, 100};
const int WINDOW_WIDTH = 900, WINDOW_HEIGHT = 600;
class Ball
{
public:
    int x;
    int y;
    int radius;
    Color color;
    int speed_x;
    int speed_y;
    Ball(int x, int y, int radius, int speed_x, int speed_y, Color color = WHITE)
    {
        this->x = x;
        this->y = y;
        this->radius = radius;
        this->color = color;
        this->speed_x = speed_x;
        this->speed_y = speed_y;
    }
    void Draw()
    {
        DrawCircle(x, y, radius, color);
    }
    void Update(int &score1, int &score2)
    {
        x += speed_x;
        y += speed_y;
        if (x - radius <= 0)
        {
            speed_x *= -1;
            score2++;
            Reset() ;
        }
        if (x + radius >= WINDOW_WIDTH)
        {
            speed_x *= -1;
            score1++;
            Reset() ;
        }
        if (y - radius <= 0 || y + radius >= WINDOW_HEIGHT)
        {
            speed_y *= -1;
        }
    }
    void Reset()
    {
        x = WINDOW_WIDTH/2 ;
        y = WINDOW_HEIGHT/2 ;
        int speed_choice [] = {1 , -1} ;
        speed_x *= speed_choice[GetRandomValue(0 , 1)] ;
        speed_y *=speed_choice[GetRandomValue(0 , 1)] ;
    }
};

class Player
{
public:
    int x;
    int y;
    int width;
    int height;
    int score;
    int speed;
    Color color;
    Player(int x, int y, int width, int height, int speed, Color color = WHITE)
    {
        this->score = 0;
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
        this->speed = speed;
        this->color = color;
    }

    void Draw()
    {
        DrawRectangle(x, y, width, height, color);
    }
    void DrawScore(int x)
    {
        DrawText(std::to_string(this->score).c_str(), x, 20, 30, WHITE);
    }
    void Update(KeyboardKey key_up, KeyboardKey key_down)
    {
        if (IsKeyDown(key_up))
        {
            y -= 5;
            if (y < 0)
                y = 0;
        }
        if (IsKeyDown(key_down))
        {
            y += 5;
            if (y + height > WINDOW_HEIGHT)
                y = WINDOW_HEIGHT - height;
        }
    }
};
void DrawBoard()
{
    DrawRectangle(0, 0, WINDOW_WIDTH / 2, WINDOW_HEIGHT, DARK_BLUE);
    DrawRectangle(WINDOW_WIDTH / 2, 0, WINDOW_WIDTH / 2, WINDOW_HEIGHT, LIGHT_BLUE);
    DrawCircle(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 150, WHITE_ALPHA);
    DrawLine(WINDOW_WIDTH / 2, 0, WINDOW_WIDTH / 2, WINDOW_HEIGHT, WHITE);
}
void CheckCollisionPlayerWithBall(Ball &ball, const Player &p1, const Player &p2)
{
    if (CheckCollisionCircleRec(Vector2{(float)ball.x, (float)ball.y}, ball.radius,
                                Rectangle{(float)p1.x, (float)p1.y, (float)p1.width, (float)p1.height}) ||
        CheckCollisionCircleRec(Vector2{(float)ball.x, (float)ball.y}, ball.radius,
                                Rectangle{(float)p2.x, (float)p2.y, (float)p2.width, (float)p2.height}))
    {
        ball.speed_x *= -1;
    }
}
int main()
{
    InitWindow(900, 600, "Pong");
    SetTargetFPS(60);
    Player p1(0, 0, 20, 80, 5);
    Player p2(WINDOW_WIDTH - 20, 0, 20, 80, 5);
    Ball ball(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 20, 2, 2, YELLOW);
    while (WindowShouldClose() == false)
    {
        p1.Update(KEY_UP, KEY_DOWN);
        p2.Update(KEY_W, KEY_S);
        ball.Update(p1.score, p2.score);
        CheckCollisionPlayerWithBall(ball, p1, p2);
        BeginDrawing();
        DrawBoard();
        p1.Draw();
        p2.Draw();
        p1.DrawScore(WINDOW_WIDTH / 4);
        p2.DrawScore(0.75 * WINDOW_WIDTH);
        ball.Draw();
        EndDrawing();
    }
    CloseWindow();
}