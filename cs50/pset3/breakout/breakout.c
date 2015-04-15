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
#define PADDLE_HEIGHT (BRICK_HEIGHT)

// Maximum component (x or y) velocity: Pixels per tick (ms)
#define TICK_TIME 10
#define MAX_VEL 3.17396

// prototypes
void initBricks(GWindow window);
GOval initBall(GWindow window);
GRect initPaddle(GWindow window);
GLabel initScoreboard(GWindow window);
void updateScoreboard(GWindow window, GLabel label, int points, int lives);
GObject detectCollision(GWindow window, GOval ball);

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

    // Dummy object for collision detection
    GObject object;
    
    // Randomized initial velocity of ball
    double velX = MAX_VEL * (drand48() - 0.5);
    double velY = sqrt((MAX_VEL * MAX_VEL) - (velX * velX));
    
    // Flag to determine if state was reset
    bool reset = false;

    // Add the created UI elements to the window
    if(ball != NULL && paddle != NULL && paddle != NULL)
    {
        printf("Added ball to window!\n");
        add(window, ball);
        
        printf("Added paddle to window!\n");
        add(window, paddle); 
        
        printf("Added scoreboard to window!\n");
        add(window, label);
    }

    // First update to scoreboard
    updateScoreboard(window, label, points, lives);

    // keep playing until game over
    while (lives > 0 && bricks > 0)
    {     
        move(ball, velX, velY);
        
        // Colliding with left edge of window
        if(getX(ball) <= 0)
        {
            velX *= -1.0;
        }
                
        // Colliding with top edge of window
        if(getY(ball) <= 0)
        {
            velY *= -1.0;
        }
        // Colliding with right edge of window
        if(getX(ball) + 2 * RADIUS >= getWidth(window))
        {
            velX *= -1.0;
        }
        // Zoomed past our paddle
        if(getY(ball) + 2 * RADIUS >= getY(paddle))
        {
            // We lost a life!
            lives--;
            waitForClick();
            
            // Relocate to center
            setLocation(
                ball, 
                (getWidth(window) - getWidth(ball)) / 2, 
                (getHeight(window) - getWidth(ball)) / 2
            );
            
            reset = true;
        }
        
        if(!reset)
        {
            GObject object = detectCollision(window, ball);
            
            if(object != NULL)
            {           
                if(object == paddle)
                {
                    velY *= -1.0;
                }
                else if(strcmp(getType(object), "GRect") == 0)
                {
                    // Reach 522 speed ultimately
                    velY *= -1.01;
                    velX *= 1.01;
                    removeGWindow(window, object);
                    bricks--, points++;
                }
            }
        }
        
        updateScoreboard(window, label, points, lives);
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
    sprintf(s, "%i P : %i L", points, lives);

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
