import os
import requests
from bs4 import BeautifulSoup

def fetch_wikipedia_page(topic):
    base_url = "https://en.wikipedia.org/wiki/"
    url = base_url + topic.replace(" ", "_")

    try:
        response = requests.get(url)
        response.raise_for_status()  # Raise an exception for HTTP errors
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

def extract_page_text(soup):
    if soup is None:
        return None
    
    # Extract all text from the page
    content = soup.get_text()
    return content.strip()

def save_text_to_file(normalized_title, page_text):
    directory = "./data"
    os.makedirs(directory, exist_ok=True)  # Create the directory if it doesn't exist

    file_path = os.path.join(directory, f"{normalized_title}.txt")
    with open(file_path, "w", encoding="utf-8") as file:
        file.write(page_text)
    print(f"Text content saved to {file_path}")

if __name__ == "__main__":
    topic = input("Enter a topic to search on Wikipedia: ").strip()
    soup = fetch_wikipedia_page(topic)

    if soup:
        title = extract_title(soup)
        if title:
            normalized_title = normalize_title(title)
            page_text = extract_page_text(soup)
            if page_text:
                save_text_to_file(normalized_title, page_text)
            else:
                print("No text content found on the page.")
        else:
            print("No content found.")
