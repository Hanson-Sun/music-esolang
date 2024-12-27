# Polyphony - A MIDI Based Esolang

Polyphony is a concatenative stack based language that is written using MIDI files. Inspirtations for the language design stem from languages like Forth, Joy, and uiua.

Polyphony is interpreted.

Polyphony only supports one type of literals: 64 byte integers (`int64_t`). This was done to reduce the complexity of the 


In general, there are certain chords reserved as keywords. For our purposes, a chord is defined as 2 or more notes played simultaneously. Outside of this, single notes are considered as literals.

Polyphony operates on a base 12 number system. Each semitone represents a digit, with `C` being `0` and `B` being `11`. The octave doesn't matter so both `C#4` and `C#5` represent 1. 

As opposed to being identified by the specific notes in a chord, chords are identified by the intervals between its notes (More specifically, the amount of semitones between any 2 consective notes of a chord as opposed to intervals based on scales). From now on, we will represent a chord by its intervals. ex: an octave is written as `(13)` and the chord $CC#E$ is written as `(2,4)`. 

If a chord that is not recognized as a keyword is played, the highest note of the chord will be used as the literal.
