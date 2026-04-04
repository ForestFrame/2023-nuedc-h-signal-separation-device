module fre_pha_data_ctrl #(
    parameter N = 32,  //相位累加器位宽
    parameter M = 12,  //相位调制器位宽
    parameter FRE_WIDTH = 10,  //三路频率输入的位宽，三路频率的单位分别为MHz,kHz,Hz
    parameter PHA_WIDTH = 8,  //两路相位输入的位宽，(x+1/y)pi
    parameter DATA_WIDTH = 64
) (
    input wire clk_50M,
    input wire rstn,

    input wire [FRE_WIDTH-1:0] fre_x,  //MHz
    input wire [FRE_WIDTH-1:0] fre_y,  //kHz
    input wire [FRE_WIDTH-1:0] fre_z,  //Hz
    input wire [FRE_WIDTH-1:0] fre_w,  //0.1Hz

    input wire [PHA_WIDTH-1:0] pha_x,  //x*pi
    input wire [PHA_WIDTH-1:0] pha_y,  //(1/y)*pi

    output wire [N-1:0] fre_step,  //频率字输入，相当于一个步进值，每个时钟周期增加的值
    output wire [M-1:0] pha_step,  //相位字输入，相当于一个步进值，每个时钟周期增加的值

    input wire [1:0] key_flag
);

  /* ----------频率数据处理 fre_step---------- */
  parameter _1MHZ = 1_000_000;
  parameter _1KHZ = 1_000;
  parameter CLK_IN = 64'd50 * _1MHZ;

  wire [DATA_WIDTH-1:0] fre_out;  //实际输出信号频率
  wire [DATA_WIDTH-1:0] temp;  //中间值，fre_out*(2^N)的值

  reg  [DATA_WIDTH-1:0] fre_reg_x;  //频率输入x缓存信号
  reg  [DATA_WIDTH-1:0] fre_reg_y;  //频率输入y缓存信号
  reg  [DATA_WIDTH-1:0] fre_reg_z;  //频率输入z缓存信号

  wire [DATA_WIDTH-1:0] fre_step_temp;

  //单位MHz，化为0.1Hz
  always @(posedge clk_50M or negedge rstn) begin
    if (rstn == 1'b0) begin
      fre_reg_x <= 64'd0;
    end else begin
      fre_reg_x <= fre_x * _1MHZ * 10;
    end
  end

  //单位kHz，化为0.1Hz
  always @(posedge clk_50M or negedge rstn) begin
    if (rstn == 1'b0) begin
      fre_reg_y <= 64'd0;
    end else begin
      fre_reg_y <= fre_y * _1KHZ * 10;
    end
  end

  //单位0.1Hz，寄存器缓存
  always @(posedge clk_50M or negedge rstn) begin
    if (rstn == 1'b0) begin
      fre_reg_z <= 64'd0;
    end else begin
      fre_reg_z <= fre_z * 10 + fre_w;
    end
  end

  //将三者相加得到实际输出频率
  assign fre_out = fre_reg_x + fre_reg_y + fre_reg_z;

  //将实际输出频率乘以2^N次方，即左移N位
  assign temp    = fre_out << N;

  //将temp除以时钟频率CLK_IN
  div_64_64 div_64_64_inst1 (
      .denom   (CLK_IN * 10),
      .numer   (temp),
      .quotient(fre_step_temp),
      .remain  ()
  );

  assign fre_step = fre_step_temp[N-1:0];

  /* ----------相位数据处理 pha_step---------- */
  reg  [DATA_WIDTH-1:0] pha_reg_x;  //相位输入x缓存信号
  reg  [DATA_WIDTH-1:0] pha_reg_y;  //相位输入y缓存信号

  wire [DATA_WIDTH-1:0] temp_x;
  wire [DATA_WIDTH-1:0] temp_y;

  wire [DATA_WIDTH-1:0] pha_step_temp;

  //相位输入x缓存器
  always @(posedge clk_50M or negedge rstn) begin
    if (rstn == 1'b0) begin
      pha_reg_x <= 1'd1;
    end else begin
      pha_reg_x <= pha_x;
    end
  end

  //相位输入y缓存器
  always @(posedge clk_50M or negedge rstn) begin
    if (rstn == 1'b0) begin
      pha_reg_y <= 64'd1;
    end else begin
      pha_reg_y <= pha_y;
    end
  end

  //计算X*2^(M-1)
  assign temp_x = pha_reg_x << (M - 1);

  //计算(1/Y)*2^(M-1)
  div_64_64 div_64_64_inst2 (
      .denom   (pha_reg_y),
      .numer   (64'd2048),
      .quotient(temp_y),
      .remain  ()
  );

  //计算pha_step，总表达式为(X+1/Y)*2^(M-1)
  assign pha_step_temp = temp_x + temp_y;

  assign pha_step      = pha_step_temp[M-1:0];

endmodule  //fre_pha_data_ctrl
