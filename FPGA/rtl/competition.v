module competition (
    input wire clk,  //系统时钟,25MHz
    input wire rstn, //复位信号,低电平有效

    input wire uart_rx,

    output wire       dac_clk_A,  //输入DAC模块时钟
    output wire [7:0] data_out_A, //输入DAC模块波形数据

    output wire       dac_clk_B,  //输入DAC模块时钟
    output wire [7:0] data_out_B  //输入DAC模块波形数据
);

  parameter FRE_WIDTH = 10;
  parameter PHA_WIDTH = 8;

  wire [          0:0] clk_50M;

  wire [FRE_WIDTH-1:0] fre_y_A;
  wire  [FRE_WIDTH-1:0] fre_z_A;
  wire  [FRE_WIDTH-1:0] fre_w_A;

  wire  [PHA_WIDTH-1:0] pha_x_A;
  wire  [PHA_WIDTH-1:0] pha_y_A;

  wire  [FRE_WIDTH-1:0] fre_y_B;
  wire  [FRE_WIDTH-1:0] fre_z_B;
  wire  [FRE_WIDTH-1:0] fre_w_B;

  wire  [PHA_WIDTH-1:0] pha_x_B;
  wire  [PHA_WIDTH-1:0] pha_y_B;

  wire  [          3:0] f_type_A;
  wire  [          3:0] f_type_B;

  pll_50M pll_50M_inst1 (
      .areset(~rstn),
      .inclk0(clk),
      .c0    (clk_50M),
      .locked()
  );

  //dac_clka:DAC模块时钟
  assign dac_clk_A = ~clk_50M;
  assign dac_clk_B = ~clk_50M;

  stm32_control u_stm32_control (
      .clk_50M (clk_50M),
      .rstn    (rstn),
      .uart_rx (uart_rx),
      .f_type_A(f_type_A),
      .f_type_B(f_type_B),
      .fre_y_A (fre_y_A),
      .fre_z_A (fre_z_A),
      .fre_w_A (fre_w_A),
      .pha_x_A (pha_x_A),
      .pha_y_A (pha_y_A),
      .fre_y_B (fre_y_B),
      .fre_z_B (fre_z_B),
      .fre_w_B (fre_w_B),
      .pha_x_B (pha_x_B),
      .pha_y_B (pha_y_B)
  );

  wave_ctrl_fre_pha_data_ctrl u_wave_ctrl_fre_pha_data_ctrl1 (
      .clk_50M (clk_50M),
      .rstn    (rstn),
      .fre_x   (10'd0),
      .fre_y   (fre_y_A),
      .fre_z   (fre_z_A),
      .fre_w   (fre_w_A),
      .pha_x   (pha_x_A),
      .pha_y   (pha_y_A),
      .wave_sel(f_A_type),
      .data_out(data_out_A)
  );

  wave_ctrl_fre_pha_data_ctrl u_wave_ctrl_fre_pha_data_ctrl2 (
      .clk_50M (clk_50M),
      .rstn    (rstn),
      .fre_x   (10'd0),
      .fre_y   (fre_y_B),
      .fre_z   (fre_z_B),
      .fre_w   (fre_w_B),
      .pha_x   (pha_x_B),
      .pha_y   (pha_y_B),
      .wave_sel(f_B_type),
      .data_out(data_out_B)
  );

endmodule  //top_dds
