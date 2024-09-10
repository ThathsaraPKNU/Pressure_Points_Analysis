from http.server import BaseHTTPRequestHandler, HTTPServer
import urllib.parse as urlparse
import pandas as pd
import os
from datetime import datetime

# Full path to the CSV file
data_file = 'D:\\Works\\sensor_data6.csv'

class RequestHandler(BaseHTTPRequestHandler):
    def do_POST(self):
        try:
            content_length = int(self.headers['Content-Length'])
            post_data = self.rfile.read(content_length)
            data = urlparse.parse_qs(post_data.decode('utf-8'))

            value1 = data.get('value1', [''])[0]
            value2 = data.get('value2', [''])[0]
            value3 = data.get('value3', [''])[0]
            value4 = data.get('value4', [''])[0]
            value5 = data.get('value5', [''])[0]
            value6 = data.get('value6', [''])[0]

            # Get the current date and time
            current_time = datetime.now()
            date = current_time.strftime('%Y-%m-%d')
            time = current_time.strftime('%H:%M:%S')

            # Print received data
            print(f"Received data: value1={value1}, value2={value2}, value3={value3}, value4={value4}, value5={value5}, value6={value6}, date={date}, time={time}")

            # Append data to CSV file
            new_data = pd.DataFrame([[value1, value2, value3, value4, value5, value6, date, time]], columns=['Value1', 'Value2', 'Value3', 'Value4', 'Value5', 'Value6', 'Date', 'Time'])
            if not os.path.isfile(data_file):
                new_data.to_csv(data_file, index=False)
                print(f"CSV file created at: {data_file}")
            else:
                new_data.to_csv(data_file, mode='a', header=False, index=False)
                print(f"Data appended to: {data_file}")

            self.send_response(200)
            self.end_headers()
            self.wfile.write(b'OK')
        except Exception as e:
            self.send_response(500)
            self.end_headers()
            self.wfile.write(f"Error: {e}".encode('utf-8'))
            print(f"Error processing request: {e}")

def run(server_class=HTTPServer, handler_class=RequestHandler, port=8080):
    server_address = ('', port)
    httpd = server_class(server_address, handler_class)
    print(f'Server running on port {port}...')
    print(f"Saving data to: {data_file}")
    httpd.serve_forever()

if __name__ == '__main__':
    run()
