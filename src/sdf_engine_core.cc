#include "../include/application.hh"

int main(int argc, char* argv[])
{
    Application* app = init_application(800, 600, "SDF Engine");
    run_application(app);
    return 0;
}
