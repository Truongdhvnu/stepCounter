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


![z4407572446357_236b684f6dac4ae0b6114ae23ebd2399](https://github.com/Truongdhvnu/stepCounter/assets/122275694/df9059fa-5779-4ad0-9348-b3716b112c57)
