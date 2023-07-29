/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ViatorgraphiceqAudioProcessor::ViatorgraphiceqAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
, _treeState(*this, nullptr, "PARAMETERS", createParameterLayout())
#endif
{
    // sliders
    for (int i = 0; i < _parameterMap.getSliderParams().size(); i++)
    {
        _treeState.addParameterListener(_parameterMap.getSliderParams()[i].paramID, this);
    }
    
    // buttons
    for (int i = 0; i < _parameterMap.getButtonParams().size(); i++)
    {
        _treeState.addParameterListener(_parameterMap.getButtonParams()[i]._id, this);
    }
    
    // menus
    for (int i = 0; i < _parameterMap.getMenuParams().size(); i++)
    {
        _treeState.addParameterListener(_parameterMap.getMenuParams()[i].ID, this);
    }
}

ViatorgraphiceqAudioProcessor::~ViatorgraphiceqAudioProcessor()
{
    // sliders
    for (int i = 0; i < _parameterMap.getSliderParams().size(); i++)
    {
        _treeState.removeParameterListener(_parameterMap.getSliderParams()[i].paramID, this);
    }
    
    // buttons
    for (int i = 0; i < _parameterMap.getButtonParams().size(); i++)
    {
        _treeState.removeParameterListener(_parameterMap.getButtonParams()[i]._id, this);
    }
    
    // menus
    for (int i = 0; i < _parameterMap.getMenuParams().size(); i++)
    {
        _treeState.removeParameterListener(_parameterMap.getMenuParams()[i].ID, this);
    }
}

//==============================================================================
const juce::String ViatorgraphiceqAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ViatorgraphiceqAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ViatorgraphiceqAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ViatorgraphiceqAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ViatorgraphiceqAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ViatorgraphiceqAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ViatorgraphiceqAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ViatorgraphiceqAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ViatorgraphiceqAudioProcessor::getProgramName (int index)
{
    return {};
}

void ViatorgraphiceqAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

juce::AudioProcessorValueTreeState::ParameterLayout ViatorgraphiceqAudioProcessor::createParameterLayout()
{
    std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;
    
    // sliders
    for (int i = 0; i < _parameterMap.getSliderParams().size(); i++)
    {
        auto param = _parameterMap.getSliderParams()[i];
        
        if (param.isInt == ViatorParameters::SliderParameterData::NumericType::kInt)
        {
            auto range = juce::NormalisableRange<float>(param.min, param.max);
            
            if (param.isSkew == ViatorParameters::SliderParameterData::SkewType::kSkew)
            {
                range.setSkewForCentre(param.center);
            }
            
            params.push_back (std::make_unique<juce::AudioProcessorValueTreeState::Parameter>(juce::ParameterID { param.paramID, 1 }, param.name, param.name, range, param.initial, valueToTextFunctionInt, textToValueFunction));
        }
        
        else
        {
            auto range = juce::NormalisableRange<float>(param.min, param.max, 0.1f);
            
            if (param.isSkew == ViatorParameters::SliderParameterData::SkewType::kSkew)
            {
                range.setSkewForCentre(param.center);
            }
            
            params.push_back (std::make_unique<juce::AudioProcessorValueTreeState::Parameter>(juce::ParameterID { param.paramID, 1 }, param.name, param.name, range, param.initial, valueToTextFunctionFloat, textToValueFunction));
        }
    }
    
    // buttons
    for (int i = 0; i < _parameterMap.getButtonParams().size(); i++)
    {
        auto param = _parameterMap.getButtonParams()[i];
        params.push_back (std::make_unique<juce::AudioParameterBool>(juce::ParameterID { param._id, 1 }, param._name, false));
    }
    
    // menus
    for (int i = 0; i < _parameterMap.getMenuParams().size(); i++)
    {
        auto param = _parameterMap.getMenuParams()[i];
        params.push_back (std::make_unique<juce::AudioParameterInt>(juce::ParameterID { param.ID, 1 }, param.name, param.min, param.max, param.defaultIndex));
    }
        
    return { params.begin(), params.end() };
}

void ViatorgraphiceqAudioProcessor::parameterChanged(const juce::String &parameterID, float newValue)

{
    if (_eqFilters.size() == 10)
    {
        updateParameters();
    }
}

void ViatorgraphiceqAudioProcessor::updateParameters()
{
    // input
    _inputProcessor.setGainDecibels(_treeState.getRawParameterValue(ViatorParameters::inputID)->load());
    
    // filters
    _lpFilter.setParameter(viator_dsp::SVFilter<float>::ParameterId::kCutoff, _treeState.getRawParameterValue(ViatorParameters::lowpassID)->load());
    _hpFilter.setParameter(viator_dsp::SVFilter<float>::ParameterId::kCutoff, _treeState.getRawParameterValue(ViatorParameters::highpassID)->load());
    
    // eq
    _eqFilters[0]->setParameter(viator_dsp::SVFilter<float>::ParameterId::kGain, _treeState.getRawParameterValue(ViatorParameters::band1GainID)->load());
    _eqFilters[1]->setParameter(viator_dsp::SVFilter<float>::ParameterId::kGain, _treeState.getRawParameterValue(ViatorParameters::band2GainID)->load());
    _eqFilters[2]->setParameter(viator_dsp::SVFilter<float>::ParameterId::kGain, _treeState.getRawParameterValue(ViatorParameters::band3GainID)->load());
    _eqFilters[3]->setParameter(viator_dsp::SVFilter<float>::ParameterId::kGain, _treeState.getRawParameterValue(ViatorParameters::band4GainID)->load());
    _eqFilters[4]->setParameter(viator_dsp::SVFilter<float>::ParameterId::kGain, _treeState.getRawParameterValue(ViatorParameters::band5GainID)->load());
    _eqFilters[5]->setParameter(viator_dsp::SVFilter<float>::ParameterId::kGain, _treeState.getRawParameterValue(ViatorParameters::band6GainID)->load());
    _eqFilters[6]->setParameter(viator_dsp::SVFilter<float>::ParameterId::kGain, _treeState.getRawParameterValue(ViatorParameters::band7GainID)->load());
    _eqFilters[7]->setParameter(viator_dsp::SVFilter<float>::ParameterId::kGain, _treeState.getRawParameterValue(ViatorParameters::band8GainID)->load());
    _eqFilters[8]->setParameter(viator_dsp::SVFilter<float>::ParameterId::kGain, _treeState.getRawParameterValue(ViatorParameters::band9GainID)->load());
    _eqFilters[9]->setParameter(viator_dsp::SVFilter<float>::ParameterId::kGain, _treeState.getRawParameterValue(ViatorParameters::band10GainID)->load());
    
    _eqFilters[0]->setParameter(viator_dsp::SVFilter<float>::ParameterId::kQ, std::abs(_treeState.getRawParameterValue(ViatorParameters::band1GainID)->load()) * 0.04);
    _eqFilters[1]->setParameter(viator_dsp::SVFilter<float>::ParameterId::kQ, std::abs(_treeState.getRawParameterValue(ViatorParameters::band2GainID)->load()) * 0.04);
    _eqFilters[2]->setParameter(viator_dsp::SVFilter<float>::ParameterId::kQ, std::abs(_treeState.getRawParameterValue(ViatorParameters::band3GainID)->load()) * 0.04);
    _eqFilters[3]->setParameter(viator_dsp::SVFilter<float>::ParameterId::kQ, std::abs(_treeState.getRawParameterValue(ViatorParameters::band4GainID)->load()) * 0.04);
    _eqFilters[4]->setParameter(viator_dsp::SVFilter<float>::ParameterId::kQ, std::abs(_treeState.getRawParameterValue(ViatorParameters::band5GainID)->load()) * 0.04);
    _eqFilters[5]->setParameter(viator_dsp::SVFilter<float>::ParameterId::kQ, std::abs(_treeState.getRawParameterValue(ViatorParameters::band6GainID)->load()) * 0.04);
    _eqFilters[6]->setParameter(viator_dsp::SVFilter<float>::ParameterId::kQ, std::abs(_treeState.getRawParameterValue(ViatorParameters::band7GainID)->load()) * 0.04);
    _eqFilters[7]->setParameter(viator_dsp::SVFilter<float>::ParameterId::kQ, std::abs(_treeState.getRawParameterValue(ViatorParameters::band8GainID)->load()) * 0.04);
    _eqFilters[8]->setParameter(viator_dsp::SVFilter<float>::ParameterId::kQ, std::abs(_treeState.getRawParameterValue(ViatorParameters::band9GainID)->load()) * 0.04);
    _eqFilters[9]->setParameter(viator_dsp::SVFilter<float>::ParameterId::kQ, std::abs(_treeState.getRawParameterValue(ViatorParameters::band10GainID)->load()) * 0.04);
    
    // drive
    auto rawDrive = _treeState.getRawParameterValue(ViatorParameters::driveID)->load();
    _drive.store(juce::Decibels::decibelsToGain(rawDrive));
    
    // output
    _outputProcessor.setGainDecibels(_treeState.getRawParameterValue(ViatorParameters::outputID)->load());
}

//==============================================================================
void ViatorgraphiceqAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    _spec.maximumBlockSize = samplesPerBlock;
    _spec.numChannels = getTotalNumInputChannels();
    _spec.sampleRate = sampleRate;
    
    _inputProcessor.prepare(_spec);
    _inputProcessor.setRampDurationSeconds(0.02);
    
    _eqFilters.clear();
    for (int i = 0; i < 10; i++)
    {
        _eqFilters.add(std::make_unique<viator_dsp::SVFilter<float>>());
        _eqFilters[i]->prepare(_spec);
        _eqFilters[i]->setParameter(viator_dsp::SVFilter<float>::ParameterId::kType, viator_dsp::SVFilter<float>::FilterType::kBandShelf);
        _eqFilters[i]->setParameter(viator_dsp::SVFilter<float>::ParameterId::kQType, viator_dsp::SVFilter<float>::QType::kParametric);
        _eqFilters[i]->setParameter(viator_dsp::SVFilter<float>::ParameterId::kCutoff, _eqCutoffs[i]);
    }
    
    _lpFilter.prepare(_spec);
    _lpFilter.setParameter(viator_dsp::SVFilter<float>::ParameterId::kType, viator_dsp::SVFilter<float>::FilterType::kLowPass);
    _lpFilter.setParameter(viator_dsp::SVFilter<float>::ParameterId::kQType, viator_dsp::SVFilter<float>::QType::kParametric);
    
    _hpFilter.prepare(_spec);
    _hpFilter.setParameter(viator_dsp::SVFilter<float>::ParameterId::kType, viator_dsp::SVFilter<float>::FilterType::kHighPass);
    _hpFilter.setParameter(viator_dsp::SVFilter<float>::ParameterId::kQType, viator_dsp::SVFilter<float>::QType::kParametric);
    
    _outputProcessor.prepare(_spec);
    _outputProcessor.setRampDurationSeconds(0.02);
    
    levelGain.reset(sampleRate, 0.5);
    
    updateParameters();
}

void ViatorgraphiceqAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ViatorgraphiceqAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    return layouts.getMainOutputChannelSet() == juce::AudioChannelSet::mono()
        || layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo();
}
#endif

void ViatorgraphiceqAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    // mono to stereo
    if (getTotalNumInputChannels() == 1 && getTotalNumOutputChannels() == 2)
    {
        // copy main left input to main stereo out
        for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
        {
            auto* mainSamplesIn = buffer.getWritePointer(0);
            auto* mainSamplesOut = buffer.getWritePointer(channel);

            for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
            {
                mainSamplesOut[sample] = mainSamplesIn[sample];
            }
        }
    }
    
    juce::dsp::AudioBlock<float> block {buffer};
    
    // input
    _inputProcessor.process(juce::dsp::ProcessContextReplacing<float>(block));
    
    // filters
    _lpFilter.process(juce::dsp::ProcessContextReplacing<float>(block));
    _hpFilter.process(juce::dsp::ProcessContextReplacing<float>(block));
    for (auto& filter : _eqFilters)
    {
        filter->process(juce::dsp::ProcessContextReplacing<float>(block));
    }
    
    // drive
    if (_drive.load() > 1.0)
    {
        viator_utils::utils::multiplyBlock(block, _drive.load());
        viator_utils::utils::softClipBlock(block);
    }
    
    // output
    _outputProcessor.process(juce::dsp::ProcessContextReplacing<float>(block));
    
    // get meter value
    calculatePeakSignal(buffer);
}

void ViatorgraphiceqAudioProcessor::calculatePeakSignal(juce::AudioBuffer<float> &buffer)
{
    levelGain.skip(buffer.getNumSamples());
    peakDB = buffer.getMagnitude(0, 0, buffer.getNumSamples());
    
    if (peakDB < levelGain.getCurrentValue())
    {
        levelGain.setTargetValue(peakDB);
    }

    else
    {
        levelGain.setCurrentAndTargetValue(peakDB);
    }
}

float ViatorgraphiceqAudioProcessor::getCurrentPeakSignal()
{
    return juce::Decibels::gainToDecibels(levelGain.getNextValue());
}

//==============================================================================
bool ViatorgraphiceqAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ViatorgraphiceqAudioProcessor::createEditor()
{
    return new ViatorgraphiceqAudioProcessorEditor (*this);
    //return new juce::GenericAudioProcessorEditor (*this);
}

//==============================================================================
void ViatorgraphiceqAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    _treeState.state.appendChild(variableTree, nullptr);
    juce::MemoryOutputStream stream(destData, false);
    _treeState.state.writeToStream (stream);
}

void ViatorgraphiceqAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    auto tree = juce::ValueTree::readFromData (data, size_t(sizeInBytes));
    variableTree = tree.getChildWithName("Variables");
    
    if (tree.isValid())
    {
        _treeState.state = tree;
        _width = variableTree.getProperty("width");
        _height = variableTree.getProperty("height");
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ViatorgraphiceqAudioProcessor();
}
