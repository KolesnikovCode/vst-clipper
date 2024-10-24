/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/

class GainReductionMeter : public juce::Component, public juce::Timer
{
public:
    GainReductionMeter(VstclipperAudioProcessor& processor)
        : audioProcessor(processor)
    {
        startTimerHz(24); // Per second
    }
    
    float smoothedGainReduction = 0.0f;

    void paint(juce::Graphics& g) override
    {
        g.fillAll(juce::Colours::darkgrey);
        g.setColour(juce::Colours::white);

        auto height = static_cast<float>(getHeight()) * (juce::jlimit(0.0f, 40.0f, smoothedGainReduction) * 10.0f);

        g.fillRect(0.0f, 0.0f, static_cast<float>(getHeight()), height);
    }

    void timerCallback() override
    {
        // Gliding average
        auto currentGainReduction = audioProcessor.currentGainReduction.load();
        
        float smoothingFactor = 0.2f;

        smoothedGainReduction = smoothedGainReduction * (1.0f - smoothingFactor) + currentGainReduction * smoothingFactor;

        repaint();
    }


private:
    VstclipperAudioProcessor& audioProcessor;
};

class VstclipperAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                        public juce::Slider::Listener,
                                        public juce::Button::Listener
{
public:
    void sliderValueChanged(juce::Slider* slider) override;
    void buttonClicked(juce::Button* button) override;
    
    VstclipperAudioProcessorEditor (VstclipperAudioProcessor&);
    ~VstclipperAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void updateButtonStates();

private:
    VstclipperAudioProcessor& audioProcessor;
    
    juce::TextButton softBtn { "Soft" };
    juce::TextButton hardBtn { "Hard" };
    juce::Slider inputGainSlider;
    juce::Slider thresholdGainSlider;
    GainReductionMeter gainReductionMeter;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VstclipperAudioProcessorEditor)
};
