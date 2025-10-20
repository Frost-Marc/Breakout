# Breakout

W Kavanagh & N Merchant. Summer 2024 

## controls

A/D to move the paddle right and left.
P to pause.

## Powerups

big/small paddle (blue)
fast/slow ball (fancy yellow)
fire ball (green)

# Tasklist

## Suggested fixes

* Fix the compiler issues in the code

## Suggested tasks

* Implement mouse input for pad
* Improved VFX (ball trail, paddle hit response, brick particle destruction)
* Better UI (progress bar rather than timer for countdown).
* GameLoop
* Better ball physics (Box2D)
* Leaderboards
* More ball types (e.g., multiball, sticky ball [where you shoot the ball from the paddle every time], tiny ball, big ball, brick-tracking ball)
* Sounds with increasing tone between bounces.
* Implement commentary with calls to an LLM such as LLama

# Time Details and Changelist
<Add information to this section about the time you've taken for this task along with a professional changelist.>

- fixed compiler error by commenting out #include of fire ball power up class from the .h of the base power up class to stop its circular dependancy - 5 mins
- chnaged colour of fireball power up to green to match specifications - 2 mins
- added trail vfx to ball, trail follows balls movement and direction by creating a ball at its past positions then removes them over time when it reaches the max trail length - 1hr
- added to trail vfx so that when the ball breaks a brick the trails colour is randomized and changed except for when it has the fireball power up - 1hr
- added new ball power up, big ball, doubles the radius of the ball - 1hr
- added new ball power up, small ball, halfs the radius of the ball - 15 mins
- added screen shake to the game, the screen shakes at varieying intensities and durations when the ball hits walls, ceiling and when loosing a life - 30mins
