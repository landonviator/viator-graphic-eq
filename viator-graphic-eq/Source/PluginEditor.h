#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "components/Header.h"
#include "Globals/Parameters.h"

//==============================================================================
/**
*/
class ViatorgraphiceqAudioProcessorEditor  : public juce::AudioProcessorEditor, private juce::Timer
{
public:
    ViatorgraphiceqAudioProcessorEditor (ViatorgraphiceqAudioProcessor&);
    ~ViatorgraphiceqAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ViatorgraphiceqAudioProcessor& audioProcessor;
    
    Header _headerComp;
    
    // bands sliders
    juce::OwnedArray<viator_gui::ImageFader> _bandSliders;
    juce::OwnedArray<juce::AudioProcessorValueTreeState::SliderAttachment> _bandSliderAttachments;
    void initBandSliders();
    
    // filter sliders
    juce::OwnedArray<viator_gui::Dial> _filterSliders;
    juce::OwnedArray<juce::AudioProcessorValueTreeState::SliderAttachment> _filterSliderAttachments;
    void initFilterSliders();
    
    // io sliders
    juce::OwnedArray<viator_gui::Dial> _ioSliders;
    juce::OwnedArray<juce::AudioProcessorValueTreeState::SliderAttachment> _ioSliderAttachments;
    void initIOSliders();
    
    // band labels
    juce::OwnedArray<juce::Label> _bandLabels;
    juce::StringArray _bandLabelTexts =
    {
      "31", "63", "125", "250", "500", "1K", "2K", "4K", "8K", "15K"
    };
    void initBandLabels();
    
    // vu meter
    viator_gui::VUMeter _vuMeter;
    
    void timerCallback() override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ViatorgraphiceqAudioProcessorEditor)
};
