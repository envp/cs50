//
// breakout.c
//
// Computer Science 50
// Problem Set 3
//

// standard libraries
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

// Stanford Portable Library
#include <spl/gevents.h>
#include <spl/gobjects.h>
#include <spl/gwindow.h>

// height and width of game's window in pixels
#define HEIGHT 600
#define WIDTH 400

// number of rows of bricks
#define ROWS 5

// number of columns of bricks
#define COLS 10

// radius of ball in pixels
#define RADIUS 10

// lives
#define LIVES 3

// brick dimensions
#define BRICK_WIDTH 35
#define BRICK_HEIGHT 10
#define BRICK_SPACING 5

// X- Position of top left edge of first brick
#define EDGE_PAD_X 2

// Y - Position of top left edge of first brick
// Also minimum vertical padding
#define EDGE_PAD_Y 30

// Paddle constants
#define PADDLE_WIDTH (BRICK_WIDTH + BRICK_WIDTH / 2)
#define PADDLE_HEIGHT (BRICK_HEIGHT + BRICK_HEIGHT / 2)

// Maximum component (x or y) velocity: Pixels per tick (ms)
#define TICK_TIME 10
#define MAX_VEL 3.17396

// Non-boring angles for launching ball (Random sine between given angles)
#define RSINE(a, b) ((sin(a)) + drand48() * ((sin(b)) - (sin(a))))
#define SWEEP_START ((-22.5 / 180.0) * M_PI)
#define SWEEP_END ((202.5 / 180.0) * M_PI)

// prototypes
void initBricks(GWindow window);
GOval initBall(GWindow window);
GRect initPaddle(GWindow window);
GLabel initScoreboard(GWindow window);
void updateScoreboard(GWindow window, GLabel label, int points, int lives);
GObject detectCollision(GWindow window, GOval ball);
void bindToMouseX(GRect paddle);
void initVelocity(double* vx, double* vy);
// Inline functions to mutate game state based on collision events
int detectWindowCollision(GWindow window, GOval ball);

int main(void)
{
    // seed pseudorandom number generator
    srand48(time(NULL));

    // instantiate window
    GWindow window = newGWindow(WIDTH, HEIGHT);

    // instantiate bricks
    initBricks(window);

    // instantiate ball, centered in middle of window
    GOval ball = initBall(window);

    // instantiate paddle, centered at bottom of window
    GRect paddle = initPaddle(window);

    // instantiate scoreboard, centered in middle of window, just above ball
    GLabel label = initScoreboard(window);

    // number of bricks initially
    int bricks = COLS * ROWS;

    // number of lives initially
    int lives = LIVES;

    // number of points initially
    int points = 0;

    // Randomized initial velocity of ball
    double velY, velX;
    initVelocity(&velX, &velY);

    // Flag to determine if state was reset
    bool reset = false;

    // Crash test dummy with wireless reporting!
    GObject object = NULL;

    // Add the created UI elements to the window
    if(ball != NULL && paddle != NULL && paddle != NULL)
    {
        add(window, ball);
        add(window, paddle);
        add(window, label);
    }

    // First update to scoreboard
    updateScoreboard(window, label, points, lives);

    // Don't start unless we have some mouse activity in the window
    waitForEvent(MOUSE_EVENT);
   
    // keep playing until game over
    while (lives > 0 && bricks > 0)
    {
        reset = false;
        move(ball, velX, velY);
        bindToMouseX(paddle);
        
        switch(detectWindowCollision(window, ball))
        {
            // Collides with the left border
            case 0:
                velX *= -1.0;
                break;
            // Collides with the top border
            case 1:
                velY *= -1.0;
                break;
            // Collides with the right border
            case 2:
                velX *= -1.0;
                break;
            // 
            case 3:
                // Lose a life, wait for click to restart game
                lives--;
                waitForClick();
                setLocation(
                    ball,
                    (getWidth(window) - getWidth(ball)) / 2,
                    (getHeight(window) - getWidth(ball)) / 2
                );
                initVelocity(&velX, &velY);
                reset = true;
                break;
            default:
                break;
        }
        
        // Proceed only if the ball can still be played with
        if(!reset)
        {
            object = detectCollision(window, ball);
            if(object != NULL)
            {
                if(object == paddle)
                {
                    velY *= -1.0;
                }
                else if(strcmp(getType(object), "GRect") == 0)
                {
                    velY *= -1.0;
                    removeGWindow(window, object);
                    bricks--;
                    points = points + 1;
                    updateScoreboard(window, label, points, lives);
                }
            }
        }
        pause(TICK_TIME);
    }

    // wait for click before exiting
    waitForClick();

    // game over
    closeGWindow(window);
    return 0;
}

/**
 * Initializes window with a grid of bricks.
 */
void initBricks(GWindow window)
{
    // brick row & col counters
    int row = 0, col = 0;
    char* colors[ROWS] = {"RED", "GREEN", "BLUE", "ORANGE", "GRAY"};

    for(row = 0; row < ROWS; ++row)
    {
        for(col = 0; col < COLS; ++col)
        {
            // Instantiate at the top left corner
            GRect brick = newGRect(0, 0, BRICK_WIDTH, BRICK_HEIGHT);

            // Set the color and fill
            setColor(brick, colors[row]);
            setFilled(brick, true);

            // Relocate to correct position
            setLocation(
                brick,
                EDGE_PAD_X + col * (BRICK_WIDTH + BRICK_SPACING),
                EDGE_PAD_Y + row * (BRICK_HEIGHT + BRICK_SPACING)
            );

            // Add to window
            add(window, brick);
        }
    }
    return ;
}

/**
 * Instantiates ball in center of window.  Returns ball.
 */
GOval initBall(GWindow window)
{
    // Create a new ball at the top left corner
    GOval ball = newGOval(0, 0, 2 * RADIUS, 2 * RADIUS);
    setColor(ball, "DARK_GRAY");
    setFilled(ball, true);

    // Relocate it to the center
    setLocation(
        ball,
        getWidth(window) / 2 - RADIUS,
        getHeight(window) / 2 - RADIUS
    );

    // Returns ball or NULL (depending on success)
    return ball;
}

/**
 * Instantiates paddle in bottom-middle of window.
 */
GRect initPaddle(GWindow window)
{
    // THE PADDLE IS A RUN AWAY BRICK!
    // Again, instantiate at top-left
    GRect paddle = newGRect(0, 0, PADDLE_WIDTH, PADDLE_HEIGHT);

    // Color it
    setColor(paddle, "BLACK");
    setFilled(paddle, true);

    // Ship it
    setLocation(
        paddle,
        (getWidth(window) - PADDLE_WIDTH) / 2,
        getHeight(window) - EDGE_PAD_Y
    );
    return paddle;
}

/**
 * Instantiates, configures, and returns label for scoreboard.
 */
GLabel initScoreboard(GWindow window)
{
    GLabel label = newGLabel("");
    return label;
}

/**
 * Updates scoreboard's label, keeping it centered in window.
 */
void updateScoreboard(GWindow window, GLabel label, int points, int lives)
{
    // update label
    char s[12];
    sprintf(s, "%d P : %d L", points, lives);

    // Using fixed width font makes it easier to calculate
    setFont(label, "Liberation Mono-Bold-32");

    // A very light gray to keep focus off this
    setColor(label, "LIGHT_GRAY");

    setLabel(label, s);

    // center label in window
    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = (getHeight(window) - getHeight(label)) / 2;
    setLocation(label, x, y);
}

/**
 * Detects whether ball has collided with some object in window
 * by checking the four corners of its bounding box (which are
 * outside the ball's GOval, and so the ball can't collide with
 * itself).  Returns object if so, else NULL.
 */
GObject detectCollision(GWindow window, GOval ball)
{
    // ball's location
    double x = getX(ball);
    double y = getY(ball);

    // for checking for collisions
    GObject object;

    // check for collision at ball's top-left corner
    object = getGObjectAt(window, x, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's top-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-left corner
    object = getGObjectAt(window, x, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // no collision
    return NULL;
}

/**
 * Detects collisions of the ball with the window border
 * Returns the following values for the respective sides:
 * NOTHING: -1
 * LEFT: 0
 * TOP: 1
 * RIGHT: 2
 * BOTTOM: 3
 */
int detectWindowCollision(GWindow window, GOval ball)
{
    // Colliding with left edge of window
    if(getX(ball) <= 0)
    {
        return 0;
    }

    // Colliding with top edge of window
    else if(getY(ball) <= 0)
    {
        return 1;
    }
    // Colliding with right edge of window
    else if(getX(ball) + 2 * RADIUS >= getWidth(window))
    {
        return 2;
    }
    // Check against paddle_y to potentially avoid http://a.pomf.se/sfpubq.gif
    else if(getY(ball) + 2 * RADIUS > getHeight(window) - EDGE_PAD_Y + PADDLE_HEIGHT)
    {
        return 3;
    }
    return -1;
}

/**
 * Would be better if the signature could be void bindToMouseX(GObject)
 * Not a binding per-se but makes the paddle follow mouse_x
 */
inline void bindToMouseX(GRect paddle)
{
    GEvent event = getNextEvent(MOUSE_EVENT);
    if(event != NULL)
    {
        if(getEventType(event) == MOUSE_MOVED)
        {
            setLocation(
                paddle,
                getX(event) - getWidth(paddle) / 2,
                getY(paddle)
            );
        }
    }
}

void initVelocity(double* vx, double* vy)
{
    double y = MAX_VEL * RSINE(SWEEP_START, SWEEP_END);
    *vx = sqrt(MAX_VEL * MAX_VEL - y * y);
    *vy = y;
}