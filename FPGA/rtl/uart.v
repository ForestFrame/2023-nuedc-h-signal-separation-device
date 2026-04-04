module uart (
    input clk,
    input rst_n,

    output reg  [55:0] A_reg,
    output reg  [55:0] B_reg,
    input              uart_rx,
    output wire        updata_flag,
    output             uart_tx
);

  parameter CLK_FRE = 50;  //Mhz

  wire [ 7:0] rx_data;
  wire        rx_data_valid;
  wire        rx_data_ready;
  reg  [55:0] DATA_store;

  assign rx_data_ready = 1'b1;  //always can receive data,

  reg [3:0] store_cnt;

  always @(posedge clk or negedge rst_n) begin
    if (~rst_n) begin
      store_cnt <= 4'hf;
    end else if ((rx_data == 8'h55) & rx_data_valid) begin
      store_cnt <= 4'h0;
    end else if ((rx_data == 8'hAA) & rx_data_valid) begin
      store_cnt <= 4'hf;
    end else if (rx_data_valid & (store_cnt != 4'hf)) begin
      store_cnt <= store_cnt + 1'b1;
    end
  end

  assign updata_flag = ((rx_data == 8'hAA) & rx_data_valid);

  always @(posedge clk or negedge rst_n) begin
    if (~rst_n) begin
      DATA_store <= 'd0;
    end else if (rx_data_valid) begin
      case (store_cnt)
        4'd0: DATA_store <= {DATA_store[55:8], rx_data};  //0x0A or 0x0B
        4'd1: DATA_store <= {DATA_store[55:16], rx_data, DATA_store[7:0]};  // signal type: 0001 or 0100
        4'd2: DATA_store <= {DATA_store[55:24], rx_data, DATA_store[15:0]};  //frequency: 20~100 kHz
        4'd3: DATA_store <= {DATA_store[55:32], rx_data, DATA_store[23:0]};  //add: 2 or minus: 1
        4'd4: DATA_store <= {DATA_store[55:40], rx_data, DATA_store[31:0]};  //delta f: 0~256 Hz
        4'd5: DATA_store <= {DATA_store[55:48], rx_data, DATA_store[39:0]};  //initial phase x
        4'd6: DATA_store <= {rx_data, DATA_store[47:0]};  //initial phase y
        default: DATA_store <= DATA_store;
      endcase

    end
  end

  always @(posedge clk or negedge rst_n) begin
    if (~rst_n) begin
      A_reg <= 56'd0;
      B_reg <= 56'd0;
    end else if (DATA_store[7:0] == 8'h0A) begin
      A_reg <= DATA_store;
      B_reg <= B_reg;
    end else if (DATA_store[7:0] == 8'h0B) begin
      A_reg <= A_reg;
      B_reg <= DATA_store;
    end
  end

  uart_rx #(
      .CLK_FRE  (CLK_FRE),
      .BAUD_RATE(115200)
  ) uart_rx_inst (
      .clk          (clk),
      .rst_n        (rst_n),
      .rx_data      (rx_data),
      .rx_data_valid(rx_data_valid),
      .rx_data_ready(rx_data_ready),
      .rx_pin       (uart_rx)
  );

endmodule
