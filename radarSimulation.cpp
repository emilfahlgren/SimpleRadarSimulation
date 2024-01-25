#include <iostream>
#include <vector>
#include <memory>
#include <thread>
#include <chrono>

// Base class for all radar components
class RadarComponent 
{
public:
    virtual void operate() = 0;
    virtual ~RadarComponent() = default;
};

// Transmitter class
class Transmitter : public RadarComponent 
{
public:
    constexpr static double frequency = 2.45e9;
    constexpr static double wavelength = 3.0e8 / frequency;
    constexpr static double freqGHz = frequency / 1.0e9;
    constexpr static double wavelengthMilli = wavelength * 1.0e3;

    void operate() override 
    {
        for(;;) 
        {
            std::cout << "Transmitting at " << freqGHz << " GHz with a wavelength of "
                        << wavelengthMilli << " millimeters." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
};

// Receiver class
class Receiver : public RadarComponent 
{
public:
    void operate() override 
    {
        // Wait 1 second before starting to simulate the travel time of the wave.
        std::this_thread::sleep_for(std::chrono::seconds(1));
        for(;;) 
        {
            std::cout << "Receiving signals." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
};

// Radar system consisting of a transmitter and receiver
class RadarSystem 
{
    std::vector<std::unique_ptr<RadarComponent>> components;
    std::vector<std::jthread> jthreads;
public:
    void addComponent(std::unique_ptr<RadarComponent> component) 
    {
        components.push_back(std::move(component));
    }

    void startSimulation() 
    {
        for (auto& comp : components) 
        {
            // Launch a thread for each component's operate function
            jthreads.emplace_back(&RadarComponent::operate, comp.get());
        }
    }
};

int main() 
{
    auto radarSystem = std::make_unique<RadarSystem>();
    radarSystem->addComponent(std::make_unique<Transmitter>());
    radarSystem->addComponent(std::make_unique<Receiver>());

    radarSystem->startSimulation();

    return 0;
}
