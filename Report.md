github : https://github.com/ku1248/ESL_HW4<br>

General description or introduction of the problem and your solution：<br>
&ensp;&ensp;We are required to port the Gaussian Blur module to the riscv-vp platform in order to run Gaussian Blur on riscv-vp. In Lab 8, we have run Sobel filter on riscv-vp. To run Sobel filter on riscv-vp, there are several steps to do which are similar to the steps in this homework but we have to change Sobel filter into Gaussian Blur. The implementation details will be described following.<br>
<br>
Implementation details (data structure, flows and algorithms) :<br>
&ensp;&ensp;We have to perform several changes in riscv-vp in order to run Gaussian Blur correctly. In main.cpp, where we implement the platform. We have to add the addresses for new module(Gaussian Blur module) which are Gaussian Blur start address, Gaussian Blur size, and Gaussian Blur end address. Also, since there are modules added to the system bus, we have to change the declaration of bus. And then instantiate the Gaussian Blur module, add port address mapping for the new module, and completer the bus initiator socket binding to Gaussian Blur module target socket. And for the Gaussian Blur module, we have to change Sobel filter module into Gaussian Blur module. We have implemented the Gaussian Blur algorithm several times in previous homework so I do not describe detailly here.<br>
<br>
Additional features of your design and models :<br>
&ensp;&ensp;Compared to the Gaussian Blur we implemented in previous homework, using riscv-vp platform to implement it, we are able to use DMA to transfer the data. By using DMA, we can transfer the data from testbench into Gaussian Blur module directly without using CPU.<br>
<br>
Experimental results (on different benchmarks and settings) :<br>
&ensp;&ensp;By using DMA to transfer data, we are able to transfer the data in higher speed since DMA provides us the function to transfer data directly. If we transfer data without DMA, we have to transfer data by CPU(memcpy). Because that transfer by CPU will use a lot of load store instruction which cost a lot of cycles, and during these cycles, we cannot perform computation. Thus, using CPU to transfer data takes more cycles to do Gaussian Blur algorithm.<br>
<br>
Discussions and conclusions :<br>
&ensp;&ensp;We have tried to use riscv-vp to implement Gaussian Blur module. And by riscv-vp platform, we are able to communicate between modules using DMA. And by using DMA, we can speed up the speed for Gaussian Blur comparing to previous homework. Also, by using riscv-vp platform, we can implement many modules which shares the same memory which may make the computation more efficiently. 
