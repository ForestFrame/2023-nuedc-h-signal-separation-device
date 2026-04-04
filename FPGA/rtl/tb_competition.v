`timescale 1ns / 1ps

module tb_competition;

  // competition Parameters
  parameter PERIOD = 10;
  parameter FRE_WIDTH = 10;
  parameter PHA_WIDTH = 8;

  // competition Inputs
  reg        clk = 0;
  reg        rstn = 0;
  reg        uart_rx = 0;

  // competition Outputs
  wire       dac_clk_A;
  wire [7:0] data_out_A;
  wire       dac_clk_B;
  wire [7:0] data_out_B;

  localparam IDLE = 0;
  localparam SEND = 1;  //send HELLO ALINX\r\n
  localparam WAIT = 2;  //wait 1 second and send uart received data
  reg  [ 7:0] tx_data;
  reg  [ 7:0] tx_str;
  reg         tx_data_valid;
  wire        tx_data_ready;
  reg  [ 7:0] tx_cnt;
  reg  [31:0] wait_cnt;
  reg  [ 3:0] state;

  always #10 clk = ~clk;

  initial begin
    #(PERIOD * 2) rstn = 1;
  end

always@(posedge clk or posedge rstn) begin
  if(~rstn) begin
    wait_cnt <= 32'd0;
		tx_data <= 8'd0;
		state <= IDLE;
		tx_cnt <= 8'd0;
		tx_data_valid <= 1'b0;
    end
	else
	case(state)
		IDLE:
			state <= SEND;
		SEND:
		begin
			wait_cnt <= 32'd0;
			tx_data <= tx_str;

			if(tx_data_valid == 1'b1 && tx_data_ready == 1'b1 && tx_cnt < 8'd10)//Send 12 bytes data
			begin
				tx_cnt <= tx_cnt + 8'd1; //Send data counter
			end
			else if(tx_data_valid && tx_data_ready)//last byte sent is complete
			begin
				tx_cnt <= 8'd0;
				tx_data_valid <= 1'b0;
				state <= WAIT;
			end
			else if(~tx_data_valid)
			begin
				tx_data_valid <= 1'b1;
			end
		end
		WAIT:
		begin
			wait_cnt <= wait_cnt + 32'd1;

			if(tx_data_valid && tx_data_ready)
			begin
				tx_data_valid <= 1'b0;
			end
			else if(wait_cnt >= 100) // wait for 1 second
				state <= SEND;
		end
		default:
			state <= IDLE;
	endcase
end

  competition #(
      .FRE_WIDTH(FRE_WIDTH),
      .PHA_WIDTH(PHA_WIDTH)
  ) u_competition (
      .clk    (clk),
      .rstn   (rstn),
      .uart_rx(uart_rx),

      .dac_clk_A (dac_clk_A),
      .data_out_A(data_out_A[7:0]),
      .dac_clk_B (dac_clk_B),
      .data_out_B(data_out_B[7:0])
  );

  //combinational logic
  //Send "HELLO ALINX\r\n"
  always @(*) begin
    case (tx_cnt)
      8'd0: tx_str <= 8'h55;
      8'd1: tx_str <= 8'h0A;
      8'd2: tx_str <= 8'h0C;
      8'd3: tx_str <= 8'h01;
      8'd4: tx_str <= 8'h0E;
      8'd5: tx_str <= 8'h02;
      8'd6: tx_str <= 8'h03;
      8'd7: tx_str <= 8'h04;
      8'd8: tx_str <= 8'hAA;
      8'd9: tx_str <= "\r";
      8'd10: tx_str <= "\n";
      default: tx_str <= 8'd0;
    endcase
  end

  uart_tx #(
      .CLK_FRE  (50),
      .BAUD_RATE(115200)
  ) uart_tx_inst (
      .clk          (clk),
      .rst_n        (~rst),
      .tx_data      (tx_data),
      .tx_data_valid(tx_data_valid),
      .tx_data_ready(tx_data_ready),
      .tx_pin       (uart_tx)
  );

endmodule
