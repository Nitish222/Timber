// Timber.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include <iostream>
#include <SFML/Graphics.hpp>
#include <sstream>
#include <SFML/Audio.hpp>

using namespace sf;
//function declaration for branch
void updateBranches(int seed);

const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];
//position of branch (left or right)
enum class side{LEFT, RIGHT , NONE};
side branchePositions[NUM_BRANCHES];

int main()
{
    //create a video mod object that setting the window resolution
    VideoMode vm(1920, 1080);
    //create and open a fullscreen window for the game
    RenderWindow window(vm, "Timber!!!", Style::Fullscreen);

    //create texture to hold graphic on GPU
    Texture textureBackground;

    //load graphic into the texture p
    textureBackground.loadFromFile("Graphic/background.png");

    //create a sprite
    Sprite spriteBackground;

    //attach the texture to sprite
    spriteBackground.setTexture(textureBackground);
    
    //set the spriteBackground to cover the screen 
    spriteBackground.setPosition(0, 0);

    //create tree
    Texture textureTree;
    textureTree.loadFromFile("Graphic/tree.png");
    Sprite spriteTree;
    spriteTree.setTexture(textureTree);
    spriteTree.setPosition(810, 0);

    //create bee
    Texture textureBee;
    textureBee.loadFromFile("Graphic/bee.png");
    Sprite spriteBee;
    spriteBee.setTexture(textureBee);
    spriteBee.setPosition(0, 800);

    //is the bee currently moving?
    bool beeActive = false;

    //how fast can bee fly 
    float beeSpeed = 0.0f;

    //create cloud
    Texture textureCloud;
    textureCloud.loadFromFile("Graphic/cloud.png");
    Sprite spriteCloud1;
    Sprite spriteCloud2;
    Sprite spriteCloud3;
    
    spriteCloud1.setTexture(textureCloud);
    spriteCloud1.setPosition(0,0);
    spriteCloud2.setTexture(textureCloud);
    spriteCloud2.setPosition(0,250);
    spriteCloud3.setTexture(textureCloud);
    spriteCloud3.setPosition(0, 500);
    
    //are cloud active?
    bool cloud1Active = false;
    bool cloud2Active = false;
    bool cloud3Active = false;

    //speed of clouds
    float cloud1speed = 0.0f;
    float cloud2speed = 0.0f;
    float cloud3speed = 0.0f;
    
    //to control time
    Clock clock;
    
    //timebar
    RectangleShape timeBar;
    float timeBarStartWidth = 400;
    float timeBarHeight = 80;
    timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
    timeBar.setFillColor(Color::Red);
    timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);

    Time gameTimeTotal;
    float timeRemaining = 6.0f;
    float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

    //for pause frame
    bool pause = true;
    
    //variable for score and pause screen message
    unsigned int score = 0;
    Text messageText;
    Text scoreText;
    Font font;
    
    //choose font 
    font.loadFromFile("Font/KOMIKAP_.ttf");
    
    //set font
    messageText.setFont(font);
    scoreText.setFont(font);

    //Assign the message
    messageText.setString("Press Enter to Start!!");
    scoreText.setString("Score=0");

    //Set size
    messageText.setCharacterSize(77);
    messageText.setCharacterSize(100);

    //Choose color
    messageText.setFillColor(Color::White);
    scoreText.setFillColor(Color::White);

    //Position the Text
    FloatRect textRect = messageText.getLocalBounds();
    
    messageText.setOrigin(textRect.left + textRect.width / 2.0f, 
                          textRect.top + textRect.height / 2.0f);

    messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
    scoreText.setPosition(20, 20);
    
    //prepare branches
    Texture textureBranch;
    textureBranch.loadFromFile("Graphic/branch.png");

    //set the texture for each sprite branch
    for (int i = 0; i < NUM_BRANCHES; i++)
    {

        branches[i].setTexture(textureBranch);
        branches[i].setPosition(-2000, -2000);
        //set origin as center, we can spin it without changing its position
        branches[i].setOrigin(220, 20);
    }
    /*
    updateBranches(1);
    updateBranches(2);
    updateBranches(3);
    updateBranches(4); 
    updateBranches(5);
    */
    
    //********************************************************* Player and stuff ********************************************************

    //prepare player
    Texture texturePlayer;
    texturePlayer.loadFromFile("Graphic/player.png");
    
    Sprite spritePlayer;
    spritePlayer.setTexture(texturePlayer);
    spritePlayer.setPosition(580, 720);

    //player starts on LEFT
    side playerSide = side::LEFT;

    //Prepare gravestone
    Texture textureRIP;
    textureRIP.loadFromFile("Graphic/rip.png");
    Sprite spriteRIP;
    spriteRIP.setTexture(textureRIP);
    spriteRIP.setPosition(600, 860);

    //prepare Axe
    Texture textureAXE;
    textureAXE.loadFromFile("Graphic/axe.png");
    Sprite spriteAXE;
    spriteAXE.setTexture(textureAXE);
    spriteAXE.setPosition(700, 830);

    //line the axe up with the tree

    const float AXE_POSITION_LEFT = 700;
    const float AXE_POSITION_RIGHT = 1075;

    //control player input 
    bool acceptInput = false;

    //prepare flying log
    Texture textureLog;
    textureLog.loadFromFile("Graphic/log.png");
    Sprite spriteLog;
    spriteLog.setTexture(textureLog);
    spriteLog.setPosition(810, 720);

    //Log related variables
    
    bool logActive = false;
    float logSpeedX = 1000;
    float logSpeedy = -1500;

    //prepare sounds 
    //chopping
    SoundBuffer chopBuffer;
    chopBuffer.loadFromFile("Sound/chop.wav");
    Sound chop;
    chop.setBuffer(chopBuffer);

    //death sound
    SoundBuffer deathBuffer;
    deathBuffer.loadFromFile("Sound/death.wav");
    Sound death;
    death.setBuffer(deathBuffer);

    //Out of time
    SoundBuffer ootBuffer;
    ootBuffer.loadFromFile("Sound/out_of_time.wav");
    Sound outOfTime;
    outOfTime.setBuffer(ootBuffer);


    
    while (window.isOpen())
    {
        Event event;

        while (window.pollEvent(event))
        {
            if (event.type == Event::KeyReleased && !pause)
            {
                //Listen for key presses again 
                acceptInput = true;

                //hide AXE
                spriteAXE.setPosition(2000, spriteAXE.getPosition().y);
            }

        }
        // Handle Player input
        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            window.close();
        }
        //start the game
        if (Keyboard::isKeyPressed(Keyboard::Return))
        {
            pause = false;

            //reset the time and score
            score = 0;
            timeRemaining = 6;

            //make all branches disappear 
            //starting in the second pos

            for (int i = 0; i < NUM_BRANCHES; i++)
            {
                branchePositions[i] = side::NONE;
            }
            // Make sure gravestone is hidden
            spriteRIP.setPosition(675, 2000);

            //move player into position 
            spritePlayer.setPosition(580, 720);

            acceptInput = true;
        }
        if (acceptInput)
        {
            if (Keyboard::isKeyPressed(Keyboard::Right))
            {
                //make sur eplayer is on right side 
                playerSide = side::RIGHT;

                score++;
                //add to the amount of time remaining 
                timeRemaining += (2 / score) + .15;

                spriteAXE.setPosition(AXE_POSITION_RIGHT, spriteAXE.getPosition().y);

                spritePlayer.setPosition(1200, 720);

                //update the branches 
                updateBranches(score);

                //set flying log to LEFT

                spriteLog.setPosition(810, 720);
                logSpeedX = -5000;
                logActive = true;

                acceptInput = false;
                
               //play sound of chop
                chop.play();
            }

            if (Keyboard::isKeyPressed(Keyboard::Left))
            {
                //make sure player is on the left side
                playerSide = side::LEFT;

                score++;

                //add time
                timeRemaining += (2 / score) + .15;

                spriteAXE.setPosition(AXE_POSITION_LEFT, spriteAXE.getPosition().y);

                spritePlayer.setPosition(580, 720);

                //update branches
                updateBranches(score);

                //set the log flying 
                spriteLog.setPosition(810, 720);
                logSpeedX = 5000;
                logActive = true;

                acceptInput = false;
                chop.play();
            }


        }

        //clear last frame
        window.clear();


        //measure time


        if (!pause)
        {
            //measure time
            Time dt = clock.restart();

            //subtract from the time remaining 
            timeRemaining -= dt.asSeconds();
            //size up the time bar
            timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));

            if (timeRemaining <= 0.0f)
            {
                pause = true;
                messageText.setString("Out of Time!! :(");
                FloatRect textRect = messageText.getLocalBounds();
                messageText.setOrigin(textRect.left + textRect.width / 2.0f,
                    textRect.top + textRect.height / 2.0f);
                messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
                
                //Play out of time sound
                outOfTime.play();
            }
            if (!beeActive)
            {
                //how fast is the bee
                srand((int)time(0));
                beeSpeed = (rand() % 200) + 200;


                srand((int)time(0) * 10);
                float height = (rand() % 500) + 500;
                spriteBee.setPosition(2000, height);
                beeActive = true;
            }

            else
            {
                spriteBee.setPosition(
                    spriteBee.getPosition().x -
                    (beeSpeed * dt.asSeconds()), spriteBee.getPosition().y);
                //has reached left edge
                if (spriteBee.getPosition().x < -100)
                {
                    beeActive = false;
                }
            }


            if (!cloud1Active)
            {
                srand((int)time(0) * 10);
                cloud1speed = (rand() % 200);
                srand((int)time(0) * 10);
                float height = (rand() % 150);
                spriteCloud1.setPosition(-200, height);
                cloud1Active = true;
            }

            else
            {
                spriteCloud1.setPosition
                (
                    spriteCloud1.getPosition().x +
                    (cloud1speed * dt.asSeconds()),
                    spriteCloud1.getPosition().y
                );
                if (spriteCloud1.getPosition().x > 1920)
                {
                    //make new cloud
                    cloud1Active = false;
                }

            }

            if (!cloud2Active)
            {
                srand((int)time(0) * 20);
                cloud2speed = (rand() % 200);
                srand((int)time(0) * 20);
                float height = (rand() % 150);
                spriteCloud2.setPosition(-200, height);
                cloud2Active = true;
            }

            else
            {
                spriteCloud2.setPosition
                (
                    spriteCloud2.getPosition().x +
                    (cloud2speed * dt.asSeconds()),
                    spriteCloud2.getPosition().y
                );
                if (spriteCloud2.getPosition().x > 1920)
                {
                    //make new cloud
                    cloud2Active = false;
                }

            }

            if (!cloud3Active)
            {
                srand((int)time(0) * 30);
                cloud3speed = (rand() % 200);
                srand((int)time(0) * 30);
                float height = (rand() % 150);
                spriteCloud3.setPosition(-200, height);
                cloud3Active = true;
            }

            else
            {
                spriteCloud3.setPosition
                (
                    spriteCloud3.getPosition().x +
                    (cloud3speed * dt.asSeconds()),
                    spriteCloud3.getPosition().y
                );
                if (spriteCloud3.getPosition().x > 1920)
                {
                    //make new cloud
                    cloud3Active = false;
                }

            }
            //update the score
            std::stringstream ss;
            ss << "Score=" << score;
            scoreText.setString(ss.str());

        //updating the branch sprites

        for (int i = 0; i < NUM_BRANCHES; i++)
        {
            float height = i * 150;
            if (branchePositions[i] == side::LEFT)
            {
                //move branches sprite to left side
                branches[i].setPosition(610, height);

                //flip the branch sprite
                branches[i].setRotation(180);
            }
            else if (branchePositions[i] == side::RIGHT)
            {
                //move the sprite to the right side
                branches[i].setPosition(1330, height);
                //set the sprite rotation to normal
                branches[i].setRotation(0);
            }
            else
            {
                //hide the branch
                branches[i].setPosition(3000, height);
            }
        }
            //Handle flying log
            if (logActive)
            {
                spriteLog.setPosition(spriteLog.getPosition().x + (logSpeedX * dt.asSeconds()), spriteLog.getPosition().y + (logSpeedy * dt.asSeconds()));
                //Has log reached right hand edge
                if (spriteLog.getPosition().x < -100 || spriteLog.getPosition().x>2000)
                {
                    //ready to be a new log
                    logActive = false;
                    spriteLog.setPosition(810, 720);
                }

            }

            //player has been squished by log
            if (branchePositions[5] == playerSide)
            {
                //death
                pause = true;
                acceptInput = false;

                // draw gravestone
                spriteRIP.setPosition(525, 760);

                //hide player
                spritePlayer.setPosition(2000, 660);

                //change text of message
                messageText.setString("SQUISHED!");

                //center the message
                FloatRect textRect = messageText.getLocalBounds();

                messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);

                messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
                
                //play death sound
                death.play();
            }
      
        }//end if(!paused)

        //draw the game scene
        window.draw(spriteBackground);
        
        //draw cloud behind tree
        window.draw(spriteCloud1);
        window.draw(spriteCloud2);
        window.draw(spriteCloud3);
        
        //draw branches
        for (int i = 0; i < NUM_BRANCHES; i++)
        {
            window.draw(branches[i]);
        }

        //draw tree
        window.draw(spriteTree);

        //draw player
        window.draw(spritePlayer);

        //draw axe
        window.draw(spriteAXE);

        //draw flying log
        window.draw(spriteLog);

        //draw gravestone
        window.draw(spriteRIP);
        
        //draw bee infront of tree
        window.draw(spriteBee);

        //draw score
        window.draw(scoreText);

        //draw timebar
        window.draw(timeBar);
        
        if(pause)
        {
            //draw our message
            window.draw(messageText);
        }
        
        //show everything we just draw
        window.display();
    }
    return 0;
}

void updateBranches(int seed) 
{
    //move all branches down one place
    for (int i = NUM_BRANCHES-1; i>0; i--)
    {
        branchePositions[i] = branchePositions[i - 1];
    }
    //swapping new branch
    //LEFT RIGHT or NONE
    srand((int)time(0) + seed);
    int r = (rand() % 5);

    switch (r)
    {
    case 0:
        branchePositions[0] = side::LEFT;
        break;

    case 1:
        branchePositions[0] = side::RIGHT;
        break;
    default:
        branchePositions[0] = side::NONE;
        break;
    }
}