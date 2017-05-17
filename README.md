# Keybot
A robot which plays a keyboard as an accompaniment to live music. Project developed for Embedded Systems course, Summer 2016, Saarland University

Feed it an audio stream through an aux-in line, adjust the tempo and input scale of the song, and just place it on your garageband (or any similar) software!
And listen to the robot play!

This robot was made using 2 Ardunos and an Audio Codec Shield. The audio input is fed in using the Audio Codec Shield, the signal processing and chord identification is done in the 1st arduino, and aucuator control is done in the 2nd Arduino.

The primary challenge here was to do a highly accurate spectrum analysis, and make it happen quickly enough so that the robot can plays the chord in time.
This led us to come up with our own modified version of DFT, and an intelligent note-mapping algorithm on top of it.

Unfortunately, we had to dismantle the robot after our final presentation, but you can see a video of it here: [Keybot on Youtube](https://www.youtube.com/watch?v=9O1LR_27O3g)
