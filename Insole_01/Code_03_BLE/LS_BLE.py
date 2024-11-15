import asyncio
from bleak import BleakClient
import csv
from datetime import datetime
import os

# Replace with the BLE address of your ESP32 and the UUID of the characteristic
BLE_ADDRESS = "94:b5:55:7c:9b:7a"  # Replace with your ESP32's BLE MAC address
CHARACTERISTIC_UUID = "abcd1234-abcd-1234-abcd-12345678abcd"  # Match with ESP32 characteristic UUID

# Path to save the CSV file
csv_file_path = "D:\\Data1_Prathiksha.csv"

# Ensure the directory exists
os.makedirs(os.path.dirname(csv_file_path), exist_ok=True)

# Write header row with FSR sensor values and timestamp only once at the start
with open(csv_file_path, mode="w", newline="") as file:
    csv_writer = csv.writer(file)
    csv_writer.writerow(["Value1", "Value2", "Value3", "Value4", "Value5", "Value6", "Value7", "Value8", "Date", "Time"])

# Function to handle notifications from ESP32
def handle_notification(sender, data):
    try:
        data_str = data.decode("utf-8")  # Decode the byte data to a string
        values = data_str.split(",")  # Assuming values are comma-separated

        if len(values) == 8:  # Ensure we have 8 FSR values
            current_time = datetime.now()
            date = current_time.strftime("%Y-%m-%d")
            time = current_time.strftime("%H:%M:%S")

            # Open CSV file in append mode to save data
            with open(csv_file_path, mode="a", newline="") as file:
                csv_writer = csv.writer(file)
                csv_writer.writerow(values + [date, time])
            print("Data saved:", values + [date, time])

        else:
            print("Received data with incorrect format:", data_str)

    except Exception as e:
        print(f"Error handling notification: {e}")

async def main():
    try:
        # Connect to the ESP32 BLE device
        async with BleakClient(BLE_ADDRESS) as client:
            print("Connected to ESP32 BLE device.")

            # Start receiving notifications
            await client.start_notify(CHARACTERISTIC_UUID, handle_notification)
            print("Listening for data...")

            # Run for a specified duration to collect data
            await asyncio.sleep(60)  # Adjust the duration as needed

            # Stop notifications
            await client.stop_notify(CHARACTERISTIC_UUID)
            print("Stopped listening.")

    except Exception as e:
        print(f"Failed to connect or communicate with ESP32: {e}")

# Run the async function
asyncio.run(main())
