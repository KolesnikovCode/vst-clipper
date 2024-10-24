/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
VstclipperAudioProcessorEditor::VstclipperAudioProcessorEditor (VstclipperAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), gainReductionMeter(p)
{
    // Set window size
    setSize (420, 270);
    
    // Make visible our components
    addAndMakeVisible(softBtn);
    addAndMakeVisible(hardBtn);
    addAndMakeVisible(inputGainSlider);
    addAndMakeVisible(thresholdGainSlider);
    addAndMakeVisible(gainReductionMeter);
    
    // Set components
    inputGainSlider.setRange(0, 2.0);
    thresholdGainSlider.setRange(0.1, 1.0);
    
    inputGainSlider.setValue(1.0);
    thresholdGainSlider.setValue(1.0);
    
    inputGainSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    thresholdGainSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    
    
    inputGainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    thresholdGainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    
    inputGainSlider.addListener(this);
    thresholdGainSlider.addListener(this);
    
    // TRY LAMBDA, PO PRIKOLU EPT)
    softBtn.onClick = [this]
    {
        audioProcessor.isHardClippingMode = false;
        
        softBtn.setAlpha(1.0);
        hardBtn.setAlpha(0.6);
    };
    hardBtn.onClick = [this]
    {
        audioProcessor.isHardClippingMode = true;

        softBtn.setAlpha(0.6);
        hardBtn.setAlpha(1.0);
    };
}

VstclipperAudioProcessorEditor::~VstclipperAudioProcessorEditor()
{
}

//==============================================================================
void VstclipperAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour::fromRGB(39, 36, 36));
}

void VstclipperAudioProcessorEditor::resized()
{
    int sliderSize = 100;
    
    inputGainSlider.setBounds(getWidth() / 2 - (sliderSize + sliderSize / 2), 100, sliderSize, sliderSize);
    thresholdGainSlider.setBounds(getWidth() / 2 + (sliderSize / 2), 100, sliderSize, sliderSize);
    
    softBtn.setBounds(getWidth() / 2 - 100, 20, 100, 50);
    hardBtn.setBounds(getWidth() / 2 + 10, 20, 100, 50);
    
    gainReductionMeter.setBounds(getWidth() - 32, 20 , 10, getHeight() - 40);
}

void VstclipperAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &inputGainSlider)
    {
        audioProcessor.inputGainValue = inputGainSlider.getValue();
    }
    
    if (slider == &thresholdGainSlider)
    {
        audioProcessor.thresholdGainValue = thresholdGainSlider.getValue();
    }
}

void VstclipperAudioProcessorEditor::buttonClicked(juce::Button* button)
{
}

