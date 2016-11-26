TODO:
# For DAC, ADC, VGA
## Write structure to store current state:
	- isInitialized
	- OutputState
	- PowerState
## Use Mutex for SPI bus
## On every DeInit mehtod call check if You can disable clk for GPIO port
# Create API for scope
# Error handler may generate error report, with stack content, on sd card