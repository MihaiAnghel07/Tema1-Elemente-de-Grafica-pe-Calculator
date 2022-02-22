
#include <vector>
#include <iostream>
#include "lab_m1/Tema1/Tema1.h"
#include "lab_m1/Tema1/object2D.h"
#include "lab_m1/Tema1/transform2D.h"
#include <math.h>
 


using namespace std;
using namespace m1;


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
    auto camera = GetSceneCamera();
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    // coordonatele viewport-ului
    logicSpace.x = 0;     
    logicSpace.y = 0;       
    logicSpace.width = 3.8f; 
    logicSpace.height = 3.8f;  

    // corner este un vector cu coordonatele punctului de unde vreau sa inceapa 
    // crearea obiectelor
    glm::vec3 corner = glm::vec3(0, 0, 0);
    
    lengthSquare1 = 1;
    lengthMap = 5;
    lengthPlayer = 1;
    lengthBullet = 0.1f;

    playerX = 2.2f;
    playerY = 2;
    sx = 0.3f;
    sy = 0.3f;
    radiansPlayer = 0;
    
    enemyX = 1;
    enemyY = 1;
    enemyScaleX = sx / 1.5f;
    enemyScaleY = sy / 1.5f;

    fire = false;
    bulletX = playerX;
    bulletY = playerY;
    speedBulletX = 1;
    speedBulletY = 1;
    radiansBullet = 0;
    bulletSpeed = 1.5f;
    
    healthX = 3.1f;
    healthY = 3.3;
    healthScaleX = 1;
    healthScaleY = 0.15f;
    healthDecreaseFactor = 0;
    
    mapScaleX = 1;
    mapScaleY = 1.1f;

    flag = 0;
    flag2 = 0;
    flag3 = 0;
    
    score = 0;
    gameOver = 0;
    // fireRate in secunde
    fireRate = 0.5f;
    

    // square1 il folosesc pentru crearea obstacolelor
    Mesh* square1 = object2D::CreateSquare("square1", corner, lengthSquare1, glm::vec3(0, 0.9f, 0), true);
    Mesh* map = object2D::CreateSquare("map", corner, lengthMap, glm::vec3(0.7f, 0.7f, 0.7f), true);
    Mesh* health = object2D::CreateSquare("health", corner, lengthSquare1, glm::vec3(1, 0, 0), true);
    Mesh* bullet = object2D::CreateSquare("bullet", corner, lengthBullet, glm::vec3(0, 0, 0), true);
    Mesh* healthFrame = object2D::CreateHealthBarFrame("healthFrame", corner, lengthSquare1, glm::vec3(1, 0, 0), true);
    Mesh* player = object2D::CreatePlayer("player", corner, lengthPlayer, glm::vec3(1, 1, 0.2f), glm::vec3(1, 0.5f, 0), true);
    Mesh* enemy = object2D::CreateEnemy("enemy", corner, lengthPlayer, glm::vec3(0.76f, 0, 0.152f), glm::vec3(1, 0, 0), true);
    
    AddMeshToList(square1);
    AddMeshToList(map);
    AddMeshToList(health);
    AddMeshToList(healthFrame);
    AddMeshToList(player);
    AddMeshToList(enemy);
    AddMeshToList(bullet);

    // setez o pozitie initiala si o viteza random pentru inamici
    for (int i = 0; i < 10; i++) {
        enemyStruct[i].available = 0;
        enemyStruct[i].x = rand() % 6;
        enemyStruct[i].y = rand() % 6;
        enemyStruct[i].radiansEnemy = 0;
        enemyStruct[i].speed = (float)((rand() % 120) + 1) / 100;
    }
    enemyStruct[0].available = 1;
    start = clock();
}

// Functia calculeaza matricea de vizualizare (de transformare fereastra - poarta) 
glm::mat3 Tema1::VisualizationTransf2D(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
{
    float sx = viewSpace.width / logicSpace.width;
    float sy = viewSpace.height / logicSpace.height;
    float tx = viewSpace.x - sx * logicSpace.x;
    float ty = viewSpace.y - sy * logicSpace.y;

    return glm::transpose(glm::mat3(
        sx, 0.0f, tx,
        0.0f, sy, ty,
        0.0f, 0.0f, 1.0f));
}


void Tema1::SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor, bool clear)
{
    glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    glEnable(GL_SCISSOR_TEST);
    glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    glClearColor(colorColor.r, colorColor.g, colorColor.b, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);

    GetSceneCamera()->SetOrthographic((float)viewSpace.x, (float)(viewSpace.x + viewSpace.width), (float)viewSpace.y, (float)(viewSpace.y + viewSpace.height), 0.1f, 400);
    GetSceneCamera()->Update();
}


void Tema1::FrameStart()
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();

    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema1::Update(float deltaTimeSeconds)
{  
    glm::ivec2 resolution = window->GetResolution();

    // setez dimensiunile viwport-ului si locul unde este plasat
    viewSpace = ViewportSpace(resolution.x / 4, 0, resolution.x / 2, resolution.y);
    SetViewportArea(viewSpace, glm::vec3(0.5f), true);

    // calculez matricea de vizualizare
    visMatrix = glm::mat3(1);
    visMatrix *= VisualizationTransf2D(logicSpace, viewSpace);
    
    DrawScene(visMatrix, deltaTimeSeconds);
    generateEnemyStartPosition();
    attackPlayer(enemyStruct, deltaTimeSeconds);
    if (gameOver == 1) {
        cout << "GAME OVER!" << endl;
        cout << "Your SCORE is: " << score << endl;
        exit(-1);
    }
}

void Tema1::DrawScene(glm::mat3 visMatrix, float deltaTimeSeconds)
{   
    
    modelMatrix = visMatrix * transform2D::Translate(healthX + healthDecreaseFactor / 2, healthY) * transform2D::Scale(healthScaleX - healthDecreaseFactor, healthScaleY) * transform2D::Translate(-(lengthSquare1 / 2), -(lengthSquare1 / 2));
    RenderMesh2D(meshes["health"], shaders["VertexColor"], modelMatrix);
    
    modelMatrix = visMatrix * transform2D::Translate(healthX, healthY) * transform2D::Scale(healthScaleX, healthScaleY) * transform2D::Translate(-(lengthSquare1 / 2), -(lengthSquare1 / 2));
    RenderMesh2D(meshes["healthFrame"], shaders["VertexColor"], modelMatrix);
    
    if (fire) {
        if (flag == 0) {
            radiansBullet = -radiansPlayer;
        }
        shootBullet(deltaTimeSeconds);
        modelMatrix = visMatrix * transform2D::Translate(bulletX, bulletY) * transform2D::Rotate(radiansBullet) * transform2D::Translate(-0.02f, 0.2f);
        RenderMesh2D(meshes["bullet"], shaders["VertexColor"], modelMatrix);
    }

    modelMatrix = visMatrix * transform2D::Scale(1.3f, 0.3f) * transform2D::Translate(0, 0);
    RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix * transform2D::Translate(4 + (lengthSquare1 / 2), 0.2f + (lengthSquare1 / 2)) * transform2D::Scale(0.3f, 1.4f) * transform2D::Translate(-(lengthSquare1 / 2), -(lengthSquare1 / 2));
    RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix * transform2D::Translate(1.5, 1.5) * transform2D::Scale(0.5f, 0.5f) * transform2D::Translate(0, 0);
    RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix * transform2D::Translate(0.5f, 3.5f) * transform2D::Scale(0.5f, 2) * transform2D::Translate(-(lengthSquare1 / 2), -(lengthSquare1 / 2));
    RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix * transform2D::Translate(3, 3);
    RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix * transform2D::Translate(playerX, playerY) * transform2D::Rotate(-radiansPlayer) * transform2D::Scale(sx, sy) * transform2D::Translate(-(lengthPlayer / 2), -(lengthPlayer / 2));
    RenderMesh2D(meshes["player"], shaders["VertexColor"], modelMatrix);

    DrawEnemy(visMatrix);
    
    // desenez harta (la final pentru ca celelalte elemente sa se poata suprapuna peste harta)
    // deci harta va avea punctele definitorii: (0, 0) stanga jos, si (5, 5.5) dreapta sus
    modelMatrix = visMatrix * transform2D::Translate((lengthMap / 2), (lengthMap / 2) + 0.25f) * transform2D::Scale(mapScaleX, mapScaleY) * transform2D::Translate(-(lengthMap / 2), -(lengthMap / 2));
    RenderMesh2D(meshes["map"], shaders["VertexColor"], modelMatrix);
    
}

void Tema1::shootBullet(float deltaTimeSeconds)
{   
    /* proiectilul poate atinge o anumita distanta daca nu intalneste nimic
     calculez acea distanta, implicit noile coordonate unde trebuie sa 
     ajunga in cazul in care nu intalneste un inamic / obstacol*/
    if (flag == 0) {
        flag = 1;
        bulletX = playerX;
        bulletY = playerY;
        deltaY = bulletSpeed * cos(radiansPlayer);
        deltaX = bulletSpeed * sin(radiansPlayer);
        newBulletX = bulletX + deltaX;
        newBulletY = bulletY + deltaY;
        speedBulletX = 1;
        speedBulletY = 1;

        // pentru a merge cu o viteza linniara, calculez un factor de scalara (o viteza)
        // pentru fiecare axa
        if (abs(newBulletX - bulletX) > abs(newBulletY - bulletY)) {
            speedBulletY = abs(newBulletY - bulletY) / abs(newBulletX - bulletX);
            if (speedBulletY == 0)
                speedBulletY = 1;
        }
        else {
            speedBulletX = abs(newBulletX - bulletX) / abs(newBulletY - bulletY);
            if (speedBulletX == 0)
                speedBulletX = 1;
        }
    }

    // calculez limitele proiectilului
    bulletUpperY = bulletY + 0.15f;
    bulletBottomY = bulletY - 0.15f;
    bulletLeftX = bulletX - 0.15f;
    bulletRightX = bulletX + 0.15;

    /* verific coliziunile proiectilului cu marginea mapei sau cu obiecte statice
     coliziunea cu inamicii se calculeaza in momentul in care sunt inamici in joc
     daca intalnesc o coliziune, proiectilul dispare*/
    if (((abs(bulletX - newBulletX) < 0.02 && abs(bulletY - newBulletY) < 0.02)) 
        || isUpperCollision(bulletUpperY, bulletBottomY, bulletLeftX, bulletRightX)
        || isBottomCollision(bulletUpperY, bulletBottomY, bulletLeftX, bulletRightX)
        || isLeftCollision(bulletUpperY, bulletBottomY, bulletLeftX, bulletRightX)
        || isRightCollision(bulletUpperY, bulletBottomY, bulletLeftX, bulletRightX)) {
        fire = false;
        flag = 0;
    }
    else {
        // fac deplasarea proiectilului daca nu exista coliziune
        if (bulletX < newBulletX)
            bulletX += speedBulletX * 1.5f * deltaTimeSeconds;
        if (bulletX > newBulletX)
            bulletX -= speedBulletX * 1.5f * deltaTimeSeconds;
        if (bulletY < newBulletY)
            bulletY += speedBulletY * 1.5f * deltaTimeSeconds;
        if (bulletY > newBulletY)
            bulletY -= speedBulletY * 1.5f * deltaTimeSeconds;
    }
    
}


void Tema1::DrawEnemy(glm::mat3 visMatrix)
{   
    // generez un nou inamic o data la 3 secunde 
    end = clock();
    sec = (int)trunc(((float)end - start) / CLOCKS_PER_SEC);
    if (sec % 3 == 0) {
        if (flag2 == 0) {
            flag2 = 1;
            randomIndex = rand() % 10;
            while (enemyStruct[randomIndex].available == 1) {
                randomIndex = rand() % 10;
            }
            enemyStruct[randomIndex].available = 1;
        }
    } 
    else {
        flag2 = 0;
    }
    
    if (enemyStruct[0].available) {
        modelMatrix = visMatrix * transform2D::Translate(enemyStruct[0].x, enemyStruct[0].y) * transform2D::Rotate(-enemyStruct[0].radiansEnemy) * transform2D::Scale(enemyScaleX, enemyScaleY) * transform2D::Translate(-(lengthPlayer / 2), -(lengthPlayer / 2));
        RenderMesh2D(meshes["enemy"], shaders["VertexColor"], modelMatrix);
    }

    if (enemyStruct[1].available) {
        modelMatrix = visMatrix * transform2D::Translate(enemyStruct[1].x, enemyStruct[1].y) * transform2D::Rotate(-enemyStruct[1].radiansEnemy) * transform2D::Scale(enemyScaleX, enemyScaleY) * transform2D::Translate(-(lengthPlayer / 2), -(lengthPlayer / 2));
        RenderMesh2D(meshes["enemy"], shaders["VertexColor"], modelMatrix);
    }
    
    if (enemyStruct[2].available) {
        modelMatrix = visMatrix * transform2D::Translate(enemyStruct[2].x, enemyStruct[2].y) * transform2D::Rotate(-enemyStruct[2].radiansEnemy) * transform2D::Scale(enemyScaleX, enemyScaleY) * transform2D::Translate(-(lengthPlayer / 2), -(lengthPlayer / 2));
        RenderMesh2D(meshes["enemy"], shaders["VertexColor"], modelMatrix);
    }
    
    if (enemyStruct[3].available) {
        modelMatrix = visMatrix * transform2D::Translate(enemyStruct[3].x, enemyStruct[3].y) * transform2D::Rotate(-enemyStruct[3].radiansEnemy) * transform2D::Scale(enemyScaleX, enemyScaleY) * transform2D::Translate(-(lengthPlayer / 2), -(lengthPlayer / 2));
        RenderMesh2D(meshes["enemy"], shaders["VertexColor"], modelMatrix);
    }
    
    if (enemyStruct[4].available) {
        modelMatrix = visMatrix * transform2D::Translate(enemyStruct[4].x, enemyStruct[4].y) * transform2D::Rotate(-enemyStruct[4].radiansEnemy) * transform2D::Scale(enemyScaleX, enemyScaleY) * transform2D::Translate(-(lengthPlayer / 2), -(lengthPlayer / 2));
        RenderMesh2D(meshes["enemy"], shaders["VertexColor"], modelMatrix);
    }
    
    if (enemyStruct[5].available) {
        modelMatrix = visMatrix * transform2D::Translate(enemyStruct[5].x, enemyStruct[5].y) * transform2D::Rotate(-enemyStruct[5].radiansEnemy) * transform2D::Scale(enemyScaleX, enemyScaleY) * transform2D::Translate(-(lengthPlayer / 2), -(lengthPlayer / 2));
        RenderMesh2D(meshes["enemy"], shaders["VertexColor"], modelMatrix);
    }
    
    if (enemyStruct[6].available) {
        modelMatrix = visMatrix * transform2D::Translate(enemyStruct[6].x, enemyStruct[6].y) * transform2D::Rotate(-enemyStruct[6].radiansEnemy) * transform2D::Scale(enemyScaleX, enemyScaleY) * transform2D::Translate(-(lengthPlayer / 2), -(lengthPlayer / 2));
        RenderMesh2D(meshes["enemy"], shaders["VertexColor"], modelMatrix);
    }
    
    if (enemyStruct[7].available) {
        modelMatrix = visMatrix * transform2D::Translate(enemyStruct[7].x, enemyStruct[7].y) * transform2D::Rotate(-enemyStruct[7].radiansEnemy) * transform2D::Scale(enemyScaleX, enemyScaleY) * transform2D::Translate(-(lengthPlayer / 2), -(lengthPlayer / 2));
        RenderMesh2D(meshes["enemy"], shaders["VertexColor"], modelMatrix);
    }
    
    if (enemyStruct[8].available) {
        modelMatrix = visMatrix * transform2D::Translate(enemyStruct[8].x, enemyStruct[8].y) * transform2D::Rotate(-enemyStruct[8].radiansEnemy) * transform2D::Scale(enemyScaleX, enemyScaleY) * transform2D::Translate(-(lengthPlayer / 2), -(lengthPlayer / 2));
        RenderMesh2D(meshes["enemy"], shaders["VertexColor"], modelMatrix);
    }
    
    if (enemyStruct[9].available) {
        modelMatrix = visMatrix * transform2D::Translate(enemyStruct[9].x, enemyStruct[9].y) * transform2D::Rotate(-enemyStruct[9].radiansEnemy) * transform2D::Scale(enemyScaleX, enemyScaleY) * transform2D::Translate(-(lengthPlayer / 2), -(lengthPlayer / 2));
        RenderMesh2D(meshes["enemy"], shaders["VertexColor"], modelMatrix);
    }
}


void Tema1::FrameEnd()
{
}


void Tema1::attackPlayer(myEnemy enemyStruct[], float deltaTimeSeconds)
{   
    /* parcurg lista cu inamici, celor activi (prezenti in joc) le actualizez pozitia
     apropiindu-i de player (puteam sa fac miscarea inamicilor la fel ca cea a proiectilului,
     insa aceasta miscare mi s-a parut oarecum mai interesanta si mai putin predictibila)*/
    for (int i = 0; i < 10; i++) {
        if (enemyStruct[i].available) {
            // calculez limitele inamicilor
            enemyUpperY = enemyStruct[i].y + (enemyScaleY * (lengthPlayer + 0.1f) / 2);
            enemyBottomY = enemyStruct[i].y - (enemyScaleY * lengthPlayer / 2);
            enemyRightX = enemyStruct[i].x + (enemyScaleX * (lengthPlayer + 0.1f) / 2);
            enemyLeftX = enemyStruct[i].x - (enemyScaleX * lengthPlayer / 2);

            // actualizez pozitiile + verific coliziunea cu mapa
            if (enemyStruct[i].x < playerX && enemyRightX < (mapScaleY * lengthMap)) {
                enemyStruct[i].x += enemyStruct[i].speed * deltaTimeSeconds;
            }
            if (enemyStruct[i].x > playerX && enemyLeftX > 0) {
                enemyStruct[i].x -= enemyStruct[i].speed * deltaTimeSeconds;
            }
            if (enemyStruct[i].y < playerY && enemyUpperY < (mapScaleY * lengthMap)) {
                enemyStruct[i].y += enemyStruct[i].speed * deltaTimeSeconds;
            }
            if (enemyStruct[i].y > playerY && enemyBottomY > 0) {
                enemyStruct[i].y -= enemyStruct[i].speed * deltaTimeSeconds;
            }

            /* coliziunea dintre player si inamici am implementat-o considerand coliziune in momentul
             in care coordonatele inamicului sunt la o apropiere de sub 10% de coordonatele player-ului, 
             extremitatile se vor suprapune intr-o anumita masura)*/
            if (abs(enemyStruct[i].x - playerX) < 0.1 && abs(enemyStruct[i].y - playerY) < 0.1) {
                enemyStruct[i].available = 0;
                healthDecreaseFactor += 0.1f;
                if (healthScaleX - healthDecreaseFactor < 0.01f) {
                    gameOver = 1;
                }
            }

            // coliziunea dintre proiectil si inamici am implementat-o asemanator coordonatele player-ului
            if (abs(enemyStruct[i].x - bulletX) < 0.2f && abs(enemyStruct[i].y - bulletY) < 0.2f && fire) {
                enemyStruct[i].available = 0;
                fire = false;
                flag = 0;
                score += 10;
                cout << "SCORE = " << score << endl;
            }
            
            /* calculez unghiul cu care trebuie sa se roteasca inamicul pentru
             a fi cu fata catre player. Offset-ul '60' a fost pus din aceleasi motive ca la calcularea 
             unghiului dintre player si coordonatele mouse-ului*/
            enemyStruct[i].radiansEnemy = RADIANS(atan2(playerX - enemyStruct[i].x, playerY - enemyStruct[i].y)) * 60;
            
        }
    }
}


void Tema1::generateEnemyStartPosition()
{
    // de cate ori se genereaza un inamic, i se genereaza o noua pozitie de start
    // si o noua viteza
    for (int i = 0; i < 10; i++) {
        if (enemyStruct[i].available == 0) {
            enemyStruct[i].x = rand() % 6;
            enemyStruct[i].y = rand() % 6;
            enemyStruct[i].speed = (float)((rand() % 120) + 1) / 100;

        }
    }
}

// functie folosita de catre player si proiectil pentru a verifica daca apare o coliziune superioara
bool Tema1::isUpperCollision(float upper, float bottom, float left, float right)
{   
    
    if (upper >= (mapScaleY * lengthMap)
        || (upper >= 1.5f && upper <= 2 && ((left <= 2 && left >= 1.5) || (right >= 1.5 && right <= 2)))
        || (upper >= 3 && upper <= 4 && ((left <= 4 && left >= 3) || (right >= 3 && right <= 4)))
        || (upper >= 2.47f && upper <= 4.5 && ((left <= 0.75f && left >= 0.25f) || (right >= 0.25f && right <= 0.75f))))
        return true;
    
    return false;
}

// functie folosita de catre player si proiectil pentru a verifica daca apare o coliziune inferioara
bool Tema1::isBottomCollision(float upper, float bottom, float left, float right)
{   
    
    if (bottom <= 0
        || (bottom >= 1.5f && bottom <= 2 && ((left <= 2 && left >= 1.5) || (right >= 1.5 && right <= 2)))
        || (bottom <= 0.3f && left <= 1.3f)
        || (bottom <= 1.4f && ((left <= 4.65f && left >= 4.3f) || (right >= 4.35f && right <= 4.7f)))
        || (bottom >= 3 && bottom <= 4 && ((left <= 4 && left >= 3) || (right >= 3 && right <= 4)))
        || (bottom >= 2.5f && bottom <= 4.5 && ((left <= 0.75f && left >= 0.25f) || (right >= 0.25f && right <= 0.75f))))
        return true;
   
    return false;

}

// functie folosita de catre player si proiectil pentru a verifica daca apare o coliziune la dreapta
bool Tema1::isRightCollision(float upper, float bottom, float left, float right)
{
    
    if (right >= lengthMap
        || (right >= 1.5f && right <= 2 && ((upper <= 2 && upper >= 1.5) || (bottom >= 1.5 && bottom <= 2)))
        || (right >= 4.33f && right <= 4.6f && bottom <= 1.4f)
        || (right >= 3 && right <= 4 && ((upper <= 4 && upper >= 3) || (bottom >= 3 && bottom <= 4))))
        return true;
    
    return false;
}

// functie folosita de catre player si proiectil pentru a verifica daca apare o coliziune la stanga
bool Tema1::isLeftCollision(float upper, float bottom, float left, float right)
{
    
    if (left <= 0 
        || (left >= 1.5f && left <= 2 && ((upper <= 2 && upper >= 1.5) || (bottom >= 1.5 && bottom <= 2)))
        || (left <= 1.3f && bottom <= 0.3f)
        || (left >= 4.33f && left <= 4.68f && bottom <= 1.4f)
        || (left >= 3 && left <= 4 && ((upper <= 4 && upper >= 3) || (bottom >= 3 && bottom <= 4)))
        || (left >= 0.25f && left <= 0.75 && ((upper <= 4.5 && upper >= 2.5) || (bottom >= 2.5 && bottom <= 4.5))))
        return true;
    
    return false;
}


void Tema1::OnInputUpdate(float deltaTime, int mods)
{   
    playerUpperY = playerY + (sy * (lengthPlayer + 0.1f) / 2);
    playerBottomY = playerY - (sy * lengthPlayer / 2);
    playerRightX = playerX + (sx * (lengthPlayer + 0.4f) / 2);
    playerLeftX = playerX - (sx * lengthPlayer / 2);

    if (window->KeyHold(GLFW_KEY_W)) {
        if (!isUpperCollision(playerUpperY, playerBottomY, playerLeftX, playerRightX)) {
            logicSpace.y += deltaTime;
            playerY += deltaTime;
            healthY += deltaTime;
        }
    }

    if (window->KeyHold(GLFW_KEY_S)) {
        if (!isBottomCollision(playerUpperY, playerBottomY, playerLeftX, playerRightX)) {
            logicSpace.y -= deltaTime;
            playerY -= deltaTime;
            healthY -= deltaTime;
        }
    }

    if (window->KeyHold(GLFW_KEY_D)) {
        if (!isRightCollision(playerUpperY, playerBottomY, playerLeftX, playerRightX)) {
            logicSpace.x += deltaTime;
            playerX += deltaTime;
            healthX += deltaTime;
        }
    }

    if (window->KeyHold(GLFW_KEY_A)) {
        if (!isLeftCollision(playerUpperY, playerBottomY, playerLeftX, playerRightX)) {
            logicSpace.x -= deltaTime;
            playerX -= deltaTime;
            healthX -= deltaTime;
        }
    }
}


void Tema1::OnKeyPress(int key, int mods)
{
}

void Tema1::OnKeyRelease(int key, int mods)
{
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{   
    /* in ceea ce priveste formula:
     am intentionat sa fac (mouseY - playerY) si (mouseX - playerX) insa nu se modifica unghiul
     aproape deloc deoarece mouse-ul functioneaza intre rezolutia declarata: 1280X720, iar personajul se misca
     intre (0,0  5, 5.5). Tinand cont ca mouse-ul va fi in general in fereastra jocului (care este jumatate
     din intreaga fereastra) am impartit rezolutiile la 2 si practic am limitat diferenta dintre mouseXY si playerXY
     offset-ul (*60 - 300) l-am hardcodat astfel:
     am aplicat formula insa player-ul se misca destul de putin si m-am gandit sa inmultesc rezultatul
     pentru a obtine valori mai mari si as am ajuns la 60, apoi player-ul avea un offset fata de mouse 
     si am adunat si scazut valori ca sa il aduc in pozitia corecta, si asa am ajuns la -300*/ 
    radiansPlayer = RADIANS(atan2((mouseY - window->GetResolution().y / 2) - playerY, (mouseX - window->GetResolution().x / 2) - playerX)) * 60 - 300;
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // folosesc aceasta implementare pentru a integra si fireRate
    if (button == 1 && flag3 == 0) {
        fire = true;
        flag3 = 1;

    }
    else if (button == 1 && flag3) {
        end2 = clock();
        if (((float)end2 - start2) / CLOCKS_PER_SEC > fireRate) {
            fire = true;
        }
    }

    
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{   
    if (button == 1) {
        start2 = clock();
    }
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}
