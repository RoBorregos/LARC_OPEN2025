"""
@file download.py
@brief Downloads images from a Google Drive folder for model training.
@author Hector Tovar
@date 2025-07-08

@requirements:
- Ask for the file to the Google service account .json file if not found.
- Ensure required libraries are installed (see requirements.txt).
"""

#!/bin/bash

import os
import io
from google.oauth2 import service_account
from googleapiclient.discovery import build
from googleapiclient.http import MediaIoBaseDownload
from PIL import Image
from pillow_heif import register_heif_opener

# .HEIC to .JPG
register_heif_opener()

# --- Configuration ---
SERVICE_ACCOUNT_FILE = os.path.join("vision", "model", "vision-larc.json")
FOLDER_ID = "1tGUKke8N-33MbegT78y3Nzx0cq5wmleR"
DEST_FOLDER = os.path.join("vision", "model", "dataset")
SCOPES = ["https://www.googleapis.com/auth/drive"]
API_NAME = "drive"
API_VERSION = "v3"

# --- Google Drive Auth ---
credentials = service_account.Credentials.from_service_account_file(
    SERVICE_ACCOUNT_FILE, scopes=SCOPES
)
service = build(API_NAME, API_VERSION, credentials=credentials)

# --- Local folder if does not exist---
if not os.path.exists(DEST_FOLDER):
    os.makedirs(DEST_FOLDER)

# --- Files being downloaded ---
query = f"'{FOLDER_ID}' in parents and mimeType contains 'image/' and trashed = false"
page_token = None
while True:
    response = service.files().list(
        q=query,
        spaces='drive',
        fields="nextPageToken, files(id, name)",
        pageToken=page_token
    ).execute()

    for file in response.get('files', []):
        file_id = file['id']
        file_name = file['name']
        file_path = os.path.join(DEST_FOLDER, file_name)

        if os.path.exists(file_path):
            print(f"[↷] Ya existe: {file_name}, saltando descarga.")
        else:
            request = service.files().get_media(fileId=file_id)
            with io.FileIO(file_path, 'wb') as fh:
                downloader = MediaIoBaseDownload(fh, request)
                done = False
                while not done:
                    status, done = downloader.next_chunk()
            print(f"[✓] Descargado: {file_name}")

        # If its heic -> jpg
        if file_name.lower().endswith(".heic"):
            jpg_name = file_name.rsplit(".", 1)[0] + ".jpg"
            jpg_path = os.path.join(DEST_FOLDER, jpg_name)

            if not os.path.exists(jpg_path):
                try:
                    img = Image.open(file_path)
                    img.save(jpg_path, "JPEG")
                    os.remove(file_path)  # borrar el .HEIC original
                    print(f"[★] Convertido a JPG: {jpg_name}")
                except Exception as e:
                    print(f"[!] Error al convertir {file_name}: {e}")

    page_token = response.get('nextPageToken', None)
    if page_token is None:
        break