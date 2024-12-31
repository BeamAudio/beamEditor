1. Project Overview

Project Name: BeamEditor
Project Description: 

BeamEditor is a cross-platform audio editor designed for real-time audio processing and effects. 
It provides a framework for building custom audio processing pipelines with support for multi-channel audio, concurrent processing, and a variety of audio effects.

Target Audience:
--Audio developers and engineers.
--Researchers in audio processing and signal processing.
--Individuals interested in building custom audio effects and plugins.

2. Code Structure

Modular Design: The codebase is organized into well-defined modules with clear responsibilities:
Audio Engine: Handles core audio processing tasks, including:
--Audio I/O (input/output)
--Audio buffering and chunking
--Multi-threaded processing
--Synchronization between processing stages
--Effects Processing: Provides a framework for creating and applying audio effects:
--Circuit class: Represents an abstract audio effect unit.
--Concrete Circuit implementations for specific effects (e.g., EQ, Reverb, Delay).
--Utilities: Provides helper functions for common audio processing tasks (e.g., audio format conversion, signal analysis).

3. Key Classes and Modules

Mixer:
--Manages the audio mixing process.
--Coordinates the application of effects to multiple audio channels.
--Handles audio buffering, synchronization, and data flow between processing stages.

Circuit:
--Abstract base class for audio effects.
--Defines an interface for processing audio data.
--Concrete subclasses implement specific audio effects (e.g., EQCircuit, ReverbCircuit).

CircuitWorker:
--Handles the processing of audio chunks for a specific Circuit on a separate thread.
--Ensures efficient and concurrent processing of audio data.

AudioUtils:
--Provides a collection of utility functions for audio processing tasks:
---makeChunks(): Splits audio data into smaller chunks for efficient processing.
---mixChannelsChunk(): Mixes multiple audio channels together chunk by chunk.
