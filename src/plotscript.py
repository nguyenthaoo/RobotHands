import serial
import matplotlib.pyplot as plt

# Configure the serial connection
ser = serial.Serial('COM4', 9600)  # Replace 'COM3' with the correct serial port and baud rate

# Initialize empty lists to store the data
timestamps = []
temperatures = []

# Set the number of data points to plot (you can adjust this as needed)
max_data_points = 100

# Start reading and plotting the data
try:
    while True:
        line = ser.readline().decode().strip()  # Read the data from the serial port and decode it
        value = line.split(':')[1]
        if "Temp" in line:
            temperature = float(value)
            print(f"Temperature: {temperature} °C")
        else:
            continue

        # Store the data in the lists
        timestamps.append(len(timestamps) + 1)
        temperatures.append(temperature)

        # Limit the data points to the maximum specified
        if len(timestamps) > max_data_points:
            timestamps.pop(0)
            temperatures.pop(0)

        # Plot the data
        plt.clf()
        plt.plot(timestamps, temperatures, marker='o', linestyle='-', color='b')
        plt.xlabel('Time')
        plt.ylabel('Temperature (°C)')
        plt.title('Temperature Data')
        plt.grid(True)
        plt.pause(0.1)

except KeyboardInterrupt:
    print("Script interrupted by user.")

# Close the serial connection
ser.close()
