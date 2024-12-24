import os
import requests
from bs4 import BeautifulSoup

def fetch_wikipedia_page(topic):
    base_url = "https://en.wikipedia.org/wiki/"
    url = base_url + topic.replace(" ", "_")

    try:
        response = requests.get(url)
        response.raise_for_status() 
    except requests.exceptions.RequestException as e:
        print(f"Error fetching the page: {e}")
        return None

    return BeautifulSoup(response.text, "html.parser")

def extract_title(soup):
    if soup is None:
        return None

    title = soup.find("title")
    if title:
        return title.get_text().strip()
    else:
        print("No title found on the page.")
        return None

def normalize_title(title):
    normalized = title.lower().replace(" ", "_").replace(":", "").replace("/", "")
    return normalized

def save_html_to_file(normalized_title, html_content):
    directory = "./data"
    os.makedirs(directory, exist_ok=True)

    file_path = os.path.join(directory, f"{normalized_title}.html")
    with open(file_path, "w", encoding="utf-8") as file:
        file.write(html_content)
    print(f"HTML content saved to {file_path}")

if __name__ == "__main__":
    topic = input("Enter a topic: ").strip()
    soup = fetch_wikipedia_page(topic)

    if soup:
        title = extract_title(soup)
        if title:
            normalized_title = normalize_title(title)
            save_html_to_file(normalized_title, str(soup))
        else:
            print("No content found.")
