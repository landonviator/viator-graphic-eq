/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ViatorgraphiceqAudioProcessorEditor::ViatorgraphiceqAudioProcessorEditor (ViatorgraphiceqAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
, _vuMeter("vumeter", juce::ImageCache::getFromMemory(BinaryData::vu_meter_png, BinaryData::vu_meter_pngSize))
{
    // header
    addAndMakeVisible(_headerComp);
    
    // band sliders
    initBandSliders();
    
    // filter sliders
    initFilterSliders();
    
    // io sliders
    initIOSliders();
    
    // band labels
    initBandLabels();
    
    // vu meter
    addAndMakeVisible(_vuMeter);
    
    startTimer(20);
    
    // window
    viator_utils::PluginWindow::setPluginWindowSize(audioProcessor._width, audioProcessor._height, *this, 2.0, 1.0);
}

ViatorgraphiceqAudioProcessorEditor::~ViatorgraphiceqAudioProcessorEditor()
{
    stopTimer();
}

//==============================================================================
void ViatorgraphiceqAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
    
    auto bgImage = juce::ImageCache::getFromMemory(BinaryData::back_png, BinaryData::back_pngSize);
    
    auto bgWidth = getWidth() * 0.9;
    auto bgHeight = getHeight() * 0.82;
    auto bgY = getHeight() * 0.13;
    g.drawImage(bgImage, getLocalBounds().toFloat().withSizeKeepingCentre(bgWidth, bgHeight).withY(bgY));
}

void ViatorgraphiceqAudioProcessorEditor::resized()
{
    // header
    const auto headerHeightMult = 0.08;
    const auto headerHeight = getHeight() * headerHeightMult;
    _headerComp.setBounds(0, 0, getWidth(), headerHeight);
    
    // band sliders
    auto sliderX = getWidth() * 0.1;
    auto sliderY = getHeight() * 0.27;
    auto sliderWidth = getWidth() * 0.05;
    auto sliderHeight = getHeight() * 0.65;
    for (auto& slider : _bandSliders)
    {
        slider->setBounds(sliderX, sliderY, sliderWidth, sliderHeight);
        sliderX += sliderWidth * 1.1;
    }
    
    // filter sliders
    auto filterSliderX = getWidth() * 0.69;
    auto filterSliderY = getHeight() * 0.5;
    auto filterSliderSize = getWidth() * 0.09;
    for (auto& slider : _filterSliders)
    {
        slider->setBounds(filterSliderX, filterSliderY, filterSliderSize, filterSliderSize);
        slider->setDialTextBoxWidth(filterSliderSize);
        filterSliderX += filterSliderSize;
    }
    
    // io sliders
    auto ioSliderX = getWidth() * 0.69;
    auto ioSliderY = _filterSliders[0]->getBottom() * 1.05;
    auto ioSliderSize = filterSliderSize;
    for (auto& slider : _ioSliders)
    {
        slider->setBounds(ioSliderX, ioSliderY, ioSliderSize, ioSliderSize);
        slider->setDialTextBoxWidth(ioSliderSize);
        ioSliderX += ioSliderSize;
    }
    
    // band labels
    for (auto& label : _bandLabels)
    {
        label->setFont(juce::Font("Helvetica", sliderWidth * 0.25, juce::Font::FontStyleFlags::bold));
    }
    
    // vu meter
    auto vuX = getWidth() * 0.65;
    auto vuY = getHeight() * 0.15;
    auto vuWidth = getWidth() * 0.27;
    auto vuHeight = getHeight() * 0.37;
    _vuMeter.setBounds(vuX, vuY, vuWidth, vuHeight);
    
    // Save plugin size in value tree
    savePluginBounds();
}

void ViatorgraphiceqAudioProcessorEditor::initBandSliders()
{
    auto image = juce::ImageCache::getFromMemory(BinaryData::Ver_slider_png, BinaryData::Ver_slider_pngSize);
    auto params = audioProcessor._parameterMap.getBandSliderParams();
    for (int i = 0; i < params.size(); i++)
    {
        _bandSliders.add(std::make_unique<viator_gui::ImageFader>(256, image));
        _bandSliderAttachments.add(std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor._treeState, params[i].paramID, _bandSliders[i]->getVUMeter()));
        addAndMakeVisible(*_bandSliders[i]);
    }
}

void ViatorgraphiceqAudioProcessorEditor::initFilterSliders()
{
    auto image = juce::ImageCache::getFromMemory(BinaryData::Knob_mid_png, BinaryData::Knob_mid_pngSize);
    auto params = audioProcessor._parameterMap.getFilterSliderParams();
    for (int i = 0; i < params.size(); i++)
    {
        _filterSliders.add(std::make_unique<viator_gui::Dial>(params[i].name, image));
        _filterSliderAttachments.add(std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor._treeState, params[i].paramID, *_filterSliders[i]));
        _filterSliders[i]->setNumFrames(256);
        addAndMakeVisible(*_filterSliders[i]);
    }
}

void ViatorgraphiceqAudioProcessorEditor::initIOSliders()
{
    auto image = juce::ImageCache::getFromMemory(BinaryData::Knob_small_png, BinaryData::Knob_small_pngSize);
    auto params = audioProcessor._parameterMap.getIOSliderParams();
    for (int i = 0; i < params.size(); i++)
    {
        _ioSliders.add(std::make_unique<viator_gui::Dial>(params[i].name, image));
        _ioSliderAttachments.add(std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor._treeState, params[i].paramID, *_ioSliders[i]));
        _ioSliders[i]->setNumFrames(128);
        addAndMakeVisible(*_ioSliders[i]);
    }
}

void ViatorgraphiceqAudioProcessorEditor::initBandLabels()
{
    for (int i = 0; i < 10; i++)
    {
        _bandLabels.add(std::make_unique<juce::Label>(_bandLabelTexts[i], _bandLabelTexts[i]));
        _bandLabels[i]->setJustificationType(juce::Justification::centred);
        _bandLabels[i]->setColour(juce::Label::ColourIds::textColourId, juce::Colour::fromRGB(161, 168, 181).darker(0.3f));
        _bandLabels[i]->attachToComponent(_bandSliders[i], false);
        addAndMakeVisible(*_bandLabels[i]);
    }
}

void ViatorgraphiceqAudioProcessorEditor::timerCallback()
{
    _vuMeter.getVUMeter().setValue(audioProcessor.getCurrentPeakSignal());
}

void ViatorgraphiceqAudioProcessorEditor::savePluginBounds()
{
    audioProcessor.variableTree.setProperty("width", getWidth(), nullptr);
    audioProcessor.variableTree.setProperty("height", getHeight(), nullptr);
    audioProcessor._width = getWidth();
    audioProcessor._height = getHeight();
}
