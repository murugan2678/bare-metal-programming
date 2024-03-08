
                                               Device name : STM32F401CCU6 

					        program for stepper motor

                                                                         
Author : Murugan M						



what is stepper motor
   
    * A stepper mottor is a brushless, synchronous electric motor that converts digital pulses into mechanical shaft rotation.     
    * They convert electric energy into mechanical energy.
    * A stepper mottor is a brusless DC electric motor. DC direct current.
    * stepper motor is maximum speed for 1000rpms  rpms ---> revolutions per minute

-------------------------------------------------------------------------------------------------------------------------------------------

Hardware 
     
    * stepper motor 5v voltage  DC direct current
    * stepper motor model 28BYJ-48, 5-wire, 4phase
    * driver L293D
    * 5-wires color pinout

        --> 1. Blue for first pin
	--> 2. Pink for second pin
	--> 3. Yellow for third pin
	--> 4. Orange for fourth pin
	--> 5. Red for fifth pin

    * stepper motor step by step rotate
    * this motor delay give means 5ms mille seconds properly rotate

-------------------------------------------------------------------------------------------------------------------------------------------

stepper motor are usage
    
    * cars, printers, computers, washing machines, electric razors 
 
stepper motor inside 
    
    * permanent magnet rotor 96-pole, upper pole 48, lower pole 48, 48 + 48 = totally 96 pole 
    * 8 pole stator coil

-------------------------------------------------------------------------------------------------------------------------------------------

register config program steps

    * RCC_CR clock control register
    * RCC_CFGR clock config register 
    * RCC_AHB1ENR peripheral clock enable register
    * GPIOA_MODER port mode register GPIO pin config
    * GPIOA_ODR port data output register
    * STK_CTRL STK systick timer, CTRL control and status register
    * STK_LOAD STK systick timer, LOAD reload value register

-------------------------------------------------------------------------------------------------------------------------------------------

stepper motor config
    
    * select for 48 pole magnent
    * select for 8 pole stator coil
    * you create char array[] this array send data for hexa decimal value in 8 bit value

-------------------------------------------------------------------------------------------------------------------------------------------

systick timer config
    
    * STK_LOAD systick reload value register, reload 0th bit to 23th bit, this value load = 124999 5ms milli seconds
    * STK_CTRL systick control and status register, counter enable 0th bit set 1
    * STK_CTRL systick control and status register, clock source selection, 2nd bit set 1, processor clock (AHB)
    * STK_CTRL systick control and status register, return 1 if timer count to 0 last time this was read
