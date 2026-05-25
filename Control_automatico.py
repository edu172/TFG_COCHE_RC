import cv2
from ultralytics import YOLO
import serial
import time

# ==============================
# CONFIGURACIÓN
# ==============================

STREAM_URL = "http://10.152.206.250:81/stream"

# Puerto serie del ESP32 controlador
# CAMBIAR por el puerto correcto
SERIAL_PORT = "COM3"

BAUDRATE = 115200

# Modelo YOLOv8
MODEL = YOLO("yolov8n.pt")

# Umbral de confianza
CONF_THRESHOLD = 0.40

# Offset para evitar correcciones constantes
OFFSET = 50

# ==============================
# INICIALIZACIÓN
# ==============================

# Conexión serie con ESP32
ser = serial.Serial(SERIAL_PORT, BAUDRATE)
time.sleep(2)

# Captura stream ESP32-CAM
cap = cv2.VideoCapture(STREAM_URL)

if not cap.isOpened():
    print("No se pudo abrir el stream")
    exit()

# ==============================
# BUCLE PRINCIPAL
# ==============================

while True:

    ret, frame = cap.read()

    if not ret:
        print("No se recibió frame")
        break

    # Centro horizontal pantalla
    frame_center_x = frame.shape[1] // 2

    # Ejecutar YOLO
    results = MODEL(frame, conf=CONF_THRESHOLD)[0]

    detected_phone = False

    # Procesar detecciones
    for box in results.boxes:

        cls = int(box.cls[0])
        label = MODEL.names[cls]

        # SOLO detectar teléfonos
        if label == "cell phone":

            detected_phone = True

            # Coordenadas caja
            x1, y1, x2, y2 = map(int, box.xyxy[0])

            # Centroide
            cx = int((x1 + x2) / 2)
            cy = int((y1 + y2) / 2)

            # Dibujar caja
            cv2.rectangle(frame, (x1, y1), (x2, y2), (0,255,0), 2)

            # Dibujar centroide
            cv2.circle(frame, (cx, cy), 5, (0,0,255), -1)

            # Línea centro pantalla
            cv2.line(frame,
                     (frame_center_x, 0),
                     (frame_center_x, frame.shape[0]),
                     (255,0,0), 2)

            # ==========================
            # CONTROL DIRECCIÓN
            # ==========================

            # IZQUIERDA
            if cx < frame_center_x - OFFSET:

                command = "LEFT\n"
                ser.write(command.encode())

                cv2.putText(frame,
                            "LEFT",
                            (50,50),
                            cv2.FONT_HERSHEY_SIMPLEX,
                            1,
                            (0,0,255),
                            2)

            # DERECHA
            elif cx > frame_center_x + OFFSET:

                command = "RIGHT\n"
                ser.write(command.encode())

                cv2.putText(frame,
                            "RIGHT",
                            (50,50),
                            cv2.FONT_HERSHEY_SIMPLEX,
                            1,
                            (0,0,255),
                            2)

            # CENTRADO -> AVANZAR
            else:

                command = "FORWARD\n"
                ser.write(command.encode())

                cv2.putText(frame,
                            "FORWARD",
                            (50,50),
                            cv2.FONT_HERSHEY_SIMPLEX,
                            1,
                            (0,255,0),
                            2)

            break

    # Si no detecta teléfono -> STOP
    if not detected_phone:

        ser.write(b"STOP\n")

        cv2.putText(frame,
                    "NO PHONE DETECTED",
                    (50,50),
                    cv2.FONT_HERSHEY_SIMPLEX,
                    1,
                    (0,0,255),
                    2)

    # Mostrar imagen
    cv2.imshow("Autonomous RC Car", frame)

    # Salir con Q
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# ==============================
# FINALIZAR
# ==============================

cap.release()
cv2.destroyAllWindows()
ser.close()