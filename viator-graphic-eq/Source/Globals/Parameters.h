#pragma once
#include <JuceHeader.h>
#include "Globals.h"

namespace ViatorParameters
{
// Param data
struct SliderParameterData
{
    enum SkewType
    {
        kSkew,
        kNoSkew
    };
    
    enum NumericType
    {
        kInt,
        kFloat
    };
    
    public:
        juce::String paramID;
        juce::String name;
        float min;
        float max;
        float initial;
        SkewType isSkew;
        float center;
        NumericType isInt;
};

struct ButtonParameterData
{
    public:
        juce::String _id;
        juce::String _name;
        bool _initial;
};

struct MenuParameterData
{
    public:
        juce::String ID;
        juce::String name;
        int min;
        int max;
        int defaultIndex;
};


    class Params
    {
    public:
        Params();
        
        // Get a ref to the param data
        std::vector<ViatorParameters::SliderParameterData>& getSliderParams(){return _sliderParams;};
        std::vector<ViatorParameters::ButtonParameterData>& getButtonParams(){return _buttonParams;};
        std::vector<ViatorParameters::MenuParameterData>& getMenuParams(){return _menuParams;};
        
        // for attachments
        std::vector<ViatorParameters::SliderParameterData>& getBandSliderParams(){return _bandSliderParams;};
        std::vector<ViatorParameters::SliderParameterData>& getFilterSliderParams(){return _filterSliderParams;};
        std::vector<ViatorParameters::SliderParameterData>& getIOSliderParams(){return _ioSliderParams;};
        
        int getTotalNumberOfParams(){return static_cast<int>(_sliderParams.size()) + static_cast<int>(_buttonParams.size()) + static_cast<int>(_menuParams.size());}
        
    private:
        // Adds params to the vector
        void initSliderParams();
        void initButtonParams();
        void initMenuParams();
        
        
    private:
        // Vector holding param data
        std::vector<ViatorParameters::SliderParameterData> _sliderParams;
        std::vector<ViatorParameters::ButtonParameterData> _buttonParams;
        std::vector<ViatorParameters::MenuParameterData> _menuParams;
        
        // for attachments
        std::vector<ViatorParameters::SliderParameterData> _bandSliderParams;
        std::vector<ViatorParameters::SliderParameterData> _filterSliderParams;
        std::vector<ViatorParameters::SliderParameterData> _ioSliderParams;
    };
}
