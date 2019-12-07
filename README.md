# Nissan-Leaf-BatterySaver
Code that powers the BatterySaver feature found on Muxsan CAN-bridge installs made by Dala's EV Repair.
Note that this is simply an extract of the lines specific to the BatterySaver, but very useful if you wish to implement your own similar logic. Also feel free to point out potential improvement areas :)

## What is BatterySaver
BatterySaver is a fully customizable maximum charge limiter, extending on the one that was originally shipped with 2011-2013 LEAFs. The stock limiter only allowed setting two limiters, 80 and 100%. The BatterySaver can set the max charge percentage in six steps, 50, 58, 66, 75, 83, 92, 100 %. 

## Why do I need a maximum charge limiter?
To preserve battery life. By not charging the battery fully, you avoid long term battery degradation. Lithium batteries degrade faster when they are sitting at high voltages. The lower you get your discharge cycles, the better the battery will perform long term. For example much better to stay between 20-80% than it is to be at 0-100%. 

## What is this suitable for?
Owners of Nissan Leaf/e-NV200, particularly with bigger batteries (30/40/62kWh)

## How to use it?
To set the charge limiter, turn on re-circulation, and set fan speed to max (7). You'll see the dash capacity bars start to move, along with the state of charge %. When the desired charge max % is reached, it can be set by turning down the fan speed. Check out the video below.

BatterySaver in action, youtube link: 
[![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/tOSR3UZ7kgU/0.jpg)](https://www.youtube.com/watch?v=tOSR3UZ7kgU)
