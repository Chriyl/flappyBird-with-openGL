#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <unistd.h>
#include <vector>

#define XMIN -1.33f
#define YMIN -1.0f
#define XMAX 1.33f
#define YMAX 1.0f
#define G -9.8 * pow(10, -3)
#define DELTA 0.00694f  // approssimiamo per i 144fps

class Circle {
    private:
    float cx;
    float cy;
    float radius;
    float speed;
    float vy;
    public:
    Circle(float cx, float cy,float radius, float speed){
        this->cx = cx;
        this->cy = cy;
        this->radius = radius;
        this->speed = speed;
        this->vy = 0;
    }
    
    void drawCircle( int segments){
        glBegin(GL_TRIANGLE_FAN); // disegno il cerchio usando come vertice comune dei triangolo il centro del cerchio
        glVertex2f(this->cx,this->cy); // scelgo il centro del cerchio
        for(int i = 0; i <= segments; i++){ // disegna il cerchio
            float theta = 2.0f * M_PI * float(i) / float(segments); // calcolo l'angolo per ogni segmento 2pigreco
            float x = this->radius * cosf(theta); // calcolo le coordinate in base al raggio
            float y = this->radius * sinf(theta);
            glVertex2f(x +  this->cx, y + this->cy); // posizione il cerchio
        }
        glEnd();
    }

    float getRadius(){
        return this->radius;
    }
    
    float getX(){
        return this->cx;
    }

    float getY(){
        return this->cy;
    }
   

    void processInput(GLFWwindow* win) {
    
        // Gestione del salto
        if (glfwGetKey(win, GLFW_KEY_SPACE) == GLFW_PRESS) /*&& (this->cy - this->radius) <= YMIN && std::fabs(this->vy) < 0.01f)*/ {
            this->vy = 0.005f;  // Imposta la velocità verticale per il salto
        }
    }
    
    void gravity() {
        this->vy += G * DELTA;  // Applica la gravità
        this->cy += this->vy;   // Aggiorna la posizione verticale
    
        // Se il cerchio tocca il suolo
        if ((this->cy - this->radius) <= YMIN) {
            this->cy = YMIN + this->radius;  // Imposta la posizione del cerchio sulla superficie
            this->vy *= -0.8f;  // Inverte e riduce la velocità verticale per il rimbalzo
    
            // Se la velocità verticale è molto bassa, ferma la palla
            if (std::fabs(this->vy) < 0.001f) {
                this->vy = 0;  // Ferma la palla quando la velocità è troppo bassa per rimbalzare
            }
        }
    }
    
};

class Rectangle {
    private:
        float vertex1X, vertex1Y;  // Punti di partenza
        float vertex2X, vertex2Y;
        float vertex3X, vertex3Y;
        float vertex4X, vertex4Y;
        float speed;
    
    public:

        Rectangle(float startX, float startY, float width, float height, float speed){
            // Vertex 1 (angolo superiore sinistro)
            this->vertex1X = startX;
            this->vertex1Y = startY;
            
            // Vertex 2 (angolo superiore destro)
            this->vertex2X = startX + width;
            this->vertex2Y = startY;
            
            // Vertex 3 (angolo inferiore destro)
            this->vertex3X = startX + width;
            this->vertex3Y = startY - height;
            
            // Vertex 4 (angolo inferiore sinistro)
            this->vertex4X = startX;
            this->vertex4Y = startY - height;
    
            this->speed = speed;
        }
        ~Rectangle(){}
    
        void drawRectangle(){
            glBegin(GL_QUADS);
                glVertex2f(this->vertex1X, this->vertex1Y);
                glVertex2f(this->vertex2X, this->vertex2Y);
                glVertex2f(this->vertex3X, this->vertex3Y);
                glVertex2f(this->vertex4X, this->vertex4Y);
            glEnd();
        }
    
        void moveLeft(){
            float distance = this->speed * DELTA;
            this->vertex1X -= distance;
            this->vertex2X -= distance;
            this->vertex3X -= distance;
            this->vertex4X -= distance;
        }

        void physic(){
            this->moveLeft();
        }

    
};

void gameCicle(GLFWwindow* window){
    Circle circle = Circle(0.0f, 0.0f, 0.120f,0.002f);
    Rectangle rectangleDown(1.20f, -0.3f, 0.3f, 0.7f, 0.4f); 
    Rectangle  rectangleUp = Rectangle(1.2f, 1.0f, 0.3f, 0.7f, 0.4f);
    
    while (!glfwWindowShouldClose(window)){
        
        glClear(GL_COLOR_BUFFER_BIT); // pulisce il buffer colori
        glColor3f(1.0f,1.0f,1.0f); // setto il colore dei modelli
        rectangleDown.drawRectangle();
        rectangleDown.physic();
        rectangleUp.drawRectangle();
        rectangleUp.physic();
        circle.gravity();
        circle.processInput(window);

        //drawCircle(0.0f, 0.0f, 0.5f, 100); //disegno il modello (cerchio)  al centro 0,0 di raggio 0.5 con 50 segmenti
        circle.drawCircle(100);
        glfwSwapBuffers(window); // aggiorna il buffer con il disegno appena fatto
        glfwPollEvents(); // gestisce gli eventi
        }
}

int program(){
    int width, height;
    if (!glfwInit()) { // inizializzo wsl
        std::cerr << "Errore inizializzazione GLFW\n";
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "lmao get rekt", nullptr, nullptr); // creo una finestra
    if (!window) {
        std::cerr << "Errore nella creazione della finestra\n";
        glfwTerminate();
        
        return -1;
    }

    glfwMakeContextCurrent(window); // miro alla finestra per le prossime operazioni

    glfwGetFramebufferSize(window, &width, &height); // prende le dimensione della finestra
    glViewport(0,0,width, height); // per utilizzare tutta la finestra

    glMatrixMode(GL_PROJECTION); // imposto la matriche in modo che le operazioni vengano svolte qui
    glLoadIdentity(); // resetto la matrice
    float aspect = float(width) / float(height); // calcolo il rapporto di proporzione
    

    glOrtho(-1.0f * aspect, 1.0f * aspect, -1.0f, 1.0f, -1.0f, 1.0f); // faccio la proiezione in modo che scali in base alla finestra
    glMatrixMode(GL_MODELVIEW); // mi metto in visualizzazione

    //glClearColor(1.0f,1.0f,1.0f,1.0f);

    gameCicle(window);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}



int main() {
    
    program();
    return 0;
    
}
