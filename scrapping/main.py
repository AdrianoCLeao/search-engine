import os
import requests
from bs4 import BeautifulSoup
from tqdm import tqdm

"""
Fetches the content of a Wikipedia page for a given topic.

Args:
    topic (str): The topic to search for on Wikipedia.

Returns:
    BeautifulSoup: Parsed HTML content of the Wikipedia page, or None if the request fails.
"""
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

def clean_text(soup):
    """Extract and clean main content from a Wikipedia page."""
    if soup is None:
        return None

    body_content = soup.find("div", {"id": "bodyContent"})
    if not body_content:
        print("Body content not found.")
        return None

    paragraphs = body_content.find_all("p")
    cleaned_text = []
    
    for p in paragraphs:
        text = p.get_text().strip()
        if text and not text.startswith(("This article", "Wikipedia", "Please help")):
            cleaned_text.append(text)

    return "\n\n".join(cleaned_text)

def extract_links(soup):
    """Extract all internal Wikipedia links from the page."""
    if soup is None:
        return []

    base_url = "https://en.wikipedia.org"
    links = []

    for a_tag in soup.find_all("a", href=True):
        href = a_tag['href']
        if href.startswith("/wiki/") and not (":" in href or href.startswith("/wiki/Main_Page")):
            full_url = base_url + href
            links.append(full_url)

    return list(set(links))

def save_text_to_file(normalized_title, page_text):
    directory = "./data"
    os.makedirs(directory, exist_ok=True) 

    file_path = os.path.join(directory, f"{normalized_title}.txt")
    with open(file_path, "w", encoding="utf-8") as file:
        file.write(page_text)

if __name__ == "__main__":
    os.system('cls' if os.name == 'nt' else 'clear')
    topic = input("Enter a topic to search on Wikipedia: ").strip()
    soup = fetch_wikipedia_page(topic)

    if soup:
        main_title = extract_title(soup)
        if main_title:
            normalized_title = normalize_title(main_title)
            main_page_text = clean_text(soup)
            if main_page_text:
                save_text_to_file(normalized_title, main_page_text)
        
        links = extract_links(soup)
        print(f"Found {len(links)} related links. Starting download...")

        for link in tqdm(links, desc="Processing related pages"):
            subtopic = link.split("/wiki/")[-1]
            sub_soup = fetch_wikipedia_page(subtopic)
            if sub_soup:
                sub_title = extract_title(sub_soup)
                if sub_title:
                    normalized_sub_title = normalize_title(sub_title)
                    sub_page_text = clean_text(sub_soup)
                    if sub_page_text:
                        save_text_to_file(normalized_sub_title, sub_page_text)
