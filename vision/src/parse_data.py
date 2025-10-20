import math
from ultralytics import YOLO
import cv2

# --- Configuration ---
model = YOLO("model/nanoModel.engine")
IGNORE_CLASS = "green bean" 
BEAN_CLASSES = {name for name in model.names.values() if name != IGNORE_CLASS} # idk check for tree and boxes name classes

LEFT_HALF_THRESHOLD = 0.5 
HORIZONTAL_ALIGNMENT_TOLERANCE = 0.05 
MIN_VERTICAL_DISTANCE = 0.1
LOWER_HALF_THRESHOLD = 0.5

def find_bean_matrix(results, frame_width, frame_height):
    """
    Analyzes detection results to find two vertically arranged bean objects 
    on the left side of the frame and returns their class names in a 1x2 list.
    """
    bean_detections = []
    
    for r in results:
        for data in r.boxes.data: # [x1, y1, x2, y2, conf, cls]
            x_min, y_min, x_max, y_max, conf, cls_id = data.tolist()
            class_name = model.names.get(int(cls_id))
            
            if class_name in BEAN_CLASSES:
                center_x_norm = ((x_min + x_max) / 2) / frame_width
                center_y_norm = ((y_min + y_max) / 2) / frame_height
                
                if center_x_norm < LEFT_HALF_THRESHOLD:
                    bean_detections.append({
                        'name': class_name,
                        'x_norm': center_x_norm,
                        'y_norm': center_y_norm
                    })

    # 2. Check for Vertical Arrangement (1x2 Matrix)
    if len(bean_detections) >= 2:
        bean_detections.sort(key=lambda d: d['y_norm']) # sort by vertical position (y_norm)
        
        top_bean = bean_detections[0]
        bottom_bean = bean_detections[1]
        
        delta_x = abs(top_bean['x_norm'] - bottom_bean['x_norm'])
        delta_y = abs(top_bean['y_norm'] - bottom_bean['y_norm'])

        if (delta_x < HORIZONTAL_ALIGNMENT_TOLERANCE and #or sort by size and take biggest two if this doesn't work
            delta_y >= MIN_VERTICAL_DISTANCE):
            return [top_bean['name'], bottom_bean['name']]
    
    elif len(bean_detections) == 1:
        bean = bean_detections[0]
        return ["NULL", bean['name']] if bean['y_norm'] >= LOWER_HALF_THRESHOLD else [bean['name'], "NULL"]
    
    else:
        return ["NULL", "NULL"]


cap = cv2.VideoCapture(0)
if not cap.isOpened():
    print("Can't open camera.")
    exit()

width  = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))

cap.set(cv2.CAP_PROP_BUFFERSIZE, 1)
cap.set(cv2.CAP_PROP_FPS, 60)

print("ESC to exit.")

try:
    while True:
        ret, frame = cap.read()
        if not ret:
            break

        results = model(frame, verbose=False)
        
        vertical_matrix = find_bean_matrix(results, width, height)
        
        if vertical_matrix:
            cv2.putText(
                frame, 
                f"Top: {vertical_matrix[0]}", 
                (10, 30), 
                cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 255, 0), 2
            )
            cv2.putText(
                frame, 
                f"Bottom: {vertical_matrix[1]}", 
                (10, 60), 
                cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 255, 0), 2
            )
        
        # Display results
        annotated_img = results[0].plot(
            boxes=False,
            labels=True,
            conf=False
        )
        cv2.imshow('Detection Matrix', annotated_img)
        
        # Exit on ESC key
        if cv2.waitKey(1) & 0xFF == 27:  # ESC key
            break

finally:
    cap.release()
    cv2.destroyAllWindows()