# DrumKit

Drum Modules for VCVRack.

## Modules

### BD-9

![BD-9](images/BD9.png)

16 bass drums based on the TR909.

### Snare-N

![SNARE](images/Snare.png)

16 snare drums based on the Novation Drumstation.

### Closed HiHats

![HH-C](images/HHC.png)

15 closed high hats.

### Open HiHats

![HH-O](images/HHO.png)

14 open high hats.

### DMX

![DMX](images/DMX.png)

Full DMX Kit, starting at Octave 4, C#, and continuing for 12 steps.

Anything above/below this will not trigger a note.  If a note is triggered, it
will not trigger again until a note out of range is sent.

### CR78

![CR79](images/CR78.png)

CompuRhythm-78 Kit, starting at Octave 4, C#, and continuing for 7 steps.

Anything above/below this will not trigger a note.  If a note is triggered, it
will not trigger again until a note out of range is sent.

### Synthetic Bass Drum

![SBD](images/SBD.png)

Synthetic Bass Drum, with a submodulator.

### DMX mk2

![DMXmk2](images/DMXmk2.png)

A less experimental version of the DMX module.  It contains a DMX kit, that is
accessible by 1 of the 12 samples via the `V/OCT` input, with Octave 4, C# as the
first.  Unlike the DMX module, it is triggered when `GATE` is brought up to or above
`1.7v`.  In addition, you can control the tone by either adjusting the `TONE`
knob, or via the `TONE CV` input.

### CR78 mk2

![CR78mk2](images/CR78mk2.png)

A less experimental version of the CR78 module.  It contains a CR78 kit, that is
accessible by 1 of the 7 samples via the `V/OCT` input, with Octave 4, C# as the
first.  Unlike the CR78 module, it is triggered when `GATE` is brought up to or above
`1.7v`.  In addition, you can control the tone by either adjusting the `TONE`
knob, or via the `TONE CV` input.

## Building

Building requires [SynthDevKit](https://github.com/JerrySievert/SynthDevKit),
which will be checked out as part of the build initialization.

```
$ git clone https://github.com/JerrySievert/DrumKit
$ cd DrumKit
$ git submodule init
$ git submodule update
$ make
```
