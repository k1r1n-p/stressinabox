import kociemba
import cv2
import numpy as np
import serial
import time

def detect_color(roi):
    """Detect the color of a region of interest (ROI) with improved accuracy"""
    # Check if ROI is valid
    if roi is None or roi.size == 0:
        return 'white'  # Default color
    
    # Convert to HSV
    hsv = cv2.cvtColor(roi, cv2.COLOR_BGR2HSV)
    
    # Calculate average color
    avg_color = np.mean(hsv, axis=(0, 1))
    h, s, v = avg_color
    
    # Improved color detection with better ranges for problematic colors
    
    # White: high brightness, low saturation
    if v > 170 and s < 70:
        return 'white'
    
    # Yellow: hue 20-40, good saturation (improved range)
    elif 20 <= h <= 40 and s > 70 and v > 120:
        return 'yellow'
    
    # Orange: hue 8-20, high saturation (tighter range to avoid red confusion)
    elif 8 <= h <= 20 and s > 90 and v > 100:
        return 'orange'
    
    # Red: hue 0-8 or 170-180, high saturation (adjusted to avoid orange)
    elif (h <= 8 or h >= 170) and s > 90 and v > 80:
        return 'red'
    
    # Green: hue 40-85, medium-high saturation (improved lower bound)
    elif 40 <= h <= 85 and s > 60 and v > 50:
        return 'green'
    
    # Blue: hue 85-130, medium saturation
    elif 85 <= h <= 130 and s > 60 and v > 50:
        return 'blue'
    
    # Fallback logic with better boundaries
    elif h < 8:
        return 'red'
    elif 8 <= h < 20:
        return 'orange'
    elif 20 <= h < 40:
        return 'yellow'
    elif 40 <= h < 85:
        return 'green'
    elif 85 <= h < 135:
        return 'blue'
    else:
        return 'white'

def get_color_initial(color):
    """Get single letter initial for a color"""
    color_map = {
        'white': 'W',
        'yellow': 'Y',
        'red': 'R',
        'orange': 'O',
        'green': 'G',
        'blue': 'B'
    }
    return color_map.get(color, '?')

def draw_grid(frame, grid_size=3, margin=50, colors=None):
    """Draw a 3x3 grid on the frame for cube alignment with color initials"""
    height, width = frame.shape[:2]
    
    # Calculate grid area (centered square)
    size = min(width, height) - 2 * margin
    start_x = (width - size) // 2
    start_y = (height - size) // 2
    
    cell_size = size // grid_size
    
    # Draw grid lines
    for i in range(grid_size + 1):
        # Vertical lines
        x = start_x + i * cell_size
        cv2.line(frame, (x, start_y), (x, start_y + size), (0, 255, 0), 2)
        
        # Horizontal lines
        y = start_y + i * cell_size
        cv2.line(frame, (start_x, y), (start_x + size, y), (0, 255, 0), 2)
    
    # If colors are provided, draw color initials in each cell
    if colors:
        for row in range(3):
            for col in range(3):
                idx = row * 3 + col
                if idx < len(colors):
                    # Calculate center of cell
                    x = start_x + col * cell_size + cell_size // 2
                    y = start_y + row * cell_size + cell_size // 2
                    
                    # Get color initial
                    initial = get_color_initial(colors[idx])
                    
                    # Draw initial with background for visibility
                    text_size = cv2.getTextSize(initial, cv2.FONT_HERSHEY_SIMPLEX, 1.5, 3)[0]
                    bg_x1 = x - text_size[0] // 2 - 10
                    bg_y1 = y - text_size[1] // 2 - 10
                    bg_x2 = x + text_size[0] // 2 + 10
                    bg_y2 = y + text_size[1] // 2 + 10
                    
                    # Draw semi-transparent background
                    overlay = frame.copy()
                    cv2.rectangle(overlay, (bg_x1, bg_y1), (bg_x2, bg_y2), (0, 0, 0), -1)
                    cv2.addWeighted(overlay, 0.6, frame, 0.4, 0, frame)
                    
                    # Draw initial
                    cv2.putText(frame, initial, (x - text_size[0] // 2, y + text_size[1] // 2),
                              cv2.FONT_HERSHEY_SIMPLEX, 1.5, (0, 255, 255), 3)
    
    return start_x, start_y, cell_size

def capture_face_colors(frame, start_x, start_y, cell_size):
    """Capture colors from the 9 grid positions"""
    colors = []
    
    for row in range(3):
        for col in range(3):
            # Calculate center of each cell
            x = start_x + col * cell_size + cell_size // 2
            y = start_y + row * cell_size + cell_size // 2
            
            # Extract ROI (region of interest) around center
            roi_size = cell_size // 3
            x1 = max(0, x - roi_size // 2)
            y1 = max(0, y - roi_size // 2)
            x2 = min(frame.shape[1], x + roi_size // 2)
            y2 = min(frame.shape[0], y + roi_size // 2)
            
            roi = frame[y1:y2, x1:x2]
            
            if roi.size > 0:
                color = detect_color(roi)
                colors.append(color)
            else:
                colors.append('white')  # Default if ROI is invalid
    
    return colors

def display_detected_colors(colors):
    """Display the 3x3 grid of detected colors with initials"""
    print("\n=== DETECTED COLORS ===")
    print("Grid layout:")
    for i in range(3):
        row = colors[i*3:(i+1)*3]
        # Ensure all colors are strings, replace None with 'unknown'
        row = [c if c is not None else 'unknown' for c in row]
        # Display with initials in brackets
        initials = [get_color_initial(c) for c in row]
        print(f"[{initials[0]}] {row[0]:8}  [{initials[1]}] {row[1]:8}  [{initials[2]}] {row[2]:8}")
    print()

def confirm_or_correct_colors(colors, face_letter, face_name):
    """Ask user to confirm or correct detected colors"""
    while True:
        display_detected_colors(colors)
        
        response = input("Are these colors correct? (y/n): ").strip().lower()
        
        if response == 'y':
            return colors, False  # False means don't rescan
        elif response == 'n':
            print("\nWhat would you like to do?")
            print("1. Manually correct colors")
            print("2. Rescan this face")
            
            choice = input("Enter choice (1 or 2): ").strip()
            
            if choice == '1':
                # Manual correction
                print("\nEnter position to correct (e.g., F1, F2, ... F9) or 'done' when finished:")
                while True:
                    correction = input("Position to correct (or 'done'): ").strip().upper()
                    
                    if correction == 'DONE':
                        break
                    
                    # Parse position (e.g., "F1" -> position 1)
                    if len(correction) >= 2 and correction[0] == face_letter:
                        try:
                            position = int(correction[1:])
                            if 1 <= position <= 9:
                                new_color = input(f"Enter correct color for {correction}: ").strip().lower()
                                if new_color in ['white', 'yellow', 'orange', 'red', 'green', 'blue']:
                                    colors[position - 1] = new_color
                                    print(f"Updated {correction} to {new_color}")
                                    display_detected_colors(colors)
                                else:
                                    print("Invalid color. Use: white, yellow, orange, red, green, blue")
                            else:
                                print("Position must be 1-9")
                        except ValueError:
                            print("Invalid position format")
                    else:
                        print(f"Invalid format. Use {face_letter}1, {face_letter}2, etc.")
                
                # Show updated colors and ask for confirmation again
                continue
                
            elif choice == '2':
                # Rescan
                return None, True  # True means rescan
            else:
                print("Invalid choice. Please enter 1 or 2")
        else:
            print("Please enter 'y' or 'n'")

def scan_face_with_camera(face_name, face_letter):
    """Scan one face using camera with live color preview"""
    while True:  # Loop to allow rescanning
        print(f"\n{'='*50}")
        print(f"Scanning {face_name} Face ({face_letter})")
        print(f"{'='*50}")
        print("Position the cube face in the green grid")
        print("Press SPACEBAR to capture")
        print("Press Q to quit")
        print(f"{'='*50}\n")
        
        cap = cv2.VideoCapture(0)
        
        if not cap.isOpened():
            print("Error: Could not open camera")
            return None
        
        colors = None
        preview_colors = None
        
        while True:
            ret, frame = cap.read()
            
            if not ret:
                print("Error: Could not read frame")
                break
            
            # Get live preview colors for display
            start_x, start_y, cell_size = draw_grid(frame, colors=None)
            preview_colors = capture_face_colors(frame, start_x, start_y, cell_size)
            
            # Draw grid with color initials
            draw_grid(frame, colors=preview_colors)
            
            # Add text instructions
            cv2.putText(frame, f"Scanning: {face_name} ({face_letter})", 
                        (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 255, 0), 2)
            cv2.putText(frame, "Press SPACEBAR to capture", 
                        (10, 60), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 255, 0), 2)
            cv2.putText(frame, "Press Q to quit", 
                        (10, 90), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 255, 0), 2)
            
            # Show frame
            cv2.imshow('Rubik\'s Cube Scanner', frame)
            
            key = cv2.waitKey(1) & 0xFF
            
            if key == ord(' '):  # Spacebar pressed
                # Capture colors
                colors = preview_colors
                print("\nColors captured!")
                break
            elif key == ord('q'):  # Q pressed
                print("\nScan cancelled")
                cap.release()
                cv2.destroyAllWindows()
                return None
        
        cap.release()
        cv2.destroyAllWindows()
        
        if colors:
            # Confirm or correct
            colors, should_rescan = confirm_or_correct_colors(colors, face_letter, face_name)
            
            if should_rescan:
                print("\nRescanning face...")
                continue  # Go back to the beginning of the while loop
            else:
                return colors  # Colors confirmed, return them
        else:
            return None

def collect_all_colors_with_camera():
    """Collect colors for all 6 faces using camera"""
    all_faces = {}
    
    # Updated face order with GREEN front and WHITE top
    faces = [
        ("Front (Green center)", "F"),
        ("Right (Red center)", "R"),
        ("Back (Blue center)", "B"),
        ("Left (Orange center)", "L"),
        ("Up/Top (White center)", "U"),
        ("Down/Bottom (Yellow center)", "D")
    ]
    
    for face_name, face_letter in faces:
        colors = scan_face_with_camera(face_name, face_letter)
        
        if colors is None:
            print("\nScan cancelled. Exiting...")
            return None
        
        all_faces[face_letter] = colors
        
        if face_letter != "D":  # Not the last face
            input("\nPress Enter to scan next face...")
    
    return all_faces

def convert_to_kociemba_string(all_faces):
    """
    Convert collected colors to kociemba format string.
    Kociemba expects: U R F D L B (9 stickers each = 54 total)
    Each sticker represented by its face letter based on center color
    
    Updated mapping for GREEN front, WHITE top orientation
    """
    # Color to face letter mapping (based on center positions)
    color_to_face = {
        'green': 'F',
        'red': 'R',
        'blue': 'B',
        'orange': 'L',
        'white': 'U',
        'yellow': 'D'
    }
    
    # Kociemba order: U, R, F, D, L, B
    kociemba_order = ['U', 'R', 'F', 'D', 'L', 'B']
    
    cube_string = ""
    
    for face in kociemba_order:
        for color in all_faces[face]:
            # Convert color to face letter
            face_letter = color_to_face[color]
            cube_string += face_letter
    
    return cube_string

def send_to_arduino(solution):
    """Send solution to Arduino via serial"""
    print("\n" + "=" * 50)
    print("SENDING SOLUTION TO ARDUINO")
    print("=" * 50)
    
    # Try different common serial ports
    possible_ports = ['/dev/ttyACM0', '/dev/ttyACM1', '/dev/ttyUSB0', '/dev/ttyUSB1']
    arduino = None
    
    for port in possible_ports:
        try:
            print(f"Trying to connect to Arduino on {port}...")
            arduino = serial.Serial(port, 115200, timeout=1)
            print(f"✓ Connected to Arduino on {port}")
            break
        except:
            continue
    
    if arduino is None:
        print("✗ Error: Could not connect to Arduino")
        print("Make sure:")
        print("  1. Arduino is connected via USB")
        print("  2. Arduino code is uploaded")
        print("  3. Correct port is available")
        return False
    
    # Wait for Arduino to initialize
    print("Waiting for Arduino to be ready...")
    time.sleep(3)
    
    # Read Arduino's ready message
    start_time = time.time()
    while time.time() - start_time < 10:
        if arduino.in_waiting > 0:
            line = arduino.readline().decode('utf-8').strip()
            print(f"Arduino: {line}")
            if "READY" in line:
                break
    
    # Send solution
    print(f"\nSending solution: {solution}")
    arduino.write(f"{solution}\n".encode())
    
    # Read Arduino's response
    print("\n" + "=" * 50)
    print("ARDUINO EXECUTION")
    print("=" * 50)
    
    try:
        while True:
            if arduino.in_waiting > 0:
                line = arduino.readline().decode('utf-8').strip()
                if line:
                    print(f"Arduino: {line}")
                    
                    # Check if solution is complete
                    if "Complete" in line:
                        print("\n✓ Cube solved successfully!")
                        break
            
            # Add timeout
            time.sleep(0.1)
    except KeyboardInterrupt:
        print("\nStopped by user")
    finally:
        arduino.close()
        print("\nArduino connection closed")
    
    return True

def main():
    print("=" * 50)
    print("RUBIK'S CUBE SOLVER WITH CAMERA")
    print("=" * 50)
    print("\nHold your cube with:")
    print("- GREEN center facing you (Front)")
    print("- WHITE center on top (Up)")
    print("\nYou will scan each face in this order:")
    print("Front -> Right -> Back -> Left -> Up -> Down")
    print("\nColor initials will appear in the grid:")
    print("W=White, Y=Yellow, R=Red, O=Orange, G=Green, B=Blue")
    print("=" * 50)
    
    input("\nPress Enter to start scanning...")
    
    # Collect all sticker colors using camera
    all_faces = collect_all_colors_with_camera()
    
    if all_faces is None:
        return
    
    # Convert to kociemba format
    cube_string = convert_to_kociemba_string(all_faces)
    
    print("\n" + "=" * 50)
    print("Cube string for kociemba:", cube_string)
    print("=" * 50)
    
    # Solve the cube
    try:
        solution = kociemba.solve(cube_string)
        print("\n" + "=" * 50)
        print("SOLUTION:")
        print("=" * 50)
        print(solution)
        print("\nNotation guide:")
        print("U=Up, D=Down, F=Front, B=Back, L=Left, R=Right")
        print("'=Counter-clockwise, 2=180 degrees")
        print("=" * 50)
        
        # Ask user if they want to send to Arduino
        response = input("\nSend solution to Arduino? (y/n): ").strip().lower()
        
        if response == 'y':
            send_to_arduino(solution)
        else:
            print("\nSolution not sent to Arduino")
            
    except Exception as e:
        print("\n" + "=" * 50)
        print("ERROR: Could not solve cube")
        print(f"Error message: {e}")
        print("=" * 50)
        print("\nPossible issues:")
        print("- Invalid cube configuration")
        print("- Incorrect color entries")
        print("- Unsolvable cube state")

if __name__ == "__main__":
    main()