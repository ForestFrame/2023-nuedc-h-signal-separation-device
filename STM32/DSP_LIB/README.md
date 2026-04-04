- 导入.lib文件；

- 添加宏定义 `__FPU_PRESENT=1,__FPU_USED=1,ARM_MATH_CM4,__CC_ARM` 每两个之间英文逗号隔开；

- 在Target中添加DSP_LIB文件夹中的Include路径；

- 在main.h文件中添加#include "arm_math.h"和#include "arm_const_structs.h"头文件。

  

