Stopwatch with Dual Mode (Increment and Countdown) Using ATmega32

Overview

This project implements a digital stopwatch with two operational modes: increment (count-up) and countdown. It is built using an ATmega32 microcontroller, six multiplexed seven-segment displays (common anode), and push buttons for user interaction.

Features

Increment Mode (Default): Counts upwards from zero, displaying hours, minutes, and seconds.

Countdown Mode: Counts down from a user-defined time.

Reset Function: Resets the time to zero in both modes.

Pause Function: Pauses the time counting.

Resume Function: Resumes counting from the paused time.

Mode Toggle: Switches between increment and countdown modes.

Hardware Requirements

ATmega32 Microcontroller

Six Common Anode Seven-Segment Displays

Push Buttons for control (Pause, Resume, Reset, Mode Toggle, Increment/Decrement Time)

Buzzer (for alert on countdown completion)

Resistors & Capacitors as needed

Software & Tools

AVR Studio / Atmel Studio for programming

AVR GCC Compiler

USBasp or Any AVR Programmer

Proteus (Optional for Simulation)

Proteus Simulation

Proteus can be used to simulate the stopwatch circuit before actual hardware implementation. The simulation setup includes:

ATmega32 microcontroller with the preloaded firmware.

Virtual seven-segment displays to show the time.

Push button simulations to test increment, decrement, pause, reset, and mode toggle functions.

Buzzer component to confirm countdown completion.

Using Proteus allows for debugging the circuit design, checking timing accuracy, and verifying button operations before moving to real hardware.

Working Principle

The seven-segment displays are multiplexed to show hours, minutes, and seconds.

A timer interrupt (CTC mode) is used for accurate time tracking.

External interrupts handle the reset, pause, resume, and mode toggle functions.

The buzzer alerts when the countdown reaches zero.

Installation & Usage

Flash the provided code to the ATmega32 using a programmer.

Connect the seven-segment displays, buttons, and other peripherals.

Use the buttons to:

Start/Stop/Pause the stopwatch

Switch between Increment and Countdown mode

Set time manually in Countdown mode

The display updates in real time to reflect the current time value.

Future Enhancements

Adding an LCD display for better readability.

Implementing a Real-Time Clock (RTC) module.

EEPROM storage to retain time during power loss.

License

This project is open-source. Feel free to modify and improve it!