Target Chase
=============

Target chase is a small method to chase and hit down a moving target.

This method assumes that there is no obstacle between the target and the weapon,
or our snake in this simulation.

We draw a line connecting the target and the snake and follow it. This line is 
updated as the snake moves. This process is repeated again and again until the 
target is down.

This simultion requires Turbo C graphics library to run. The user needs to supply
the initial coordinates of the snake and the target. Also, as the snake is made
to run in a straight line (to simplify the simulation), you also need to supply 
the increment in x and y coordinates of the snake as it moves in a 2D plane.
