#include "canvas.h"
#include <gtkmm/application.h>
#include <gtkmm/window.h>


int main(int argc, char** argv)
{
//    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.cairo.tut");
    auto app = Gtk::Application::create(argc, argv, "Ann.Zdanevich.ru");

    Gtk::Window window;
    //window.resize(800,600);
    //window.maximize();
    window.fullscreen();
    window.set_title("ArkAnnoid");

    Arkanoid area;
    //area.Init();
    window.add(area);
    area.setParentWindow(&window);
    area.show();

    return app->run(window);
}
