import socket, struct

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(('0.0.0.0', 5005))  # must match destPort from Arduino

while True:
    data, addr = sock.recvfrom(1024)
    if len(data) == 16:
        dhtT, dhtH, bmpT, bmpP = struct.unpack('ffff', data)
        print(f"From {addr}: Temp(DHT Sensor)={dhtT:.2f}°C, Hum={dhtH:.2f}%, Temp(BMP Sensor)={bmpT:.2f}°C, Pressure={bmpP:.2f} hPa")
