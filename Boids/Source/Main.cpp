////////////////////////////////////////////////////////////////////////////////
// Filename: Main.cpp
// Author:   Tobias Savinainen
// Year:     2014
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Header files
////////////////////////////////////////////////////////////////////////////////
#include <SFX/Sfx.hpp>
#include "Simulation.hpp"
#include <iomanip>

////////////////////////////////////////////////////////////////////////////////
// Methods
////////////////////////////////////////////////////////////////////////////////
void initGui(sfx::GuiManager& guiManager, Simulation& simulation);
std::string convert(const std::string& prefix, float value, int precision);

////////////////////////////////////////////////////////////////////////////////
// Entry point of application
////////////////////////////////////////////////////////////////////////////////
int main()
{
    sfx::Application application(1600, 900, "Boids");
    unsigned int width  = application.getSize().x;
    unsigned int height = application.getSize().y;

    const int padding = 25;
    Simulation simulation(200, padding, width - padding, height - padding);

    sfx::GuiManager guiManager(application);
    initGui(guiManager, simulation);

    for(int i = 0; i < 100; i++)
    {
        float x = static_cast<float>(sfx::getRandom(0, width));
        float y = static_cast<float>(sfx::getRandom(0, height));

        simulation.addBoid(Boid(application.getTexture("Assets/Images/Boid.png"), {x, y}));
    }

    sf::Clock clock;
    while(application.isOpen())
    {
        sf::Event event;
        while(application.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                application.close();
            if(event.type == sf::Event::KeyPressed)
            {
                if(event.key.code == sf::Keyboard::Escape)
                    application.close();
            }

            guiManager.onEvent(event);
        }

        simulation.update(clock.restart().asSeconds());
        guiManager.onUpdate();

        application.clear();
        application.draw(guiManager);
        application.draw(simulation);
        application.display();
    }

    return 0;
}

void initGui(sfx::GuiManager& guiManager, Simulation& simulation)
{
    const std::string background = "Assets/Images/Background.png";
    const std::string slider     = "Assets/Images/Slider";
    const std::string font       = "Assets/Fonts/Arial.ttf";

    const int characterSize      = 15;

    const std::string cohesion         = convert("Cohesion: ", simulation.getCohesion(), 0);
    const std::string separation       = convert("Separation: ", simulation.getSeparation(), 3);
    const std::string separationRadius = convert("Separation radius: ", static_cast<float>(simulation.getSeparationRadius()), 0);
    const std::string alignment        = convert("Alignment: ", simulation.getAlignment(), 0);
    const std::string maxVelocity      = convert("Max velocity: ", simulation.getMaxVelocity(), 0);

    float columns[] = {10.f, 5.f};
    float rows[] = {25.f, 50.f, 75.f, 100.f, 125.f, 150.f, 175.f, 200.f, 225.f, 250.f, 275.f, 300.f};

    guiManager.createPicture("background", background);

    auto& labelCohesion         = guiManager.createLabel("labelCohesion",         cohesion,         font, columns[1], rows[0], characterSize);
    auto& labelSeparation       = guiManager.createLabel("labelSeparation",       separation,       font, columns[1], rows[2], characterSize);
    auto& labelSeparationRadius = guiManager.createLabel("labelSeparationRadius", separationRadius, font, columns[1], rows[4], characterSize);
    auto& labelAlignment        = guiManager.createLabel("labelAlignment",        alignment,        font, columns[1], rows[6], characterSize);
    auto& labelMaxVelocity      = guiManager.createLabel("labelMaxVelocity",      maxVelocity,      font, columns[1], rows[8], characterSize);

    auto& sliderCohesion         = guiManager.createSlider("sliderCohesion",         slider, columns[0], rows[1], 1.f, 200.f, simulation.getCohesion());
    auto& sliderSeparation       = guiManager.createSlider("sliderSeparation",       slider, columns[0], rows[3], 0.0f, 10.f, simulation.getSeparation());
    auto& sliderSeparationRadius = guiManager.createSlider("sliderSeparationRadius", slider, columns[0], rows[5], 1, 100, static_cast<float>(simulation.getSeparationRadius()));
    auto& sliderAlignment        = guiManager.createSlider("sliderAlignment",        slider, columns[0], rows[7], 1.f, 200.f, simulation.getAlignment());
    auto& sliderMaxVelocity      = guiManager.createSlider("sliderMaxVelocity",      slider, columns[0], rows[9], 0.f, 1000.f, simulation.getMaxVelocity());

    sliderCohesion.callback(1, [&sliderCohesion, &labelCohesion, &simulation]{
        labelCohesion.setText(convert("Cohesion: ", sliderCohesion.getValue(), 0));
        simulation.setCohesion(sliderCohesion.getValue());
    });

    sliderSeparation.callback(1, [&sliderSeparation, &labelSeparation, &simulation] {
        labelSeparation.setText(convert("Separation: ", sliderSeparation.getValue(), 3));
        simulation.setSeparation(sliderSeparation.getValue());
    });

    sliderSeparationRadius.callback(1, [&sliderSeparationRadius, &labelSeparationRadius, &simulation]{
        labelSeparationRadius.setText(convert("Seperation radius: ", sliderSeparationRadius.getValue(), 0));
        simulation.setSeperationRadius(static_cast<int>(sliderSeparationRadius.getValue()));
    });

    sliderAlignment.callback(1, [&sliderAlignment, &labelAlignment, &simulation]{
        labelAlignment.setText(convert("Alignment: ", sliderAlignment.getValue(), 0));
        simulation.setAlignment(sliderAlignment.getValue());
    });

    sliderMaxVelocity.callback(1, [&sliderMaxVelocity, &labelMaxVelocity, &simulation]{
        labelMaxVelocity.setText(convert("Max velocity: ", sliderMaxVelocity.getValue(), 0));
        simulation.setMaxVelocity(sliderMaxVelocity.getValue());
    });
}

std::string convert(const std::string& prefix, float value, int precision)
{
    std::stringstream ss;
    ss.precision(precision);
    ss << prefix << std::fixed << value;

    return ss.str();
}