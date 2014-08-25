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
void initGui(sfx::Application& application, sfx::GuiManager& guiManager, Simulation& simulation);
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

    for(int i = 0; i < 100; i++)
    {
        float x = static_cast<float>(sfx::getRandom(0, width));
        float y = static_cast<float>(sfx::getRandom(0, height));

        simulation.addBoid(Boid(application.getTexture("Assets/Images/Boid.png"), {x, y}));
    }

    sfx::GuiManager guiManager(application);
    initGui(application, guiManager, simulation);

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

        simulation.setMousePosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(application)));
        simulation.update(clock.restart().asSeconds());

        guiManager.onUpdate();

        application.clear();
        application.draw(guiManager);
        application.draw(simulation);
        application.display();
    }

    return 0;
}

void initGui(sfx::Application& application, sfx::GuiManager& guiManager, Simulation& simulation)
{
    const std::string background    = "Assets/Images/Background.png";
    const std::string slider        = "Assets/Images/Slider";
    const std::string texteditBox   = "Assets/Images/TextEditBox";
    const std::string font          = "Assets/Fonts/Arial.ttf";
    const std::string buttonAddBoid = "Assets/Images/ButtonAdd";
    const std::string buttonSubBoid = "Assets/Images/ButtonSub";
    const std::string checkbox      = "Assets/Images/Checkbox";

    const int characterSize      = 15;

    const std::string cohesion         = convert("Cohesion: ", simulation.getCohesion(), 0);
    const std::string separation       = convert("Separation: ", simulation.getSeparation(), 3);
    const std::string separationRadius = convert("Separation radius: ", static_cast<float>(simulation.getSeparationRadius()), 0);
    const std::string alignment        = convert("Alignment: ", simulation.getAlignment(), 0);
    const std::string maxVelocity      = convert("Max velocity: ", simulation.getMaxVelocity(), 0);
    const std::string boids            = convert("Boids: ", static_cast<float>(simulation.getBoids()), 0);
    const std::string followMouse      = "Follow mouse: ";
    const std::string avoidMouse       = "Avoid mouse: ";
    const std::string drawMouseRadius  = "Draw mouse radius:";
    const std::string mouseStrength    = convert("Mouse strength: ", simulation.getMouseStrength(), 3);
    const std::string mouseRadius      = convert("Mouse radius: ", static_cast<float>(simulation.getMouseRadius()), 0);
    const std::string wrapEdge         = "Wrap edge: ";

    float columns[] = {10.f, 5.f, 95.f, 145.f};
    float rows[] = {25.f, 50.f, 75.f, 100.f, 125.f, 150.f, 175.f, 200.f, 225.f, 250.f, 275.f, 
                    300.f, 350.f, 375.f, 400.f, 450.f, 475.f, 500.f, 525.f, 560.f, 585.f, 
                    650.f};

    guiManager.createPicture("background", background);

    auto& labelCohesion         = guiManager.createLabel("labelCohesion",         cohesion,         font, columns[1], rows[0],  characterSize);
    auto& labelSeparation       = guiManager.createLabel("labelSeparation",       separation,       font, columns[1], rows[2],  characterSize);
    auto& labelSeparationRadius = guiManager.createLabel("labelSeparationRadius", separationRadius, font, columns[1], rows[4],  characterSize);
    auto& labelAlignment        = guiManager.createLabel("labelAlignment",        alignment,        font, columns[1], rows[6],  characterSize);
    auto& labelMaxVelocity      = guiManager.createLabel("labelMaxVelocity",      maxVelocity,      font, columns[1], rows[8],  characterSize);
    auto& labelBoids            = guiManager.createLabel("labelBoids",            boids,            font, columns[1], rows[10], characterSize);
    auto& labelFollowMouse      = guiManager.createLabel("labelFollowMouse",      followMouse,      font, columns[1], rows[12], characterSize);
    auto& labelAvoidMouse       = guiManager.createLabel("labelAvoidMouse",       avoidMouse,       font, columns[1], rows[13], characterSize);
    auto& labelDrawMouseRadius  = guiManager.createLabel("labelDrawMouseRadius",  drawMouseRadius,  font, columns[1], rows[14], characterSize);
    auto& labelMouseStrength    = guiManager.createLabel("labelMouseStrength",    mouseStrength,    font, columns[1], rows[15], characterSize);
    auto& labelMouseRadius      = guiManager.createLabel("labelMouseRadius",      mouseRadius,      font, columns[1], rows[17], characterSize);
    auto& labelWrapEdge         = guiManager.createLabel("labelWrapEdge",         wrapEdge,         font, columns[1], rows[19], characterSize);

    auto& sliderCohesion         = guiManager.createSlider("sliderCohesion",         slider, columns[0], rows[1], 1.f, 200.f, simulation.getCohesion());
    auto& sliderSeparation       = guiManager.createSlider("sliderSeparation",       slider, columns[0], rows[3], 0.0f, 10.f, simulation.getSeparation());
    auto& sliderSeparationRadius = guiManager.createSlider("sliderSeparationRadius", slider, columns[0], rows[5], 1, 100, static_cast<float>(simulation.getSeparationRadius()));
    auto& sliderAlignment        = guiManager.createSlider("sliderAlignment",        slider, columns[0], rows[7], 1.f, 200.f, simulation.getAlignment());
    auto& sliderMaxVelocity      = guiManager.createSlider("sliderMaxVelocity",      slider, columns[0], rows[9], 0.f, 1000.f, simulation.getMaxVelocity());
    auto& sliderMouseStrength    = guiManager.createSlider("sliderMouseStrength",    slider, columns[0], rows[16], 1.f, 10.f, simulation.getMouseStrength());
    auto& sliderMouseRadius      = guiManager.createSlider("sliderMouseRadius",      slider, columns[0], rows[18], 25.f, 500.f, static_cast<float>(simulation.getMouseRadius()));
    
    auto& textBoids = guiManager.createTextEditBox("textBoids", texteditBox, font, columns[1], rows[11], characterSize);
    textBoids.setText("10");
    textBoids.setTextPosition(columns[1] + 3.f, rows[11] + characterSize / 4);

    auto& buttonAdd = guiManager.createButton("buttonAdd", buttonAddBoid, columns[2], rows[11]);
    auto& buttonSub = guiManager.createButton("buttonSub", buttonSubBoid, columns[3], rows[11]);

    auto& checkboxFollowMouse     = guiManager.createCheckBox("checkboxFollowMouse",     checkbox, columns[3], rows[12] + 3.f);
    auto& checkboxAvoidMouse      = guiManager.createCheckBox("checkboxAvoidMouse",      checkbox, columns[3], rows[13] + 3.f);
    auto& checkboxDrawMouseRadius = guiManager.createCheckBox("checkboxDrawMouseRadius", checkbox, columns[3], rows[14] + 3.f);
    auto& checkboxWrapEdge        = guiManager.createCheckBox("checkboxWrapEdge",        checkbox, columns[3], rows[19] + 3.f);

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

    sliderMouseStrength.callback(1, [&sliderMouseStrength, &labelMouseStrength, &simulation]{
        labelMouseStrength.setText(convert("Mouse strength: ", sliderMouseStrength.getValue(), 3));
        simulation.setMouseStrength(sliderMouseStrength.getValue());
    });

    sliderMouseRadius.callback(1, [&sliderMouseRadius, &labelMouseRadius, &simulation]{
        labelMouseRadius.setText(convert("Mouse radius: ", sliderMouseRadius.getValue(), 0));
        simulation.setMouseRadius(static_cast<int>(sliderMouseRadius.getValue()));
    });

    buttonAdd.callback(1, [&application, &labelBoids, &simulation, &textBoids] {
        int boids = sfx::convert<int>(textBoids.getText());

        for(int i = 0; i < boids; i++)
        {
            float x = static_cast<float>(sfx::getRandom(0, application.getSize().x));
            float y = static_cast<float>(sfx::getRandom(0, application.getSize().y));

            simulation.addBoid(Boid(application.getTexture("Assets/Images/Boid.png"), {x, y}));
        }

        labelBoids.setText(convert("Boids: ", static_cast<float>(simulation.getBoids()), 0));
    });

    buttonSub.callback(1, [&labelBoids, &simulation, &textBoids] {
        int boids = sfx::convert<int>(textBoids.getText());

        for(int i = 0; i < boids; i++)
            simulation.popBoid();

        labelBoids.setText(convert("Boids: ", static_cast<float>(simulation.getBoids()), 0));
    });

    checkboxFollowMouse.callback(0, [&simulation] { simulation.setFollowMouse(true); });
    checkboxFollowMouse.callback(1, [&simulation] { simulation.setFollowMouse(false); });
    checkboxAvoidMouse.callback(0, [&simulation] { simulation.setAvoidMouse(true); });
    checkboxAvoidMouse.callback(1, [&simulation] { simulation.setAvoidMouse(false); });
    checkboxDrawMouseRadius.callback(0, [&simulation] { simulation.setDrawMouseRadius(true); });
    checkboxDrawMouseRadius.callback(1, [&simulation] { simulation.setDrawMouseRadius(false); });
    checkboxWrapEdge.callback(0, [&simulation] { simulation.setWrapEdge(true); });
    checkboxWrapEdge.callback(1, [&simulation] { simulation.setWrapEdge(false); });
}

std::string convert(const std::string& prefix, float value, int precision)
{
    std::stringstream ss;
    ss.precision(precision);
    ss << prefix << std::fixed << value;

    return ss.str();
}