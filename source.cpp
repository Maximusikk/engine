#include <GL/glut.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// Путь к модели
const char* modelPath = "C:/Users/user/Downloads/bros.obj";

// Переменные для хранения данных модели
const aiScene* scene = nullptr;
Assimp::Importer importer;

// Функция для загрузки модели
bool LoadModel(const char* path) {
    scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene) {
        printf("Ошибка загрузки модели: %s\n", importer.GetErrorString());
        return false;
    }
    return true;
}

// Переменные для хранения камеры
float cameraPosX = 0.0f;
float cameraPosY = 0.0f;
float cameraPosZ = -5.0f;
float cameraAngleX = 0.0f;
float cameraAngleY = 0.0f;
float mouseX = 0.0f;
float mouseY = 0.0f;
// Функция для отображения модели
// Функция для отображения модели
void RenderModel(const aiScene* sc) {
    // Enable lighting
    glEnable(GL_LIGHTING);

    // Set up a simple ambient light
    GLfloat ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);

    // Set up a bright red material
    GLfloat material[] = { 1.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, material);

    // Apply a rotation and translation transformation
    glPushMatrix();
    glTranslatef(cameraPosX, cameraPosY, cameraPosZ);
    glRotatef(cameraAngleX, 0.0f, 1.0f, 0.0f);
    glRotatef(cameraAngleY, 1.0f, 0.0f, 0.0f);

    // Отрисовка пола
    glPushMatrix();
    glTranslatef(0.0f, -5.0f, 0.0f); // Перемещаем пол вниз
    glScalef(10.0f, 1.0f, 10.0f); // Увеличиваем размер пола
    glBegin(GL_QUADS);
    glColor3f(0.5f, 0.5f, 0.5f); // Цвет пола
    glVertex3f(-5.0f, 0.0f, -5.0f);
    glVertex3f(5.0f, 0.0f, -5.0f);
    glVertex3f(5.0f, 0.0f, 5.0f);
    glVertex3f(-5.0f, 0.0f, 5.0f);
    glEnd();
    glPopMatrix();

    // Отрисовка модели
    for (unsigned int i = 0; i < sc->mNumMeshes; i++) {
        const aiMesh* mesh = sc->mMeshes[i];
        glBegin(GL_TRIANGLES);
        for (unsigned int j = 0; j < mesh->mNumFaces; j++) {
            const aiFace& face = mesh->mFaces[j];
            for (unsigned int k = 0; k < face.mNumIndices; k++) {
                int index = face.mIndices[k];
                glVertex3fv(&mesh->mVertices[index].x);
            }
        }
        glEnd();
    }

    glPopMatrix();

    // Disable lighting
    glDisable(GL_LIGHTING);
}


// Функция для инициализации OpenGL
void InitOpenGL() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    // Set up the viewport
    glViewport(0, 0, 800, 600);

    // Set up the projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, 800.0f / 600.0f, 1.0f, 100.0f);

    // Set up the modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Функция для отображения сцены
void Display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (scene) {
        RenderModel(scene);
    }
    glutSwapBuffers();
}

// Функция для обработки клавиш
// Функция для обработки клавиш
void Keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'a':
        cameraPosX -= 0.1f;
        break;
    case 'd':
        cameraPosX += 0.1f;
        break;
    case 'w':
        cameraPosZ += 0.1f;
        break;
    case 's':
        cameraPosZ -= 0.1f;
        break;
    case 'q':
        cameraPosY -= 0.1f;
        break;
    case 'e':
        cameraPosY += 0.1f;
        break;
    }
    glutPostRedisplay();
}

// Функция для обработки мыши
void Mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        mouseX = x;
        mouseY = y;
    }
}

// Функция для обработки движения мыши
void MouseMotion(int x, int y) {
    cameraAngleX += (x - mouseX) * 0.1f;
    cameraAngleY += (y - mouseY) * 0.1f;
    mouseX = x;
    mouseY = y;
    glutPostRedisplay();
}
// Главная функция
int main(int argc, char** argv) {
    if (!LoadModel(modelPath)) {
        return 1;
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("OpenGL + Assimp");

    InitOpenGL();
    glutDisplayFunc(Display);
    glutKeyboardFunc(Keyboard);
    glutMouseFunc(Mouse);
    glutMotionFunc(MouseMotion);
    glutMainLoop();

    return 0;
}
