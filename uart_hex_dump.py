import serial

# Cấu hình cổng UART
port = '/dev/ttyUSB0'
baudrate = 115200
bytes_per_line = 32  # Bao nhiêu byte mỗi lần in ra

ser = serial.Serial(port, baudrate, timeout=1)

print(f"Listening on {port} at {baudrate} baud... Press Ctrl+C to stop.")

try:
    while True:
        data = ser.read(bytes_per_line)
        if data:
            hex_output = ' '.join(f'{byte:02X}' for byte in data)
            print(hex_output)
except KeyboardInterrupt:
    print("\nStopped by user.")
finally:
    ser.close()
