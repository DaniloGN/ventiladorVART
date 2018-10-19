// main.cpp - transform animation example

// V-ART example: Moving objects with an idle handler

// This application shows how to use ViewerGlutOGL's idle handler to animate objects.

// Changelog
// Jun 17, 2008 - Bruno de Oliveira Schneider
// - Application created.

#include <vart/scene.h>
#include <vart/light.h>
#include <vart/sphere.h>
#include <vart/transform.h>
#include <vart/contrib/viewerglutogl.h>
#include <vart/meshobject.h>

#include <iostream>

using namespace std;

class MyIHClass : public VART::ViewerGlutOGL::IdleHandler
{
    public:
        MyIHClass() : redRadians(0), corpoRadians(0){}
        virtual ~MyIHClass() {}
        virtual void OnIdle() {
			if(corpoRadians > 2) teste = false;
			if(corpoRadians < -1) teste = true;
			if(teste == true){
				corpoRadians += 0.02;
			}
			else{
				corpoRadians -= 0.02;
			}
            redRadians += 0.5;
            redRotPtr->MakeRotation(VART::Point4D(0, 1.237193, 0, 1),VART::Point4D::X(),redRadians);
			corpoPrt->MakeRotation(VART::Point4D(0, 0 , 0, 1),VART::Point4D::Y(),corpoRadians);
            viewerPtr->PostRedisplay();
        }
    //protected:
        VART::Transform* redRotPtr;
        VART::Transform* corpoPrt;
    private:
        float redRadians;
        float corpoRadians;
        bool teste = true;
};

// The application itself:
int main(int argc, char* argv[])
{
    VART::ViewerGlutOGL::Init(&argc, argv); // Initialize GLUT
    static VART::Scene scene; // create a scene
    static VART::ViewerGlutOGL viewer; // create a viewer (application window)

    // create a camera (scene observer)
    VART::Camera camera(VART::Point4D(0,0,6),VART::Point4D(0,0,0),VART::Point4D(0,1,0,0));
    // create some objects
    list<VART::MeshObject*> objects;
    MyIHClass idleHandler;

    // Initialize scene objects
    VART::MeshObject::ReadFromOBJ("fan.obj", &objects);
    VART::Transform Giro;
    VART::Transform HeliceRotation;

    // Build up the scene
	list<VART::MeshObject*>::iterator iter = objects.begin();
    for (; iter != objects.end(); ++iter) {
		if((*iter)->GetDescription() == "helice"){
			HeliceRotation.MakeRotation(VART::Point4D(0, 1.237193, 0, 1),VART::Point4D::X(),1);
			HeliceRotation.AddChild(**iter);
		}
		else if((*iter)->GetDescription() == "base"){
			scene.AddObject(*iter);
		}
		else if((*iter)->GetDescription() != "base"){
			Giro.MakeRotation(VART::Point4D(0, 0, 0, 1),VART::Point4D::Z(),0);
			Giro.AddChild(**iter);
		}
	}
	Giro.AddChild(HeliceRotation);
	scene.AddObject(&Giro);
    scene.AddLight(VART::Light::BRIGHT_AMBIENT());
    scene.AddCamera(&camera);

    // Set up the Idle Handler
    idleHandler.redRotPtr = &HeliceRotation;
	idleHandler.corpoPrt = &Giro;
    // Set up the viewer
    viewer.SetTitle("V-ART Glut idle handler example");
    viewer.SetScene(scene); // attach the scene
    viewer.SetIdleHandler(&idleHandler); // attach the idle handler to the viewer

    scene.DrawLightsOGL(); // Set OpenGL's lights' state
    VART::ViewerGlutOGL::MainLoop(); // Enter main loop (event loop)
    return 0;
}
