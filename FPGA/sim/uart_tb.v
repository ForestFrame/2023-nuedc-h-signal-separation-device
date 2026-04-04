`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/08/04 00:46:21
// Design Name: 
// Module Name: uart_tb
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module uart_tb(

    );
wire uart_rx,uart_tx;
reg clk,rst;
    
  uart uart_inst(
	.clk(clk),
	.rst(rst),
	.uart_rx(uart_tx),
	.uart_tx()  
);
initial 
begin
    #0 clk = 0;
    rst=1;
    #30 
    rst=0;
end

always# 10 clk = ~clk; 
//tx test
localparam                       IDLE =  0;
localparam                       SEND =  1;   //send HELLO ALINX\r\n
localparam                       WAIT =  2;   //wait 1 second and send uart received data
reg[7:0]                         tx_data;
reg[7:0]                         tx_str;
reg                              tx_data_valid;
wire                             tx_data_ready;
reg[7:0]                         tx_cnt;
reg[31:0]                        wait_cnt;
reg[3:0]                         state;

always@(posedge clk or posedge rst)
begin
	if(rst)
	begin
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

//combinational logic
//Send "HELLO ALINX\r\n"
always@(*)
begin
	case(tx_cnt)
		8'd0 :  tx_str <= 8'h55;
		8'd1 :  tx_str <= 8'h0A;
		8'd2 :  tx_str <= 8'h0C;
		8'd3 :  tx_str <= 8'h01;
		8'd4 :  tx_str <= 8'h0E;
		8'd5 :  tx_str <= 8'h02;
		8'd6 :  tx_str <= 8'h03;
		8'd7 :  tx_str <= 8'h04;
		8'd8 :  tx_str <= 8'hAA;
		8'd9:  tx_str <= "\r";
		8'd10:  tx_str <= "\n";
		default:tx_str <= 8'd0;
	endcase
end

uart_tx#
(
	.CLK_FRE(50),
	.BAUD_RATE(115200)
) uart_tx_inst
(
	.clk                        (clk                      ),
	.rst_n                      (~rst                    ),
	.tx_data                    (tx_data                  ),
	.tx_data_valid              (tx_data_valid            ),
	.tx_data_ready              (tx_data_ready            ),
	.tx_pin                     (uart_tx                  )
);

endmodule
