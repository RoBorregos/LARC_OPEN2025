import gdown

folder_url = "https://drive.google.com/drive/folders/1tGUKke8N-33MbegT78y3Nzx0cq5wmleR?usp=sharing"

gdown.download_folder(folder_url, quiet=False, use_cookies=False)
