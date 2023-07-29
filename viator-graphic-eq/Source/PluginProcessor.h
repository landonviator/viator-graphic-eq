#pragma once
#include <JuceHeader.h>
#include "Globals/Parameters.h"

class ViatorgraphiceqAudioProcessor  : public juce::AudioProcessor
, public juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    ViatorgraphiceqAudioProcessor();
    ~ViatorgraphiceqAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    // params
    ViatorParameters::Params _parameterMap;
    juce::AudioProcessorValueTreeState _treeState;
    
    juce::ValueTree variableTree
    { "Variables", {},
        {
        { "Group", {{ "name", "Vars" }},
            {
                { "Parameter", {{ "id", "width" }, { "value", 0.0 }}},
                { "Parameter", {{ "id", "height" }, { "value", 0.0 }}}
            }
        }
        }
    };
    
    float _width = 0.0f;
    float _height = 0.0f;
    
    void calculatePeakSignal(juce::AudioBuffer<float>& buffer);
    float getCurrentPeakSignal();
    
private:
    
    // dsp
    juce::dsp::ProcessSpec _spec;
    std::vector<float> _eqCutoffs =
    {
        31.0f, 63.0f, 125.0f, 250.0f, 500.0f, 1000.0f, 2000.0f, 4000.0f, 8000.0f, 15000.0f
    };
    
    juce::OwnedArray<viator_dsp::SVFilter<float>> _eqFilters;
    
    std::vector<float> _eqGains;
    std::vector<float> _eqQs;
    
    viator_dsp::SVFilter<float> _lpFilter;
    viator_dsp::SVFilter<float> _hpFilter;
    
    juce::dsp::Gain<float> _inputProcessor;
    juce::dsp::Gain<float> _outputProcessor;
    
    std::atomic<float> _drive;
    static constexpr float _softClipCompensate = 1.318;
    
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    void parameterChanged (const juce::String& parameterID, float newValue) override;
    using Parameter = juce::AudioProcessorValueTreeState::Parameter;
    static juce::String valueToTextFunctionInt(float x) { return juce::String(static_cast<int>(x)); }
    static juce::String valueToTextFunctionFloat(float x)
    {
        return juce::String(x, 1);
    }
    
    static float textToValueFunction(const juce::String& str) { return str.getFloatValue(); }
    
    void updateParameters();
    
    juce::SmoothedValue<double> levelGain = -60.0;
    float peakDB = -60.0;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ViatorgraphiceqAudioProcessor)
};
