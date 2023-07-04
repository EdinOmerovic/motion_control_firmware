import serial
import csv
import time

# Setup the serial port and baud rate. Adjust as necessary.
SERIAL_PORT = 'COM8'
BAUD_RATE = 460800

# Setup the output CSV file.
CSV_FILE = 'output.csv'

# Time to wait for incoming data (in seconds)
TIMEOUT = 1

def read_from_serial_port(serial_port):
    """
    Read data from the serial port and return it as a string.
    """
    if serial_port.in_waiting > 0:
        return serial_port.readline().decode().strip()

    return None

def write_to_csv(data, csv_writer):
    """
    Write data to the CSV file.
    """
    csv_writer.writerow([data])

def main():
    # Open the serial port.
    with serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=TIMEOUT) as ser:
        # Open the CSV file.
        with open(CSV_FILE, 'w', newline='') as csvfile:
            csv_writer = csv.writer(csvfile)

            # Continuously read from the serial port and write to the CSV file.
            while True:
                data = read_from_serial_port(ser)
                
                if data is not None:
                    #print(data)
                    write_to_csv(data, csv_writer)

if __name__ == "__main__":
    main()
