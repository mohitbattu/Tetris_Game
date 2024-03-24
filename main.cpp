#define TETRIS_ROWS 20
#define TETRIS_COLS 11
#define BLOCK_SIZE 35

#include <SFML/Graphics.hpp>
#include <iostream>
#include <SFML/Audio.hpp>s
#include <unordered_map>
#include <sstream>

struct SSavedTetraminos {
public:
    int rows = 0;
    int cols = 0;
    int shapeSelected=0;
};

namespace {
    std::vector<sf::Color> tetraminoColors{
            sf::Color::Blue,
            sf::Color::Yellow,
            sf::Color(128, 0, 128),
            sf::Color::Green,
            sf::Color::Red,
            sf::Color::Blue,
            sf::Color(255, 165, 0)
    };
}
class CTetrisGame {
public:
    CTetrisGame() : mWindow(sf::VideoMode({TETRIS_COLS * BLOCK_SIZE + 300, TETRIS_ROWS * BLOCK_SIZE}),
                            "Tetris_Game") {

        mWindow.setFramerateLimit(120);// Main while loop is executed 60 times which helps the game to run smoothly.
        mTetraminos.setSize(sf::Vector2f(sf::Vector2f(BLOCK_SIZE - 1, BLOCK_SIZE - 1)));

    }

    // Random Shuffle
    int RandomShuffle(SSavedTetraminos& save,  sf::RectangleShape& tetraminos, sf::RenderWindow& window) {
        int i=0;
        if(mMemoryTetraminos.size()>0){
            mMemoryTetraminos.erase(mMemoryTetraminos.begin());
        }
        while(mMemoryTetraminos.size()<2) {
            auto random = (rand() % 7);
            mMemoryTetraminos.push_back(random);
        }
        NexBlockShape(mMemoryTetraminos[2], save, tetraminos, window);

        return mMemoryTetraminos[0];
    }

    std::vector<std::vector<int>> GetTetraminoCurrentShape (SSavedTetraminos& save) {
            std::vector<std::vector<std::vector<int>>> tetraminoShapes{
                // I Shape - rows, cols sleeping
                {{save.rows,save.cols-1 },{save.rows ,save.cols},{save.rows,save.cols+1}, {save.rows, save.cols-2}},
                // O Shape
                {{save.rows, save.cols}, {save.rows+1, save.cols}, {save.rows, save.cols+1}, {save.rows+1,save.cols+1}},
                // T Shape inverse shape
                {{save.rows, save.cols}, {save.rows+1, save.cols}, {save.rows+1, save.cols-1}, {save.rows+1,save.cols+1}},
                // S Shape
                {{save.rows, save.cols}, {save.rows+1, save.cols}, {save.rows+1, save.cols-1}, {save.rows,save.cols+1}},
                // Z Shape
                {{save.rows, save.cols}, {save.rows+1, save.cols}, {save.rows+1, save.cols+1}, {save.rows,save.cols-1}},
                // J Shape inverse
                {{save.rows, save.cols}, {save.rows+1, save.cols}, {save.rows+2, save.cols}, {save.rows,save.cols+1}},
                // L Shape
                {{save.rows, save.cols}, {save.rows+1, save.cols}, {save.rows+2, save.cols}, {save.rows+2,save.cols+1}}
            };

         // Clockwise
        // +90 degrees Rotated
        std::vector<std::vector<std::vector<int>>> tetraminoShapes_90rotated{
                // I Shape - rows, cols
                {{save.rows ,save.cols+1},{save.rows+1,save.cols+1}, {save.rows+2, save.cols+1}, {save.rows+3, save.cols+1}},
                // O Shape
                {{save.rows, save.cols}, {save.rows+1, save.cols}, {save.rows, save.cols+1}, {save.rows+1,save.cols+1}},
                // T Shape
                {{save.rows, save.cols}, {save.rows+1, save.cols}, {save.rows+2, save.cols}, {save.rows+1,save.cols+1}},
                // S Shape
                {{save.rows, save.cols}, {save.rows+1, save.cols}, {save.rows+1, save.cols+1}, {save.rows+2,save.cols+1}},
                // Z Shape
                {{save.rows, save.cols}, {save.rows+1, save.cols}, {save.rows+1, save.cols-1}, {save.rows+2,save.cols-1}},
                // J Shape
                {{save.rows, save.cols}, {save.rows, save.cols+1}, {save.rows, save.cols+2}, {save.rows+1,save.cols+2}},
                // L Shape
                {{save.rows, save.cols}, {save.rows, save.cols-1}, {save.rows, save.cols-2}, {save.rows+1,save.cols-2}}
        };
        // +90 degrees rotated
        std::vector<std::vector<std::vector<int>>> tetraminoShapes_180rotated{
                // I Shape - rows, cols
                {{save.rows,save.cols-1 },{save.rows ,save.cols},{save.rows,save.cols+1}, {save.rows, save.cols-2}},
                // O Shape
                {{save.rows, save.cols}, {save.rows+1, save.cols}, {save.rows, save.cols+1}, {save.rows+1,save.cols+1}},
                // T Shape
                {{save.rows, save.cols}, {save.rows, save.cols+1}, {save.rows, save.cols-1}, {save.rows+1,save.cols}},
                // S Shape
                {{save.rows, save.cols}, {save.rows+1, save.cols}, {save.rows+1, save.cols-1}, {save.rows,save.cols+1}},
                // Z Shape
                {{save.rows, save.cols}, {save.rows+1, save.cols}, {save.rows+1, save.cols+1}, {save.rows,save.cols-1}},
                // J Shape
                {{save.rows, save.cols}, {save.rows+1, save.cols}, {save.rows+2, save.cols}, {save.rows+2,save.cols-1}},
                // L Shape
                {{save.rows, save.cols}, {save.rows, save.cols-1}, {save.rows+1, save.cols}, {save.rows+2,save.cols}}
        };

        // +90 degrees rotated
        std::vector<std::vector<std::vector<int>>> tetraminoShapes_270rotated{
                // I Shape - rows, cols
                {{save.rows ,save.cols+1},{save.rows+1,save.cols+1}, {save.rows+2, save.cols+1}, {save.rows+3, save.cols+1}},
                // O Shape
                {{save.rows, save.cols}, {save.rows+1, save.cols}, {save.rows, save.cols+1}, {save.rows+1,save.cols+1}},
                // T Shape
                {{save.rows, save.cols}, {save.rows+1, save.cols}, {save.rows+2, save.cols}, {save.rows+1,save.cols-1}},
                // S Shape
                {{save.rows, save.cols}, {save.rows+1, save.cols}, {save.rows+1, save.cols+1}, {save.rows+2,save.cols+1}},
                // Z Shape
                {{save.rows, save.cols}, {save.rows+1, save.cols}, {save.rows+1, save.cols-1}, {save.rows+2,save.cols-1}},
                // J Shape
                {{save.rows, save.cols}, {save.rows+1, save.cols}, {save.rows+1, save.cols+1}, {save.rows+1,save.cols+2}},
                // L Shape
                {{save.rows, save.cols}, {save.rows+1, save.cols}, {save.rows+1, save.cols-1}, {save.rows+1,save.cols-2}}
        };


        if (mDegree == 0) {
            return tetraminoShapes[save.shapeSelected];
        } else if (mDegree == 90 || mDegree==-270) {
            return tetraminoShapes_90rotated[save.shapeSelected];
        } else if (mDegree == 180 || mDegree==-180) {
            return tetraminoShapes_180rotated[save.shapeSelected];
        } else if (mDegree == 270 || mDegree==-90) {
            return tetraminoShapes_270rotated[save.shapeSelected];
        } else if (mDegree > 270 || mDegree<-270) {
            mDegree = 0;
            return tetraminoShapes[save.shapeSelected];
        }
        return {};
    };


    void NexBlockShape(int shapeSelected, SSavedTetraminos& save, sf::RectangleShape& tetraminos, sf::RenderWindow& window) {
        std::vector<std::vector<std::vector<int>>> tetraminoShapes{
                // I Shape - rows, cols sleeping
                {{save.rows,save.cols-1 },{save.rows ,save.cols},{save.rows,save.cols+1}, {save.rows, save.cols-2}},
                // O Shape
                {{save.rows, save.cols}, {save.rows+1, save.cols}, {save.rows, save.cols+1}, {save.rows+1,save.cols+1}},
                // T Shape inverse shape
                {{save.rows, save.cols}, {save.rows+1, save.cols}, {save.rows+1, save.cols-1}, {save.rows+1,save.cols+1}},
                // S Shape
                {{save.rows, save.cols}, {save.rows+1, save.cols}, {save.rows+1, save.cols-1}, {save.rows,save.cols+1}},
                // Z Shape
                {{save.rows, save.cols}, {save.rows+1, save.cols}, {save.rows+1, save.cols+1}, {save.rows,save.cols-1}},
                // J Shape inverse
                {{save.rows, save.cols}, {save.rows+1, save.cols}, {save.rows+2, save.cols}, {save.rows,save.cols+1}},
                // L Shape
                {{save.rows, save.cols}, {save.rows+1, save.cols}, {save.rows+2, save.cols}, {save.rows+2,save.cols+1}}
        };

        auto drawFunction=[&](int shape, const sf::Color& color) {
            for (auto tetraminoDrawShape: tetraminoShapes[shape]) {
                tetraminos.setFillColor(color);
                tetraminos.setPosition(
                        {(float) tetraminoDrawShape[1] + 300, (float) tetraminoDrawShape[0]+ 100 });
                window.draw(tetraminos);
            }
        };


        switch(shapeSelected) {
            case 0:
                // Draw Shape
                // cols, rows
                drawFunction(shapeSelected, tetraminoColors[shapeSelected]);
                break;

            case 1:
                drawFunction(shapeSelected, tetraminoColors[shapeSelected]);
                break;

            case 2:
                // Purple Color
                drawFunction(shapeSelected, tetraminoColors[shapeSelected]);
                break;

            case 3:
                drawFunction(shapeSelected, tetraminoColors[shapeSelected]);
                break;

            case 4:
                drawFunction(shapeSelected, tetraminoColors[shapeSelected]);
                break;

            case 5:
                drawFunction(shapeSelected, tetraminoColors[shapeSelected]);
                break;

            case 6:
                // Orange Color
                drawFunction(shapeSelected, tetraminoColors[shapeSelected]);
                break;

        }
    }

    // Tetraminos Shapes
    void DrawTetraminosShapes(sf::RenderWindow& window, sf::RectangleShape& tetraminos, SSavedTetraminos& save) {
        // I Shape
        auto tetramino_shape=GetTetraminoCurrentShape(save);
        auto drawFunction=[&](int shape, const sf::Color& color) {
            for (const auto &tetraminoDrawShape: tetramino_shape) {
                tetraminos.setFillColor(color);
                tetraminos.setPosition(
                        {(float) tetraminoDrawShape[1] * BLOCK_SIZE, (float) tetraminoDrawShape[0] * BLOCK_SIZE});
                window.draw(tetraminos);
            }
        };

        switch(save.shapeSelected) {
            case 0:
                // Draw Shape
                // cols, rows
                drawFunction(save.shapeSelected, tetraminoColors[save.shapeSelected]);
                break;

            case 1:
                drawFunction(save.shapeSelected, tetraminoColors[save.shapeSelected]);
                break;

            case 2:
                // Purple Color
                drawFunction(save.shapeSelected, tetraminoColors[save.shapeSelected]);
                break;

            case 3:
                drawFunction(save.shapeSelected, tetraminoColors[save.shapeSelected]);
                break;

            case 4:
                drawFunction(save.shapeSelected, tetraminoColors[save.shapeSelected]);
                break;

            case 5:
                drawFunction(save.shapeSelected, tetraminoColors[save.shapeSelected]);
                break;

            case 6:
                // Orange Color
                drawFunction(save.shapeSelected, tetraminoColors[save.shapeSelected]);
                break;

        }

    }
    void StartGame() {
        sf::Clock clock;
        float totalTime = 0;
        float countDownTimer = 0.250;
        srand(time(0));
        SSavedTetraminos save;
        save.cols = (TETRIS_COLS/2);
        std::vector<SSavedTetraminos> savedTetraminos{};
        bool collisionDetection;
        bool groundDetection;
        int degreeClockWise=25;
        save.shapeSelected =  RandomShuffle(save, mTetraminos, mWindow);
        sf::Font font;
        font.loadFromFile("tetris_fonts.ttf");
        bool gameOverCheck;
        sf::Music music;
        if(!music.openFromFile("tetris_song.mp3")){
        }
//        music.play();

        while (mWindow.isOpen()) {
            // Handle Input
            //TODO HANDLE INPUT
            //ValidPosition
            sf::Event event;
            while (mWindow.pollEvent(event)) {

                if (event.type == sf::Event::Closed) {
                    mWindow.close();
//                    music.stop();
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                    save.rows=save.rows+1;

                    mScore+=5;
                    auto collisionAhead = false;
                    for(auto currentTetramino: GetTetraminoCurrentShape(save)) {
                        if(mBoardGridFillCheck[currentTetramino[0]][currentTetramino[1]]) {
                            save.rows=save.rows-1;
                            for (auto tetraminoShape: GetTetraminoCurrentShape(save)) {
                                savedTetraminos.push_back(
                                        {tetraminoShape[0], tetraminoShape[1], save.shapeSelected});
                                mBoardGridFillCheck[tetraminoShape[0]][tetraminoShape[1]] = true;
                            }
                            save.shapeSelected = RandomShuffle(save, mTetraminos, mWindow);
                            save.cols = (TETRIS_COLS / 2);
                            save.rows = 0;
                            collisionAhead=true;
                            break;
                        }
                        if(currentTetramino[0]>=TETRIS_ROWS){
                            save.rows=save.rows-1;
                            for (auto tetraminoShape: GetTetraminoCurrentShape(save)) {
                                savedTetraminos.push_back(
                                        {tetraminoShape[0], tetraminoShape[1], save.shapeSelected});
                                mBoardGridFillCheck[tetraminoShape[0]][tetraminoShape[1]] = true;
                            }
                            save.shapeSelected = RandomShuffle(save, mTetraminos, mWindow);
                            save.cols = (TETRIS_COLS / 2);
                            save.rows = 0;
                            collisionDetection=true;
                            break;
                        }
                    }
                    if(collisionAhead){
                        for (auto tetraminos: savedTetraminos) {
                            mTetraminos.setFillColor(tetraminoColors[tetraminos.shapeSelected]);
                            mTetraminos.setPosition(
                                    {(float) tetraminos.cols * BLOCK_SIZE, (float) tetraminos.rows * BLOCK_SIZE});
                            mWindow.draw(mTetraminos);
                        }
                        continue;
                    }
                }

                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                    save.cols++;
                    auto newTetraBlockAdded = false;
                    for (auto currentTetramino: GetTetraminoCurrentShape(save)) {
                        if(currentTetramino[1]>=TETRIS_COLS){
                            --save.cols;
                            break;
                        }
                        if(mBoardGridFillCheck[currentTetramino[0]][currentTetramino[1]]){
                            --save.cols;
                            break;
                        }
                    }
                }

                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                    save.cols--;
                    auto newTetraBlockAdded = false;
                    for (auto currentTetramino: GetTetraminoCurrentShape(save)) {
                        if(currentTetramino[1]<0){
                            save.cols++;
                            break;
                        }
                        if(mBoardGridFillCheck[currentTetramino[0]][currentTetramino[1]]){
                            save.cols++;
                            break;
                        }
                    }

                }

                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
                    // ClockWise Rotation
                    mDegree+=90;
                    auto newTetraBlockAdded = false;
                    for (auto currentTetramino: GetTetraminoCurrentShape(save)) {
                        if(currentTetramino[1]<0 || currentTetramino[1]>=TETRIS_COLS){
                            mDegree-=90;
                            break;
                        }
                        if(mBoardGridFillCheck[currentTetramino[0]][currentTetramino[1]]){
                            mDegree-=90;
                            break;
                        }
                    }
                }
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::E) {
                    // ClockWise Rotation
                    mDegree-=90;
                    auto newTetraBlockAdded = false;
                    for (auto currentTetramino: GetTetraminoCurrentShape(save)) {
                        if(currentTetramino[1]<0 || currentTetramino[1]>=TETRIS_COLS){
                            mDegree+=90;
                            break;
                        }
                        if(mBoardGridFillCheck[currentTetramino[0]][currentTetramino[1]]){
                            mDegree+=90;
                            break;
                        }
                    }
                }
            }

            // Handle Logic
            //TODO HANDLE LOGIC
            collisionDetection=false;
            groundDetection=false;
            auto framePerSec = clock.restart();
            totalTime += framePerSec.asSeconds();
            countDownTimer -= framePerSec.asSeconds();

            // Physics Engine:- Is a set of rules
            if (countDownTimer < 0) {
                countDownTimer = 0.250;
                save.rows++;

                // Ground Detection
                for (const auto& tetraminoShape: GetTetraminoCurrentShape(save)) {
                    if(tetraminoShape[0]>=TETRIS_ROWS){
                        groundDetection = true;
                        break;
                    }
                }

                if(groundDetection) {
                    save.rows=save.rows-1;
                    for(const auto& tetraminoShape: GetTetraminoCurrentShape(save)) {
                        savedTetraminos.push_back({tetraminoShape[0], tetraminoShape[1], save.shapeSelected});
                        mBoardGridFillCheck[tetraminoShape[0]][tetraminoShape[1]]=true;
                    }

                    save.rows = 0;
                    save.cols = (TETRIS_COLS/2);
                    save.shapeSelected = RandomShuffle(save, mTetraminos, mWindow);
                }

                for(auto collision: savedTetraminos){
                    if(collision.rows == 0 || collision.rows == 1){
                        gameOverCheck = true;
                    }
                }

                if((!groundDetection)) {
                        for (auto collision: savedTetraminos) {
                            for (auto currentTetramino: GetTetraminoCurrentShape(save)) {
                                if ((currentTetramino[0] + 1 == collision.rows &&
                                     (currentTetramino[1] == collision.cols)) ||
                                    mBoardGridFillCheck[currentTetramino[0]][currentTetramino[1]]) {
                                    collisionDetection = true;
                                    break;
                                }
                            }
                            if (collisionDetection) {
                                for (auto tetraminoShape: GetTetraminoCurrentShape(save)) {
                                    savedTetraminos.push_back(
                                            {tetraminoShape[0], tetraminoShape[1], save.shapeSelected});
                                    mBoardGridFillCheck[tetraminoShape[0]][tetraminoShape[1]] = true;
                                }
                                save.shapeSelected = RandomShuffle(save, mTetraminos, mWindow);
                                save.cols = (TETRIS_COLS / 2);
                                save.rows = 0;
                                break;
                            }
                        }
                }
            }



            //Line Clearence has two parts removing the saved blocks in the line and incrementing the value of rows to 1.
            for(int rows=0; rows<TETRIS_ROWS;rows++) {
                std::vector<bool> lineFillCheckRowWise;
                bool checkRowTrue = false;
                for(int cols=0; cols<TETRIS_COLS; cols++) {
                    lineFillCheckRowWise.push_back(mBoardGridFillCheck[rows][cols]);
                }
                int cols = 0;
                checkRowTrue = std::all_of(lineFillCheckRowWise.begin(), lineFillCheckRowWise.end(), [](bool result) {
                    return result;
                });
                if (checkRowTrue) {
                    mScore+=100;
                    while(cols<TETRIS_COLS) {
                        savedTetraminos.erase(std::remove_if(savedTetraminos.begin(), savedTetraminos.end(),
                                    [cols, rows](SSavedTetraminos& tetraminos) {
                                        return (tetraminos.rows == rows && tetraminos.cols == cols);
                                    }), savedTetraminos.end());
                        mBoardGridFillCheck[rows][cols]=false;
                        cols++;
                    }
                    for(int i = 0; i < savedTetraminos.size(); i++) {
                        if(savedTetraminos[i].rows < rows) {
                            mBoardGridFillCheck[savedTetraminos[i].rows][savedTetraminos[i].cols] = false;
                            savedTetraminos[i].rows+=1;
                            mBoardGridFillCheck[savedTetraminos[i].rows][savedTetraminos[i].cols] = true;
                        }
                    }
                }
            }

            // Render
            //TODO RENDERING
            mWindow.clear();

            // Game Over
            if(gameOverCheck){
//                music.stop();
                sf::Text scoreText(font, "Total Score: " + std::to_string(mScore));
                scoreText.setFont(font);
                scoreText.setCharacterSize(30);
                scoreText.setStyle(sf::Text::Style::Bold);
                scoreText.setFillColor(sf::Color::White);
                scoreText.setPosition({(float) 80, (float) 150});
                mWindow.draw(scoreText);

                sf::Text gameOver(font, "GAME OVER!!");
                gameOver.setFont(font);
                gameOver.setCharacterSize(90);
                gameOver.setStyle(sf::Text::Style::Bold);
                gameOver.setFillColor(sf::Color::White);
                gameOver.setPosition({(float) 100, (float) 300});
                mWindow.draw(gameOver);
                mWindow.display();
                sf::sleep(sf::seconds(10));
                break;
            }

            // Drawing a BluePrint Grid
            for(int cols =0; cols < TETRIS_COLS;cols++) {
                for (int rows=0; rows < TETRIS_ROWS; rows++) {
                    mTetraminos.setFillColor(sf::Color(73, 73, 85));
                    mTetraminos.setPosition({(float) BLOCK_SIZE*cols, (float) BLOCK_SIZE*rows});
                    mWindow.draw(mTetraminos);
                }
            }

            // Score Card
            sf::Text scoreText(font, "Score: "+std::to_string(mScore));
            scoreText.setFont(font);
            scoreText.setCharacterSize(60);
            scoreText.setStyle(sf::Text::Style::Bold);
            scoreText.setFillColor(sf::Color::White);
            scoreText.setPosition({(float) 400, (float) 10});
            mWindow.draw(scoreText);

            // Line Border Addition
            sf::Vector2f startPoint(385.0f, 0.0f);
            sf::Vector2f endPoint(700.0f, 700.0f);
            float thickness = 2.0f;
            sf::Color lineColor(sf::Color::White);
            sf::RectangleShape line;
            line.setSize(sf::Vector2f(thickness, endPoint.y - startPoint.y));
            line.setFillColor(lineColor);
            line.setPosition(startPoint);
            mWindow.draw(line);

            // Drawing Saved Tetraminos
            for (auto tetraminos: savedTetraminos) {
                mTetraminos.setFillColor(tetraminoColors[tetraminos.shapeSelected]);
                mTetraminos.setPosition(
                        {(float) tetraminos.cols * BLOCK_SIZE, (float) tetraminos.rows * BLOCK_SIZE});
                mWindow.draw(mTetraminos);
            }

            // Falling Blocks
            if(!collisionDetection) {
                 DrawTetraminosShapes(mWindow, mTetraminos, save);
                collisionDetection=false;
            }

            mWindow.draw(mTetraminos);


            mWindow.display();
        }
    }

private:
    sf::RenderWindow mWindow;
    sf::RectangleShape mTetraminos;
    bool mBoardGridFillCheck[TETRIS_ROWS][TETRIS_COLS] = {false};
    std::vector<int> mMemoryTetraminos;

    int mDegree=0;
    unsigned int mScore = 0;
};

int main() {
    CTetrisGame tetrisGame;
    tetrisGame.StartGame();
}
