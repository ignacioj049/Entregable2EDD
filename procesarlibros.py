import os
import sys
import json
import xml.etree.ElementTree as ET  #se utiliza la librería de element tree xml

def get_text(element, tag):    #se extrae el texto de un tag hijo de un dado elemento, y retorna None si no existe o está vacío el tag
    child = element.find(tag)
    if child is None or child.text is None:
        return None
    text = child.text.strip()
    return text if text else None   
 
def get_int(element, tag):     #se extrae el texto de un tag y lo convierte a entero, y retorna None si no existe, está vacío o no es un número válido
    text = get_text(element, tag)
    if text is None:
        return None
    try:
        return int(text)
    except ValueError:
        return None

def get_float(element, tag):    #se extrae el texto de un tag y lo convierte a float, y retorna None si no existe, está vacío o no es un número válido
    text = get_text(element, tag)
    if text is None:
        return None
    try:
        return float(text)
    except ValueError:
        return None
    
def procesar_libro_similar(book_elem):      #se procesa un elemento "book" dentro de "similar_books" y retorna un diccionario con el titulo,isbn y el año de publicación, y retorna None si el libro no tiene título
    titulo = get_text(book_elem, 'title')
    if titulo is None:
        return None
 
    return {
        "titulo":   titulo,
        "isbn":     get_text(book_elem, 'isbn') or "",
        "ano_pub": get_int(book_elem, 'publication_year')
    }
 
def procesar_archivo(filepath):     #se procesa un archivo XML infividual y retorna un diccionario con todos los campos necesarios del libro, incluyendo su lista de libros similares, y retorna None si el archivo tiene errores o no tiene ID
    try:
        tree = ET.parse(filepath)
        root = tree.getroot()
    except ET.ParseError:
        return None
    book = root.find('book')
    if book is None:
        return None
 
    book_id = get_text(book, 'id')
    if book_id is None:
        return None
    
    similares = []
    similar_books_elem = book.find('similar_books')
    if similar_books_elem is not None:
        for sim_book_elem in similar_books_elem.findall('book'):
            similar = procesar_libro_similar(sim_book_elem)
            if similar is not None:
                similares.append(similar)

    return {
        "id":         book_id,
        "titulo":     get_text(book, 'title') or "",
        "isbn":       get_text(book, 'isbn') or "",
        "ano_pub":   get_int(book, 'publication_year'),
        "idioma":     get_text(book, 'language_code') or "",
        "descripcion": get_text(book, 'description') or "",
        "rating":     get_float(book, 'average_rating'),
        "num_paginas": get_int(book, 'num_pages'),
        "similares":  similares
    }

def procesar_dataset(carpeta_xml, archivo_salida):      #se recorren todos los archivos xml, se procesan y se guardan los resultados en archivo_salida como un json
    archivos = [f for f in os.listdir(carpeta_xml) if f.endswith('.xml')]
    total = len(archivos)
    print(f"Archivos encontrados: {total}")
 
    libros = []
    errores = 0
 
    for i, nombre_archivo in enumerate(archivos):
        filepath = os.path.join(carpeta_xml, nombre_archivo)
        libro = procesar_archivo(filepath)
 
        if libro is not None:
            libros.append(libro)
        else:
            errores += 1
        if (i + 1) % 500 == 0 or (i + 1) == total:      #se muestra el progreso cada 500 archivos
            print(f"  Procesados: {i + 1}/{total} , OK: {len(libros)} , Errores: {errores}")
    with open(archivo_salida, 'w', encoding='utf-8') as f:
        json.dump(libros, f, ensure_ascii=False, indent=2)
 
    print(f"\nListo, {len(libros)} libros guardados en '{archivo_salida}'")
    if errores > 0:
        print(f"Advertencia: {errores} archivos no pudieron procesarse.")
 
 
if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Uso: python procesarlibros.py <carpeta_xml> <archivo_salida>")
        sys.exit(1)
 
    carpeta_xml    = sys.argv[1]
    archivo_salida = sys.argv[2]
 
    if not os.path.isdir(carpeta_xml):
        print(f"Error: '{carpeta_xml}' no es una carpeta válida.")
        sys.exit(1)
 
    procesar_dataset(carpeta_xml, archivo_salida)  
    