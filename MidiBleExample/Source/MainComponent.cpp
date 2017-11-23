/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public AudioAppComponent,
                                private Button::Listener
{
public:
    //==============================================================================
    MainContentComponent()
    {
////////////////
        keyboardComponent = new MidiKeyboardComponent(keyboardState, MidiKeyboardComponent::horizontalKeyboard);
        addAndMakeVisible(keyboardComponent);
        
        addAndMakeVisible(deviceSettingButton);
        deviceSettingButton.setButtonText("Device Setting");
        deviceSettingButton.addListener(this);
        
        deviceManager.addMidiInputCallback(String(), &midiCollector);
  ////////////////
        
        setSize (800, 600);
        
        // specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }

    ~MainContentComponent()
    {
        shutdownAudio();
        keyboardComponent = nullptr;
    }

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override
    {
        // This function will be called when the audio device is started, or when
        // its settings (i.e. sample rate, block size, etc) are changed.

        // You can use this function to initialise any resources you might need,
        // but be careful - it will be called on the audio thread, not the GUI thread.

        // For more details, see the help for AudioProcessor::prepareToPlay()
    }

    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        // Your audio-processing code goes here!

        // For more details, see the help for AudioProcessor::getNextAudioBlock()

        // Right now we are not producing any data, in which case we need to clear the buffer
        // (to prevent the output of random noise)
        bufferToFill.clearActiveBufferRegion();

        ////////////
        // fill a midi buffer with incoming messages from the midi input.
        MidiBuffer incomingMidi;
        
        midiCollector.removeNextBlockOfMessages(incomingMidi, bufferToFill.numSamples);

        keyboardState.processNextMidiBuffer(incomingMidi, 0, bufferToFill.numSamples, true);
    }

    void releaseResources() override
    {
        // This will be called when the audio device stops, or when it is being
        // restarted due to a setting change.

        // For more details, see the help for AudioProcessor::releaseResources()
    }

    //==============================================================================
    void paint (Graphics& g) override
    {
        // (Our component is opaque, so we must completely fill the background with a solid colour)
        g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

        // You can add your drawing code here!
    }

    void resized() override
    {
        // This is called when the MainContentComponent is resized.
        // If you add any child components, this is where you should
        // update their positions.
        
        deviceSettingButton.setBoundsRelative(0.3, 0.2, 0.4, 0.2);
        
        keyboardComponent->setBoundsRelative(0.0, 0.7, 1.0, 0.3);
        
    }
    
    void buttonClicked (Button* button) override
    {
        if(button == &deviceSettingButton)
            showDeviceSetting();
    }


private:
    //==============================================================================

    // Your private member variables go here...
    void showDeviceSetting() {
        AudioDeviceSelectorComponent selector(deviceManager,
                                              0, 256,
                                              0, 256,
                                              true, true,
                                              true, false);
        
        selector.setSize(400, 600);
        
        DialogWindow::LaunchOptions dialog;
        dialog.content.setNonOwned(&selector);
        dialog.dialogTitle = "Audio/MIDI Device Settings";
        dialog.componentToCentreAround = this;
        dialog.dialogBackgroundColour = getLookAndFeel().findColour(ResizableWindow::backgroundColourId);
        dialog.escapeKeyTriggersCloseButton = true;
        dialog.useNativeTitleBar = false;
        dialog.resizable = false;
        dialog.useBottomRightCornerResizer = false;
        
        dialog.runModal();
    }
    
    MidiMessageCollector midiCollector;
    
    TextButton deviceSettingButton;
    
    MidiKeyboardState keyboardState;
    MidiKeyboardComponent* keyboardComponent;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent()     { return new MainContentComponent(); }
