import requests

url="https://getpython.wordpress.com/"
BASE_URL = "https://getpython.wordpress.com/"

source=requests.get(url)

print(source)