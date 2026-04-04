# 2023 电赛 H 题 信号分离装置（STM32 + FPGA）

推荐 GitHub 仓库名：`2023-NUEDC-H-Signal-Separation-STM32-FPGA`

本项目为 2023 年全国大学生电子设计竞赛 H 题“信号分离装置”的完整工程实现，包含：

- `STM32` 侧的混合信号采样、FFT 频谱分析、波形/频率判别、参数下发与人机交互
- `FPGA` 侧的串口参数接收、频率字/相位字转换、DDS 波形重建与双路输出
- `doc` 目录下的赛题、框图和设计报告，便于快速理解整套系统

## 项目概述

系统目标是将两路输入信号 `A`、`B` 经加法器混合得到 `C = A + B` 后，再从混合信号 `C` 中分离恢复出两路信号 `A'`、`B'`。

根据当前工程和设计报告，本系统采用如下分工：

- 前级模拟电路：完成信号叠加与 ADC 前端调理
- STM32F407ZGT6：对混合信号 `C` 进行 ADC + DMA 采样与 FFT 分析，识别波形类型和频率，并通过串口发送给 FPGA
- FPGA（EP4CE6F17C8）：根据 STM32 下发的参数进行 DDS 波形重建，输出两路分离信号，并支持初相位差设置

设计报告中给出的实现结果包括：

- 能从混合信号中分离出 `A'`、`B'`
- 输出峰峰值不小于 1V
- 分离后波形与原波形保持一致，不失真
- 可设置两路输出的初相位差

## 目录结构

```text
.
├─doc/                         赛题、框图、设计报告
│  ├─H题_信号分离装置.pdf
│  ├─大学生电子设计竞赛设计报告.docx
│  └─电赛框图.jpg
├─STM32/                       STM32F407 工程
│  ├─Src/                      主程序与外设初始化
│  ├─Inc/                      头文件
│  ├─FFT/                      FFT 封装
│  ├─TIMER_DMA_ADC/            ADC + DMA + TIM 采样逻辑
│  ├─UART/                     串口通信与参数交互
│  ├─SD/                       SD 卡相关代码
│  ├─DSP_LIB/                  DSP/FFT 相关库
│  └─MDK-ARM/                  Keil MDK 工程
├─FPGA/
│  ├─rtl/                      Verilog 源码与顶层设计
│  ├─sim/                      仿真测试文件
│  ├─matlab_prj/               ROM 波形表生成相关文件
│  └─quartus_prj/              Quartus 工程
└─README.md
```

## 关键实现说明

### STM32 部分

STM32 主要负责信号识别与参数发送：

- 使用定时器触发 ADC 采样，DMA 搬运采样数据
- 对采样结果执行 FFT，提取频谱幅值
- 判断混合信号中的两路频率与波形类型
- 将识别结果通过串口发送给 FPGA
- 处理串口输入的阈值、相位等控制参数
- 向 HMI/串口屏输出识别结果

代码重点可以从这些位置开始看：

- `STM32/Src/main.c`
- `STM32/FFT/fft.c`
- `STM32/TIMER_DMA_ADC/timer_dma_adc.c`
- `STM32/UART/uart.c`

### FPGA 部分

FPGA 主要负责参数接收与波形重建：

- 串口接收来自 STM32 的波形类型、频率和相位参数
- 将频率参数转换为 DDS 所需频率字
- 将相位参数转换为相位控制字
- 通过 ROM 查表方式生成正弦波、三角波、方波、锯齿波
- 输出两路 DAC 数据

代码重点可以从这些位置开始看：

- `FPGA/rtl/competition.v`
- `FPGA/rtl/stm32_control.v`
- `FPGA/rtl/fre_pha_data_ctrl.v`
- `FPGA/rtl/wave_ctrl.v`
- `FPGA/rtl/wave_ctrl_fre_pha_data_ctrl.v`

## 开发环境

### STM32

- 主控：`STM32F407ZGT6`
- 工程形式：STM32CubeMX + Keil MDK-ARM
- 主要模块：ADC、DMA、TIM、USART、SDIO、CMSIS DSP

建议使用：

- STM32CubeMX 打开 `STM32/Competition.ioc`
- Keil 打开 `STM32/MDK-ARM/Competition.uvprojx`

### FPGA

- 器件：`EP4CE6F17C8`
- 工程形式：Quartus 工程 + Verilog RTL
- 仿真：ModelSim/Nativelink

建议使用：

- Quartus 打开 `FPGA/quartus_prj/competition.qpf`
- 顶层 RTL 位于 `FPGA/rtl/competition.v`

## 资料说明

`doc` 目录建议随仓库一并保留，便于后来者快速理解项目背景和设计思路：

- `H题_信号分离装置.pdf`：原始赛题
- `大学生电子设计竞赛设计报告.docx`：完整设计说明
- `电赛框图.jpg`：系统框图

如果后续准备公开仓库，建议在上传前检查文档中是否包含：

- 姓名、学号、学校等个人信息
- 指导教师信息
- 不希望公开的联系信息

## GitHub 上传建议

本仓库已配套提供 `.gitignore`，会忽略以下常见生成文件：

- Keil/MDK 的构建输出、用户配置、调试缓存
- Quartus 的 `db/`、`incremental_db/`、`output_files/`、仿真缓存
- ModelSim 仿真库和中间文件
- 常见系统临时文件

建议保留并提交的内容：

- `STM32/Src`、`STM32/Inc`、功能模块源码
- `STM32/Competition.ioc`
- `STM32/MDK-ARM/Competition.uvprojx`
- `FPGA/rtl`、`FPGA/sim`、`FPGA/matlab_prj`
- `FPGA/quartus_prj/competition.qpf`
- `FPGA/quartus_prj/competition.qsf`
- `doc` 目录

## 后续可继续完善

如果准备把这个项目正式公开到 GitHub，下一步通常会继续做这些整理：

- 补一个中文 `LICENSE`
- 增加仓库首页图片或系统框图预览
- 再细化一版“硬件连接说明”
- 补充 STM32 与 FPGA 的串口协议说明
- 检查并清理不必要的大体积生成文件

如果你愿意，我下一步可以继续直接帮你把仓库整理成“适合公开上传”的版本，比如继续补 `LICENSE`、精简目录、初始化 Git 仓库，或者顺手把 README 再升级成更适合展示的版本。
