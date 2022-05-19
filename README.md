1.Download the updated software source code to your working directory $EE6470.<br>
cd $EE6470<br>
git clone https://github.com/ku1248/ESL_HW4.git<br>
cp -r ESL_HW4/riscv-vp/vp/src/platform/* $EE6470/riscv-vp/vp/src/platform<br>
cp -r ESL_HW4/riscv-vp/sw/* $EE6470/riscv-vp/sw<br>
<br>
2.Build the "basic-acc" platform of riscv-vp:<br>
cd $EE6470<br>
cd riscv-vp/vp/build<br>
cmake ..<br>
make install<br><br>
Build Gaussian filter software:<br>
cd $EE6470<br>
cd riscv-vp/sw<br>
cd basic-gaussian<br>
make<br>
make sim<br>
