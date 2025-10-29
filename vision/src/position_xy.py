import argparse
import math
import argparse
import math
import sys
import cv2
from ultralytics import YOLO

def parse_args():
	p = argparse.ArgumentParser(description="Detección y posición XY desde cámara")
	p.add_argument("--model", default="../model/nanoModel.pt", help="ruta al .pt del modelo YOLO")
	p.add_argument("--camera", type=int, default=0, help="índice de la cámara (default 0)")
	p.add_argument("--real-height", type=float, default=None,
				   help="altura real del objeto en metros (ej. 0.05)")
	p.add_argument("--hfov", type=float, default=None, help="FOV horizontal de la cámara en grados")
	p.add_argument("--focal-px", type=float, default=None, help="Focal en píxeles (fx). Si se da, sobrescribe hfov")
	return p.parse_args()


def focal_from_hfov(hfov_deg, width_px):
	# fx = (width/2) / tan(hfov/2)
	hfov = math.radians(hfov_deg)
	return (width_px / 2.0) / math.tan(hfov / 2.0)


def main():
	args = parse_args()

	cap = cv2.VideoCapture(args.camera)
	if not cap.isOpened():
		print("No se pudo abrir la cámara.")
		sys.exit(1)

	# Leer primer frame para obtener tamaño
	ret, frame = cap.read()
	if not ret:
		print("No se pudo leer frame inicial de la cámara.")
		cap.release()
		sys.exit(1)

	height, width = frame.shape[:2]
	cx = width / 2.0
	cy = height / 2.0

	if args.focal_px is not None:
		fx = fy = float(args.focal_px)
	elif args.hfov is not None:
		fx = focal_from_hfov(args.hfov, width)
		fy = fx  # suposición: píxeles cuadrados
	else:
		# Valor por defecto razonable si no hay info: asumir hfov ~ 70 deg
		fx = focal_from_hfov(70.0, width)
		fy = fx
		print("Aviso: no se proporcionó hfov ni focal-px. Usando hfov=70deg por defecto.")

	print(f"Camera resolution: {width}x{height}, fx={fx:.1f}, fy={fy:.1f}")

	print(f"Cargando modelo: {args.model}")
	model = YOLO(args.model)
	class_names = model.names

	print("Presiona ESC para salir.")

	try:
		while True:
			ret, frame = cap.read()
			if not ret:
				break

			# Inferencia
			results = model(frame, verbose=False)
			r = results[0]

			# Obtener boxes: manejamos dos posibles APIs (Boxes como lista o como objeto)
			boxes = []
			if hasattr(r, 'boxes') and r.boxes is not None:
				# En ultralytics, r.boxes.xyxy puede existir
				try:
					for b in r.boxes:
						# b.xyxy -> tensor o array; b.cls -> tensor
						xyxy = b.xyxy[0].cpu().numpy() if hasattr(b.xyxy, 'cpu') else (b.xyxy[0] if hasattr(b.xyxy, '__len__') else None)
						if xyxy is None:
							# tratar como arreglo simple
							xyxy = b.xyxy
						cls = int(b.cls[0]) if hasattr(b, 'cls') else None
						conf = float(b.conf[0]) if hasattr(b, 'conf') else None
						boxes.append((xyxy, cls, conf))
				except Exception:
					# alternativa: r.boxes.xyxy numpy array
					try:
						xy = r.boxes.xyxy.cpu().numpy()
						cls_arr = r.boxes.cls.cpu().numpy()
						conf_arr = r.boxes.conf.cpu().numpy() if hasattr(r.boxes, 'conf') else [None]*len(xy)
						for i in range(len(xy)):
							boxes.append((xy[i], int(cls_arr[i]), float(conf_arr[i]) if conf_arr is not None else None))
					except Exception:
						boxes = []

			# Procesar cada bbox y dibujar sobre la imagen anotada que devuelve el modelo
			# Usamos results[0].plot() como pidió el usuario
			try:
				annotated_img = results[0].plot()
			except Exception:
				# fallback a la imagen original si plot falla
				annotated_img = frame.copy()

			for (xyxy, cls, conf) in boxes:
				if xyxy is None:
					continue
				x1, y1, x2, y2 = map(float, xyxy[:4])
				u = (x1 + x2) / 2.0
				v = (y1 + y2) / 2.0
				h_px = max(1.0, (y2 - y1))

				label = class_names[int(cls)] if (cls is not None and int(cls) in class_names) else (str(cls) if cls is not None else 'obj')

				if args.real_height is not None:
					# calculamos Z internamente para obtener X,Y en metros, pero no devolvemos Z
					Z = (fy * args.real_height) / h_px
					X = (u - cx) * Z / fx
					Y = (v - cy) * Z / fy
					txt = f"{label} X={X:.3f}m Y={Y:.3f}m"
					# Imprimimos solo X,Y (sin Z)
					print(f"{label}: X={X:.3f} m, Y={Y:.3f} m (conf={conf})")
				else:
					# Sin tamaño real, mostramos offsets en píxeles respecto al centro
					X = u - cx
					Y = v - cy
					txt = f"{label} u={int(u)} v={int(v)}"
					print(f"{label}: center(px)=({int(u)},{int(v)}), Xoff={int(X)} px, Yoff={int(Y)} px, conf={conf}")

				# Dibujar encima del annotated_img devuelto por el modelo
				cv2.circle(annotated_img, (int(u), int(v)), 3, (0, 0, 255), -1)
				cv2.putText(annotated_img, txt, (int(x1), int(y1) - 6), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (255, 255, 0), 2)

			# Mostrar la ventana como en modelIRL.py
			cv2.imshow('Detection', annotated_img)
			if cv2.waitKey(1) & 0xFF == 27:
				break

	finally:
		cap.release()
		cv2.destroyAllWindows()


if __name__ == '__main__':
	main()

