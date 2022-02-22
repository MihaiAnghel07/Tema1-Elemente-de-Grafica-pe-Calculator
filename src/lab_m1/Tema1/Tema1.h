#pragma once

#include "components/simple_scene.h"
#include "utils/glm_utils.h"


namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {   // structura viewport - ului
        struct ViewportSpace
        {
            ViewportSpace() : x(0), y(0), width(1), height(1) {}
            ViewportSpace(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {}
            int x;
            int y;
            int width;
            int height;
        };

        // structura spatiului logic
        struct LogicSpace
        {  
            LogicSpace() : x(0), y(0), width(1), height(1) {}
            LogicSpace(float x, float y, float width, float height)
                : x(x), y(y), width(width), height(height) {}
            float x;
            float y;
            float width;
            float height;
        };

        //structura unui inamic
        typedef struct myEnemy {
            float x, y;
            int available;
            float radiansEnemy;
            float speed;
        }myEnemy;

    public:
        Tema1();
        ~Tema1();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;
        
        void DrawScene(glm::mat3 visMatrix, float deltaTimeSeconds);
        void DrawEnemy(glm::mat3 visMatrix);
        
        void generateEnemyStartPosition();
        void attackPlayer(myEnemy enemyStruct[], float deltaTimeSeconds);
        void shootBullet(float deltaTimeSeconds);
        bool isUpperCollision(float upper, float bottom, float left, float right);
        bool isBottomCollision(float upper, float bottom, float left, float right);
        bool isRightCollision(float upper, float bottom, float left, float right);
        bool isLeftCollision(float upper, float bottom, float left, float right);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        glm::mat3 VisualizationTransf2D(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);
        void SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor = glm::vec3(0), bool clear = true);

    protected:
   
        ViewportSpace viewSpace;
        LogicSpace logicSpace;
        glm::mat3 modelMatrix, visMatrix;

        // lungimile obiectelor
        float lengthSquare1, lengthMap, lengthPlayer, lengthBullet;
        // coordonatele player-ului
        float playerX, playerY;
        // limitele player-ului
        float playerUpperY, playerBottomY, playerRightX, playerLeftX;
        // factorii de scalare ai player- ului
        float sx, sy;
        // coordonatele proiectilului
        float bulletX, bulletY;
        // limitele player-ului
        float bulletUpperY, bulletBottomY, bulletRightX, bulletLeftX;
        // viteza proiectilului
        float bulletSpeed;
        // coordonatele health bar-ului
        float healthX, healthY;
        // factorii de scalare ai health bar-ului
        float healthScaleX, healthScaleY;
        // unghiul cu care trebuie player-ul sa se roteasca spre mouse
        float radiansPlayer;
        // coordonatele inamicului
        float enemyX, enemyY;
        // viteza inamicului
        float enemySpeed;
        // variabila care anunta tragerea unui proiectil
        bool fire;
        // factorul cu care scade viata cand este atacat player-ul de inamici
        float healthDecreaseFactor;
        // limitele unui inamic
        float enemyUpperY, enemyBottomY, enemyRightX, enemyLeftX;
        // factorii de scalare ai unui inamic
        float enemyScaleX, enemyScaleY;
        // flag-urie le folosesc pentru diferite metode de realizare a task-urilor
        int flag;
        int flag2;
        int flag3;
        // variabila in care calculez secundele
        int sec;
        // distanta pe care o are de parcurs proiectilul pana la tinta
        float deltaY, deltaX;
        // tinta proiectilului
        float newBulletX, newBulletY;
        // viteza proiectilului pe axe
        float speedBulletX, speedBulletY;
        // unghiul proiectilului
        float radiansBullet;
        // variabila in care calculez un index random pentru activarea unui inamic
        int randomIndex;
        // punctajul player - ului
        int score;
        // flag care anunta daca e sfarsitul jocului
        int gameOver;
        float fireRate;
        // factorii de scalare ai mapei 
        float mapScaleX;
        float mapScaleY;
        // array cu inamici
        myEnemy enemyStruct[10];
        // variabile cu care calculez anumite perioade de timp
        clock_t start, start2;
        clock_t end, end2;    
    };
}