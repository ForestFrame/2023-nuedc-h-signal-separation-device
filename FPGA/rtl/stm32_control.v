module stm32_control #(
    parameter FRE_WIDTH = 10,
    parameter PHA_WIDTH = 8
) (
    input wire clk_50M,
    input wire rstn,
    input wire uart_rx,

    output wire [3:0] f_type_A,
    output wire [3:0] f_type_B,

    output reg [FRE_WIDTH-1:0] fre_y_A,  //kHz 
    output reg [FRE_WIDTH-1:0] fre_z_A,  //1Hz
    output reg [FRE_WIDTH-1:0] fre_w_A,  //0.1Hz

    output wire [PHA_WIDTH-1:0] pha_x_A,
    output wire [PHA_WIDTH-1:0] pha_y_A,

    output reg [FRE_WIDTH-1:0] fre_y_B,
    output reg [FRE_WIDTH-1:0] fre_z_B,
    output reg [FRE_WIDTH-1:0] fre_w_B,

    output wire [PHA_WIDTH-1:0] pha_x_B,
    output wire [PHA_WIDTH-1:0] pha_y_B
);

  wire [55:0] A_reg;
  wire [55:0] B_reg;
  wire [0:0] updata_flag;

  uart u_uart (
      .clk    (clk_50M),
      .rst_n    (rstn),
      .A_reg  (A_reg),
      .B_reg  (B_reg),
      .uart_rx(uart_rx),
      .updata_flag(updata_flag),
      .uart_tx()
  );

  assign pha_x_A = A_reg[47:40];
  assign pha_y_A = A_reg[55:48];
  assign pha_x_B = B_reg[47:40];
  assign pha_y_B = B_reg[55:48];

  assign f_type_A = A_reg[15:8];
  assign f_type_B = B_reg[15:8];

  always @(posedge clk_50M or negedge rstn) begin
    if(rstn == 1'b0) begin
        fre_y_A <= 10'd0;
    end
    else if (updata_flag == 1'b1 && A_reg[31:24] == 8'd1) begin
        fre_y_A <= A_reg[23:16] - 8'd1;
    end
    else 
        fre_y_A <= A_reg[23:16];
  end

  always @(posedge clk_50M or negedge rstn) begin
    if(rstn == 1'b0) begin
        fre_z_A <= 10'd0;
    end
    else if(updata_flag == 1'b1 && A_reg[31:24] == 8'd1) begin
        fre_z_A <= 10'd999;
    end
    else if(updata_flag == 1'b1 && A_reg[31:24] == 8'd2) begin
        fre_z_A <= 10'd0;
    end
    else
        fre_z_A <= fre_z_A;
  end

  always @(posedge clk_50M or negedge rstn) begin
    if(rstn == 1'b0) begin
        fre_w_A <= 10'd0;
    end
    else if (updata_flag == 1'b1 && A_reg[31:24] == 8'd2) begin
        fre_w_A <= A_reg[39:32];
    end
    else if (updata_flag == 1'b1 && A_reg[31:24] == 8'd1) begin
        fre_w_A <= 10 - A_reg[39:32];
    end
    else
        fre_w_A <= fre_w_A;
  end

  //--------------------------------------------------------------------------------//

  always @(posedge clk_50M or negedge rstn) begin
    if(rstn == 1'b0) begin
        fre_y_B <= 10'd0;
    end
    else if (updata_flag == 1'b1 && B_reg[31:24] == 8'd1) begin
        fre_y_B <= B_reg[23:16] - 8'd1;
    end
    else 
        fre_y_B <= B_reg[23:16];
  end

  always @(posedge clk_50M or negedge rstn) begin
    if(rstn == 1'b0) begin
        fre_z_B <= 10'd0;
    end
    else if(updata_flag == 1'b1 && B_reg[31:24] == 8'd1) begin
        fre_z_B <= 10'd999;
    end
    else if(updata_flag == 1'b1 && B_reg[31:24] == 8'd2) begin
        fre_z_B <= 10'd0;
    end
    else
        fre_z_B <= fre_z_B;
  end

  always @(posedge clk_50M or negedge rstn) begin
    if(rstn == 1'b0) begin
        fre_w_B <= 10'd0;
    end
    else if (updata_flag == 1'b1 && B_reg[31:24] == 8'd2) begin
        fre_w_B <= B_reg[39:32];
    end
    else if (updata_flag == 1'b1 && B_reg[31:24] == 8'd1) begin
        fre_w_B <= 10 - B_reg[39:32];
    end
    else
        fre_w_B <= fre_w_B;
  end

endmodule  //stm32_control
