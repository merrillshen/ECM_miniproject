# Individual project - Energy saving automatic outside light

## Script and Function Description

The main script has 5 main components that have been separated by a blank line.
The initial portions are used as initialisers for the other function scripts being used, as well as variables that will be used in the functions.
Testing mode can be changed in line 63 of main.c (see comments)

Thereafter, the next chunk of variables are to be set by ther user upon initilisation of the clock, which includes the current date, time and day of the week, as well as the brightness level at which for the light to turn off/on. 

Within the while loops, we will go through each segment chronologically:

- 1st Segment: Turns on the LED when the brightness level of the ambiant environment falls below the preset brightness level, and turns off when is above or the time is between 1am and 5am. 

- 2nd Segment: Adjusts for the leap year if the year is devisable by 4. If so, it changes the days in February to 29, and back to 28 if not a leap year. 

- 3rd Segment: The clock function to keep track of time. 
Using the timer.c script, every passing second toggles the on-board LED RD7 (chosen for visualisation), where the main script detects the change in value and thereby adds 1 second to the clock. Upon reaching 60 seconds, the function adds 1 minute and resets the second clock back to 0. This carries on for hours, days, day of the week, months and year. Flags dawn_and_dusk and daylight_flag are also reset for each month here. Its function is explained below in the 4th and 5th Segment. 

- 4th Segment: Adjusting for Daylight Savings
This function checks if it is the last Sunday of the month, and then adds and subtracts an hour for the month of March and October respectively. After executing for the first time for the respective months, the daylight_flag is then set to 1 to ensure that it only is done once during the whole month, and is reset once the month is over in the 3rd Segment. 

- 5th Segment: Sychronisation with the Sun Monthly 
Triggered every 25th of the month, the function first checks if it is the first time it is being executed for the month. Thereafter, the time at which sunrise and sunset is recorded in the array dawn_dusk, ensuring that it is within the stipulated time ranges of 4am and 9am for mornings and 3pm and 10pm for evenings for increased accuracy. The timings are converted into minutes for easier calculation of the midtime, which is then compared to 1200 noon which is 720 in minutes. Thereafter, the clock is adjusted accordingly to either a surplace or deficit. 

The while loop ends with some code to display the current time and date on the LCD screen for easy testing and debugging purposes, ensuring that the code is functional.



# Original Project Description
## Learning outcomes

The principal learning objectives for this project are:

- Implement a working microprocessor based system to achieve a more complex real world task
- Develop your ability to independently plan, organise and structure your code 
- Improve your grasp of the C language and writing your own functions

## Brief

Outside lights that respond to ambient light levels are commonplace (i.e. street lights, porch lights, garden lights etc). These types of lights switch on at dusk and then turn off at dawn. However, energy and money can be saved if these lights are switched off during the small hours of the morning (for example, between 1am and 5am), when there are very few people around. Many councils in the UK have implemented/trialled this idea for street lights (https://www.bbc.co.uk/news/uk-england-derbyshire-16811386). Your task is to use the knowledge of microcontrollers and hardware that you have gained in labs 1-3 from this module to develop a fully automated solution.

## Specification
Design and program a device that meets the following requirements:

1. Monitors light level with the LDR and turns on an LED in low light conditions (i.e. night-time) and off in bright conditions (i.e. daytime)
1. Displays the current hour of day on the LED array in binary
1. Turns the light off between approx. 1am and 5am
1. Adjusts for daylight savings time
1. Maintain synchronicity with the sun indefinitely
1. Be fully automatic (requires zero maintenance after installation)

Please use this GitHub repo to manage your software development and submit your individual project code.

## Supplementary information and help
At first the task may seem quite straightforward but there are several points that often prove more tricky. The first is how to test code during development? You could test in real world conditions but you would be limited to one test cycle per day and this would severely slow down your development and debugging progress. To get around this you could implement a "testing mode" and pretend that a day lasts 24 seconds. This could be done using a #define directive to switch between "normal" and "testing" modes for your code.

Adjusting for daylight savings time is not too tricky. The clocks always change (in the UK) on the last Sunday in March (they go forward an hour) and the last Sunday in October (they go back an hour). One method of achieving this is to initialise what day it is when device is first switched on (using manual input) and then keep track of the days that pass and what the day of the week it is. Another method might be to automatically figure out what time of year it is (see below). Also don't forget about leap years! 

No clock is perfect, they can all run slightly fast/slow and can by influenced be external factors such as temperature. Ultimately this will result in drift over time and eventually the time will drift so far out of sync with real time that it is meaningless. For the purposes of our device the main requirement is that it remains in sync with the sun. You could use light from the sun to keep your clock in sync. Although the length of daylight varies considerably during the year, the midpoint between dusk and dawn only varies by a few minutes. This is termed solar midnight approx. 12am or solar noon approx. 12pm. One method of staying in sync with the sun is to use the LDR and record/calculate when these times occur and adjust your clock accordingly. The length of daylight also tells us information about what time of year it is and can be used to help us know when to adjust for daylight savings time.

![Day length](gifs/day-length-london.jpg)
http://wordpress.mrreid.org/2010/10/31/why-change-the-clocks/





