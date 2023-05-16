# step_counter
## Project name and motivation
- As its name, this project be created in order to count the number of steps when people walk 
- This project is created for learning STM32
## Hardware required
- STM32F103C8T6
- LCD 1602A and driver HD44780 to display the number of step
- sensor GY-521 6DOF IMU MPU6050
## Project description
- Both LCD and IMU sensor using I2C communicate with uC
- Using a timer to sample data form sensor, pass the obtained data through a low filter to reduce noise and detect the peek of data stream
 and treat a peek like a step

![total_a_and_g](https://github.com/Truongdhvnu/stepCounter/assets/122275694/8c71e22d-74c3-46b4-b9b8-76ecf0ed0442)
