import time
import cv2
from ultralytics import YOLO

# Models to compare
models = {
    "PyTorch (.pt)": "model/nanoModel.pt",
    "ONNX (.onnx)": "model/nanoModel.onnx",
    "TensorRT (.engine)": "model/nanoModel.engine"
}

cap = cv2.VideoCapture(0)

cap.set(cv2.CAP_PROP_BUFFERSIZE, 1)
cap.set(cv2.CAP_PROP_FPS, 30)

# To measure average FPS
def benchmark_model(model_path):
    print(f"\nTesting model: {model_path}")
    model = YOLO(model_path)
    
    for _ in range(3):
        ret, frame = cap.read()
        if not ret:
            continue
        _ = model(frame, verbose=False)
    
    print("▶ Measuring performance...")
    frames = 0
    start = time.time()
    
    while True:
        ret, frame = cap.read()
        if not ret:
            break
        
        results = model(frame, verbose=False)
        annotated = results[0].plot()

        cv2.imshow("Model Comparison", annotated)
        frames += 1
        
        # exit with ESC
        if cv2.waitKey(1) & 0xFF == 27:
            break
    
    elapsed = time.time() - start
    fps = frames / elapsed if elapsed > 0 else 0
    print(f"Average: {fps:.2f} FPS")
    return fps

# Run tests
results = {}
for name, path in models.items():
    try:
        fps = benchmark_model(path)
        results[name] = fps
    except Exception as e:
        print(f"Error with {name}: {e}")

cap.release()
cv2.destroyAllWindows()

print("\n📊 Final results:")
for name, fps in results.items():
    print(f"{name:<25}: {fps:.2f} FPS")