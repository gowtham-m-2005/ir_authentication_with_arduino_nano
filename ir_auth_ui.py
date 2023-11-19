import serial
import tkinter as tk
import customtkinter as ctk 
import threading

def read_serial():
    while True:
        serial_data = ser.readline().decode().strip()
        print(serial_data);
        if serial_data == "Access Granted!":
            show_custom_message(title="Access Granted",message="You have logged in Successfully")
        elif serial_data == "Enter your Passcode":
            show_custom_message(title="Enter Your Passcode",message="Point the Authenticator at the Sensor")
        elif serial_data == "Access Denied!":
            show_custom_message(title="Access Denied!",message="Account Locked")
        elif serial_data == "Validation Passcode,Please Wait...":
            show_custom_message(title="Please Wait...",message="Validating Passcode...")

def show_custom_message(title, message):
    custom_message = tk.Toplevel(app)
    custom_message.title(title)

    label = tk.Label(custom_message, text=message)
    label.pack(padx=20, pady=20)

    ok_button = tk.Button(custom_message, text="OK", command=custom_message.destroy)
    ok_button.pack(pady=10)

arduino_port = '/dev/ttyUSB0'
baud_rate = 300

#initializing serial connection
ser = serial.Serial(arduino_port, baud_rate)
print("Connected to IR Authenticator")

#ctk code
ctk.set_appearance_mode("dark") 
ctk.set_default_color_theme("blue") 
  
app = ctk.CTk() 
app.geometry("1920x1080") 
app.title("AXK - Admin Panel") 
  
label = ctk.CTkLabel(app,text="AXK Login Panel") 
label.pack(pady=20)

#heading bar
heading_bar = tk.Frame(app, bg="blue", height=50)
heading_bar.pack(side="top", fill="x")

heading_label = tk.Label(heading_bar, text="AXK Admin Panel", fg="white", bg="blue", font=("Times New Roman", 14, "bold"))
heading_label.pack(pady=10)

#loading image
bg_image = tk.PhotoImage(file="images/background.png")

#creating a canvas for the background image
canvas = tk.Canvas(app,width=1920,height=1080)
canvas.place(x=0,y=0)

#placing the background image on the canvas
canvas.create_image(0,0,anchor = tk.NW, image = bg_image)

heading_bar.lift()

#starting a thread for reading serial output
serial_thread = threading.Thread(target=read_serial)
serial_thread.daemon = True
serial_thread.start()

app.mainloop()