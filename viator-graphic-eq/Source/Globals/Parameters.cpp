#include "Parameters.h"


ViatorParameters::Params::Params()
{
    initSliderParams();
    initButtonParams();
    initMenuParams();
}

void ViatorParameters::Params::initSliderParams()
{
    using skew = SliderParameterData::SkewType;
    using type = SliderParameterData::NumericType;
    
    // globals
    _sliderParams.push_back({ViatorParameters::inputID,
        ViatorParameters::inputName,
        -20.0f, 20.0f, 0.0f,
        skew::kNoSkew,
        0.0,
        type::kFloat});
    _sliderParams.push_back({ViatorParameters::outputID,
        ViatorParameters::outputName,
        -20.0f, 20.0f, 0.0f,
        skew::kNoSkew,
        0.0,
        type::kFloat});
    _sliderParams.push_back({ViatorParameters::driveID,
        ViatorParameters::driveName,
        0.0f, 15.0f, 0.0f,
        skew::kNoSkew,
        0.0,
        type::kFloat});
    
    // filters
    _sliderParams.push_back({ViatorParameters::lowpassID,
        ViatorParameters::lowpassName,
        20.0f, 20000.0f, 20000.0f,
        skew::kSkew,
        1000.0,
        type::kFloat});
    
    _sliderParams.push_back({ViatorParameters::highpassID,
        ViatorParameters::highpassName,
        20.0f, 20000.0f, 20.0f,
        skew::kSkew,
        1000.0,
        type::kFloat});
    
    _sliderParams.push_back({ViatorParameters::band1GainID,
        ViatorParameters::band1GainName,
        -15.0f, 15.0f, 0.0f,
        skew::kNoSkew,
        0.0,
        type::kFloat});
    
    _sliderParams.push_back({ViatorParameters::band2GainID,
        ViatorParameters::band2GainName,
        -15.0f, 15.0f, 0.0f,
        skew::kNoSkew,
        0.0,
        type::kFloat});
    
    _sliderParams.push_back({ViatorParameters::band3GainID,
        ViatorParameters::band3GainName,
        -15.0f, 15.0f, 0.0f,
        skew::kNoSkew,
        0.0,
        type::kFloat});
    
    _sliderParams.push_back({ViatorParameters::band4GainID,
        ViatorParameters::band4GainName,
        -15.0f, 15.0f, 0.0f,
        skew::kNoSkew,
        0.0,
        type::kFloat});
    
    _sliderParams.push_back({ViatorParameters::band5GainID,
        ViatorParameters::band5GainName,
        -15.0f, 15.0f, 0.0f,
        skew::kNoSkew,
        0.0,
        type::kFloat});
    
    _sliderParams.push_back({ViatorParameters::band6GainID,
        ViatorParameters::band6GainName,
        -15.0f, 15.0f, 0.0f,
        skew::kNoSkew,
        0.0,
        type::kFloat});
    
    _sliderParams.push_back({ViatorParameters::band7GainID,
        ViatorParameters::band7GainName,
        -15.0f, 15.0f, 0.0f,
        skew::kNoSkew,
        0.0,
        type::kFloat});
    
    _sliderParams.push_back({ViatorParameters::band8GainID,
        ViatorParameters::band8GainName,
        -15.0f, 15.0f, 0.0f,
        skew::kNoSkew,
        0.0,
        type::kFloat});
    
    _sliderParams.push_back({ViatorParameters::band9GainID,
        ViatorParameters::band9GainName,
        -15.0f, 15.0f, 0.0f,
        skew::kNoSkew,
        0.0,
        type::kFloat});
    
    _sliderParams.push_back({ViatorParameters::band10GainID,
        ViatorParameters::band10GainName,
        -15.0f, 15.0f, 0.0f,
        skew::kNoSkew,
        0.0,
        type::kFloat});
}

void ViatorParameters::Params::initButtonParams()
{
    _buttonParams.push_back({ViatorParameters::hqID,
        ViatorParameters::hqName,
        false});
}

void ViatorParameters::Params::initMenuParams()
{
}
